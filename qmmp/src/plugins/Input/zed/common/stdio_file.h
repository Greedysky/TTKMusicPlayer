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

#ifndef STDIO_FILE_H
#define STDIO_FILE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#ifndef	MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef	MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

///
FILE * stdio_open(const char *fname);
void stdio_close(FILE *stream);
size_t stdio_read(void *ptr, size_t size, size_t nmemb, FILE *stream);
int stdio_seek(FILE *stream, int64_t offset, int whence);
int64_t stdio_tell(FILE *stream) ;
void stdio_rewind(FILE *stream);
int64_t stdio_length(FILE *stream);
int stdio_get_leading_size(FILE *stream);
///

#endif //STDIO_FILE_H
