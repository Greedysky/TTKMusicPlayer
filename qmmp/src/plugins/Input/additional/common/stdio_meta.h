/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#ifndef STDIO_META_H
#define STDIO_META_H

#include "stdio_file.h"

typedef struct id3v2_frame_s
{
    struct id3v2_frame_s *next;
    char id[5];
    uint32_t size;
    uint8_t flags[2];
    uint8_t data[0];
} id3v2_frame_t;

typedef struct id3v2_tag_s
{
    uint8_t version[2];
    uint8_t flags;
    id3v2_frame_t *frames;
} id3v2_tag_t;

typedef struct apev2_frame_s
{
    struct apev2_frame_s *next;
    uint32_t flags;
    char key[256];
    uint32_t size;
    uint8_t data[0];
} apev2_frame_t;

typedef struct apev2_tag_s
{
    uint32_t version;
    uint32_t flags;
    apev2_frame_t *frames;
} apev2_tag_t;

typedef struct metaInfo_s
{
    struct metaInfo_s *next;
    const char *key;
    const char *value;
} metaInfo_t;

///
int stdio_apev2_add_frame(metaInfo_t *it, apev2_tag_t *tag_store, apev2_frame_t **tail, uint32_t itemsize, uint32_t itemflags, const char *key, const uint8_t *value);
int stdio_add_track_meta(metaInfo_t *it, const char *track);
int stdio_add_disc_meta(metaInfo_t *it, const char *disc);
int stdio_load_comm_frame(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size);
int stdio_id3v2_sync_frame(uint8_t *data, int size);
int stdio_id3v2_add_genre(metaInfo_t *it, char *genre);
int stdio_id3v2_load_txx(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size);
int stdio_id3v2_load_rva2(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size);
int stdio_id3v2_load_ufid(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size);
///
int stdio_iconv(const char *in, int inlen, char *out, int outlen, const char *cs_in, const char *cs_out);
const char *stdio_detect_charset(const char *s);
const char *stdio_detect_charset_len(const char *s, int len);
///
const char *stdio_find_meta_raw(metaInfo_t *it, const char *key);
void stdio_add_meta(metaInfo_t *it, const char *key, const char *value);
void stdio_append_meta(metaInfo_t *it, const char *key, const char *value);
void stdio_replace_meta(metaInfo_t *it, const char *key, const char *value);
///
int stdio_apev2_read(metaInfo_t *it, apev2_tag_t *tag_store, FILE *fp);
int stdio_id3v2_read(metaInfo_t *it, id3v2_tag_t *tag_store, FILE *fp);
int stdio_id3v1_read(metaInfo_t *it, FILE *fp);
int stdio_id3v1_read_init(metaInfo_t *it, char *buffer, const char **charset);
///

#endif //STDIO_META_H
