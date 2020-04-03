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

#include "sc68helper.h"

void in_c68_meta_from_music_info(QVariantMap &data, sc68_music_info_t *ti, int trk)
{
    // add metainfo
    if(!ti->title || !ti->title[0])
    {
        // title is empty, this call will set track title to filename without extension
        data.insert("title", QString());
    }
    else
    {
        data.insert("title", ti->title);
    }

    if(ti->artist && ti->artist[0])
    {
        data.insert("artist", ti->artist);
    }

    if(ti->album && ti->album[0])
    {
        data.insert("album", ti->album);
    }

    if(ti->genre && ti->genre[0])
    {
        data.insert("genre", ti->genre);
    }

    if(ti->year && ti->year[0])
    {
        data.insert("year", ti->year);
    }

    if(ti->format && ti->format[0])
    {
        data.insert("SC68_FORMAT", ti->format);
    }

    if(ti->ripper && ti->ripper[0])
    {
        data.insert("SC68_RIPPER", ti->ripper);
    }

    if(ti->converter && ti->converter[0])
    {
        data.insert("SC68_CONVERTER", ti->converter);
    }

    data.insert("track", trk);
}

SC68Helper::SC68Helper(const QString &path)
{
    m_path = path;
    m_info = (sc68_info_t*)calloc(sizeof(sc68_info_t), 1);
    m_totalTime = 0;
}

SC68Helper::~SC68Helper()
{
    close();
}

void SC68Helper::close()
{
    if(m_info) 
    {
        if(m_info->sc68)
        {
            sc68_destroy(m_info->sc68);
        }
        free(m_info);
    }
}

bool SC68Helper::initialize()
{
    m_info->sc68 = sc68_create(nullptr);
    if(!m_info->sc68)
    {
        return false;
    }

    // Load an sc68 file.
    int res = sc68_load_uri(m_info->sc68, m_path.toLocal8Bit().constData());
    if(res)
    {
        return false;
    }

    m_info->trk = 0;

    sc68_music_info_t info;
    res = sc68_music_info(m_info->sc68, &info, m_info->trk + 1, 0);
    if(res < 0)
    {
        return false;
    }

    m_info->loop = info.trk.time_ms == 0;
    if(info.trk.time_ms > 0)
    {
        m_info->totalsamples = (uint64_t)info.trk.time_ms * samplerate() / 1000;
    }
    else
    {
        m_info->totalsamples = 2 * 60 * samplerate();
    }

    m_totalTime = m_info->totalsamples / samplerate();
    m_info->readpos = 0;

    sc68_play(m_info->sc68, m_info->trk + 1, m_info->loop);

    return true;
}

int SC68Helper::totalTime() const
{
    return m_totalTime;
}

int SC68Helper::bitrate() const
{
    return 2 * bitsPerSample() / 8;
}

int SC68Helper::samplerate() const
{
    return 44100;
}

int SC68Helper::channels() const
{
    return 2;
}

int SC68Helper::bitsPerSample() const
{
    return 16;
}

int SC68Helper::read(unsigned char *buf, int size)
{
    if(m_info->currentsample >= m_info->totalsamples)
    {
        return 0;
    }

    m_info->currentsample += size / (channels() * bitsPerSample() / 8);
    const int initsize = size;

    while(size > 0)
    {
        int n = size>>2;
        int res = sc68_process(m_info->sc68, buf, &n);
        if(res & SC68_END)
        {
            break;
        }
        size -= n<<2;
    }
    return initsize - size;
}

void SC68Helper::seek(qint64 time)
{
    const int sample = time * samplerate();
    if(sample < m_info->currentsample)
    {
        sc68_stop(m_info->sc68);
        sc68_play(m_info->sc68, m_info->trk + 1, m_info->loop);
        m_info->currentsample = 0;
    }

    char buffer[512 * 4];
    while(m_info->currentsample < sample)
    {
        int sz = (int)(sample - m_info->currentsample);
        sz = MIN(sz, sizeof(buffer)>>2);
        int res = sc68_process(m_info->sc68, buffer, &sz);
        if(res & SC68_END)
        {
            break;
        }
        m_info->currentsample += sz;
    }
    m_info->readpos = (float)m_info->currentsample / samplerate();
}

QVariantMap SC68Helper::readMetaTags()
{
    sc68_music_info_t info;
    int err = sc68_music_info(m_info->sc68, &info, 0, 0);
    if(err < 0)
    {
        return m_meta;
    }

    for(int tr = 0; tr < info.tracks; tr++)
    {
        sc68_music_info_t ti;
        memset(&ti, 0, sizeof(ti));
        err = sc68_music_info(m_info->sc68, &ti, tr + 1, 0);
        if(err < 0)
        {
            continue;
        }

        in_c68_meta_from_music_info(m_meta, &ti, tr);
    }

    return m_meta;
}
