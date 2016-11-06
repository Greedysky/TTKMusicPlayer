/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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

#include <QSettings>
#include <math.h>
#include <stdlib.h>
#include <qmmp/qmmp.h>
#include "soxresampler.h"

SoXResampler::SoXResampler() : Effect()
{
    m_soxr = 0;
    m_out = 0;
    m_out_samples = 0;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_overSamplingFs = settings.value("SOXR/sample_rate", 48000).toInt();
    m_quality = soxr_quality_spec(settings.value("SOXR/quality", SOXR_HQ).toInt(), 0);
}

SoXResampler::~SoXResampler()
{
    freeSoXR();
}

void SoXResampler::applyEffect(Buffer *b)
{
    if(m_soxr && b->samples > 0)
    {
        size_t done = 0;
        soxr_process(m_soxr, b->data, b->samples / channels(), 0,
                     m_out, m_out_samples / channels(), &done);

        b->samples = done * channels();
        if(b->samples > b->size)
        {
            delete [] b->data;
            b->data = new float[b->samples];
            b->size = b->samples;
        }

        memcpy(b->data, m_out, b->samples * sizeof(float));
    }
}

void SoXResampler::configure(quint32 freq, ChannelMap map)
{
    freeSoXR();
    if(freq != m_overSamplingFs)
    {
        soxr_error_t error = 0;
        m_soxr = soxr_create(freq, m_overSamplingFs, map.count(), &error, 0, &m_quality, 0);
        double ratio = (double)m_overSamplingFs/freq;
        m_out_samples = ratio * QMMP_BLOCK_FRAMES * map.count() * 2 + 2;
        m_out = new float[m_out_samples];
    }
    Effect::configure(m_overSamplingFs, map);
}

void SoXResampler::freeSoXR()
{
    if(m_soxr)
    {
        soxr_delete(m_soxr);
        m_soxr = 0;
    }
    if(m_out)
    {
        delete [] m_out;
        m_out = 0;
        m_out_samples = 0;
    }
}
