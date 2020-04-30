/*
 * ALAC (Apple Lossless Audio Codec) decoder
 * Copyright (c) 2005 David Hammerton
 * All rights reserved.
 *
 * This is the actual decoder.
 *
 * http://crazney.net/programs/itunes/alac.html
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef ALAC_DEMUX_H
#define ALAC_DEMUX_H

#ifdef _WIN32
#include "alac_stdint_win.h"
#else
#include <stdint.h>
#endif
#include "alac_stream.h"

typedef uint32_t fourcc_t;

typedef struct
{
    int format_read;

    uint16_t num_channels;
    uint16_t sample_size;
    uint32_t sample_rate;
    fourcc_t format;
    void *buf;

    struct {
        uint32_t sample_count;
        uint32_t sample_duration;
    } *time_to_sample;
    uint32_t num_time_to_samples;

    uint32_t *sample_byte_size;
    uint32_t num_sample_byte_sizes;

    uint32_t codecdata_len;
    void *codecdata;

    uint32_t mdat_len;
#if 0
    void *mdat;
#endif
} demux_res_t;

int qtmovie_read(stream_t *stream, demux_res_t *demux_res);

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ( \
    ( (int32_t)(char)(ch0) << 24 ) | \
    ( (int32_t)(char)(ch1) << 16 ) | \
    ( (int32_t)(char)(ch2) << 8 ) | \
    ( (int32_t)(char)(ch3) ) )
#endif

#ifndef SLPITFOURCC
/* splits it into ch0, ch1, ch2, ch3 - use for printf's */
#define SPLITFOURCC(code) \
    (char)((int32_t)code >> 24), \
    (char)((int32_t)code >> 16), \
    (char)((int32_t)code >> 8), \
    (char)code
#endif

#endif
