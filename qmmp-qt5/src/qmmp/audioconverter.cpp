/***************************************************************************
 *   Copyright (C) 2010-2016 by Ilya Kotov                                 *
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

#include <math.h>
#include <string.h>
#include <QtGlobal>
#include <QtEndian>
#include "audioconverter.h"

#define INT_TO_FLOAT(TYPE,SWAP,in,out,samples,offset,max) \
{ \
    TYPE *in_ref = (TYPE *) (in); \
    for(size_t i = 0; i < samples; ++i) \
        out[i] = (float) (SWAP(in_ref[i]) - offset) / max; \
}

#define FLOAT_TO_INT(TYPE,SWAP,in,out,samples,offset,min,max) \
{ \
    TYPE *out_ref = (TYPE *) (out); \
    float tmp; \
    for(size_t i = 0; i < samples; ++i) \
    { \
        tmp = in[i] * max; \
        out_ref[i] = SWAP(offset + (TYPE) lrintf(qBound(-(float)min, tmp, (float)max))); \
    } \
}

AudioConverter::AudioConverter()
{
    m_format = Qmmp::PCM_UNKNOWM;
}

void AudioConverter::configure(Qmmp::AudioFormat f)
{
    m_format = f;
}

void AudioConverter::toFloat(const unsigned char *in, float *out, size_t samples)
{
    switch (m_format)
    {
    case Qmmp::PCM_S8:
        INT_TO_FLOAT(qint8,,in, out, samples, 0, 0x80);
        break;
    case Qmmp::PCM_U8:
        INT_TO_FLOAT(quint8,,in, out, samples, 0x80, 0x80);
        break;
    case Qmmp::PCM_S16LE:
        INT_TO_FLOAT(qint16, qFromLittleEndian, in, out, samples, 0, 0x8000);
        break;
    case Qmmp::PCM_S16BE:
        INT_TO_FLOAT(qint16, qFromBigEndian, in, out, samples, 0, 0x8000);
        break;
    case Qmmp::PCM_U16LE:
        INT_TO_FLOAT(quint16, qFromLittleEndian, in, out, samples, 0x8000, 0x8000);
        break;
    case Qmmp::PCM_U16BE:
        INT_TO_FLOAT(quint16, qFromBigEndian, in, out, samples, 0x8000, 0x8000);
        break;
    case Qmmp::PCM_S24LE:
        INT_TO_FLOAT(qint32, qFromLittleEndian, in, out, samples, 0, 0x800000);
        break;
    case Qmmp::PCM_S24BE:
        INT_TO_FLOAT(qint32, qFromBigEndian, in, out, samples, 0, 0x800000);
        break;
    case Qmmp::PCM_U24LE:
        INT_TO_FLOAT(quint32, qFromLittleEndian, in, out, samples, 0x800000, 0x800000);
        break;
    case Qmmp::PCM_U24BE:
        INT_TO_FLOAT(quint32, qFromBigEndian, in, out, samples, 0x800000, 0x800000);
        break;
    case Qmmp::PCM_S32LE:
        INT_TO_FLOAT(qint32, qFromLittleEndian, in, out, samples, 0, 0x80000000);
        break;
    case Qmmp::PCM_S32BE:
        INT_TO_FLOAT(qint32, qFromBigEndian, in, out, samples, 0, 0x80000000);
        break;
    case Qmmp::PCM_U32LE:
        INT_TO_FLOAT(quint32, qFromLittleEndian, in, out, samples, 0x80000000, 0x80000000);
        break;
    case Qmmp::PCM_U32BE:
        INT_TO_FLOAT(quint32, qFromBigEndian, in, out, samples, 0x80000000, 0x80000000);
        break;
    case Qmmp::PCM_FLOAT:
    case Qmmp::PCM_UNKNOWM:
        memcpy((void*)out, (void*)in, samples * sizeof(float));
    }
}

void AudioConverter::fromFloat(const float *in, const unsigned char *out, size_t samples)
{
    switch (m_format)
    {
    case Qmmp::PCM_S8:
        FLOAT_TO_INT(qint8,, in, out, samples, 0, 0x80, 0x7F);
        break;
    case Qmmp::PCM_U8:
        FLOAT_TO_INT(qint8,, in, out, samples, 0x80, 0x80, 0x7F);
        break;
    case Qmmp::PCM_S16LE:
        FLOAT_TO_INT(qint16, qToLittleEndian, in, out, samples, 0, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_S16BE:
        FLOAT_TO_INT(qint16, qToBigEndian, in, out, samples, 0, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_U16LE:
        FLOAT_TO_INT(quint16, qToLittleEndian, in, out, samples, 0x8000, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_U16BE:
        FLOAT_TO_INT(quint16, qToBigEndian, in, out, samples, 0x8000, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_S24LE:
        FLOAT_TO_INT(qint32, qToLittleEndian, in, out, samples, 0, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_S24BE:
        FLOAT_TO_INT(qint32, qToBigEndian, in, out, samples, 0, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_U24LE:
        FLOAT_TO_INT(quint32, qToLittleEndian, in, out, samples, 0x800000, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_U24BE:
        FLOAT_TO_INT(quint32, qToBigEndian, in, out, samples, 0x800000, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_S32LE:
        FLOAT_TO_INT(qint32, qToLittleEndian, in, out, samples, 0, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_S32BE:
        FLOAT_TO_INT(qint32, qToBigEndian, in, out, samples, 0, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_U32LE:
        FLOAT_TO_INT(quint32, qToLittleEndian, in, out, samples, 0x80000000, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_U32BE:
        FLOAT_TO_INT(quint32, qToBigEndian, in, out, samples, 0x80000000, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_FLOAT:
    case Qmmp::PCM_UNKNOWM:
        memcpy((void*)out, (void*)in, samples * sizeof(float));
    }
}
