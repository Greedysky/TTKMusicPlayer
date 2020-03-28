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

#include "ttahelper.h"
#include "decoder_tta.h"

DecoderTTA::DecoderTTA(const QString &path) : Decoder()
{
    m_tta = new TTAHelper(path);
}

DecoderTTA::~DecoderTTA()
{
    delete m_tta;
}

bool DecoderTTA::initialize()
{
    if(!m_tta->initialize())
    {
        return false;
    }

    int rate = m_tta->samplerate();
    int channels = m_tta->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderTTA::totalTime() const
{
    return m_tta->totalTime();
}

void DecoderTTA::seek(qint64 pos)
{
    m_tta->seek(pos);
}

int DecoderTTA::bitrate() const
{
    return m_tta->bitrate();
}

qint64 DecoderTTA::read(unsigned char *data, qint64 size)
{
    return m_tta->read(data, size);
}
