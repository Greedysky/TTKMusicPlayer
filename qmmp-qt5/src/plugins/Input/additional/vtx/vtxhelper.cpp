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

#include "vtxhelper.h"

int ayemu_vtx_get_next_frame(vtx_info_t *info)
{
    int numframes = info->decoder->regdata_size / AY_FRAME_SIZE;
    if(info->vtx_pos++ >= numframes)
    {
        return 0;
    }

    unsigned char *p = info->decoder->regdata + info->vtx_pos;
    for(int n = 0 ; n < AY_FRAME_SIZE ; n++, p += numframes) 
    {
        info->regs[n] = *p;
    }
    return 1;
}

VTXHelper::VTXHelper(const QString &path)
{
    m_path = path;
    m_info = (vtx_info_t*)calloc(sizeof(vtx_info_t), 1);
    m_totalTime = 0;
}

VTXHelper::~VTXHelper()
{
    close();
}

void VTXHelper::close()
{
    if(m_info) 
    {
        if(m_info->decoder)
         {
            ayemu_vtx_free(m_info->decoder);
            m_info->decoder = nullptr;
        }
        ayemu_reset(&m_info->ay);
        free(m_info);
    }
}

bool VTXHelper::initialize()
{
    ayemu_init(&m_info->ay);
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    const size_t sz = stdio_length(file);
    if(sz <= 0)
    {
        stdio_close(file);
        return false;
    }

    char *buf = (char *)malloc(sz);
    if(!buf)
    {
        stdio_close(file);
        return false;
    }

    if(stdio_read(buf, 1, sz, file) != sz)
    {
        free(buf);
        stdio_close(file);
        return false;
    }
    
    stdio_seek(file, 0, SEEK_SET);

    char header_buf[0x4000] = {0};
    size_t header_sz = stdio_read(header_buf, 1, sizeof(header_buf), file);
    ayemu_vtx_t *hdr = ayemu_vtx_header(header_buf, header_sz);
    if(!hdr)
    {
        free(buf);
        stdio_close(file);
        return false;
    }

    m_totalTime = hdr->regdata_size * 1.0 / AY_FRAME_SIZE / hdr->playerFreq;

    m_meta.insert("title", hdr->title);
    m_meta.insert("artist", hdr->author);
    m_meta.insert("album", hdr->from);
    m_meta.insert("tracker", hdr->tracker);
    m_meta.insert("comment", hdr->comment);

    ayemu_vtx_free(hdr);
    stdio_close(file);

    m_info->decoder = ayemu_vtx_load(buf, sz);
    if(!m_info->decoder)
    {
        free(buf);
        return false;
    }
    free(buf);

    ayemu_set_chip_type(&m_info->ay, m_info->decoder->chiptype, nullptr);
    ayemu_set_chip_freq(&m_info->ay, m_info->decoder->chipFreq);
    ayemu_set_stereo(&m_info->ay, (ayemu_stereo_t)m_info->decoder->stereo, nullptr);

    m_info->left = 0;
    m_info->vtx_pos = 0;
    m_info->readpos = 0;

    ayemu_set_sound_format(&m_info->ay, samplerate(), channels(), bitsPerSample());

    m_info->rate = 2 * bitsPerSample() / 8;
    return true;
}

int VTXHelper::totalTime() const
{
    return m_totalTime;
}

void VTXHelper::seek(qint64 time)
{
    const int sample = time * samplerate();
    // get frame
    int num_frames = m_info->decoder->regdata_size / AY_FRAME_SIZE;
    int samples_per_frame = samplerate() / m_info->decoder->playerFreq;
    // start of frame
    m_info->vtx_pos = sample / samples_per_frame;
    if(m_info->vtx_pos >= num_frames)
    {
        return; // eof
    }
    // copy register data
    int n;
    unsigned char *p = m_info->decoder->regdata + m_info->vtx_pos;
    for(n = 0 ; n < AY_FRAME_SIZE ; n++, p += num_frames)
    {
        m_info->regs[n] = *p;
    }
    // set number of bytes left in frame
    m_info->left = samplerate() / m_info->decoder->playerFreq - (sample % samples_per_frame);
    // mul by rate to get number of bytes
    m_info->left *= m_info->rate;
    m_info->currentsample = sample;
    m_info->readpos = (float)m_info->currentsample / samplerate();
}

int VTXHelper::bitrate() const
{
    return m_info->rate;
}

int VTXHelper::samplerate() const
{
    return 44100;
}

int VTXHelper::channels() const
{
    return 2;
}

int VTXHelper::bitsPerSample() const
{
    return 16;
}

int VTXHelper::read(unsigned char *buf, int size)
{
    const int initsize = size;
    int donow = 0;

    while(size > 0) 
    {
        if(m_info->left > 0)
        {
            donow = (size > m_info->left) ? m_info->left : size;
            m_info->left -= donow;
            buf = (unsigned char *)ayemu_gen_sound(&m_info->ay, (char *)buf, donow);
            size -= donow;
        }
        else 
        {
            if((ayemu_vtx_get_next_frame(m_info) == 0))
            {
                break; // eof
            }
            else 
            {
                // number of samples it current frame
                m_info->left = samplerate() / m_info->decoder->playerFreq;
                // mul by rate to get number of bytes;
                m_info->left *= m_info->rate;
                ayemu_set_regs(&m_info->ay, m_info->regs);
                donow = 0;
            }
        }
    }
    m_info->currentsample += (initsize - size) / 4;
    m_info->readpos = (float)m_info->currentsample / samplerate();
    return initsize - size;
}

QVariantMap VTXHelper::readMetaTags()
{
    return m_meta;
}
