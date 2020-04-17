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

#include "fc14helper.h"

FC14Helper::FC14Helper(const QString &path)
{
    m_path = path;
    m_info = (fc14_info_t*)calloc(sizeof(fc14_info_t), 1);
}

FC14Helper::~FC14Helper()
{
    close();
}

void FC14Helper::close()
{
    if(m_info)
    {
        if(m_info->fc)
        {
            fc14dec_delete(m_info->fc);
        }
        free(m_info);
    }
}

bool FC14Helper::initialize()
{
    m_info->fc = fc14dec_new();
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    int size = stdio_length(file);
    if(size <= 0 || size > 256 * 1024)
    {
        return false;
    }

    unsigned char *module = (unsigned char *)malloc(size);
    if(!module)
    {
        return false;
    }

    stdio_read(module, size, 1, file);
    stdio_close(file);

    if(!fc14dec_detect(m_info->fc, module, size))
    {
        free(module);
        return false;
    }

    if(!fc14dec_init(m_info->fc, module, size))
    {
        free(module);
        return false;
    }
    free(module);

    // Initialize decoder's audio sample mixer.  frequency : output sample frequency
    // precision : bits per sample  channels : 1=mono, 2=stereo
    // zero : value of silent output sample (e.g. 0x80 for unsigned 8-bit, 0x0000 for signed 16-bit)
    fc14dec_mixer_init(m_info->fc, 44100, 16, 2, 0x0000);

    m_info->length = fc14dec_duration(m_info->fc);
    m_info->channels = 2;
    m_info->sample_rate = 44100;
    m_info->bits_per_sample = 16;
    m_info->bitrate = size * 8.0 / m_info->length;

    return true;
}

int FC14Helper::totalTime() const
{
    return m_info->length;
}

void FC14Helper::seek(qint64 time)
{
    fc14dec_seek(m_info->fc, time);
}

int FC14Helper::bitrate() const
{
    return m_info->bitrate;
}

int FC14Helper::samplerate() const
{
    return m_info->sample_rate;
}

int FC14Helper::channels() const
{
    return m_info->channels;
}

int FC14Helper::bitsPerSample() const
{
    return m_info->bits_per_sample;
}

int FC14Helper::read(unsigned char *buf, int size)
{
    fc14dec_buffer_fill(m_info->fc, buf, size);
    if(fc14dec_song_end(m_info->fc))
    {
        return 0;
    }
    return size;
}

QVariantMap FC14Helper::readMetaTags()
{
    QVariantMap data;
    data.insert("comment", fc14dec_format_name(m_info->fc));
    return data;
}
