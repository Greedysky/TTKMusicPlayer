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

#include "v2mhelper.h"
#include "v2mconv.h"
#include "sounddef.h"
#include "libv2.h"

extern "C" {
#include "stdio_file.h"
}

bool v2m_initialized = false;
int load_and_convert(const char *fname, uint8_t **conv, int *convlen)
{
    FILE *fp = stdio_open(fname);
    if(!fp)
    {
        return -1;
    }
    // probe
    int len = (int)stdio_length(fp);
    unsigned char *buf = (unsigned char *)malloc(len);
    int rb = (int)stdio_read(buf, 1, len, fp);
    stdio_close(fp);

    if(rb != len)
    {
        free(buf);
        return -1;
    }

    if(!v2m_initialized)
    {
        sdInit();
        v2m_initialized = true;
    }

    int ver = CheckV2MVersion(buf, len);
    if(ver < 0)
    {
        free(buf);
        return -1;
    }

    ConvertV2M(buf, len, conv, convlen);
    free(buf);

    return 0;
}

int get_total_samples(V2MPlayer *player)
{
    int totalsamples = 0;
    float buffer[2048 * 2];
    bool had_nonsilence = false;

    while(true)
    {
        player->Render(buffer, 2048);
        bool eof = true;
        for(int i = 0; i < 2048*2; i++)
        {
            float v = fabs(buffer[i]);
            if(v > 0.0000001f)
            {
                eof = false;
                if(totalsamples > 44100*2)
                {
                    had_nonsilence = true;
                }
                break;
            }
        }

        if(eof && had_nonsilence)
        {
            break;
        }
        totalsamples += 2048;
    }
    return totalsamples;
}


V2MHelper::V2MHelper(const QString &path)
{
    m_path = path;
    m_info = (v2m_info_t*)calloc(sizeof(v2m_info_t), 1);
    m_totalTime = 0;
}

V2MHelper::~V2MHelper()
{
    close();
}

void V2MHelper::close()
{
    if(m_info) 
    {
        if(m_info->tune)
        {
            free(m_info->tune);
        }

        if(m_info->player)
        {
            m_info->player->Close();
            delete m_info->player;
        }
        free(m_info);
    }
}

bool V2MHelper::initialize()
{
    if(load_and_convert(m_path.toLocal8Bit().constData(), &m_info->tune, &m_info->len) < 0)
    {
        return false;
    }

    m_info->player = new V2MPlayer;
    m_info->player->Init();
    m_info->player->Open(m_info->tune);

    int totalsamples = get_total_samples(m_info->player);
    m_totalTime = totalsamples / 44100.f;
    m_info->readpos = 0;

    m_info->player->Play();
    return true;
}

int V2MHelper::totalTime() const
{
    return m_totalTime;
}

int V2MHelper::bitrate() const
{
    return 8;
}

int V2MHelper::samplerate() const
{
    return 44100;
}

int V2MHelper::channels() const
{
    return 2;
}

int V2MHelper::bitsPerSample() const
{
    return 32;
}

int V2MHelper::read(unsigned char *buf, int size)
{
    int samplesize = (bitsPerSample() >> 3) * channels();
    int samples = size / samplesize;

    if(m_info->currsample > m_info->totalsamples)
    {
        return 0;
    }

    m_info->player->Render((float*)buf, samples);
    m_info->readpos += samples / (float)samplerate();
    m_info->currsample += samples;

    return size;
}

void V2MHelper::seek(qint64 time)
{
    const int sample = time * samplerate();
    if(sample >= m_info->totalsamples)
    {
        return; // seek beyond eof
    }

    if(sample < m_info->currsample)
    {
        m_info->player->Play(0);
        m_info->currsample = 0;
        m_info->readpos = 0;
    }

    float buffer[2048 * channels()];
    while(m_info->currsample < sample)
    {
        int samples = sample - m_info->currsample;
        if(samples > 2048)
        {
            samples = 2048;
        }
        m_info->player->Render(buffer, samples);
        m_info->currsample += samples;
    }
    m_info->readpos = sample / (float)samplerate();
}
