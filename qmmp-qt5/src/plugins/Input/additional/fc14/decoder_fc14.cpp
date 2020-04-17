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

#include "fc14helper.h"
#include "decoder_fc14.h"

DecoderFC14::DecoderFC14(const QString &path) : Decoder()
{
    m_fc14 = new FC14Helper(path);
}

DecoderFC14::~DecoderFC14()
{
    delete m_fc14;
}

bool DecoderFC14::initialize()
{
    if(!m_fc14->initialize())
    {
        return false;
    }

    int rate = m_fc14->samplerate();
    int channels = m_fc14->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderFC14::totalTime() const
{
    return m_fc14->totalTime();
}

int DecoderFC14::bitrate() const
{
    return m_fc14->bitrate();
}

qint64 DecoderFC14::read(unsigned char *data, qint64 size)
{
    return m_fc14->read(data, size);
}

void DecoderFC14::seek(qint64 pos)
{
    m_fc14->seek(pos);
}
