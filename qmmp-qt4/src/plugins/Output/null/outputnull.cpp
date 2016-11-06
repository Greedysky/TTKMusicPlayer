/***************************************************************************
 *   Copyright (C) 2010-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

#include <unistd.h>
#include "outputnull.h"


OutputNull::OutputNull() : Output()
{
   m_bytes_per_second = 0;
}

OutputNull::~OutputNull()
{}

bool OutputNull::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    switch (format)
    {
    case Qmmp::PCM_S8:
        m_bytes_per_second = freq * map.count();
        break;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S32LE:
         m_bytes_per_second = freq * map.count() * 4;
        break;
    case Qmmp::PCM_S16LE:
    default:
         m_bytes_per_second = freq * map.count() * 2;
    }
    configure(freq, map, format);
    return true;
}


qint64 OutputNull::latency()
{
    return 0;
}

qint64 OutputNull::writeAudio(unsigned char *data, qint64 maxSize)
{
   Q_UNUSED(data);
   usleep(maxSize * 1000000 / m_bytes_per_second);
   return maxSize;
}

void OutputNull::drain()
{}

void OutputNull::reset()
{}
