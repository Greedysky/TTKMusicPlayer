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

#include "shnhelper.h"
#include "decoder_shn.h"

DecoderSHN::DecoderSHN(const QString &path) : Decoder()
{
    m_shn = new SHNHelper(path);
}

DecoderSHN::~DecoderSHN()
{
    delete m_shn;
}

bool DecoderSHN::initialize()
{
    if(!m_shn->initialize())
    {
        return false;
    }

    int rate = m_shn->samplerate();
    int channels = m_shn->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderSHN::totalTime() const
{
    return m_shn->totalTime();
}

void DecoderSHN::seek(qint64 pos)
{
    m_shn->seek(pos);
}

int DecoderSHN::bitrate() const
{
    return m_shn->bitrate();
}

qint64 DecoderSHN::read(unsigned char *data, qint64 size)
{
    return m_shn->read(data, size);
}
