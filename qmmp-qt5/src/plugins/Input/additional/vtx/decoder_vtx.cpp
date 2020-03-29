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

#include "vtxhelper.h"
#include "decoder_vtx.h"

DecoderVTX::DecoderVTX(const QString &path) : Decoder()
{
    m_vtx = new VTXHelper(path);
}

DecoderVTX::~DecoderVTX()
{
    delete m_vtx;
}

bool DecoderVTX::initialize()
{
    if(!m_vtx->initialize())
    {
        return false;
    }

    int rate = m_vtx->samplerate();
    int channels = m_vtx->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderVTX::totalTime() const
{
    return m_vtx->totalTime();
}

void DecoderVTX::seek(qint64 pos)
{
    m_vtx->seek(pos);
}

int DecoderVTX::bitrate() const
{
    return m_vtx->bitrate();
}

qint64 DecoderVTX::read(unsigned char *data, qint64 size)
{
    return m_vtx->read(data, size);
}
