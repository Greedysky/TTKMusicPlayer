/***************************************************************************
 *   Copyright (C) 2011-2024 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef FFAP_H
#define FFAP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//callbacks
typedef size_t (*ffap_read_callback) (void *ptr, size_t size, size_t nmemb, void *client_data);
typedef int (*ffap_seek_callback) (int64_t offset, int whence, void *client_data);
typedef int64_t (*ffap_tell_callback) (void *client_data);
typedef int64_t (*ffap_getlength_callback) (void *client_data);

struct APEContext;

typedef struct
{
    ffap_read_callback read;
    ffap_seek_callback seek;
    ffap_tell_callback tell;
    ffap_getlength_callback getlength;
    void *client_data;
    int bps;
    int channels;
    int samplerate;
    int bitrate;
    float readpos;
    float duration;
    //private
    struct APEContext *ape_ctx;
}
FFap_decoder;

FFap_decoder *ffap_new(ffap_read_callback read_callback,
                       ffap_seek_callback seek_callback,
                       ffap_tell_callback tell_callback,
                       ffap_getlength_callback getlength_callback,
                       void *client_data);

void ffap_load();
int ffap_init(FFap_decoder *decoder);
int ffap_read(FFap_decoder *decoder, unsigned char *buffer, int size);
int ffap_seek(FFap_decoder *decoder, float seconds);
void ffap_free(FFap_decoder *decoder);

#ifdef __cplusplus
}
#endif

#endif
