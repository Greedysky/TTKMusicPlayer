/***************************************************************************
 *   Copyright (C) 2009-2014 by Ilya Kotov                                 *
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

#include "audioparameters.h"

AudioParameters::AudioParameters()
{
    m_srate = 0;
    m_format = Qmmp::PCM_S16LE;
}

AudioParameters::AudioParameters(const AudioParameters &other)
{
    m_srate = other.sampleRate();
    m_chan_map = other.channelMap();
    m_format = other.format();
}

AudioParameters::AudioParameters(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat  format)
{
    m_srate = srate;
    m_chan_map = map;
    m_format = format;
}

void AudioParameters::operator=(const AudioParameters &p)
{
    m_srate = p.sampleRate();
    m_chan_map = p.channelMap();
    m_format = p.format();
}

bool AudioParameters::operator==(const AudioParameters &p) const
{
    return m_srate == p.sampleRate() && m_chan_map == p.channelMap() && m_format == p.format();
}

bool AudioParameters::operator!=(const AudioParameters &p) const
{
    return !operator==(p);
}

quint32 AudioParameters::sampleRate() const
{
    return m_srate;
}

int AudioParameters::channels() const
{
    return m_chan_map.count();
}

const ChannelMap AudioParameters::channelMap() const
{
    return m_chan_map;
}

Qmmp::AudioFormat AudioParameters::format() const
{
    return m_format;
}

int AudioParameters::sampleSize() const
{
    return sampleSize(m_format);
}

int AudioParameters::sampleSize(Qmmp::AudioFormat format)
{
    switch(format)
    {
    case Qmmp::PCM_S8:
        return 1;
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_UNKNOWM:
        return 2;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S32LE:
        return 4;
    }
    return 2;
}
