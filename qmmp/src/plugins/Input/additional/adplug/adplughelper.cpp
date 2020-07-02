/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "adplughelper.h"

AdplugHelper::AdplugHelper(const std::string &filename)
    : m_opl(new CEmuopl(rate(), true, false)),
      m_player(CAdPlug::factory(filename.c_str(), m_opl.get()))
{

}

AdplugHelper::Frame AdplugHelper::read()
{
    size_t to_write;
    size_t bufsiz = sizeof(m_buf) / sizeof(*m_buf);

    if(m_remaining == 0)
    {
        if(!m_player->update())
        {
            return Frame(0, nullptr);
        }
        m_remaining = rate() / m_player->getrefresh();
    }

    if(m_remaining > bufsiz)
    {
        to_write = bufsiz;
    }
    else
    {
        to_write = m_remaining;
    }

    m_opl->update(m_buf, to_write);
    m_remaining -= to_write;
    return Frame(to_write * 2, reinterpret_cast<unsigned char *>(m_buf));
}

bool AdplugHelper::initialize()
{
    return m_player.get();
}

std::vector<std::string> AdplugHelper::instruments()
{
    std::vector<std::string> insts;
    for(unsigned int i = 0; i < instrument_count(); i++)
    {
        insts.push_back(m_player->getinstrument(i));
    }
    return insts;
}
