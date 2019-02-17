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
#include <qmmp/qmmp.h>
#include "monotostereoplugin.h"

MonoToStereoPlugin::MonoToStereoPlugin() : Effect()
{
    m_tmp = nullptr;
    m_size = 0;
    m_enabled = false;
}

MonoToStereoPlugin::~MonoToStereoPlugin()
{
    if(m_tmp)
    {
        delete[] m_tmp;
        m_tmp = nullptr;
    }
}

void MonoToStereoPlugin::applyEffect(Buffer *b)
{
    if(m_enabled)
    {
        if(m_size < b->samples)
        {
            if(m_tmp)
                delete [] m_tmp;
            m_tmp = new float[b->samples];
        }
        memcpy(m_tmp, b->data, b->samples * sizeof(float));

        b->samples *= 2;

        if(b->samples > b->size)
        {
            delete [] b->data;
            b->size = b->samples;
            b->data = new float[b->size];
        }

        for(size_t i = 0; i < b->samples; ++i)
            b->data[i] = m_tmp[i >> 1];
    }
}

void MonoToStereoPlugin::configure(quint32 freq, ChannelMap map)
{
    if(map.count() == 1)
    {
        m_enabled = true;
        Effect::configure(freq, ChannelMap(2));
    }
    else
    {
        m_enabled = false;
        Effect::configure(freq, map);
    }
}
