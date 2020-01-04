/***************************************************************************
 *   Copyright (C) 2006-2019 by Ilya Kotov                                 *
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

#include "adplugwrap.h"

AdplugWrap::AdplugWrap(const std::string &filename)
    : opl(new CEmuopl(rate(), true, false)),
      player(CAdPlug::factory(filename.c_str(), opl.get()))
{
    if(!player) throw InvalidFile();
}

AdplugWrap::Frame AdplugWrap::read()
{
    size_t to_write;
    size_t bufsiz = sizeof(buf) / sizeof(*buf);

    if(remaining == 0)
    {
        if(!player->update())
        {
            return Frame(0, nullptr);
        }
        remaining = rate() / player->getrefresh();
    }

    if(remaining > bufsiz)
    {
        to_write = bufsiz;
    }
    else
    {
        to_write = remaining;
    }

    opl->update(buf, to_write);
    remaining -= to_write;
    return Frame(to_write * 2, reinterpret_cast<unsigned char *>(buf));
}

std::vector<std::string> AdplugWrap::instruments()
{
    std::vector<std::string> insts;
    for(unsigned int i = 0; i < instrument_count(); i++)
    {
        insts.push_back(player->getinstrument(i));
    }
    return insts;
}
