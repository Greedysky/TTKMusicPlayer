/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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

#include <QMap>
#include "audioparameters.h"

AudioParameters::AudioParameters()
{
    m_srate = 0;
    m_format = Qmmp::PCM_S16LE;
    m_sz = 2;
}

AudioParameters::AudioParameters(const AudioParameters &other)
{
    m_srate = other.sampleRate();
    m_chan_map = other.channelMap();
    m_format = other.format();
    m_sz = other.sampleSize();
}

AudioParameters::AudioParameters(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat format)
{
    m_srate = srate;
    m_chan_map = map;
    m_format = format;
    m_sz = sampleSize(format);
}

void AudioParameters::operator=(const AudioParameters &p)
{
    m_srate = p.sampleRate();
    m_chan_map = p.channelMap();
    m_format = p.format();
    m_sz = p.sampleSize();
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
    return m_sz;
}

const QString AudioParameters::toString() const
{
    static const struct
    {
        Qmmp::AudioFormat format;
        QString name;

    }
    format_names [] =
    {
    { Qmmp::PCM_S8, "s8" },
    { Qmmp::PCM_U8, "u8" },
    { Qmmp::PCM_S16LE, "s16le" },
    { Qmmp::PCM_S16BE, "s16be" },
    { Qmmp::PCM_U16LE, "u16le" },
    { Qmmp::PCM_U16BE, "u16be" },
    { Qmmp::PCM_S24LE, "s24le" },
    { Qmmp::PCM_S24BE, "s24be" },
    { Qmmp::PCM_U24LE, "u24le" },
    { Qmmp::PCM_U24BE, "u24be" },
    { Qmmp::PCM_S32LE, "s32le" },
    { Qmmp::PCM_S32BE, "s32be" },
    { Qmmp::PCM_U32LE, "u32le" },
    { Qmmp::PCM_U32BE, "u32be" },
    { Qmmp::PCM_FLOAT, "float" },
    { Qmmp::PCM_UNKNOWM, QString() }
    };

    QString name = "unknown";
    for(int i = 0; format_names[i].format != Qmmp::PCM_UNKNOWM; ++i)
    {
        if(m_format == format_names[i].format)
        {
            name = format_names[i].name;
            break;
        }
    }

    return QString("%1 Hz, {%2}, %3").arg(m_srate).arg(m_chan_map.toString()).arg(name);
}

int AudioParameters::sampleSize(Qmmp::AudioFormat format)
{
    switch(format)
    {
    case Qmmp::PCM_S8:
    case Qmmp::PCM_U8:
        return 1;
    case Qmmp::PCM_UNKNOWM:
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_S16BE:
    case Qmmp::PCM_U16LE:
    case Qmmp::PCM_U16BE:
        return 2;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_U24LE:
    case Qmmp::PCM_U24BE:
    case Qmmp::PCM_S32LE:
    case Qmmp::PCM_S32BE:
    case Qmmp::PCM_U32LE:
    case Qmmp::PCM_U32BE:
    case Qmmp::PCM_FLOAT:
        return 4;
    }
    return 2;
}
