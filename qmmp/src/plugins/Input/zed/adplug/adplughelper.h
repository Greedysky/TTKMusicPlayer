/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef ADPLUGHELPER_H
#define ADPLUGHELPER_H

#include <memory>
#include <vector>

#include <adplug/adplug.h>
#include <adplug/emuopl.h>

/**
   @author Ilya Kotov <forkotov02@ya.ru>
*/
class AdplugHelper
{
public:
    struct Frame
    {
      Frame(size_t n, unsigned char *buf)
          : m_n(n),
            m_buf(buf)
      {

      }

      size_t m_n;
      unsigned char *m_buf;
    };

    explicit AdplugHelper(const std::string &);
    AdplugHelper(const AdplugHelper &) = delete;

    AdplugHelper &operator=(const AdplugHelper &) = delete;

    static std::string version() { return CAdPlug::get_version(); }

    bool initialize();

    int bitrate() const;
    int rate() const { return 44100; }
    int depth() const { return 16; }
    int channels() const { return 1; }

    unsigned long length() const { return m_player->songlength(); }
    void seek(unsigned long pos) const { m_player->seek(pos); }

    Frame read();

    std::string title() const { return m_player->gettitle(); }
    std::string format() const { return m_player->gettype(); }
    std::string author() const { return m_player->getauthor(); }
    std::string description() const { return m_player->getdesc(); }
    unsigned int pattern_count() const { return m_player->getpatterns(); }
    unsigned int instrument_count() const { return m_player->getinstruments(); }

    std::vector<std::string> instruments() const;

private:
    std::string m_filePath;
    std::unique_ptr<Copl> m_opl;
    std::unique_ptr<CPlayer> m_player;
    short m_buf[16384] = { 0 };
    size_t m_remaining = 0;

};

#endif
