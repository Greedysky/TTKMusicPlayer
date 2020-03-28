/*  xmms-shn - a shorten (.shn) plugin for XMMS
 *  Copyright (C) 2000-2007  Jason Jordan <shnutils@freeshell.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * $Id: shn.h,v 1.27 2007/03/23 05:49:48 jason Exp $
 */

#ifndef SHNDEC_H
#define SHNDEC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "shorten.h"

#define shn_vsnprintf(a,b,c,d) vsnprintf(a,b,c,d)

/* surely no headers will be this large.  right?  RIGHT?  */
#define OUT_BUFFER_SIZE			16384

#define BUF_SIZE                        4096

#define	ERROR_OUTPUT_DEVNULL		0
#define	ERROR_OUTPUT_STDERR		1
#define	ERROR_OUTPUT_WINDOW		2

#define	SEEK_SUFFIX			"skt"

#define	NO_SEEK_TABLE			-1

#define	SEEK_HEADER_SIGNATURE		"SEEK"
#define	SEEK_TRAILER_SIGNATURE		"SHNAMPSK"

#define SEEK_HEADER_SIZE		12
#define SEEK_TRAILER_SIZE		12
#define SEEK_ENTRY_SIZE			80
#define SEEK_RESOLUTION			25600

#define WAVE_RIFF                       (0x46464952)     /* 'RIFF' in little-endian */
#define WAVE_WAVE                       (0x45564157)     /* 'WAVE' in little-endian */
#define WAVE_FMT                        (0x20746d66)     /* ' fmt' in little-endian */
#define WAVE_DATA                       (0x61746164)     /* 'data' in little-endian */

#define AIFF_FORM                       (0x4D524F46)     /* 'FORM' in little-endian */

#define WAVE_FORMAT_UNKNOWN             (0x0000)
#define WAVE_FORMAT_PCM                 (0x0001)
#define WAVE_FORMAT_ADPCM               (0x0002)
#define WAVE_FORMAT_IEEE_FLOAT          (0x0003)
#define WAVE_FORMAT_ALAW                (0x0006)
#define WAVE_FORMAT_MULAW               (0x0007)
#define WAVE_FORMAT_OKI_ADPCM           (0x0010)
#define WAVE_FORMAT_IMA_ADPCM           (0x0011)
#define WAVE_FORMAT_DIGISTD             (0x0015)
#define WAVE_FORMAT_DIGIFIX             (0x0016)
#define WAVE_FORMAT_DOLBY_AC2           (0x0030)
#define WAVE_FORMAT_GSM610              (0x0031)
#define WAVE_FORMAT_ROCKWELL_ADPCM      (0x003b)
#define WAVE_FORMAT_ROCKWELL_DIGITALK   (0x003c)
#define WAVE_FORMAT_G721_ADPCM          (0x0040)
#define WAVE_FORMAT_G728_CELP           (0x0041)
#define WAVE_FORMAT_MPEG                (0x0050)
#define WAVE_FORMAT_MPEGLAYER3          (0x0055)
#define WAVE_FORMAT_G726_ADPCM          (0x0064)
#define WAVE_FORMAT_G722_ADPCM          (0x0065)

#define CD_BLOCK_SIZE                   (2352)
#define CD_BLOCKS_PER_SEC               (75)
#define CD_MIN_BURNABLE_SIZE            (705600)
#define CD_CHANNELS                     (2)
#define CD_SAMPLES_PER_SEC              (44100)
#define CD_BITS_PER_SAMPLE              (16)
#define CD_RATE                         (176400)

#define CANONICAL_HEADER_SIZE           (44)

#define PROBLEM_NOT_CD_QUALITY          (0x00000001)
#define PROBLEM_CD_BUT_BAD_BOUND        (0x00000002)
#define PROBLEM_CD_BUT_TOO_SHORT        (0x00000004)
#define PROBLEM_HEADER_NOT_CANONICAL    (0x00000008)
#define PROBLEM_EXTRA_CHUNKS            (0x00000010)
#define PROBLEM_HEADER_INCONSISTENT     (0x00000020)

#define PROB_NOT_CD(f)                  ((f.problems) & (PROBLEM_NOT_CD_QUALITY))
#define PROB_BAD_BOUND(f)               ((f.problems) & (PROBLEM_CD_BUT_BAD_BOUND))
#define PROB_TOO_SHORT(f)               ((f.problems) & (PROBLEM_CD_BUT_TOO_SHORT))
#define PROB_HDR_NOT_CANONICAL(f)       ((f.problems) & (PROBLEM_HEADER_NOT_CANONICAL))
#define PROB_EXTRA_CHUNKS(f)            ((f.problems) & (PROBLEM_EXTRA_CHUNKS))
#define PROB_HDR_INCONSISTENT(f)        ((f.problems) & (PROBLEM_HEADER_INCONSISTENT))

#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX    1024    /* max # of characters in a path name */
#endif

typedef struct shn_config
{
	int error_output_method;
	char seek_tables_path[PATH_MAX];
	char relative_seek_tables_path[PATH_MAX];
	int verbose;
	int swap_bytes;
} shn_config;

typedef struct shn_decode_state
{
    uint8_t *getbuf;
    uint8_t *getbufp;
	int    nbitget;
	int    nbyteget;
    uint32_t  gbuffer;
    int8_t *writebuf;
    int8_t *writefub;
	int    nwritebuf;
} shn_decode_state;

typedef struct shn_seek_header
{
    uint8_t data[SEEK_HEADER_SIZE];
    int32_t version;
    uint32_t shnFileSize;
} shn_seek_header;

typedef struct shn_seek_trailer
{
    uint8_t data[SEEK_TRAILER_SIZE];
    uint32_t seekTableSize;
} shn_seek_trailer;

typedef struct shn_seek_entry
{
    uint8_t data[SEEK_ENTRY_SIZE];
} shn_seek_entry;

typedef struct shn_wave_header
{
	const char *filename;
    char m_ss[16];

    uint32_t header_size;

    uint16_t channels,
	       block_align,
	       bits_per_sample,
	       wave_format;

    uint32_t samples_per_sec,
	      avg_bytes_per_sec,
	      rate,
	      length,
	      data_size,
	      total_size,
	      chunk_size,
	      actual_size;

	double exact_length;

	int   file_has_id3v2_tag;
	long  id3v2_tag_size;

    uint32_t problems;
} shn_wave_header;

typedef struct shn_vars
{
    FILE  *fd;
	int    seek_to;
	int    eof;
	int    going;
    int32_t  seek_table_entries;
    uint32_t  seek_resolution;
	int    bytes_in_buf;
    uint8_t  buffer[OUT_BUFFER_SIZE];
	int    bytes_in_header;
    uint8_t  header[OUT_BUFFER_SIZE];
	int    fatal_error;
    int8_t  fatal_error_msg[BUF_SIZE];
	int    reading_function_code;
    uint32_t  last_file_position;
    uint32_t  last_file_position_no_really;
    uint32_t  initial_file_position;
    uint32_t  bytes_read;
	unsigned short actual_bitshift;
	int    actual_maxnlpc;
	int    actual_nmean;
	int    actual_nchan;
	long   seek_offset;
} shn_vars;

typedef struct shn_file
{
	shn_vars          vars;
	shn_decode_state *decode_state;
	shn_wave_header   wave_header;
	shn_seek_header   seek_header;
	shn_seek_trailer  seek_trailer;
	shn_seek_entry   *seek_table;
} shn_file;


typedef struct {
    shn_file *shnfile;
    int32_t  **buffer, **offset;
    int32_t  lpcqoffset;
    int   version, bitshift;
    int   ftype;
    char  *magic;
    int   blocksize, nchan;
    int   i, chan, nwrap, nskip;
    int   *qlpc, maxnlpc, nmean;
    int   cmd;
    int   internal_ftype;
    int   blk_size;
    int   cklen;
    uint8_t tmp;

    int currentsample;
    int startsample;
    int endsample;
    int samples_to_skip;
    float readpos;
} shn_info_t;

void shn_init_config();

void shn_unload(shn_file *this_shn);
shn_file *load_shn(const char *filename);

int shn_init_decoder(shn_info_t *info);
void shn_free_decoder(shn_info_t *info);
int shn_decode(shn_info_t *info);
int shn_seek_sample(shn_info_t *info, int sample);

#endif
