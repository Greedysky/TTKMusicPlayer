/*
 * ttadec.c
 *
 * Description:	 TTAv1 decoder library for HW players
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

#include "ttadec.h"
#include "filter.h"
#include "stdio_file.h"

/******************* static variables and structures *******************/

const unsigned int crc32_table[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
	0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
	0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
	0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
	0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
	0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
	0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
	0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
	0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
	0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
	0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
	0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
	0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
	0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
	0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
}; 

const unsigned int bit_mask[] = {
	0x00000000, 0x00000001, 0x00000003, 0x00000007,
	0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
	0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
	0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
	0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
	0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
	0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
	0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
	0xffffffff
};

const unsigned int bit_shift[] = {
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x80000000, 0x80000000, 0x80000000, 0x80000000,
	0x80000000, 0x80000000, 0x80000000, 0x80000000
};

const unsigned int *shift_16 = bit_shift + 4;

static int skip_id3_tag (tta_info *info);

/************************* crc32 functions *****************************/

#define UPDATE_CRC32(x, crc) crc = \
	(((crc>>8) & 0x00FFFFFF) ^ crc32_table[(crc^x) & 0xFF])

static unsigned int 
crc32 (unsigned char *buffer, unsigned int len) {
	unsigned int i;
	unsigned int crc = 0xFFFFFFFF;

	for (i = 0; i < len; i++) UPDATE_CRC32(buffer[i], crc);

	return (crc ^ 0xFFFFFFFF);
}

/************************* bit operations ******************************/

#define GET_BINARY(value, bits) \
	while (info->bit_count < bits) { \
		if (info->bitpos == info->iso_buffers_end) { \
			if (!stdio_read(info->isobuffers, 1, \
			    ISO_BUFFERS_SIZE, info->HANDLE)) { \
			    info->STATE = READ_ERROR; \
			    return -1; } \
			info->bitpos = info->isobuffers; } \
		UPDATE_CRC32(*info->bitpos, info->frame_crc32); \
		info->bit_cache |= *info->bitpos << info->bit_count; \
		info->bit_count += 8; \
		info->bitpos++; } \
	value = info->bit_cache & bit_mask[bits]; \
	info->bit_cache >>= bits; \
	info->bit_count -= bits; \
	info->bit_cache &= bit_mask[info->bit_count];

#define GET_UNARY(value) \
	value = 0; \
	while (!(info->bit_cache ^ bit_mask[info->bit_count])) { \
		if (info->bitpos == info->iso_buffers_end) { \
			if (!stdio_read(info->isobuffers, 1, \
			    ISO_BUFFERS_SIZE, info->HANDLE)) { \
			    info->STATE = READ_ERROR; \
			    return -1; } \
			info->bitpos = info->isobuffers; } \
		value += info->bit_count; \
		info->bit_cache = *info->bitpos++; \
		UPDATE_CRC32(info->bit_cache, info->frame_crc32); \
		info->bit_count = 8; } \
	while (info->bit_cache & 1) { \
		value++; \
		info->bit_cache >>= 1; \
		info->bit_count--; } \
	info->bit_cache >>= 1; \
	info->bit_count--;

static void init_buffer_read(tta_info *info) {
	info->frame_crc32 = 0xFFFFFFFFUL;
	info->bit_count = info->bit_cache = 0;
	info->bitpos = info->iso_buffers_end;
}

static int done_buffer_read(tta_info *info) {
	unsigned int crc32, rbytes;

	info->frame_crc32 ^= 0xFFFFFFFFUL;
	rbytes = info->iso_buffers_end - info->bitpos;

	if (rbytes < sizeof(int)) {
	    memcpy(info->isobuffers, info->bitpos, 4);
	    if (!stdio_read(info->isobuffers + rbytes, 1,
		ISO_BUFFERS_SIZE - rbytes, info->HANDLE))
		return -1;
	    info->bitpos = info->isobuffers;
	}

	memcpy(&crc32, info->bitpos, 4);
	crc32 = ENDSWAP_INT32(crc32);
	info->bitpos += sizeof(int);
    
	if (crc32 != info->frame_crc32) return -1;

	info->bit_cache = info->bit_count = 0;
	info->frame_crc32 = 0xFFFFFFFFUL;

	return 0;
}

/************************* decoder functions ****************************/

const char *get_error_str (int error) {
	switch (error) {
	case NO_ERROR:      return "No errors found";
	case OPEN_ERROR:    return "Can't open file";
	case FORMAT_ERROR:  return "Not supported file format";
	case FILE_ERROR:    return "File is corrupted";
	case READ_ERROR:    return "Can't read from file";
	case MEMORY_ERROR:  return "Insufficient memory available";
	default:            return "Unknown error code";
	}
}

int open_tta_file (const char *filename, tta_info *info, unsigned int data_offset) {
	unsigned int checksum;
	unsigned int datasize;
    unsigned int origsize;
    FILE *infile;

	// clear the memory
    memset (info, 0, sizeof(tta_info));
    // open file
    infile = stdio_open(filename);
    if (!infile) {
        info->STATE = OPEN_ERROR;
        return -1;
    }
    info->HANDLE = infile;
    // get file size
    info->FILESIZE = stdio_length(infile);
    // read id3 tags
    if (!data_offset) {
        if ((data_offset = skip_id3_tag (info)) < 0) {
            stdio_close(infile);
            return -1;
        }
    } else {
        stdio_seek(infile, data_offset, SEEK_SET);
    }

    tta_hdr ttahdr;
    // read TTA header
    if (stdio_read(&ttahdr, 1, sizeof (ttahdr), infile) == 0) {
        printf("tta: failed to read header\n");
        stdio_close(infile);
        info->STATE = READ_ERROR;
        return -1;
    }

    // check for TTA3 signature
    if (ENDSWAP_INT32(ttahdr.TTAid) != TTA1_SIGN) {
        printf ("tta: format error (wrong signature)\n");
        stdio_close(infile);
        info->STATE = FORMAT_ERROR;
        return -1;
    }

    ttahdr.CRC32 = ENDSWAP_INT32(ttahdr.CRC32);
    checksum = crc32((unsigned char *) &ttahdr,
    sizeof(tta_hdr) - sizeof(int));
    if (checksum != ttahdr.CRC32) {
        printf ("tta: file error: crc32 mismatch\n");
        stdio_close(infile);
        info->STATE = FILE_ERROR;
        return -1;
    }

    ttahdr.AudioFormat = ENDSWAP_INT16(ttahdr.AudioFormat);
    ttahdr.NumChannels = ENDSWAP_INT16(ttahdr.NumChannels);
    ttahdr.BitsPerSample = ENDSWAP_INT16(ttahdr.BitsPerSample);
    ttahdr.SampleRate = ENDSWAP_INT32(ttahdr.SampleRate);
    ttahdr.DataLength = ENDSWAP_INT32(ttahdr.DataLength);

    // check for player supported formats
    if (ttahdr.AudioFormat != WAVE_FORMAT_PCM ||
        ttahdr.NumChannels > MAX_NCH ||
        ttahdr.BitsPerSample > MAX_BPS) {
        printf ("tta: format error: invalid samplerate\n");
        stdio_close(infile);
        info->STATE = FORMAT_ERROR;
        return -1;
    }

    // fill the File Info
    info->NCH = ttahdr.NumChannels;
    info->BPS = ttahdr.BitsPerSample;
    info->BSIZE = (ttahdr.BitsPerSample + 7)/8;
    info->FORMAT = ttahdr.AudioFormat;
    info->SAMPLERATE = ttahdr.SampleRate;
    info->DATALENGTH = ttahdr.DataLength;
    info->FRAMELEN = (int) (FRAME_TIME * ttahdr.SampleRate);
    info->LENGTH = ttahdr.DataLength / ttahdr.SampleRate;
    info->DATAPOS = data_offset;

    datasize = info->FILESIZE - info->DATAPOS;
    origsize = info->DATALENGTH * info->BSIZE * info->NCH;

    info->COMPRESS = (double) datasize / origsize;
    info->BITRATE = (int) (info->COMPRESS * info->SAMPLERATE * info->NCH * info->BPS / 1000);

	return 0;
}

static void rice_init(adapt *rice, unsigned int k0, unsigned int k1) {
	rice->k0 = k0;
	rice->k1 = k1;
	rice->sum0 = shift_16[k0];
	rice->sum1 = shift_16[k1];
}

static void decoder_init(decoder *tta, int nch, int byte_size) {
	int shift = flt_set[byte_size - 1];
	int i;

	for (i = 0; i < nch; i++) {
		filter_init(&tta[i].fst, shift);
		rice_init(&tta[i].rice, 10, 10);
		tta[i].last = 0;
	}
}

static void seek_table_init (unsigned int *seek_table,
	unsigned int len, unsigned int data_offset) {
	unsigned int *st, frame_len;

	for (st = seek_table; st < (seek_table + len); st++) {
		frame_len = ENDSWAP_INT32(*st);
		*st = data_offset;
		data_offset += frame_len;
	}
}

int set_position (tta_info *info, unsigned int _pos) {
    // pos: seek position = seek_time_ms / SEEK_STEP
//    int pos = _pos / SEEK_STEP * 1000 / info->SAMPLERATE;
    int pos = _pos / info->FRAMELEN;
    unsigned int seek_pos;

	if (pos >= info->fframes) return 0;
	if (!info->st_state) {
		info->STATE = FILE_ERROR;
		return -1;
	}

	seek_pos = info->DATAPOS + info->seek_table[info->data_pos = pos];
	if (stdio_seek(info->HANDLE, seek_pos, SEEK_SET) < 0) {
		info->STATE = READ_ERROR;
		return -1;
	}

	info->data_cur = 0;
	info->framelen = 0;

	// init bit reader
	init_buffer_read(info);

    printf ("seek to sample %d, skip %d (%d - %d * %d)\n", _pos, _pos - pos * info->FRAMELEN, _pos, pos, info->FRAMELEN);

	return _pos - pos * info->FRAMELEN;
}

int player_init (tta_info *info) {
	unsigned int checksum;
	unsigned int data_offset;
	unsigned int st_size;
	info->iso_buffers_end = info->isobuffers + ISO_BUFFERS_SIZE;

	info->framelen = 0;
	info->data_pos = 0;
	info->data_cur = 0;

	info->lastlen = info->DATALENGTH % info->FRAMELEN;
	info->fframes = info->DATALENGTH / info->FRAMELEN + (info->lastlen ? 1 : 0);
	st_size = (info->fframes + 1) * sizeof(int);

	info->seek_table = (unsigned int *) malloc(st_size);
	if (!info->seek_table) {
		info->STATE = MEMORY_ERROR;
		return -1;
	}

	// read seek table
	if (!stdio_read(info->seek_table, st_size, 1, info->HANDLE)) {
		info->STATE = READ_ERROR;
		return -1;
	}

	checksum = crc32((unsigned char *) info->seek_table, st_size - sizeof(int));
	info->st_state = (checksum == ENDSWAP_INT32(info->seek_table[info->fframes]));
	data_offset = sizeof(tta_hdr) + st_size;

	// init seek table
	seek_table_init(info->seek_table, info->fframes, data_offset);

	// init bit reader
	init_buffer_read(info);

	info->pcm_buffer_size = PCM_BUFFER_LENGTH * info->BSIZE * info->NCH;
	info->maxvalue = (1UL << info->BPS) - 1;

	return 0;
}

void close_tta_file (tta_info *info) {
	if (info->HANDLE) {
		stdio_close(info->HANDLE);
		info->HANDLE = NULL;
	}
}

void player_stop (tta_info *info) {
	if (info->seek_table) {
		free(info->seek_table);
		info->seek_table = NULL;
	}
}

int get_samples (tta_info *info, char *buffer) {
	unsigned int k, depth, unary, binary;
    char *p = buffer;
	decoder *dec = info->tta;
	int *prev = info->cache;
	int value, res;

	for (res = 0; p < buffer + info->pcm_buffer_size;) {
		fltst *fst = &dec->fst;
		adapt *rice = &dec->rice;
		int *last = &dec->last;

		if (info->data_cur == info->framelen) {
			if (info->data_pos == info->fframes) break;
			if (info->framelen && done_buffer_read(info)) {
			    if (set_position(info, info->data_pos)) return -1;
			    if (res) break;
			}

			if (info->data_pos == info->fframes - 1 && info->lastlen)
				info->framelen = info->lastlen;
			else info->framelen = info->FRAMELEN;

			decoder_init(info->tta, info->NCH, info->BSIZE);
			info->data_pos++; info->data_cur = 0;
		}

		// decode Rice unsigned
		GET_UNARY(unary);

		switch (unary) {
		case 0: depth = 0; k = rice->k0; break;
		default:
			depth = 1; k = rice->k1;
			unary--;
		}

		if (k) {
			GET_BINARY(binary, k);
			value = (unary << k) + binary;
		} else value = unary;

		switch (depth) {
		case 1: 
			rice->sum1 += value - (rice->sum1 >> 4);
			if (rice->k1 > 0 && rice->sum1 < shift_16[rice->k1])
				rice->k1--;
			else if (rice->sum1 > shift_16[rice->k1 + 1])
				rice->k1++;
			value += bit_shift[rice->k0];
		default:
			rice->sum0 += value - (rice->sum0 >> 4);
			if (rice->k0 > 0 && rice->sum0 < shift_16[rice->k0])
				rice->k0--;
			else if (rice->sum0 > shift_16[rice->k0 + 1])
			rice->k0++;
		}

		value = DEC(value);

		// decompress stage 1: adaptive hybrid filter
		hybrid_filter(fst, &value);

		// decompress stage 2: fixed order 1 prediction
		switch (info->BSIZE) {
		case 1: value += PREDICTOR1(*last, 4); break;	// bps 8
		case 2: value += PREDICTOR1(*last, 5); break;	// bps 16
		case 3: value += PREDICTOR1(*last, 5); break;	// bps 24
		} *last = value;

		// check for errors
		if (abs(value) > info->maxvalue) {
			unsigned int tail =
				info->pcm_buffer_size / (info->BSIZE * info->NCH) - res;
			memset(buffer, 0, info->pcm_buffer_size);
			info->data_cur += tail; res += tail;
			break;
		}

		if (dec < info->tta + (info->NCH - 1)) {
			*prev++ = value; dec++;
		} else {
			*prev = value;
			if (info->NCH > 1) {
				int *r = prev - 1;
				for (*prev += *r/2; r >= info->cache; r--)
					*r = *(r + 1) - *r;
				for (r = info->cache; r < prev; r++)
					WRITE_BUFFER(r, info->BSIZE, p)
			}
			WRITE_BUFFER(prev, info->BSIZE, p)
			prev = info->cache;
			info->data_cur++; res++;
			dec = info->tta;
		}
	}

	return res;
}

/*
 * Description:	 ID3 tags manipulation routines
 *               Provides read access to ID3 tags v1.1, v2.3.x, v2.4.x
 *               Supported ID3v2 frames: Title, Artist, Album, Track,
 *               Year, Genre, Comment.
 *
 * Copyright (c) 2004 Alexander Djourik. All rights reserved.
 *
 */

static int skip_id3_tag (tta_info *info) {
    int id3v2_size = stdio_get_leading_size(info->HANDLE);
    printf ("id3v2_size: %d\n", id3v2_size);
	if (id3v2_size < 0) {
        id3v2_size = 0;
        stdio_rewind(info->HANDLE);
    }
    else {
        stdio_seek(info->HANDLE, id3v2_size, SEEK_SET);
    }

	return id3v2_size;
}

/* eof */
