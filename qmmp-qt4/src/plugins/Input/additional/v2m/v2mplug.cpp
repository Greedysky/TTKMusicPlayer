#include "../../deadbeef.h"

#include "v2/v2mplayer.h"
#include "v2/libv2.h"
#include "v2/v2mconv.h"
#include "v2/sounddef.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

DB_functions_t *deadbeef;

static DB_decoder_t v2m_plugin;

static bool _v2m_initialized = false;

typedef struct {
    DB_fileinfo_t info;
    uint8_t *tune;
    int len;
    V2MPlayer *player;
    int currsample;
    int totalsamples;
} v2m_info_t;

DB_fileinfo_t *
v2m_open (uint32_t hints) {
    DB_fileinfo_t *_info = (DB_fileinfo_t *)malloc (sizeof (v2m_info_t));
    memset (_info, 0, sizeof (v2m_info_t));
    return _info;
}

static int
_load_and_convert (const char *fname, uint8_t **conv, int *convlen) {
    unsigned char *buf = NULL;
    DB_FILE *fp = deadbeef->fopen (fname);
    int ver = 0;

    if (!fp) {
        return -1;
    }

    // probe
    int len = (int)deadbeef->fgetlength (fp);
    buf = (unsigned char *)malloc (len);
    int rb = (int)deadbeef->fread (buf, 1, len, fp);
    deadbeef->fclose (fp);
    fp = NULL;

    if (rb != len) {
        free (buf);
        return -1;
    }

    if (!_v2m_initialized) {
        sdInit();
        _v2m_initialized = true;
    }
    ver = CheckV2MVersion(buf, len);

    if (ver < 0) {
        free (buf);
        return -1;
    }


    ConvertV2M(buf, len, conv, convlen);

    free (buf);
    
    return 0;
}

static int
get_total_samples (V2MPlayer *player) {
    int totalsamples = 0;
    float buffer[2048*2];
    bool had_nonsilence = false;
    for (;;) {
        player->Render(buffer, 2048);
        bool eof = true;
        for (int i = 0; i < 2048*2; i++) {
            float v = fabs(buffer[i]);
            if (v > 0.0000001f) {
                eof = false;
                if (totalsamples > 44100*2) {
                    had_nonsilence = true;
                }
                break;
            }
        }
        if (eof && had_nonsilence) {
            break;
        }
        totalsamples += 2048;
    }
    return totalsamples;
}

int
v2m_init (DB_fileinfo_t *_info, DB_playItem_t *it) {
    v2m_info_t *info = (v2m_info_t *)_info;

    char fname[PATH_MAX];

    if (_load_and_convert((deadbeef->pl_get_meta (it, ":URI", fname, sizeof (fname)), fname), &info->tune, &info->len) < 0) {
        return -1;
    }

    info->player = new V2MPlayer;
    info->player->Init();
    info->player->Open(info->tune);

    float dur = deadbeef->pl_get_item_duration(it);
    info->totalsamples = dur * 44100;

    _info->plugin = &v2m_plugin;
    _info->fmt.channels = 2;
    _info->fmt.bps = 32;
    _info->fmt.is_float = 1;
    _info->fmt.samplerate = 44100;
    _info->fmt.channelmask = _info->fmt.channels == 1 ? DDB_SPEAKER_FRONT_LEFT : (DDB_SPEAKER_FRONT_LEFT | DDB_SPEAKER_FRONT_RIGHT);
    _info->readpos = 0;

    info->player->Play();

    return 0;
}

void
v2m_free (DB_fileinfo_t *_info) {
    v2m_info_t *info = (v2m_info_t *)_info;
    if (info) {

        if (info->tune) {
            free (info->tune);
        }
        if (info->player) {
            info->player->Close();
            delete info->player;
        }

        free (info);
    }
}

int
v2m_read (DB_fileinfo_t *_info, char *bytes, int size) {
    v2m_info_t *info = (v2m_info_t *)_info;

    int samplesize = (_info->fmt.bps>>3) * _info->fmt.channels;
    int samples = size / samplesize;

    if (info->currsample > info->totalsamples) {
        return 0;
    }

    info->player->Render((float*) bytes, samples);

    _info->readpos += samples / (float)_info->fmt.samplerate;
    info->currsample += samples;

    return size;

}

int
v2m_seek_sample (DB_fileinfo_t *_info, int sample) {
    v2m_info_t *info = (v2m_info_t *)_info;

    if (sample >= info->totalsamples) {
        return -1; // seek beyond eof
    }

    if (sample < info->currsample) {
        info->player->Play(0);
        info->currsample = 0;
        _info->readpos = 0;
    }

    float buffer[2048 * _info->fmt.channels];
    while (info->currsample < sample) {
        int samples = sample - info->currsample;
        if (samples > 2048) {
            samples = 2048;
        }
        info->player->Render(buffer, samples);
        info->currsample += samples;
    }
    _info->readpos = sample / (float)_info->fmt.samplerate;

    return 0;
}

int
v2m_seek (DB_fileinfo_t *_info, float time) {
    int samples = time * _info->fmt.samplerate;
    return v2m_seek_sample(_info, samples);
}

DB_playItem_t *
v2m_insert (ddb_playlist_t *plt, DB_playItem_t *after, const char *fname) {
    uint8_t *conv = NULL;
    int convlen = 0;

    int res = _load_and_convert (fname, &conv, &convlen);
    if (res < 0 || !conv) {
        return NULL;
    }

    DB_playItem_t *it = deadbeef->pl_item_alloc_init (fname, v2m_plugin.plugin.id);
    deadbeef->pl_add_meta (it, ":FILETYPE", "V2M");

    V2MPlayer *player = new V2MPlayer ();
    player->Init();
    player->Open(conv, 44100);
    player->Play();

    int totalsamples = get_total_samples(player);

    player->Close();
    delete player;
    free (conv);

    deadbeef->plt_set_item_duration (plt, it, totalsamples / 44100.f);

    after = deadbeef->plt_insert_item (plt, after, it);
    deadbeef->pl_item_unref (it);
    return after;
}

static int
v2m_plugin_stop (void) {
    if (_v2m_initialized) {
        sdClose ();
    }

    return 0;
}

static const char *exts[] = { "v2m", NULL };

extern "C" DB_plugin_t *
v2m_load (DB_functions_t *api) {
    deadbeef = api;

    v2m_plugin.plugin.api_vmajor = DB_API_VERSION_MAJOR;
    v2m_plugin.plugin.api_vminor = DB_API_VERSION_MINOR;
    v2m_plugin.plugin.type = DB_PLUGIN_DECODER;
    v2m_plugin.plugin.version_major = 1;
    v2m_plugin.plugin.version_minor = 0;
    v2m_plugin.plugin.name = "V2M player";
    v2m_plugin.plugin.id = "v2m";
    v2m_plugin.plugin.descr = "Farbrausch V2M Player\n"
    "Based on https://github.com/farbrausch/fr_public v2 source code\n";
    v2m_plugin.plugin.copyright =
    "V2 code written 2000-2008 by Tammo \"kb\" Hinrichs\n"
    "\n"
    "I hereby place this code (as contained in this directory and the subdirectories\n"
    "\"bin\", \"conv2m\", \"in_v2m\", \"libv2\", \"tinyplayer\" and \"tool\")in the public domain.\n"
    "\n\n"
    "V2M plugin for DeaDBeeF Player\n"
    "Copyright (C) 2017 Alexey Yakovenko\n"
    "\n"
    "This software is provided 'as-is', without any express or implied\n"
    "warranty.  In no event will the authors be held liable for any damages\n"
    "arising from the use of this software.\n"
    "\n"
    "Permission is granted to anyone to use this software for any purpose,\n"
    "including commercial applications, and to alter it and redistribute it\n"
    "freely, subject to the following restrictions:\n"
    "\n"
    "1. The origin of this software must not be misrepresented; you must not\n"
    " claim that you wrote the original software. If you use this software\n"
    " in a product, an acknowledgment in the product documentation would be\n"
    " appreciated but is not required.\n"
    "\n"
    "2. Altered source versions must be plainly marked as such, and must not be\n"
    " misrepresented as being the original software.\n"
    "\n"
    "3. This notice may not be removed or altered from any source distribution.\n"
    ;
    v2m_plugin.plugin.website = "http://deadbeef.sf.net";
    v2m_plugin.open = v2m_open;
    v2m_plugin.init = v2m_init;
    v2m_plugin.free = v2m_free;
    v2m_plugin.read = v2m_read;
    v2m_plugin.seek = v2m_seek;
    v2m_plugin.seek_sample = v2m_seek_sample;
    v2m_plugin.insert = v2m_insert;
    v2m_plugin.exts = exts;
    v2m_plugin.plugin.stop = v2m_plugin_stop;

    return DB_PLUGIN (&v2m_plugin);
}

