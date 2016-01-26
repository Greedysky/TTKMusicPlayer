/***************************************************************************
 *   Copyright (C) 2010-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "audioconverter_p.h"

//static functions
static inline void s8_to_s16(qint8 *in, qint16 *out, qint64 samples)
{
    for(qint64 i = 0; i < samples; ++i)
        out[i] = in[i] << 8;
    return;
}

static inline void s24_to_s16(qint32 *in, qint16 *out, qint64 samples)
{
    for(qint64 i = 0; i < samples; ++i)
        out[i] = in[i] >> 8;
    return;
}

static inline void s32_to_s16(qint32 *in, qint16 *out, qint64 samples)
{
    for(qint64 i = 0; i < samples; ++i)
        out[i] = in[i] >> 16;
    return;
}

AudioConverter::AudioConverter()
{
    m_format = Qmmp::PCM_UNKNOWM;
}

void AudioConverter::configure(quint32 srate, ChannelMap map, Qmmp::AudioFormat f)
{
    m_format = f;
    Effect::configure(srate, map, Qmmp::PCM_S16LE);
}

void AudioConverter::applyEffect(Buffer *b)
{
    switch(m_format)
    {
    case Qmmp::PCM_S8:
    {
        unsigned char *out = new unsigned char[b->nbytes*2];
        s8_to_s16((qint8 *)b->data, (qint16 *) out, b->nbytes);
        delete [] b->data;
        b->data = out;
        b->nbytes <<= 1;
        break;
    }
    case Qmmp::PCM_S24LE:
        s24_to_s16((qint32 *)b->data, (qint16 *)b->data, b->nbytes >> 2);
        b->nbytes >>= 1;
        break;
    case Qmmp::PCM_S32LE:
        s32_to_s16((qint32 *)b->data, (qint16 *)b->data, b->nbytes >> 2);
        b->nbytes >>= 1;
        break;
    default:
        ;
    }
}
