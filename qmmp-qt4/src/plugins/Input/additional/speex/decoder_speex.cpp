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

#include "speexhelper.h"
#include "decoder_speex.h"

DecoderSpeex::DecoderSpeex(QIODevice *i) : Decoder(i)
{
    m_speex = new SpeexHelper(i);
}

DecoderSpeex::~DecoderSpeex()
{
    delete m_speex;
}

bool DecoderSpeex::initialize()
{
    if(!input())
        return false;
    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    if(!m_speex->initialize())
    {
        return false;
    }

    int32_t rate = m_speex->stream_get_samplerate();
    int32_t channels = m_speex->stream_get_channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderSpeex::totalTime() const
{
    return m_speex->get_samples();
}

void DecoderSpeex::seek(qint64 pos)
{
    m_speex->seek(pos);
}

int DecoderSpeex::bitrate() const
{
    return m_speex->get_bitrate();
}

qint64 DecoderSpeex::read(unsigned char *data, qint64 size)
{
    return m_speex->read(data, size);
}
