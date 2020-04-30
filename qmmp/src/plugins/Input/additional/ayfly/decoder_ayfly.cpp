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

#include "ayflyhelper.h"
#include "decoder_ayfly.h"

DecoderAyfly::DecoderAyfly(const QString &path) : Decoder()
{
    m_ayfly = new AyflyHelper(path);
}

DecoderAyfly::~DecoderAyfly()
{
    delete m_ayfly;
}

bool DecoderAyfly::initialize()
{
    if(!m_ayfly->initialize())
    {
        return false;
    }

    int rate = m_ayfly->samplerate();
    int channels = m_ayfly->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderAyfly::totalTime() const
{
    return m_ayfly->totalTime();
}

int DecoderAyfly::bitrate() const
{
    return m_ayfly->bitrate();
}

qint64 DecoderAyfly::read(unsigned char *data, qint64 size)
{
    return m_ayfly->read(data, size);
}

void DecoderAyfly::seek(qint64 pos)
{
    m_ayfly->seek(pos);
}
