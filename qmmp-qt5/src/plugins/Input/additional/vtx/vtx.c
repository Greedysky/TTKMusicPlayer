/*
    VTX plugin for DeaDBeeF Player
    Copyright (C) 2009-2014 Alexey Yakovenko

    Based on libayemu (C) 2005-2008 Alexander Sashnov

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

#include <stdlib.h>
#include <string.h>
#include "ayemu.h"

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

//#define trace(...) { fprintf (stderr, __VA_ARGS__); }
#define trace(fmt,...)

static DB_decoder_t plugin;
static DB_functions_t *deadbeef;

static const char * exts[] = { "vtx", NULL };

#define AY_FRAME_SIZE 14

typedef struct {
    DB_fileinfo_t info;
    ayemu_vtx_t *decoder;
    ayemu_ay_t ay;
    char regs[AY_FRAME_SIZE];
    int vtx_pos;
    int left;
    int rate;
    int currentsample;
} vtx_info_t;

static DB_fileinfo_t *
vtx_open (uint32_t hints) {
    DB_fileinfo_t *_info = malloc (sizeof (vtx_info_t));
    memset (_info, 0, sizeof (vtx_info_t));
    return _info;
}

static int
vtx_init (DB_fileinfo_t *_info, DB_playItem_t *it) {
    // prepare to decode the track
    // return -1 on failure
    vtx_info_t *info = (vtx_info_t *)_info;
    
    size_t sz = 0;
    char *buf = NULL;

    deadbeef->pl_lock ();
    DB_FILE *fp = deadbeef->fopen (deadbeef->pl_find_meta (it, ":URI"));
    deadbeef->pl_unlock ();
    if (!fp) {
        trace ("vtx: failed to open file %s\n", deadbeef->pl_find_meta (it, ":URI"));
        return -1;
    }

    sz = deadbeef->fgetlength (fp);
    if (sz <= 0) {
        trace ("vtx: bad file size\n");
        deadbeef->fclose (fp);
        return -1;
    }

    buf = malloc (sz);
    if (!buf) {
        trace ("vtx: out of memory\n");
        deadbeef->fclose (fp);
        return -1;
    }
    if (deadbeef->fread (buf, 1, sz, fp) != sz) {
        trace ("vtx: read failed\n");
        free (buf);
        deadbeef->fclose (fp);
        return -1;
    }

    deadbeef->fclose (fp);

    info->decoder = ayemu_vtx_load (buf, sz);
    if (!info->decoder) {
        trace ("vtx: ayemu_vtx_load failed\n");
        free (buf);
        return -1;
    }
    trace ("vtx: data=%p, size=%d\n", info->decoder->regdata, info->decoder->regdata_size);

    free (buf);

    ayemu_init (&info->ay);
    ayemu_set_chip_type (&info->ay, info->decoder->chiptype, NULL);
    ayemu_set_chip_freq (&info->ay, info->decoder->chipFreq);
    ayemu_set_stereo (&info->ay, info->decoder->stereo, NULL);

    int samplerate = deadbeef->conf_get_int ("synth.samplerate", 44100);

    info->left = 0;
    info->vtx_pos = 0;
    _info->plugin = &plugin;
    _info->fmt.bps = deadbeef->conf_get_int ("vtx.bps", 16);;
    if (_info->fmt.bps != 16 && _info->fmt.bps != 8) {
        _info->fmt.bps = 16;
    }
    _info->fmt.channels = 2;
    _info->fmt.samplerate = samplerate;
    _info->fmt.channelmask = _info->fmt.channels == 1 ? DDB_SPEAKER_FRONT_LEFT : (DDB_SPEAKER_FRONT_LEFT | DDB_SPEAKER_FRONT_RIGHT);
    _info->readpos = 0;

    ayemu_set_sound_format (&info->ay, samplerate, _info->fmt.channels, _info->fmt.bps);

    info->rate = _info->fmt.channels * _info->fmt.bps / 8;
    return 0;
}

static void
vtx_free (DB_fileinfo_t *_info) {
    // free everything allocated in _init
    vtx_info_t *info = (vtx_info_t *)_info;
    if (_info) {
        if (info->decoder) {
            ayemu_vtx_free (info->decoder);
            info->decoder = NULL;
        }
        ayemu_reset (&info->ay);
        free (_info);
    }
}

/** Get next 14-bytes frame of AY register data.
 *
 * Return value: 1 on success, 0 on eof
 */
static int
ayemu_vtx_get_next_frame (vtx_info_t *info)
{
    int numframes = info->decoder->regdata_size / AY_FRAME_SIZE;
    if (info->vtx_pos++ >= numframes) {
        return 0;
    }
    int n;
    char *p = info->decoder->regdata + info->vtx_pos;
    for(n = 0 ; n < AY_FRAME_SIZE ; n++, p += numframes) {
        info->regs[n] = *p;
    }
    return 1;
}

static int
vtx_read (DB_fileinfo_t *_info, char *bytes, int size) {
    // try decode `size' bytes
    // return number of decoded bytes
    // return 0 on EOF
    vtx_info_t *info = (vtx_info_t *)_info;
    int initsize = size;
    int donow = 0;

    while (size > 0) {
        if (info->left > 0) {
            donow = (size > info->left) ? info->left : size;
            info->left -= donow;
            bytes = ayemu_gen_sound (&info->ay, (char *)bytes, donow);
            size -= donow;
        }
        else {
            if ((ayemu_vtx_get_next_frame (info) == 0)) {
                break; // eof
            }
            else {
                // number of samples it current frame
                info->left = _info->fmt.samplerate / info->decoder->playerFreq;
                // mul by rate to get number of bytes;
                info->left *= info->rate;
                ayemu_set_regs (&info->ay, info->regs);
                donow = 0;
            }
        }
    }
    info->currentsample += (initsize - size) / 4;
    _info->readpos = (float)info->currentsample / _info->fmt.samplerate;
    return initsize - size;
}

static int
vtx_seek_sample (DB_fileinfo_t *_info, int sample) {
    // seek to specified sample (frame)
    // return 0 on success
    // return -1 on failure
    vtx_info_t *info = (vtx_info_t *)_info;

    // get frame
    int num_frames = info->decoder->regdata_size / AY_FRAME_SIZE;
    int samples_per_frame = _info->fmt.samplerate / info->decoder->playerFreq;

    // start of frame
    info->vtx_pos = sample / samples_per_frame;
    if (info->vtx_pos >= num_frames) {
        return -1; // eof
    }
    // copy register data
    int n;
    char *p = info->decoder->regdata + info->vtx_pos;
    for(n = 0 ; n < AY_FRAME_SIZE ; n++, p += num_frames) {
        info->regs[n] = *p;
    }
    // set number of bytes left in frame
    info->left = _info->fmt.samplerate / info->decoder->playerFreq - (sample % samples_per_frame);
    // mul by rate to get number of bytes
    info->left *= info->rate;
    info->currentsample = sample;
    _info->readpos = (float)info->currentsample / _info->fmt.samplerate;

    return 0;
}

static int
vtx_seek (DB_fileinfo_t *_info, float time) {
    // seek to specified time in seconds
    // return 0 on success
    // return -1 on failure
    return vtx_seek_sample (_info, time * _info->fmt.samplerate);
}

static DB_playItem_t *
vtx_insert (ddb_playlist_t *plt, DB_playItem_t *after, const char *fname) {
    // read information from the track
    // load/process cuesheet if exists
    // insert track into playlist
    // return track pointer on success
    // return NULL on failure

    trace ("vtx_insert %s\n");
    // load header
    DB_FILE *fp = deadbeef->fopen (fname);
    if (!fp) {
        trace ("vtx: failed to open file\n");
        return NULL;
    }
    char buf[0x4000];
    size_t sz;
    sz = deadbeef->fread (buf, 1, sizeof (buf), fp);
    deadbeef->fclose (fp);
    if (sz <= 0) {
        trace ("vtx: failed to read header data from file\n");
        return NULL;
    }
    ayemu_vtx_t *hdr = ayemu_vtx_header (buf, sz);
    if (!hdr) {
        trace ("vtx: failed to read header\n");
        return NULL;
    }
    trace ("vtx: datasize: %d\n", hdr->regdata_size);

    DB_playItem_t *it = deadbeef->pl_item_alloc_init (fname, plugin.plugin.id);
    deadbeef->pl_add_meta (it, ":FILETYPE", "VTX");

    int numframes = hdr->regdata_size / AY_FRAME_SIZE;
//    int totalsamples = numframes * hdr->playerFreq;
    trace ("vtx: numframes=%d, playerFreq=%d\n", numframes, hdr->playerFreq);
    deadbeef->plt_set_item_duration (plt, it, (float)numframes / hdr->playerFreq);

    // add metadata
    deadbeef->pl_add_meta (it, "title", hdr->title);
    deadbeef->pl_add_meta (it, "artist", hdr->author);
    deadbeef->pl_add_meta (it, "album", hdr->from);

    ayemu_vtx_free (hdr);
    after = deadbeef->plt_insert_item (plt, after, it);
    deadbeef->pl_item_unref (it);
    return after;
}

static int
vtx_start (void) {
    // do one-time plugin initialization here
    // e.g. starting threads for background processing, subscribing to events, etc
    // return 0 on success
    // return -1 on failure
    return 0;
}
static int
vtx_stop (void) {
    // undo everything done in _start here
    // return 0 on success
    // return -1 on failure
    return 0;
}

static const char settings_dlg[] =
    "property \"Bits per sample (8 or 16)\" entry vtx.bps 16;\n"
;

// define plugin interface
static DB_decoder_t plugin = {
    .plugin.api_vmajor = 1,
    .plugin.api_vminor = 0,
    .plugin.version_major = 1,
    .plugin.version_minor = 0,
    .plugin.type = DB_PLUGIN_DECODER,
    .plugin.id = "vtx",
    .plugin.name = "VTX player",
    .plugin.descr = "AY8910/12 chip emulator and vtx file player",
    .plugin.copyright = 
        "VTX plugin for DeaDBeeF Player\n"
        "Copyright (C) 2009-2014 Alexey Yakovenko\n"
        "\n"
        "Based on libayemu (C) 2005-2008 Alexander Sashnov\n"
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
    ,
    .plugin.website = "http://deadbeef.sf.net",
    .plugin.start = vtx_start,
    .plugin.stop = vtx_stop,
    .plugin.configdialog = settings_dlg,
    .open = vtx_open,
    .init = vtx_init,
    .free = vtx_free,
    .read = vtx_read,
    .seek = vtx_seek,
    .seek_sample = vtx_seek_sample,
    .insert = vtx_insert,
    .exts = exts,
};

DB_plugin_t *
vtx_load (DB_functions_t *api) {
    deadbeef = api;
    return DB_PLUGIN (&plugin);
}
