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

#include <QFileInfo>
#include "alachelper.h"

int host_bigendian = 0;

AlacHelper::AlacHelper(const QString &path)
{
    {
        uint32_t integer = 0x000000aa;
        unsigned char *pointer = (unsigned char*)&integer;
        host_bigendian = (pointer[0] == 0xaa) ? 0 : 1;
    }

    m_path = path;
    m_bitrate = 0;
    m_totalTime = 0;
    m_file = nullptr;
    m_alac = nullptr;
    m_stream = nullptr;
}

AlacHelper::~AlacHelper()
{
    close();
}

void AlacHelper::close()
{
    m_totalTime = 0;
    m_bitrate = 0;

    if(m_file)
        fclose(m_file);
    if(m_stream)
        stream_destroy(m_stream);
    if(m_alac)
        alac_destroy(m_alac);
}

bool AlacHelper::initialize()
{
    m_totalTime = 0;
    m_bitrate = 0;

    m_file = fopen(m_path.toLocal8Bit().constData(), "rb");
    if(m_file == nullptr)
    {
        return false;
    }

    m_stream = stream_create_file(m_file, 1);
    if(!qtmovie_read(m_stream, &m_demux))
    {
        //ALAC: alac_decode: failed to load the QuickTime movie headers!
        return false;
    }

    m_alac = create_alac(m_demux.sample_size, m_demux.num_channels);
    alac_set_info(m_alac, (char *)m_demux.codecdata);

    unsigned long output_time = 0;
    unsigned int index = 0;
    unsigned int accum = 0;
    for( unsigned int i = 0; i < m_demux.num_sample_byte_sizes; i++)
    {
        unsigned int thissample_duration;
        while((m_demux.time_to_sample[index].sample_count + accum) <= i)
        {
            accum += m_demux.time_to_sample[index].sample_count;
            index++;
            if(index >= m_demux.num_time_to_samples)
            {
                //ALAC: get_song_length: sample %i does not have a duration
                return false;
            }
        }
        thissample_duration = m_demux.time_to_sample[index].sample_duration;
        output_time += thissample_duration;
    }

    m_totalTime= (output_time / m_demux.sample_rate) * 1000;
    m_output_bytes = 4096 * 4;
    m_decoded_frames = 0;
    m_bitrate = (QFileInfo(m_path).size() * 8.0) / m_totalTime;

    return true;
}

qint64 AlacHelper::totalTime() const
{
    return m_totalTime;
}

void AlacHelper::seek(qint64 pos)
{
    stream_setpos(m_stream, pos);
}

int AlacHelper::bitrate() const
{
    return m_bitrate;
}

int AlacHelper::samplerate() const
{
    return m_demux.sample_rate;
}

int AlacHelper::channels() const
{
    return m_demux.num_channels;
}

int AlacHelper::read(unsigned char *data, int size)
{
    int copy_data;
    int current_size = size;
    while(current_size > 0)
    {
        if(m_decoded_frames == m_demux.num_sample_byte_sizes)
        {
            return -1;
        }

        stream_read(m_stream, m_demux.sample_byte_size[m_decoded_frames], m_stream_buffer);
        decode_frame(m_alac, m_stream_buffer, m_buffer, &m_output_bytes);
        m_decoded_frames = 0;

        if(current_size > m_output_bytes - m_decoded_frames)
        {
            copy_data = m_output_bytes - m_decoded_frames;
        }
        else
        {
            copy_data = current_size;
        }

        memcpy(data, m_buffer + m_decoded_frames, copy_data);
        data += copy_data;
        current_size -= copy_data;
        m_decoded_frames += copy_data;
    }

    if(current_size == -1)
    {
        return -1;
    }

    return size - current_size;
}
