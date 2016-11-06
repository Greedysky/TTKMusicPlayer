/***************************************************************************
 *  Based on madplay project                                               *
 *                                                                         *
 * Copyright (C) 2000-2004 Robert Leslie <rob@mars.org>                    *
 * Copyright (C) 2016 Ilya Kotov forkotov02@hotmail.ru                     *
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

#include "dithering_p.h"

Dithering::Dithering() : Effect()
{
    m_lsb = 0.0f;
    m_required = false;
    m_enabled = false;
}

void Dithering::configure(quint32 srate, ChannelMap map)
{
    m_chan = map.count();
    m_required = false;
    for(int i = 0; i < 9; ++i)
    {
        m_dither[i].error[0] = 0.0f;
        m_dither[i].error[1] = 0.0f;
        m_dither[i].error[2] = 0.0f;
        m_dither[i].random = 0;
    }
    Effect::configure(srate, map);
}

void Dithering::setFormats(Qmmp::AudioFormat in, Qmmp::AudioFormat out)
{
    m_required = false;
    if(AudioParameters::sampleSize(in) > AudioParameters::sampleSize(out))
    {
        switch (out)
        {
        case Qmmp::PCM_S8:
        case Qmmp::PCM_U8:
            m_lsb = 1.0f / 0x80;
            m_required = true;
            break;
        case Qmmp::PCM_S16LE:
        case Qmmp::PCM_S16BE:
        case Qmmp::PCM_U16LE:
        case Qmmp::PCM_U16BE:
            m_lsb = 1.0f / 0x8000;
            m_required = true;
            break;
        default:
            ;
        }
    }
    (m_required && m_enabled) ? qDebug("Dithering: enabled") : qDebug("Dithering: disabled");
}

void Dithering::applyEffect(Buffer *b)
{
    if(m_required && m_enabled)
    {
        for(size_t i = 0; i < b->samples; ++i)
            b->data[i] = audioLinearDither(b->data[i], &m_dither[i % m_chan]);
    }
}

void Dithering::setEnabled(bool enabled)
{
    m_enabled = enabled;
    (m_required && m_enabled) ? qDebug("Dithering: enabled") : qDebug("Dithering: disabled");
}

quint32 Dithering::prng(quint32 state) // 32-bit pseudo-random number generator
{
    return (state * 0x0019660dL + 0x3c6ef35fL) & 0xffffffffL;
}

float Dithering::audioLinearDither(float sample, AudioDither *dither)
{
    float output; quint32 random;

    /* noise shape */
    sample += dither->error[0] - dither->error[1] + dither->error[2];

    dither->error[2] = dither->error[1];
    dither->error[1] = dither->error[0] / 2;

    /* bias */
    output = sample + m_lsb;

    /* dither */
    random = prng(dither->random);
    output += (float)(random - dither->random) / 0xffffffffL * m_lsb;
    dither->random = random;

    /* clip */
    if (output > 1.0f)
    {
        output = 1.0f;

        if (sample > 1.0f)
            sample = 1.0f;
    }
    else if (output < -1.0f)
    {
        output = -1.0f;

        if (sample < -1.0f)
            sample = -1.0f;
    }

    /* error feedback */
    dither->error[0] = sample - output;

    return output;
}
