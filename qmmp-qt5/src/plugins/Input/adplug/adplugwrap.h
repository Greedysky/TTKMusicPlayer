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

#ifndef ADPLUGWRAP_H
#define ADPLUGWRAP_H

#include <memory>
#include <vector>

#include <adplug/adplug.h>
#include <adplug/emuopl.h>

/**
   @author Ilya Kotov <forkotov02@ya.ru>
*/
class AdplugWrap
{
public:
    struct Frame
    {
      Frame(size_t n, unsigned char *buf) : n(n), buf(buf) { }
      size_t n;
      unsigned char *buf;
    };

    class InvalidFile : public std::exception
    {
      public:
        InvalidFile() : std::exception() { }
    };

    explicit AdplugWrap(const std::string &);
    AdplugWrap(const AdplugWrap &) = delete;

    AdplugWrap &operator=(const AdplugWrap &) = delete;

    static std::string version() { return CAdPlug::get_version(); }

    int rate() { return 44100; }
    int depth() { return 16; }
    int channels() { return 1; }

    unsigned long length() { return m_player->songlength(); }
    void seek(unsigned long pos) { m_player->seek(pos); }

    Frame read();

    std::string title() { return m_player->gettitle(); }
    std::string format() { return m_player->gettype(); }
    std::string author() { return m_player->getauthor(); }
    std::string description() { return m_player->getdesc(); }
    unsigned int pattern_count() { return m_player->getpatterns(); }
    unsigned int instrument_count() { return m_player->getinstruments(); }

    std::vector<std::string> instruments();

private:
    std::unique_ptr<Copl> m_opl;
    std::unique_ptr<CPlayer> m_player;
    short m_buf[16384] = { 0 };
    size_t m_remaining = 0;
};

#endif
