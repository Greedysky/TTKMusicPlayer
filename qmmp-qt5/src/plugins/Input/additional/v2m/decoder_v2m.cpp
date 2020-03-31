/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#include "v2mhelper.h"
#include "decoder_v2m.h"

DecoderV2M::DecoderV2M(const QString &path) : Decoder()
{
    m_v2m = new V2MHelper(path);
}

DecoderV2M::~DecoderV2M()
{
    delete m_v2m;
}

bool DecoderV2M::initialize()
{
    if(!m_v2m->initialize())
    {
        return false;
    }

    int rate = m_v2m->samplerate();
    int channels = m_v2m->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderV2M::totalTime() const
{
    return m_v2m->totalTime();
}

void DecoderV2M::seek(qint64 pos)
{
    m_v2m->seek(pos);
}

int DecoderV2M::bitrate() const
{
    return m_v2m->bitrate();
}

qint64 DecoderV2M::read(unsigned char *data, qint64 size)
{
    return m_v2m->read(data, size);
}
