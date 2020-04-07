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

#include "asaphelper.h"
#include "decoder_asap.h"

DecoderAsap::DecoderAsap(const QString &path) : Decoder()
{
    m_asap = new AsapHelper(path);
}

DecoderAsap::~DecoderAsap()
{
    delete m_asap;
}

bool DecoderAsap::initialize()
{
    if(!m_asap->initialize())
    {
        return false;
    }

    int rate = m_asap->samplerate();
    int channels = m_asap->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderAsap::totalTime() const
{
    return m_asap->totalTime();
}

void DecoderAsap::seek(qint64 pos)
{
    m_asap->seek(pos);
}

int DecoderAsap::bitrate() const
{
    return m_asap->bitrate();
}

qint64 DecoderAsap::read(unsigned char *data, qint64 size)
{
    return m_asap->read(data, size);
}
