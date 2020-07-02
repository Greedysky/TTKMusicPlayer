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

#include "decoder_optimfrog.h"

DecoderOptimFROG::DecoderOptimFROG(QIODevice *i) : Decoder(i)
{
}

bool DecoderOptimFROG::initialize()
{
    if(!input())
        return false;
    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    m_frog = std::unique_ptr<OptimFROGHelper>(new OptimFROGHelper(input()));
    if(!m_frog->initialize())
    {
        return false;
    }

    enum Qmmp::AudioFormat format;
    switch(m_frog->depth())
    {
        case 8:
            format = Qmmp::PCM_S8;
            break;
        case 16:
            format = Qmmp::PCM_S16LE;
            break;
        default:
            return false;
    }

    configure(m_frog->rate(), m_frog->channels(), format);

    return true;
}

qint64 DecoderOptimFROG::totalTime() const
{
    return m_frog->length();
}

int DecoderOptimFROG::bitrate() const
{
    return m_frog->bitrate();
}

qint64 DecoderOptimFROG::read(unsigned char *audio, qint64 max_size)
{
    return m_frog->read(audio, max_size);
}

void DecoderOptimFROG::seek(qint64 pos)
{
    m_frog->seek(pos);
}
