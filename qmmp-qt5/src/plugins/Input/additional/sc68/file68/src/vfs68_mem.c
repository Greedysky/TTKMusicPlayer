/*
 * @file    istream68_mem.c
 * @brief   implements istream68 VFS for memory buffer
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "file68_api.h"
#include "file68_vfs_mem.h"

/* define this if you don't want MEM support. */
#ifndef ISTREAM68_NO_MEM

#include "file68_vfs_def.h"
#include "file68_uri.h"
#include "file68_str.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** vfs file structure. */
typedef struct {
  vfs68_t vfs;                  /**< vfs function.   */
  char * buffer;                        /**< memory buffer.      */
  int size;                             /**< memory buffer size. */
  int pos;                              /**< current position.   */

  /** Open modes. */
  int mode;        /**< Allowed open mode bit-0:read bit-1:write.   */
  int open;        /**< Currently open mode bit-0:read bit-1:write. */

  /* MUST BE at the end of the structure because supplemental bytes will
   * be allocated to store filename.
   */
  char name[16 + 2 * 2 * sizeof(void*)]; /**< filename (mem://start:end). */
  char internal[4];
} vfs68_mem_t;

static int mem_ismine(const char *);
static vfs68_t * mem_create(const char *, int, int, va_list);
static scheme68_t mem_scheme = {
  0, "vfs-mem", mem_ismine, mem_create
};

static int mem_ismine(const char * uri)
{
  if (!strncmp68(uri,"mem:", 4))
    return SCHEME68_ISMINE | SCHEME68_READ | SCHEME68_WRITE;
  return 0;
}

static const char * ism_name(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;

  return !ism->name[0]
    ? 0
    : ism->name
    ;
}

static int ism_open(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;

  if (!VFS68_IS_OPEN(ism->mode) || ism->open) {
    return -1;
  }
  ism->open = ism->mode & VFS68_OPEN_MASK;
  ism->pos = 0;
  return 0;
}

static int ism_close(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;

  if (!ism->open) {
    return -1;
  }
  ism->open = 0;
  return 0;
}

static int ism_read_or_write(vfs68_t * vfs, void * data, int n, int mode)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;
  int pos, endpos;

  if (!(ism->open & mode) || n < 0) {
    return -1;
  }
  if (!n) {
    return 0;
  }

  pos = ism->pos;
  endpos = pos + n;
  if (endpos > ism->size) {
    endpos = ism->size;
    n = endpos - pos;
  }
  if (n > 0) {
    void *src, *dst;
    if (mode == VFS68_OPEN_READ) {
      src = ism->buffer+pos;
      dst = data;
    } else {
      src = data;
      dst = ism->buffer+pos;
    }
    memcpy(dst, src, n);
  }
  ism->pos = endpos;
  return n;
}

static int ism_read(vfs68_t * vfs, void * data, int n)
{
  return ism_read_or_write(vfs, data, n, VFS68_OPEN_READ);
}

static int ism_write(vfs68_t * vfs, const void * data, int n)
{
  void * rw = (void *)data;
  return ism_read_or_write(vfs, rw, n, VFS68_OPEN_WRITE);
}

static int ism_flush(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;
  return -!ism->open;
}

static int ism_length(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;

  return !ism->open
    ? -1
    : ism->size
    ;
}

static int ism_tell(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;

  return !ism->open
    ? -1
    : ism->pos
    ;
}

static int ism_seek(vfs68_t * vfs, int offset)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;
  int pos;

  if (!ism->open) {
    return -1;
  }
  pos = ism->pos + offset;
  if (pos < 0 || pos > ism->size) {
    return -1;
  }
  ism->pos = pos;
  return 0;
}

static void ism_destroy(vfs68_t * vfs)
{
  vfs68_mem_t * ism = (vfs68_mem_t *)vfs;

  if (ism && VFS68_IS_SLAVE(ism->mode) && ism->buffer != ism->internal)
    free(ism->buffer);
  free(ism);
}

static const vfs68_t vfs68_mem = {
  ism_name,
  ism_open, ism_close,
  ism_read, ism_write, ism_flush,
  ism_length, ism_tell, ism_seek, ism_seek,
  ism_destroy
};

vfs68_t * vfs68_mem_create(const void * addr, int len, int mode)
{
  vfs68_mem_t *ism;
  int size;

  if (len < 0)
    return 0;

  size = sizeof(vfs68_mem_t) + (!addr ? len : 0);
  ism = malloc(size);
  if (!ism)
    return 0;
  if (!addr)
    addr = ism->internal;
  ism->vfs = vfs68_mem;
  ism->buffer  = (char *)addr;
  ism->size    = len;
  ism->mode    = mode;
  ism->open    = 0;
  ism->pos     = 0;
  sprintf(ism->name,"mem://%p:%p", addr, (char *)addr+len);

  return &ism->vfs;
}

static vfs68_t * mem_create(const char * uri, int mode,
                            int argc, va_list list)
{
  if (mem_ismine(uri) && argc == 2) {
    void * addr = va_arg(list, void *);
    int len     = va_arg(list, int);
    return vfs68_mem_create(addr, len, mode);
  }
  return 0;
}

int vfs68_mem_init(void)
{
  return uri68_register(&mem_scheme);
}

void vfs68_mem_shutdown(void)
{
  uri68_unregister(&mem_scheme);
}

#else /* #ifndef VFS68_NO_FILE */

/* vfs mem must not be include in this package. Anyway the creation
 * still exist but it always returns error.
 */

#include "file68_fvs_mem.h"
#include "file68_msg.h"

int vfs68_mem_init(void) { return 0; }
void vfs68_mem_shutdown(void) { }

#endif
