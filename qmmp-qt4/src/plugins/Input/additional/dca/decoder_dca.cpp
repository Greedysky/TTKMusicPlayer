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

#include "dcahelper.h"
#include "decoder_dca.h"

DecoderDCA::DecoderDCA(const QString &path) : Decoder()
{
    m_dca = new DCAHelper(path);
}

DecoderDCA::~DecoderDCA()
{
    delete m_dca;
}

bool DecoderDCA::initialize()
{
    if(!m_dca->initialize())
    {
        return false;
    }

    int rate = m_dca->samplerate();
    int channels = m_dca->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderDCA::totalTime() const
{
    return m_dca->totalTime();
}

int DecoderDCA::bitrate() const
{
    return m_dca->bitrate();
}

qint64 DecoderDCA::read(unsigned char *data, qint64 size)
{
    return m_dca->read(data, size);
}

void DecoderDCA::seek(qint64 pos)
{
    m_dca->seek(pos);
}
