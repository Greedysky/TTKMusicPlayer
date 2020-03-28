/*
 * ttadec.h
 *
 * Description:	 TTAv1 decoder definitions and prototypes
 * Developed by: Alexander Djourik <ald@true-audio.com>
 *               Pavel Zhilin <pzh@true-audio.com>
 *
 * Copyright (c) 2004 True Audio Software. All rights reserved.
 *
 */

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the True Audio Software nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TTADEC_H
#define TTADEC_H

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#ifdef _WIN32
#pragma pack(1)
#define __ATTRIBUTE_PACKED__
#else
#define __ATTRIBUTE_PACKED__	__attribute__((packed))
#endif

#define TTA1_SIGN	0x31415454
#define FRAME_TIME	1.04489795918367346939
#define MAX_ORDER	8

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM	1
#endif

#ifdef _WIN32
	typedef unsigned __int64 uint64;
#else
	typedef unsigned long long uint64;
#endif

typedef unsigned char byte;

#ifdef _BIG_ENDIAN
#define	ENDSWAP_INT16(x)	(((((x)>>8)&0xFF)|(((x)&0xFF)<<8)))
#define	ENDSWAP_INT32(x)	(((((x)>>24)&0xFF)|(((x)>>8)&0xFF00)|(((x)&0xFF00)<<8)|(((x)&0xFF)<<24)))
#define WRITE_BUFFER(x, bsize, out) { \
	if (bsize > 2) *out++ = (byte)(*x >> 16); \
	if (bsize > 1) *out++ = (byte)(*x >> 8); \
	*out++ = (byte) *x; }
#else
#define	ENDSWAP_INT16(x)	(x)
#define	ENDSWAP_INT32(x)	(x)
#define WRITE_BUFFER(x, bsize, out) { \
	*out++ = (byte) *x; \
	if (bsize > 1) *out++ = (byte)(*x >> 8); \
	if (bsize > 2) *out++ = (byte)(*x >> 16); }
#endif

#define PREDICTOR1(x, k)	((int)((((uint64)x << k) - x) >> k))
#define DEC(x)			(((x)&1)?(++(x)>>1):(-(x)>>1))

typedef struct {
	unsigned int TTAid;
	unsigned short AudioFormat;
	unsigned short NumChannels;
	unsigned short BitsPerSample;
	unsigned int SampleRate;
	unsigned int DataLength;
	unsigned int CRC32;
} __ATTRIBUTE_PACKED__ tta_hdr;

typedef struct {
	unsigned int k0;
	unsigned int k1;
	unsigned int sum0;
	unsigned int sum1;
} adapt;

typedef struct {
	int shift;
	int round;
	int error;
	int mutex;
	int qm[MAX_ORDER+1];
	int dx[MAX_ORDER+1];
	int dl[MAX_ORDER+1];
} fltst;

typedef struct {
	fltst fst;
	adapt rice;
	int last;
} decoder;

//#define _BIG_ENDIAN

#define MAX_BPS         24	// Max supported Bit resolution
#define MAX_NCH         8	// Max supported number of channels

#ifndef MAXLINE
#define MAX_LINE        1024
#endif

// return codes
#define NO_ERROR        0	// No errors found
#define OPEN_ERROR      1	// Can't open file
#define FORMAT_ERROR    2	// Unknown TTA format version
#define PLAYER_ERROR    3	// Not supported file format
#define FILE_ERROR      4	// File is corrupted
#define READ_ERROR      5	// Can't read from file
#define MEMORY_ERROR    6	// Insufficient memory available

#define SEEK_STEP               (int)(FRAME_TIME * 1000)

#define ISO_BUFFER_LENGTH       (1024*32)
#define ISO_NBUFFERS            (8)
#define ISO_BUFFERS_SIZE        (ISO_BUFFER_LENGTH*ISO_NBUFFERS)
#define PCM_BUFFER_LENGTH       (4608)

typedef struct {
	FILE           *HANDLE;		// file handle
	unsigned int   FILESIZE;	// compressed size
	unsigned short NCH;		// number of channels
	unsigned short BPS;		// bits per sample
	unsigned short BSIZE;		// byte size
	unsigned short FORMAT;		// audio format
	unsigned int   SAMPLERATE;	// samplerate (sps)
	unsigned int   DATALENGTH;	// data length in samples
	unsigned int   FRAMELEN;	// frame length
	unsigned int   LENGTH;		// playback time (sec)
	unsigned int   STATE;		// return code
	unsigned int   DATAPOS;		// size of ID3v2 header
	unsigned int   BITRATE;		// average bitrate (kbps)
	double         COMPRESS;	// compression ratio
    unsigned int *seek_table;	// the playing position table
    unsigned int st_state;	//seek table status
    unsigned int fframes;	// number of frames in file
    unsigned int framelen;	// the frame length in samples
    unsigned int lastlen;	// the length of the last frame in samples
    unsigned int data_pos;	// currently playing frame index
    unsigned int data_cur;	// the playing position in frame
    int maxvalue;		// output data max value
    unsigned int frame_crc32;
    unsigned int bit_count;
    unsigned int bit_cache;
    unsigned char *bitpos;
    unsigned char isobuffers[ISO_BUFFERS_SIZE + 4];
    unsigned char *iso_buffers_end;
    unsigned int pcm_buffer_size;
    decoder	tta[MAX_NCH];	// decoder state
    int	cache[MAX_NCH];	// decoder cache
} tta_info;

/*********************** Library functions *************************/

int	open_tta_file(		// FUNCTION: opens TTA file
        const char *filename,	// file to open
        tta_info *info,		// file info structure
        unsigned int offset);	// ID3v2 header size
/*
 * RETURN VALUE
 * This function returns 0 if success. Otherwise, -1 is returned
 * and the variable STATE of the currently using info structure
 * is set to indicate the error.
 *
 */

void close_tta_file(	// FUNCTION: closes currently playing file
        tta_info *info);	// file info structure

int	set_position(		// FUNCTION: sets playback position
        tta_info *info, // tta context
        unsigned int pos);	// sample number
/*
 * RETURN VALUE
 * This function returns 0 if success. Otherwise, -1 is returned
 * and the variable STATE of the currently using info structure
 * is set to indicate the error.
 *
 */

int	player_init(		// FUNCTION: initializes TTA player
        tta_info *info);	// file info structure
/*
 * RETURN VALUE
 * This function returns 0 if success. Otherwise, -1 is returned
 * and the variable STATE of the currently using info structure
 * is set to indicate the error.
 *
 */

void player_stop(tta_info *info);	// FUNCTION: destroys memory pools

int	get_samples(		// FUNCTION: decode PCM_BUFFER_LENGTH samples
        tta_info *info,
        char *buffer);	// into the current PCM buffer position
/*
 * RETURN VALUE
 * This function returns the number of samples successfully decoded.
 * Otherwise, -1 is returned and the variable STATE of the currently
 * using info structure is set to indicate the error.
 *
 */

const char *get_error_str(int error); // FUNCTION: get error description


#endif /* TTADEC_H */
