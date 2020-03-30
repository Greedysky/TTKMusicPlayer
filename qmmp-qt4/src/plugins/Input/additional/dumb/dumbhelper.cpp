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

#include "dumbhelper.h"

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
    "mod", "mdz", "stk", "m15", "fst", "oct",
    "s3m", "s3z",
    "stm", "stz",
    "it", "itz",
    "xm", "xmz",
    "ptm", "ptz",
    "mtm", "mtz",
    "669",
    "psm",
    "am", "j2b",
    "dsm",
    "amf",
    "okt", "okta",
    nullptr
};

const char **dumb_exts()
{
    return exts;
}

int is_mod_ext(const char *ext)
{
    for(int i = 0; exts[i] && i < MOD_EXT_COUNT; i++)
    {
        if(!strcasecmp(ext, exts[i]))
        {
            return 1;
        }
    }
    return 0;
}

DUH *open_module(const char *fname, const char *ext, int *start_order, int *is_it, int *is_dos, const char** filetype)
{
    *filetype = nullptr;
    DUH * duh = nullptr;

    *is_it = 0;
    *is_dos = 1;

    uint8_t ptr[2000];
    FILE *fp = stdio_open(fname);
    if(!fp)
    {
        return nullptr;
    }

    char path[256] = {0};
    memcpy(path, fname, strlen(fname));
    int size = stdio_read(ptr, 1, 2000, fp);
    stdio_close(fp);

    fname = path;
    DUMBFILE *f = dumbfile_open(fname);
    if(!f)
    {
        return nullptr;
    }

    if(size >= 4 && ptr[0] == 'I' && ptr[1] == 'M' && ptr[2] == 'P' && ptr[3] == 'M')
    {
        *is_it = 1;
        duh = dumb_read_it_quick(f);
        *filetype = "IT";
    }
    else if(size >= 17 && !memcmp(ptr, "Extended Module: ", 17))
    {
        duh = dumb_read_xm_quick(f);
        *filetype = "XM";
    }
    else if(size >= 0x30 && ptr[0x2C] == 'S' && ptr[0x2D] == 'C' && ptr[0x2E] == 'R' && ptr[0x2F] == 'M')
    {
        duh = dumb_read_s3m_quick(f);
        *filetype = "S3M";
    }
    else if(size >= 1168 && /*ptr[28] == 0x1A &&*/ ptr[29] == 2 &&
        (!strncasecmp((const char*)ptr + 20, "!Scream!", 8) ||
         !strncasecmp((const char*)ptr + 20, "BMOD2STM", 8) ||
         !strncasecmp((const char*)ptr + 20, "WUZAMOD!", 8)))
    {
        duh = dumb_read_stm_quick(f);
        *filetype = "STM";
    }
    else if(size >= 2 && ((ptr[0] == 0x69 && ptr[1] == 0x66) || (ptr[0] == 0x4A && ptr[1] == 0x4E)))
    {
        duh = dumb_read_669_quick(f);
        *filetype = "669";
    }
    else if(size >= 0x30 && ptr[0x2C] == 'P' && ptr[0x2D] == 'T' && ptr[0x2E] == 'M' && ptr[0x2F] == 'F')
    {
        duh = dumb_read_ptm_quick(f);
        *filetype = "PTM";
    }
    else if(size >= 4 && ptr[0] == 'P' && ptr[1] == 'S' && ptr[2] == 'M' && ptr[3] == ' ')
    {
        duh = dumb_read_psm_quick(f, *start_order);
        *start_order = 0;
        *filetype = "PSM";
    }
    else if(size >= 4 && ptr[0] == 'P' && ptr[1] == 'S' && ptr[2] == 'M' && ptr[3] == 254)
    {
        duh = dumb_read_old_psm_quick(f);
        *filetype = "PSM";
    }
    else if(size >= 3 && ptr[0] == 'M' && ptr[1] == 'T' && ptr[2] == 'M')
    {
        duh = dumb_read_mtm_quick(f);
        *filetype = "MTM";
    }
    else if( size >= 4 && ptr[0] == 'R' && ptr[1] == 'I' && ptr[2] == 'F' && ptr[3] == 'F')
    {
        duh = dumb_read_riff_quick(f);
        *filetype = "RIFF";
    }
    else if(size >= 32 && !memcmp(ptr, "ASYLUM Music Format", 19 ) && !memcmp(ptr + 19, " V1.0", 5))
    {
        duh = dumb_read_asy_quick(f);
        *filetype = "ASY";
    }
    else if(size >= 3 && ptr[0] == 'A' && ptr[1] == 'M' && ptr[2] == 'F')
    {
        duh = dumb_read_amf_quick(f);
        *filetype = "AMF";
    }
    else if(size >= 8 && !memcmp(ptr, "OKTASONG", 8))
    {
        duh = dumb_read_okt_quick(f);
        *filetype = "OKT";
    }

    if(!duh)
    {
        dumbfile_close(f);
        f = dumbfile_open(fname);
        *is_dos = 0;
        duh = dumb_read_mod_quick(f, is_mod_ext(ext) ? 0 : 1);
        *filetype = "MOD";
    }

    if(f)
    {
        dumbfile_close(f);
    }

    return duh;
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
    m_info = (dumb_info_t*)malloc(sizeof(dumb_info_t));
    m_totalTime = 0;

    memset(m_info, 0, sizeof(dumb_info_t));
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
    {
        dumb_register_stdfiles();
        const char *uri = m_path.toLocal8Bit().constData();
        const char *ext = uri + strlen(uri) - 1;
        while (*ext != '.' && ext > uri)
        {
            ext--;
        }
        ext++;

        int start_order = 0;
        int is_dos, is_it;
        const char *ftype;
        m_info->duh = open_module(uri, ext, &start_order, &is_it, &is_dos, &ftype);
    }

    if(!m_info->duh)
    {
        return false;
    }

    dumb_it_do_initial_runthrough(m_info->duh);

    m_info->bits_per_sample = conf_bps;
    m_info->channels = 2;
    m_info->samplerate = conf_samplerate;
    m_info->readpos = 0;

    if(cdumb_startrenderer(m_info) < 0)
    {
        return false;
    }

    m_totalTime = duh_get_length(m_info->duh) / 65536.0f;

    return true;
}

int DumbHelper::totalTime() const
{
    return m_totalTime;
}

void DumbHelper::seek(qint64 time)
{
    if(time < m_info->readpos)
    {
        if(cdumb_startrenderer(m_info) < 0)
        {
            return;
        }
    }
    else
    {
        time -= m_info->readpos;
    }

    int pos = time * samplerate();
    duh_sigrenderer_generate_samples(m_info->renderer, 0, 65536.0f / samplerate(), pos, nullptr);
    m_info->readpos = duh_sigrenderer_get_position(m_info->renderer) / 65536.f;
}

int DumbHelper::bitrate() const
{
    return 8;
}

int DumbHelper::samplerate() const
{
    return m_info->samplerate;
}

int DumbHelper::channels() const
{
    return m_info->channels;
}

int DumbHelper::bitsPerSample() const
{
    return m_info->bits_per_sample;
}

int DumbHelper::read(unsigned char *buf, int size)
{
    int samplesize = (bitsPerSample() >> 3) * channels();
    int length = size / samplesize;

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

QVariantMap DumbHelper::readTags()
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
