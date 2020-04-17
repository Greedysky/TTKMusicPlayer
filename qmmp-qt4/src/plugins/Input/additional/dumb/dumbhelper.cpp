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

#include "dumbhelper.h"
#include "modloader.h"

extern "C" {
#include "stdio_meta.h"
}

const int conf_bps = 16;
const int conf_samplerate = 44100;
const int conf_resampling_quality = 4;
const int conf_ramping_style = 2;
const int conf_global_volume = 64;

const int MOD_EXT_COUNT = 6;

const char * exts[]=
{
    "mod", "mdz", "stk", "m15", "fst", "oct", "nt"
    "s3m", "s3z",
    "stm", "stz",
    "it", "itz",
    "xm", "xmz",
    "ptm", "ptz",
    "mtm", "mtz",
    "669",
    "psm",
    "umx",
    "am", "j2b",
    "dsm",
    "amf",
    "okt", "okta",
    "mo3",
    nullptr
};

const char **dumb_exts()
{
    return exts;
}

int cdumb_startrenderer(dumb_info_t *info)
{
    // reopen
    if(info->renderer)
    {
        duh_end_sigrenderer(info->renderer);
        info->renderer = nullptr;
    }

    info->renderer = duh_start_sigrenderer(info->duh, 0, 2, 0);
    if(!info->renderer)
    {
        return -1;
    }

    DUMB_IT_SIGRENDERER *itsr = duh_get_it_sigrenderer(info->renderer);
    dumb_it_set_loop_callback(itsr, &dumb_it_callback_terminate, nullptr);

    int q = conf_resampling_quality;
    if(q < 0)
    {
        q = 0;
    }
    else if(q >= DUMB_RQ_N_LEVELS)
    {
        q = DUMB_RQ_N_LEVELS - 1;
    }

    dumb_it_set_resampling_quality(itsr, q);
    dumb_it_set_xm_speed_zero_callback(itsr, &dumb_it_callback_terminate, nullptr);
    dumb_it_set_global_volume_zero_callback(itsr, &dumb_it_callback_terminate, nullptr);

    int rq = conf_ramping_style;
    if(rq < 0)
    {
        rq = 0;
    }
    else if(rq > 2)
    {
        rq = 2;
    }

    dumb_it_set_ramp_style(itsr, rq);
    dumb_it_sr_set_global_volume(itsr, conf_global_volume);
    return 0;
}

const char *convstr(const char* str, int sz, char *out, int out_sz)
{
    int i;
    for(i = 0; i < sz; i++)
    {
        if(str[i] != ' ')
        {
            break;
        }
    }

    if(i == sz)
    {
        out[0] = 0;
        return out;
    }

    const char *cs = stdio_detect_charset(str);
    if(!cs)
    {
        return str;
    }
    else
    {
        if(stdio_iconv(str, sz, out, out_sz, cs, "utf-8") >= 0)
        {
            return out;
        }
    }
    return nullptr;
}


DumbHelper::DumbHelper(const QString &url)
{
    m_path = url;
    m_info = (dumb_info_t*)calloc(sizeof(dumb_info_t), 1);
    m_totalTime = 0;
}

DumbHelper::~DumbHelper()
{
    close();
}

void DumbHelper::close()
{
    if(m_info)
    {
        if(m_info->renderer)
        {
            duh_end_sigrenderer(m_info->renderer);
            m_info->renderer = nullptr;
        }
        if(m_info->duh)
        {
            unload_duh(m_info->duh);
            m_info->duh = nullptr;
        }
        free(m_info);
    }
}

bool DumbHelper::initialize()
{
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    const int64_t size = stdio_length(file);
    stdio_close(file);

    dumb_register_stdfiles();
    const char *uri = m_path.toLocal8Bit().constData();
    const char *ext = uri + strlen(uri) - 1;
    while (*ext != '.' && ext > uri)
    {
        ext--;
    }
    ext++;

    int is_dos, is_it, is_ptcompat;
    const char *ftype;
    m_info->duh = open_module(uri, &is_it, &is_dos, &is_ptcompat, 0, &ftype);

    if(!m_info->duh)
    {
        return false;
    }

    dumb_it_do_initial_runthrough(m_info->duh);

    m_info->readpos = 0;
    m_totalTime = duh_get_length(m_info->duh) / 65536.0f * 1000;
    m_info->bitrate = size * 8.0 / m_totalTime;

    if(cdumb_startrenderer(m_info) < 0)
    {
        return false;
    }

    return true;
}

int DumbHelper::totalTime() const
{
    return m_totalTime;
}

void DumbHelper::seek(qint64 time)
{
    float skiptime = time;
    if(skiptime < m_info->readpos)
    {
        if(cdumb_startrenderer(m_info) < 0)
        {
            return;
        }
    }
    else
    {
        skiptime -= m_info->readpos;
    }

    int pos = skiptime * samplerate();
    duh_sigrenderer_generate_samples(m_info->renderer, 0, 65536.0f / samplerate(), pos, nullptr);
    m_info->readpos = time;
}

int DumbHelper::bitrate() const
{
    return m_info->bitrate;
}

int DumbHelper::samplerate() const
{
    return conf_samplerate;
}

int DumbHelper::channels() const
{
    return 2;
}

int DumbHelper::bitsPerSample() const
{
    return conf_bps;
}

int DumbHelper::read(unsigned char *buf, int size)
{
    const int samplesize = (bitsPerSample() >> 3) * channels();
    const int length = size / samplesize;

    DUMB_IT_SIGRENDERER *itsr = duh_get_it_sigrenderer(m_info->renderer);
    if(m_info->can_loop)
    {
        dumb_it_set_loop_callback(itsr, &dumb_it_callback_loop_forever, nullptr);
    }
    else
    {
        dumb_it_set_loop_callback(itsr, &dumb_it_callback_terminate, nullptr);
    }

    long ret = duh_render(m_info->renderer, bitsPerSample(), 0, 1, 65536.f / samplerate(), length, buf);
    m_info->readpos += ret / (float)samplerate();

    return ret * samplesize;
}

QVariantMap DumbHelper::readMetaTags()
{
    QVariantMap data;
    if(!m_info->duh)
    {
        return data;
    }

    char temp[2048];
    DUMB_IT_SIGDATA * itsd = duh_get_it_sigdata(m_info->duh);
    if(itsd->name[0])
    {
        int tl = sizeof(itsd->name);
        int i;
        for(i = 0; i < tl && itsd->name[i] && itsd->name[i] == ' '; i++);

        if(i == tl || !itsd->name[i])
        {
            data.insert("title", QString());
        }
        else
        {
            data.insert("title", convstr((char*)&itsd->name, sizeof(itsd->name), temp, sizeof(temp)));
        }
    }
    else
    {
        data.insert("title", QString());
    }

    for(int i = 0; i < itsd->n_instruments; i++)
    {
        char key[100];
        snprintf(key, sizeof (key), "INST%03d", i);
        data.insert(key, convstr((char *)&itsd->instrument[i].name, sizeof(itsd->instrument[i].name), temp, sizeof(temp)));
    }

    for(int i = 0; i < itsd->n_samples; i++)
    {
        char key[100];
        snprintf(key, sizeof(key), "SAMP%03d", i);
        data.insert(key, convstr((char *)&itsd->sample[i].name, sizeof(itsd->sample[i].name), temp, sizeof(temp)));
    }

    char s[100];
    snprintf(s, sizeof(s), "%d", itsd->n_orders);
    data.insert(":MOD_ORDERS", s);
    snprintf(s, sizeof(s), "%d", itsd->n_instruments);
    data.insert(":MOD_INSTRUMENTS", s);
    snprintf(s, sizeof(s), "%d", itsd->n_samples);
    data.insert(":MOD_SAMPLES", s);
    snprintf(s, sizeof(s), "%d", itsd->n_patterns);
    data.insert(":MOD_PATTERNS", s);
    snprintf(s, sizeof(s), "%d", itsd->n_pchannels);
    data.insert(":MOD_CHANNELS", s);

    return data;
}
