/*
    Copyright (C) 2009-2016 Alexey Yakovenko <waker@users.sourceforge.net>

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

// based on fb2k dumb plugin from http://kode54.foobar2000.org
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "dumb.h"
#include "internal/dumbfile.h"
#include "internal/it.h"
#include "umr.h"
#include "modloader.h"

extern "C" {
#include "stdio_file.h"
}

// FIXME: config
static int cfg_dumb_count_patterns = 1; // MOD - Count patterns from the order list
static int cfg_autochip = 0; // ???
static int cfg_autochip_size_force = 100;
static int cfg_autochip_size_scan = 500;
static int cfg_autochip_scan_threshold = 12;
static int cfg_trim = 0;

class umr_mem_reader : public umr::file_reader
{
    const uint8_t * ptr;
    long offset, size;
public:
    umr_mem_reader(const uint8_t * buf, unsigned p_size) : ptr(buf), size(p_size), offset(0) {}

    long read( void * buf, long howmany )
    {
        long max = size - offset;
        if ( max > howmany ) max = howmany;
        if ( max )
        {
            memcpy( buf, ptr + offset, max );
            offset += max;
        }
        return max;
    }

    void seek( long where )
    {
        if ( where > size ) offset = size;
        else offset = where;
    }
};

static const char *exts[] = {"MOD","MDZ","STK","M15","FST","OCT","NT",NULL};

static bool g_test_path(const char * path)
{
    const char *name = strrchr (path, '/');
    if (name) {
        name++;
    }
    else {
        name = path;
    }
    const char *ext = strrchr(name, '.');
    if (ext) {
        ext++;
    }
    else {
        ext = name;
    }
    for( int n = 0; exts[n]; ++n )
    {
        if (!strcasecmp(ext,exts[n]) || !strcasecmp(name, exts[n])) return true;
    }
    return false;
}

void dumb_it_convert_tempos( DUMB_IT_SIGDATA * itsd, bool vsync )
{
    for ( unsigned i = 0, j = itsd->n_patterns; i < j; i++ )
    {
        IT_PATTERN * pat = &itsd->pattern[ i ];
        for ( unsigned k = 0, l = pat->n_entries; k < l; k++ )
        {
            IT_ENTRY * entry = &pat->entry[ k ];
            if ( entry->mask & IT_ENTRY_EFFECT )
            {
                if ( vsync && entry->effect == IT_SET_SONG_TEMPO ) entry->effect = IT_SET_SPEED;
                else if ( !vsync && entry->effect == IT_SET_SPEED && entry->effectvalue > 0x20 ) entry->effect = IT_SET_SONG_TEMPO;
            }
        }
    }
}

DUH * open_module(const char * path, int *is_it, int *is_dos, int *is_ptcompat, int is_vblank, const char **ftype)
{
    DUH * duh = 0;

    FILE *fp = stdio_open (path);
    if (!fp) {
        return NULL;
    }

    uint8_t buf[2000];
    size_t size = stdio_read (buf, 1, sizeof (buf), fp);

    uint8_t *ptr = buf;

    stdio_close (fp);

    DUMBFILE * f = dumbfile_open (path);
    if (!f) {
        return NULL;
    }

    *is_it = 0;
    *is_dos = 1;
    *is_ptcompat = 0;

    if (size >= 4 &&
        ptr[0] == 0xC1 && ptr[1] == 0x83 &&
        ptr[2] == 0x2A && ptr[3] == 0x9E)
    {
        FILE *fp = stdio_open (path);
        if (!fp) {
            dumbfile_close(f);
            return NULL;
        }
        size_t filesize = stdio_length(fp);
        uint8_t *buf = (uint8_t *)malloc (filesize);
        ptr = buf;
        size_t size = stdio_read (buf, 1, filesize, fp);
        stdio_close (fp);
        if (size != filesize) {
            free (buf);
            dumbfile_close(f);
            return NULL;
        }
        umr_mem_reader memreader(buf, (int)size);
        umr::upkg pkg;
        if (pkg.open(&memreader))
        {
            for (int i = 1, j = pkg.ocount(); i <= j; i++)
            {
                char * classname = pkg.oclassname(i);
                if (classname && !strcmp(pkg.oclassname(i), "Music"))
                {
                    char * type = pkg.otype(i);
                    if (!type) continue;
                    /*
                     if (!strcasecmp(type, "it"))
                     {
                     is_it = true;
                     ptr += memdata.offset = pkg.object_offset(i);
                     size = memdata.size = memdata.offset + pkg.object_size(i);
                     duh = dumb_read_it_quick(f);
                     break;
                     }
                     else if (!strcasecmp(type, "s3m"))
                     {
                     memdata.offset = pkg.object_offset(i);
                     memdata.size = memdata.offset + pkg.object_size(i);
                     duh = dumb_read_s3m_quick(f);
                     break;
                     }
                     else if (!strcasecmp(type, "xm"))
                     {
                     memdata.offset = pkg.object_offset(i);
                     memdata.size = memdata.offset + pkg.object_size(i);
                     duh = dumb_read_xm_quick(f);
                     break;
                     }
                     */
                    // blah, type can't be trusted
                    if (!strcasecmp(type, "it") || !strcasecmp(type, "s3m") || !strcasecmp(type, "xm"))
                    {
                        f->dfs->seek (f->file, 0);
                        ptr += pkg.object_offset(i);
                        size = pkg.object_size(i);

                        if (size >= 4 && ptr[0] == 'I' && ptr[1] == 'M' && ptr[2] == 'P' && ptr[3] == 'M')
                        {
                            *is_it = 1;
                            duh = dumb_read_it_quick(f);
                            *ftype = "IT";
                        }
                        else if (size >= 42 && ptr[38] == 'F' && ptr[39] == 'a' && ptr[40] == 's' && ptr[41] == 't')
                        {
                            duh = dumb_read_xm_quick(f);
                            *ftype = "XM";
                        }
                        else if (size >= 48 && ptr[44] == 'S' && ptr[45] == 'C' && ptr[46] == 'R' && ptr[47] == 'M')
                        {
                            duh = dumb_read_s3m_quick(f);
                            *ftype = "S3M";
                        }

                        break;
                    }
                }
            }
        }
        free (buf);
    }
    else if (size >= 4 &&
             ptr[0] == 'I' && ptr[1] == 'M' &&
             ptr[2] == 'P' && ptr[3] == 'M')
    {
        *is_it = 1;
        duh = dumb_read_it_quick(f);
        *ftype = "IT";
    }
    else if (size >= 17 && !memcmp(ptr, "Extended Module: ", 17))
    {
        duh = dumb_read_xm_quick(f);
        *ftype = "XM";
    }
    else if (size >= 0x30 &&
             ptr[0x2C] == 'S' && ptr[0x2D] == 'C' &&
             ptr[0x2E] == 'R' && ptr[0x2F] == 'M')
    {
        duh = dumb_read_s3m_quick(f);
        *ftype = "S3M";
    }
    else if (size >= 1168 &&
             /*ptr[28] == 0x1A &&*/ ptr[29] == 2 &&
             ( ! strncasecmp( ( const char * ) ptr + 20, "!Scream!", 8 ) ||
              ! strncasecmp( ( const char * ) ptr + 20, "BMOD2STM", 8 ) ||
              ! strncasecmp( ( const char * ) ptr + 20, "WUZAMOD!", 8 ) ) )
    {
        duh = dumb_read_stm_quick(f);
        *ftype = "STM";
    }
    else if (size >= 2 &&
             ((ptr[0] == 0x69 && ptr[1] == 0x66) ||
              (ptr[0] == 0x4A && ptr[1] == 0x4E)))
    {
        duh = dumb_read_669_quick(f);
        *ftype = "669";
    }
    else if (size >= 0x30 &&
             ptr[0x2C] == 'P' && ptr[0x2D] == 'T' &&
             ptr[0x2E] == 'M' && ptr[0x2F] == 'F')
    {
        duh = dumb_read_ptm_quick(f);
        *ftype = "PTM";
    }
    else if (size >= 4 &&
             ptr[0] == 'P' && ptr[1] == 'S' &&
             ptr[2] == 'M' && ptr[3] == ' ')
    {
        int start_order = 0;
        duh = dumb_read_psm_quick(f, start_order);
        *ftype = "PSM";
    }
    else if (size >= 4 &&
             ptr[0] == 'P' && ptr[1] == 'S' &&
             ptr[2] == 'M' && ptr[3] == 254)
    {
        duh = dumb_read_old_psm_quick(f);
        *ftype = "PSM";
    }
    else if (size >= 3 &&
             ptr[0] == 'M' && ptr[1] == 'T' &&
             ptr[2] == 'M')
    {
        *is_ptcompat = 1;
        duh = dumb_read_mtm_quick(f);
        *ftype = "MTM";
    }
    else if ( size >= 4 &&
             ptr[0] == 'R' && ptr[1] == 'I' &&
             ptr[2] == 'F' && ptr[3] == 'F')
    {
        duh = dumb_read_riff_quick(f);
        *ftype = "RIFF";
    }
    else if ( size >= 32 &&
             !memcmp( ptr, "ASYLUM Music Format", 19 ) &&
             !memcmp( ptr + 19, " V1.0", 5 ) )
    {
        duh = dumb_read_asy_quick(f);
        *ftype = "ASY";
    }
    else if ( size >= 3 &&
             ptr[0] == 'A' && ptr[1] == 'M' &&
             ptr[2] == 'F')
    {
        duh = dumb_read_amf_quick(f);
        *ftype = "AMF";
    }
    else if ( size >= 8 &&
             !memcmp( ptr, "OKTASONG", 8 ) )
    {
        duh = dumb_read_okt_quick(f);
        *ftype = "OKT";
    }

    if ( ! duh )
    {
        *is_dos = 0;
        dumbfile_seek( f, 0, DFS_SEEK_SET );
        duh = dumb_read_mod_quick(f, ( cfg_dumb_count_patterns ? 0 : 2 ) + ( g_test_path( path ) ? 0 : 1 ) );
        if ( duh && is_vblank )
        {
            DUMB_IT_SIGDATA * itsd = duh_get_it_sigdata( duh );
            dumb_it_convert_tempos( itsd, true );
        }
        if ( duh ) *is_ptcompat = 1;

        *ftype = "MOD";
    }

    dumbfile_close(f);

    if (duh && cfg_autochip)
    {
        int size_force = cfg_autochip_size_force;
        int size_scan = cfg_autochip_size_scan;
        int scan_threshold_8 = ((cfg_autochip_scan_threshold * 0x100) + 50) / 100;
        int scan_threshold_16 = ((cfg_autochip_scan_threshold * 0x10000) + 50) / 100;
        DUMB_IT_SIGDATA * itsd = duh_get_it_sigdata(duh);

        if (itsd)
        {
            for (int i = 0, j = itsd->n_samples; i < j; i++)
            {
                IT_SAMPLE * sample = &itsd->sample[i];
                if (sample->flags & IT_SAMPLE_EXISTS)
                {
                    int channels = sample->flags & IT_SAMPLE_STEREO ? 2 : 1;
                    if (sample->length < size_force) sample->max_resampling_quality = 1;
                    else if (sample->length < size_scan)
                    {
                        if ((sample->flags & (IT_SAMPLE_LOOP|IT_SAMPLE_PINGPONG_LOOP)) == IT_SAMPLE_LOOP)
                        {
                            int loop_start = sample->loop_start * channels;
                            int loop_end = sample->loop_end * channels;
                            int s1, s2;
                            if (sample->flags & IT_SAMPLE_16BIT)
                            {
                                s1 = ((signed short *)sample->data)[loop_start];
                                s2 = ((signed short *)sample->data)[loop_end - channels];
                                if (abs(s1 - s2) > scan_threshold_16)
                                {
                                    sample->max_resampling_quality = 1;
                                    continue;
                                }
                                if (channels == 2)
                                {
                                    s1 = ((signed short *)sample->data)[loop_start + 1];
                                    s2 = ((signed short *)sample->data)[loop_end - 1];
                                    if (abs(s1 - s2) > scan_threshold_16)
                                    {
                                        sample->max_resampling_quality = 1;
                                        continue;
                                    }
                                }
                            }
                            else
                            {
                                s1 = ((signed char *)sample->data)[loop_start];
                                s2 = ((signed char *)sample->data)[loop_end - channels];
                                if (abs(s1 - s2) > scan_threshold_8)
                                {
                                    sample->max_resampling_quality = 1;
                                    continue;
                                }
                                if (channels == 2)
                                {
                                    s1 = ((signed char *)sample->data)[loop_start + 1];
                                    s2 = ((signed char *)sample->data)[loop_end - 1];
                                    if (abs(s1 - s2) > scan_threshold_8)
                                    {
                                        sample->max_resampling_quality = 1;
                                        continue;
                                    }
                                }
                            }
                        }
                        if ((sample->flags & (IT_SAMPLE_SUS_LOOP|IT_SAMPLE_PINGPONG_SUS_LOOP)) == IT_SAMPLE_SUS_LOOP)
                        {
                            int sus_loop_start = sample->sus_loop_start * channels;
                            int sus_loop_end = sample->sus_loop_end * channels;
                            int s1, s2;
                            if (sample->flags & IT_SAMPLE_16BIT)
                            {
                                s1 = ((signed short *)sample->data)[sus_loop_start];
                                s2 = ((signed short *)sample->data)[sus_loop_end - channels];
                                if (abs(s1 - s2) > scan_threshold_16)
                                {
                                    sample->max_resampling_quality = 1;
                                    continue;
                                }
                                if (channels == 2)
                                {
                                    s1 = ((signed short *)sample->data)[sus_loop_start + 1];
                                    s2 = ((signed short *)sample->data)[sus_loop_end - 1];
                                    if (abs(s1 - s2) > scan_threshold_16)
                                    {
                                        sample->max_resampling_quality = 1;
                                        continue;
                                    }
                                }
                            }
                            else
                            {
                                s1 = ((signed char *)sample->data)[sus_loop_start];
                                s2 = ((signed char *)sample->data)[sus_loop_end - channels];
                                if (abs(s1 - s2) > scan_threshold_8)
                                {
                                    sample->max_resampling_quality = 1;
                                    continue;
                                }
                                if (channels == 2)
                                {
                                    s1 = ((signed char *)sample->data)[sus_loop_start + 1];
                                    s2 = ((signed char *)sample->data)[sus_loop_end - 1];
                                    if (abs(s1 - s2) > scan_threshold_8)
                                    {
                                        sample->max_resampling_quality = 1;
                                        continue;
                                    }
                                }
                            }
                        }
                        
                        int k, l = sample->length * channels;
                        if (sample->flags & IT_SAMPLE_LOOP) l = sample->loop_end * channels;
                        if (sample->flags & IT_SAMPLE_16BIT)
                        {
                            for (k = channels; k < l; k += channels)
                            {
                                if (abs(((signed short *)sample->data)[k - channels] - ((signed short *)sample->data)[k]) > scan_threshold_16)
                                {
                                    break;
                                }
                            }
                            if (k < l)
                            {
                                sample->max_resampling_quality = 1;
                                continue;
                            }
                            if (channels == 2)
                            {
                                for (k = 2 + 1; k < l; k += 2)
                                {
                                    if (abs(((signed short *)sample->data)[k - 2] - ((signed short *)sample->data)[k]) > scan_threshold_16)
                                    {
                                        break;
                                    }
                                }
                            }
                            if (k < l)
                            {
                                sample->max_resampling_quality = 1;
                                continue;
                            }
                        }
                        else
                        {
                            for (k = channels; k < l; k += channels)
                            {
                                if (abs(((signed char *)sample->data)[k - channels] - ((signed char *)sample->data)[k]) > scan_threshold_8)
                                {
                                    break;
                                }
                            }
                            if (k < l)
                            {
                                sample->max_resampling_quality = 1;
                                continue;
                            }
                            if (channels == 2)
                            {
                                for (k = 2 + 1; k < l; k += 2)
                                {
                                    if (abs(((signed char *)sample->data)[k - 2] - ((signed char *)sample->data)[k]) > scan_threshold_8)
                                    {
                                        break;
                                    }
                                }
                            }
                            if (k < l)
                            {
                                sample->max_resampling_quality = 1;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if ( duh && cfg_trim )
    {
        if ( dumb_it_trim_silent_patterns( duh ) < 0 )
        {
            unload_duh( duh );
            duh = 0;
        }
    }
    
    return duh;
}
