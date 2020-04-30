/***************************************************************************
 *   Copyright (C) 2018-2019 by Ilya Kotov                                 *
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

#include <stdlib.h>
#include "monoplugin.h"

MonoPlugin::MonoPlugin() : Effect()
{
    m_avg = 0;
    m_chan = 0;
}

MonoPlugin::~MonoPlugin()
{

}

void MonoPlugin::applyEffect(Buffer *b)
{
    if(m_chan != 2)
        return;

    m_mutex.lock();
    float *data = b->data;

    for(uint i = 0; i < b->samples; i += 2)
    {
        m_avg = (data[i] + data[i + 1]) / 2;
        m_avg = qBound(-1.0, m_avg, 1.0);
        data[i] = data[i + 1] = m_avg;
    }

    m_mutex.unlock();
}

void MonoPlugin::configure(quint32 freq, ChannelMap map)
{
    m_chan = map.count();
    Effect::configure(freq, map);
}
