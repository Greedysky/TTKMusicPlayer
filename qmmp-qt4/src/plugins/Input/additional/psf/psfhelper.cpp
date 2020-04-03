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

#include "psfhelper.h"

extern "C" {
#include "ao.h"
#include "stdio_meta.h"
}

void psfplug_add_meta(QVariantMap &data, const char *key, const char *value)
{
    char tmp[200];
    // check utf8
    if(stdio_iconv(value, strlen(value), tmp, sizeof(tmp), "utf-8", "utf-8") >= 0)
    {
        if(key)
        {
            data.insert(key, value);
        }

    }
    // check shift-jis
    if(stdio_iconv(value, strlen(value), tmp, sizeof(tmp), "SHIFT-JIS", "utf-8") >= 0)
    {
        if(key)
        {
            data.insert(key, value);
        }
    }
}


PSFHelper::PSFHelper(const QString &path)
{
    m_path = path;
    m_info = (psf_info_t*)calloc(sizeof(psf_info_t), 1);
}

PSFHelper::~PSFHelper()
{
    close();
}

void PSFHelper::close()
{
    if(m_info) 
    {
        if(m_info->type >= 0)
        {
            ao_stop(m_info->type, m_info->decoder);
        }
        if(m_info->filebuffer)
        {
            free(m_info->filebuffer);
            m_info->filebuffer = nullptr;
        }
        free(m_info);
    }
}

bool PSFHelper::initialize()
{
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    m_info->filesize = stdio_length(file);
    m_info->filebuffer = (char *)malloc(m_info->filesize);
    if(!m_info->filebuffer)
    {
        stdio_close(file);
        return false;
    }

    if(stdio_read(m_info->filebuffer, 1, m_info->filesize, file) != m_info->filesize)
    {
        stdio_close(file);
        return false;
    }
    stdio_close(file);

    m_info->type = ao_identify(m_info->filebuffer);
    if(m_info->type < 0)
    {
        return false;
    }

    m_info->decoder = ao_start(m_info->type, m_path.toLocal8Bit().constData(), (uint8 *)m_info->filebuffer, m_info->filesize);
    if(!m_info->decoder)
    {
        return false;
    }

    ao_display_info info;
    memset(&info, 0, sizeof(info));
    bool have_info = false;
    if(ao_get_info(m_info->type, m_info->decoder, &info) == AO_SUCCESS)
    {
       have_info = true;
    }

    m_info->duration = 120;
    if(!have_info)
    {
        return true;
    }

    for(int i = 1; i < 9; i++)
    {
        if(!strncasecmp(info.title[i], "Length: ", 8))
        {
            int min;
            float sec;
            if(sscanf(info.info[i], "%d:%f", &min, &sec) == 2)
            {
                m_info->duration = min * 60 + sec;
            }
            else if(sscanf(info.info[i], "%f", &sec) == 1)
            {
                m_info->duration = sec;
            }
            break;
        }
    }

    return true;
}

int PSFHelper::totalTime() const
{
    return m_info->duration;
}

int PSFHelper::bitrate() const
{
    return 2 * bitsPerSample() / 8;
}

int PSFHelper::samplerate() const
{
    return 44100;
}

int PSFHelper::channels() const
{
    return 2;
}

int PSFHelper::bitsPerSample() const
{
    return 16;
}

int PSFHelper::read(unsigned char *buf, int size)
{
    if(m_info->currentsample >= m_info->duration * samplerate())
    {
        return 0;
    }

    const int initsize = size;
    while(size > 0)
    {
        if(m_info->remaining > 0)
        {
            if(m_info->samples_to_skip > 0)
            {
                int n = MIN(m_info->samples_to_skip, m_info->remaining);
                if(m_info->remaining > n)
                {
                    memmove(m_info->buffer, m_info->buffer + n * 4, (m_info->remaining - n) * 4);
                }
                m_info->remaining -= n;
                m_info->samples_to_skip -= n;
                continue;
            }

            int n = size / 4;
            n = MIN(m_info->remaining, n);
            memcpy(buf, m_info->buffer, n * 4);
            if(m_info->remaining > n)
            {
                memmove(m_info->buffer, m_info->buffer + n * 4, (m_info->remaining - n) * 4);
            }
            m_info->remaining -= n;
            buf += n*4;
            size -= n*4;
        }

        if(!m_info->remaining)
        {
            ao_decode(m_info->type, m_info->decoder, (int16_t *)m_info->buffer, 735);
            m_info->remaining = 735;
        }
    }

    m_info->currentsample += (initsize - size) / (channels() * bitsPerSample() / 8);
    m_info->readpos = (float)m_info->currentsample / samplerate();
    return initsize - size;
}

void PSFHelper::seek(qint64 time)
{
    const int sample = time * samplerate();
    if(sample > m_info->currentsample)
    {
        m_info->samples_to_skip = sample - m_info->currentsample;
    }
    else
    {
        // restart song
        ao_command(m_info->type, m_info->decoder, COMMAND_RESTART, 0);
        m_info->samples_to_skip = sample;
    }

    m_info->currentsample = sample;
    m_info->readpos = (float)sample / samplerate();
}

QVariantMap PSFHelper::readMetaTags()
{
    if(m_info->type < 0 || !m_info->decoder)
    {
        return m_meta;
    }

    ao_display_info info;
    memset(&info, 0, sizeof(info));
    bool have_info = false;
    if(ao_get_info(m_info->type, m_info->decoder, &info) == AO_SUCCESS)
    {
       have_info = true;
    }

    if(have_info)
    {
        return m_meta;
    }

    for(int i = 1; i < 9; i++)
    {
        if(!strncasecmp(info.title[i], "Name: ", 6) || !strncasecmp(info.title[i], "Song: ", 6))
        {
            psfplug_add_meta(m_meta, "title", info.info[i]);
        }
        else if (!strncasecmp(info.title[i], "Game: ", 6))
        {
            psfplug_add_meta(m_meta, "album", info.info[i]);
        }
        else if (!strncasecmp(info.title[i], "Artist: ", 8))
        {
            psfplug_add_meta(m_meta, "artist", info.info[i]);
        }
        else if (!strncasecmp(info.title[i], "Copyright: ", 11))
        {
            psfplug_add_meta(m_meta, "copyright", info.info[i]);
        }
        else if (!strncasecmp(info.title[i], "Year: ", 6))
        {
            psfplug_add_meta(m_meta, "year", info.info[i]);
        }
        else if (!strncasecmp(info.title[i], "Fade: ", 6))
        {
            psfplug_add_meta(m_meta, "fade", info.info[i]);
        }
        else
        {
            char *colon = strchr (info.title[i], ':');
            char name[colon - info.title[i] + 1];
            memcpy(name, info.title[i], colon - info.title[i]);
            name[colon - info.title[i]] = 0;
            psfplug_add_meta(m_meta, name, info.info[i]);
        }
    }

    return m_meta;
}
