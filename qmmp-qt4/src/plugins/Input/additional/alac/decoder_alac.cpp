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

#include "decoder_alac.h"
#include "alachelper.h"

DecoderAlac::DecoderAlac(const QString &path)
    : Decoder()
{
    m_alac = new AlacHelper(path);
}

DecoderAlac::~DecoderAlac()
{
    delete m_alac;
}

bool DecoderAlac::initialize()
{
    if(!m_alac->initialize())
    {
        qDebug("ALACHelper: initialize false");
        return false;
    }

    configure(m_alac->samplerate(), 2, Qmmp::PCM_S16LE);

    qDebug("ALACHelper: initialize success");
    return true;
}

qint64 DecoderAlac::totalTime() const
{
    return m_alac->totalTime();
}

int DecoderAlac::bitrate() const
{
    return m_alac->bitrate();
}

qint64 DecoderAlac::read(unsigned char *data, qint64 maxSize)
{
    return m_alac->read(data, maxSize);
}

void DecoderAlac::seek(qint64 time)
{
    m_alac->seek(time);
}
