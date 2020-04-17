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

#include "psfhelper.h"
#include "decoder_psf.h"

DecoderPSF::DecoderPSF(const QString &path) : Decoder()
{
    m_psf = new PSFHelper(path);
}

DecoderPSF::~DecoderPSF()
{
    delete m_psf;
}

bool DecoderPSF::initialize()
{
    if(!m_psf->initialize())
    {
        return false;
    }

    int rate = m_psf->samplerate();
    int channels = m_psf->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderPSF::totalTime() const
{
    return m_psf->totalTime();
}

int DecoderPSF::bitrate() const
{
    return m_psf->bitrate();
}

qint64 DecoderPSF::read(unsigned char *data, qint64 size)
{
    return m_psf->read(data, size);
}

void DecoderPSF::seek(qint64 pos)
{
    m_psf->seek(pos);
}
