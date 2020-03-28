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

#include "ttahelper.h"

extern "C" {
#include "stdio_meta.h"
}

#define MAX_BSIZE (MAX_BPS>>3)

TTAHelper::TTAHelper(const QString &url)
{
    m_path = url;
    m_tta = (tta_info*)malloc(sizeof(tta_info));
    m_currentsample = 0;
    m_startsample = 0;
    m_endsample = 0;
    m_buffer = (char*)malloc(sizeof(char) * PCM_BUFFER_LENGTH * MAX_BSIZE * MAX_NCH);
    m_remaining = 0;
    m_samples_to_skip = 0;
    m_readpos= 0;

    memset(m_tta, 0, sizeof(tta_info));
}

TTAHelper::~TTAHelper()
{
    close();
}

void TTAHelper::close()
{
    free(m_buffer);
    player_stop(m_tta);
    close_tta_file(m_tta);
    free(m_tta);
}

bool TTAHelper::initialize()
{
    if(open_tta_file(m_path.toLocal8Bit().constData(), m_tta, 0) != 0)
    {
        return false;
    }

    if(player_init(m_tta) != 0)
    {
        return false;
    }

    m_startsample = 0;
    m_endsample = m_tta->DATALENGTH - 1;
    m_readpos = 0;

    return true;
}

int TTAHelper::totalTime() const
{
    return m_tta->LENGTH;
}

void TTAHelper::seek(qint64 time)
{
    const int sample = time * samplerate();
    m_samples_to_skip = set_position(m_tta, sample + m_startsample);
    if(m_samples_to_skip < 0)
    {
        return;
    }

    m_currentsample = sample + m_startsample;
    m_remaining = 0;
    m_readpos = sample / samplerate();
}

int TTAHelper::bitrate() const
{
    return m_tta->BITRATE;
}

int TTAHelper::samplerate() const
{
    return m_tta->SAMPLERATE;
}

int TTAHelper::channels() const
{
    return m_tta->NCH;
}

int TTAHelper::bitsPerSample() const
{
    return m_tta->BPS;
}

int TTAHelper::read(unsigned char *buf, int size)
{
    int samplesize = channels() * bitsPerSample() / 8;
    if(m_currentsample + size / samplesize > m_endsample)
    {
        size = (m_endsample - m_currentsample + 1) * samplesize;
        if(size <= 0)
        {
            return 0;
        }
    }

    int initsize = size;
    while(size > 0)
    {
        if(m_samples_to_skip > 0 && m_remaining > 0)
        {
            int skip = MIN(m_remaining, m_samples_to_skip);
            if(skip < m_remaining)
            {
                memmove(m_buffer, m_buffer + skip * samplesize, (m_remaining - skip) * samplesize);
            }
            m_remaining -= skip;
            m_samples_to_skip -= skip;
        }

        if(m_remaining > 0)
        {
            int n = size / samplesize;
            n = MIN(n, m_remaining);
            int nn = n;
            char *p = m_buffer;

            memcpy(buf, p, n * samplesize);
            buf += n * samplesize;
            size -= n * samplesize;
            p += n * samplesize;

            if(m_remaining > nn)
            {
                memmove(m_buffer, p, (m_remaining - nn) * samplesize);
            }
            m_remaining -= nn;
        }

        if(size > 0 && !m_remaining)
        {
            m_remaining = get_samples(m_tta, m_buffer);
            if(m_remaining <= 0)
            {
                break;
            }
        }
    }

    m_currentsample += (initsize - size) / samplesize;
    return initsize - size;
}

QVariantMap TTAHelper::readTags(stdio_meta_type stdio_meta)
{
    FILE *file = fopen(m_path.toLocal8Bit().constData(), "rb");
    if(file == nullptr)
    {
        return QVariantMap();
    }

    metaInfo_t *item = (metaInfo_t*)malloc(sizeof(metaInfo_t));
    switch(stdio_meta)
    {
    case 0: stdio_apev2_read(item, nullptr, file); break;
    case 1: stdio_id3v2_read(item, nullptr, file); break;
    case 2: stdio_id3v1_read(item, file); break;
    default: break;
    }

    fclose(file);

    QVariantMap data;
    if(item)
    {
        do
        {
            data.insert(item->key, item->value);
        } while(item->next);
        free(item);
    }

    return data;
}
