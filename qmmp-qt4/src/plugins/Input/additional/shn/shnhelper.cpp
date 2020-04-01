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

#include "shnhelper.h"

extern "C" {
#include "stdio_meta.h"
}

SHNHelper::SHNHelper(const QString &path)
{
    m_path = path;
    m_info = (shn_info_t*)calloc(sizeof(shn_info_t), 1);
}

SHNHelper::~SHNHelper()
{
    close();
}

void SHNHelper::close()
{
    shn_free_decoder(m_info);
    if(m_info->shnfile)
    {
        shn_unload(m_info->shnfile);
        m_info->shnfile = nullptr;
    }
    if(m_info->buffer)
    {
        free(m_info->buffer);
        m_info->buffer = nullptr;
    }
    if(m_info->offset)
    {
        free(m_info->offset);
        m_info->offset = nullptr;
    }
    if(m_info->maxnlpc > 0 && m_info->qlpc)
    {
        free(m_info->qlpc);
        m_info->qlpc = nullptr;
    }
    free(m_info);
}

bool SHNHelper::initialize()
{
    shn_init_config();

	char data[4];
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    int id3v2_tag_size = stdio_get_leading_size(file);
    if(id3v2_tag_size > 0) 
    {
        stdio_seek(file, id3v2_tag_size, SEEK_SET);
    }

	if(stdio_read((void *)data, 1, 4, file) != 4)
	{
		stdio_close(file);
		return false;
	}
	stdio_close(file);

	if(memcmp(data, MAGIC, 4))
	{
		return false;
    }

    if(!(m_info->shnfile = load_shn(m_path.toLocal8Bit().constData())))
	{
		return false;
    }

    m_info->startsample = 0;
    m_info->endsample = m_info->shnfile->wave_header.length * m_info->shnfile->wave_header.samples_per_sec - 1;
    m_info->readpos = 0;

    if(m_info->shnfile->wave_header.file_has_id3v2_tag) 
    {
        stdio_seek(m_info->shnfile->vars.fd, m_info->shnfile->wave_header.file_has_id3v2_tag, SEEK_SET);
    }
    else 
    {
        stdio_rewind(m_info->shnfile->vars.fd);
    }

    if(shn_init_decoder(m_info) < 0) 
    {
        return false;
    }
    
    return true;
}

int SHNHelper::totalTime() const
{
    return  m_info->shnfile->wave_header.length;
}

void SHNHelper::seek(qint64 pos)
{
    shn_seek_sample(m_info, pos * samplerate());
}

int SHNHelper::bitrate() const
{
    return totalTime() * 8 / 1000;
}

int SHNHelper::samplerate() const
{
    return m_info->shnfile->wave_header.samples_per_sec;
}

int SHNHelper::channels() const
{
    return m_info->shnfile->wave_header.channels;
}

int SHNHelper::bitsPerSample() const
{
    return m_info->shnfile->wave_header.bits_per_sample;
}

int SHNHelper::read(unsigned char *buf, int size)
{
    int samplesize = channels() * bitsPerSample() / 8;
    if(m_info->currentsample + size / samplesize > m_info->endsample) 
    {
        size = (m_info->endsample - m_info->currentsample + 1) * samplesize;
        if(size <= 0)
        {
            return 0;
        }
    }
    
    int initsize = size;
    while(size > 0)
    {
        if(m_info->shnfile->vars.bytes_in_buf > 0)
        {
            int n = size / samplesize;
            int nsamples = m_info->shnfile->vars.bytes_in_buf / samplesize;
            if(m_info->samples_to_skip > 0)
            {
                int nskip = MIN(nsamples, m_info->samples_to_skip);
                m_info->samples_to_skip -= nskip;
                if(nskip == nsamples) 
                {
                    m_info->shnfile->vars.bytes_in_buf = 0;
                    continue;
                }
                else 
                {
                    memmove(m_info->shnfile->vars.buffer, m_info->shnfile->vars.buffer + nskip * samplesize, m_info->shnfile->vars.bytes_in_buf - nskip * samplesize);
                    nsamples -= nskip;
                    continue;
                }
            }
            n = MIN(nsamples, n);
            char *src = (char *)m_info->shnfile->vars.buffer;
            memcpy(buf, src, samplesize * n);
            src += samplesize * n;
            buf += samplesize * n;
            size -= n * samplesize;
            
            if(n == m_info->shnfile->vars.bytes_in_buf / samplesize) 
            {
                m_info->shnfile->vars.bytes_in_buf = 0;
            }
            else 
            {
                memmove(m_info->shnfile->vars.buffer, src, m_info->shnfile->vars.bytes_in_buf - n * samplesize);
                m_info->shnfile->vars.bytes_in_buf -= n * samplesize;
            }
            continue;
        }
        
        if(shn_decode(m_info) <= 0) 
        {
            break;
        }
    }

    m_info->currentsample += (initsize-size) / samplesize;
    return initsize - size;
}

QVariantMap SHNHelper::readTags(stdio_meta_type stdio_meta)
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
