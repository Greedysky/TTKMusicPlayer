/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <QtGlobal>

#include "magic.h"

class Matcher
{
  public:
    virtual bool matches(InputStream &) = 0;
    virtual ~Matcher() = default;
};

class Magic : public Matcher
{
  public:
    Magic(std::size_t offset, const char *magic) : m_offset(offset), m_magic(magic), m_len(strlen(magic)) { }
    Magic(std::size_t offset, const char *magic, std::size_t len) : m_offset(offset), m_magic(magic), m_len(len) { }

    bool matches(InputStream &stream) override
    {
      std::vector<char> buf(m_len);

      return stream.read(m_offset, buf.data(), buf.size()) && std::memcmp(buf.data(), m_magic, buf.size()) == 0;
    }

  private:
    const std::size_t m_offset;
    const char *m_magic;
    const std::size_t m_len;
};

/* DMO decrypt code based on Adplug. */
class DMOMatcher : public Matcher
{
  public:
    bool matches(InputStream &stream) override
    {
      unsigned char header[12];

      return stream.read(0, header, sizeof header) && decrypt(header);
    }

  private:
    static quint16 le16(const unsigned char *p) { return ((quint16)    ( p[1]) <<  8) |     ( p[0]); }
    static quint32 le32(const unsigned char *p) { return ((quint32)le16(&p[2]) << 16) | le16(&p[0]); }

    static quint16 LOWORD(quint32 l) { return l & 0xffff; }
    static quint16 HIWORD(quint32 l) { return l >> 16; }
    static quint8  LOBYTE(quint16 w) { return w & 0xff; }
    static quint8  HIBYTE(quint16 w) { return w >> 8; }

    static int brand(quint32 &bseed)
    {
      quint16 range = 0xffff;
      quint16 ax, bx, cx, dx;

      ax = LOWORD(bseed);
      bx = HIWORD(bseed);
      cx = ax;
      ax = LOWORD(cx * 0x8405);
      dx = HIWORD(cx * 0x8405);
      cx <<= 3;
      cx = (((HIBYTE(cx) + LOBYTE(cx)) & 0xff) << 8) + LOBYTE(cx);
      dx += cx;
      dx += bx;
      bx <<= 2;
      dx += bx;
      dx = (((HIBYTE(dx) + LOBYTE(bx)) & 0xff) << 8) + LOBYTE(dx);
      bx <<= 5;
      dx = (((HIBYTE(dx) + LOBYTE(bx)) & 0xff) << 8) + LOBYTE(dx);
      ax += 1;
      if(ax == 0) dx += 1;

      bseed = dx;
      bseed <<= 16;
      bseed += ax;

      return HIWORD(HIWORD(LOWORD(bseed) * range) + HIWORD(bseed) * range);
    }

    static bool decrypt(const unsigned char *header)
    {
      quint32 bseed;
      quint32 seed = 0;
      unsigned char zeros[12] = { 0 };

      /* The DMO decrypt algorithm successfully “decrypts” all nulls,
       * which results in a number of false positives.  No .dmo file
       * I've seen starts with all nulls, so this should be OK.
       */
      if(std::memcmp(header, zeros, sizeof zeros) == 0) return false;

      bseed = le32(&header[0]);

      for(int i = 0; i < le16(&header[4]) + 1; i++) seed += brand(bseed);

      bseed = seed ^ le32(&header[6]);

      return le16(&header[10]) == brand(bseed);
    }
};

class OriginMatcher : public Matcher
{
  /* I've found few false-positives with this... */
  bool matches(InputStream &stream) override
  {
    unsigned char header[6];

    if(!stream.read(0, header, sizeof header)) return false;

    /* TrueType files match Origin magic, so try to detect them here.
     * All .m files I've tested pass this.
     */
    if(std::memcmp(header, "\0\1\0\0\0", 5) == 0)
    {
      return false;
    }

    return header[2] == 0 && header[3] == 0 && header[4] + ((header[5] & 0x1) << 8) == 0x100;
  }
};

class GodOfThunderMatcher : public Matcher
{
  bool matches(InputStream &stream) override
  {
    try
    {
      unsigned char header[2];
      unsigned char footer[4];
      size_t size = stream.size();

      if(size % 3 != 0 || size < 9) return false;
      if(!stream.read(0, header, sizeof header)) return false;
      if(std::memcmp(header, "\1\0", 2) != 0) return false;
      if(!stream.read(size - 4, footer, sizeof footer)) return false;
      if(std::memcmp(footer, "\0\0\0\0", 4) != 0) return false;

      return true;
    }
    catch(const InputStream::NoRandomAccess &)
    {
      return false;
    }
  }
};

bool adplug_supports(InputStream &&stream)
{
  std::vector<std::shared_ptr<Matcher>> matchers = {
    /* AdLib Tracker 2 (.a2m) */
    std::make_shared<Magic>(0, "_A2module_"),

    /* AMUSIC Adlib Tracker (.amd) */
    std::make_shared<Magic>(1062, "<o\xefQU\xeeRoR"),

    /* Bob's Adlib Music Format (.bam) */
    std::make_shared<Magic>(0, "CBMF"),

    /* BoomTracker 4.0 (.cff) */
    std::make_shared<Magic>(0, "<CUD-FM-File>"),

    /* Creative Music File Format (.cmf) */
    std::make_shared<Magic>(0, "CTMF"),

    /* EdLib (.d00) */
    std::make_shared<Magic>(0, "JCH\x26\x02\x66"),

    /* Digital-FM (.dfm) */
    std::make_shared<Magic>(0, "DFM\x1a"),

    /* DOSBox Raw OPL (.dro) */
    std::make_shared<Magic>(0, "DBRAWOPL"),

    /* DeFy Adlib Tracker (.dtm) */
    std::make_shared<Magic>(0, "DeFy DTM "),

    /* LucasArts AdLib Audio (.laa) */
    std::make_shared<Magic>(0, "ADL"),

    /* Mlat Adlib Tracker (.mad) */
    std::make_shared<Magic>(0, "MAD+"),

    /* MKJamz (.mkj) */
    std::make_shared<Magic>(0, "MKJamz"),

    /* AdLib MSCplay (.msc) */
    std::make_shared<Magic>(0, "Ceres \x13 MSCplay "),

    /* MPU-401 Trakker (.mtk) */
    std::make_shared<Magic>(0, "mpu401tr\x92kk\xeer@data"),

    /* Reality ADlib Tracker (.rad) */
    std::make_shared<Magic>(0, "RAD by REALiTY!!"),

    /* RdosPlay RAW file format (.raw) */
    std::make_shared<Magic>(0, "RAWADATA"),

    /* Softstar RIX OPL (.rix) */
    std::make_shared<Magic>(0, "\xaa\x55"),

    /* AdLib Visual Composer (.rol) */
    std::make_shared<Magic>(0, "\0\0\4\0", 4),

    /* Surprise! Adlib Tracker 1 & 2 (.sat, .sa2) */
    std::make_shared<Magic>(0, "SAdT"),

    /* Sierra's AdLib Audio (.sci) */
    std::make_shared<Magic>(0, "\x84\x00", 2),

    /* SNGPlay (.sng) */
    std::make_shared<Magic>(0, "ObsM"),

    /* Faust Music Creator (.sng) */
    std::make_shared<Magic>(0, "FMC!"),

    /* eXotic ADlib Format (.xad) */
    std::make_shared<Magic>(0, "XAD!"),

    /* XMS-Tracker (.xms) */
    std::make_shared<Magic>(1062, "MaDoKaN96"),

    /* eXtra Simple Music (.xsm) */
    std::make_shared<Magic>(0, "ofTAZ!"),

    /* BMF Adlib Tracker (.bmf) */
    std::make_shared<Magic>(0, "BMF1.1"),
    std::make_shared<Magic>(0, "BMF1.2"),

    /* SoundFX Macs Opera CMF (.cmf) */
    std::make_shared<Magic>(0, "A.H."),

    /* Note Sequencer by sopepos (.sop) */
    std::make_shared<Magic>(0, "sopepos"),

    /* Video Game Music (.vgm) */
    std::make_shared<Magic>(0, "Vgm "),

    /* Twin TrackPlayer by TwinTeam (.dmo) */
    std::make_shared<DMOMatcher>(),

    /* Origin AdLib Music Format (.m) */
    std::make_shared<OriginMatcher>(),

    /* God of Thunder Music (.got) */
    std::make_shared<GodOfThunderMatcher>(),
  };

  return std::any_of(
      matchers.begin(), matchers.end(),
      [&stream](const std::shared_ptr<Matcher> &matcher){ return matcher->matches(stream); }
  );
}
