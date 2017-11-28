/***************************************************************************
 *   Copyright (C) 2007-2015 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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
#include "srconverter.h"

SRConverter::SRConverter() : Effect()
{
    int converter_type_array[] = {SRC_SINC_BEST_QUALITY, SRC_SINC_MEDIUM_QUALITY, SRC_SINC_FASTEST,
                                  SRC_ZERO_ORDER_HOLD,  SRC_LINEAR};
    m_src_state = 0;
    m_srcError = 0;
    m_sz = 0;
    m_src_data.data_in = 0;
    m_src_data.data_out = 0;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_overSamplingFs = settings.value("SRC/sample_rate",48000).toInt();
    m_converter_type = converter_type_array[settings.value("SRC/engine", 0).toInt()];
}

SRConverter::~SRConverter()
{
    freeSRC();
    m_sz = 0;
}

void SRConverter::applyEffect(Buffer *b)
{
    if (m_src_state && b->samples > 0)
    {
        m_src_data.end_of_input = 0;
        m_src_data.data_in = b->data;
        m_src_data.input_frames = b->samples / channels();
        m_src_data.output_frames = m_src_data.src_ratio * m_src_data.input_frames + 1;

        if ((m_srcError = src_process(m_src_state, &m_src_data)) > 0)
        {
            qWarning("SRConverter: src_process(): %s\n", src_strerror(m_srcError));
        }

        b->samples = m_src_data.output_frames_gen * channels();
        m_src_data.data_in = 0;
        m_src_data.input_frames = 0;

        if(b->samples > b->size)
        {
            delete [] b->data;
            b->data = new float[b->samples];
            b->size = b->samples;
        }

        memcpy(b->data, m_src_data.data_out, b->samples * sizeof(float));
    }
}

void SRConverter::configure(quint32 freq, ChannelMap map)
{
    freeSRC();
    if(freq != m_overSamplingFs)
    {
        m_src_state = src_new(m_converter_type, map.count(), &m_srcError);
        if (m_src_state)
        {
            m_src_data.src_ratio = (float)m_overSamplingFs/(float)freq;
            src_set_ratio(m_src_state, m_src_data.src_ratio);
        }
        else
            qDebug("SRConverter: src_new(): %s", src_strerror(m_srcError));

        m_sz = audioParameters().sampleSize();
        m_src_data.data_out = new float[int(m_src_data.src_ratio * QMMP_BLOCK_FRAMES * map.count() * 2 + 2)];
    }
    Effect::configure(m_overSamplingFs, map);
}

void SRConverter::freeSRC()
{
    if (m_src_state)
    {
        src_reset(m_src_state);
        src_delete(m_src_state);
    }
    m_src_state = 0;

    if(m_src_data.data_in)
    {
        delete [] m_src_data.data_in;
        m_src_data.data_in = 0;
    }
    if(m_src_data.data_out)
    {
        delete [] m_src_data.data_out;
        m_src_data.data_out = 0;
    }

    m_src_data.end_of_input = 0;
    m_src_data.input_frames = 0;
    m_src_data.output_frames = 0;
}
