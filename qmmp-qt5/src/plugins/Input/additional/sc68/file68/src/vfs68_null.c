/*
 * @file    istream68_null.c
 * @brief   implements istream68 VFS for for null
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
#include "file68_vfs_null.h"

/* define this if you don't want NULL support. */
#ifndef VFS68_NO_NULL

#include "file68_vfs_def.h"
#include "file68_uri.h"
#include "file68_str.h"

#include <string.h>
#include <stdlib.h>

/** vfs file structure. */
typedef struct {
  vfs68_t vfs; /**< vfs function.           */
  int size;            /**< max pos R or W.             */
  int pos;             /**< current position.           */
  int open;            /**< has been opened.            */
  char name[1];        /**< filename (null://filename). */
} vfs68_null_t;

static int null_ismine(const char *);
static vfs68_t * null_create(const char *, int, int, va_list);
static scheme68_t null_scheme = {
  0, "vfs-null", null_ismine, null_create
};

static int null_ismine(const char * uri)
{
  if (!strncmp68(uri, "null:", 5))
    return SCHEME68_ISMINE | SCHEME68_READ | SCHEME68_WRITE;
  return 0;
}

static const char * isn_name(vfs68_t * vfs)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  return (!isn->name[0])
    ? 0
    : isn->name
    ;
}

static int isn_open(vfs68_t * vfs)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  if (isn->open) {
    return -1;
  }
  isn->open = 1;
  isn->pos  = 0;
  isn->size = 0;
  return 0;
}

static int isn_close(vfs68_t * vfs)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  if (!isn->open) {
    return -1;
  }
  isn->open = 0;
  return 0;
}

static int isn_read_or_write(vfs68_t * vfs, int n)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  if (!isn->open || n < 0) {
    return -1;
  }
  if (n) {
    /* No op: do not update size */
    isn->pos += n;
    if (isn->pos > isn->size) {
      isn->size = isn->pos;
    }
  }
  return n;
}

static int isn_read(vfs68_t * vfs, void * data, int n)
{
  return isn_read_or_write(vfs, n);
}

static int isn_write(vfs68_t * vfs, const void * data, int n)
{
  return isn_read_or_write(vfs, n);
}

static int isn_flush(vfs68_t * vfs)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;
  return -!isn->open;
}

static int isn_length(vfs68_t * vfs)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  return isn->size;
}

static int isn_tell(vfs68_t * vfs)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  return !isn->open
    ? -1
    : isn->pos
    ;
}

static int isn_seek(vfs68_t * vfs, int offset)
{
  vfs68_null_t * isn = (vfs68_null_t *)vfs;

  if (isn) {
    offset += isn->pos;
    if (offset >= 0) {
      isn->pos = offset;
      return 0;
    }
  }
  return -1;
}

static void isn_destroy(vfs68_t * vfs)
{
  free(vfs);
}

static const vfs68_t vfs68_null = {
  isn_name,
  isn_open, isn_close,
  isn_read, isn_write, isn_flush,
  isn_length, isn_tell, isn_seek, isn_seek,
  isn_destroy
};

int vfs68_null_init(void)
{
  return uri68_register(&null_scheme);
}

void vfs68_null_shutdown(void)
{
  uri68_unregister(&null_scheme);
}

static vfs68_t * null_create(const char * uri, int mode, int argc, va_list list)
{
  vfs68_null_t * isn;
  int size;

  if (!null_ismine(uri))
    return 0;

  size = sizeof(vfs68_null_t) + strlen(uri);

  isn = malloc(size);
  if (!isn) {
    return 0;
  }

  isn->vfs     = vfs68_null;
  isn->size    = 0;
  isn->pos     = 0;
  isn->open    = 0;
  strcpy(isn->name,uri);

  return &isn->vfs;
}

#else /* #ifndef VFS68_NO_NULL */

/* vfs mem must not be include in this package. Anyway the creation
 * still exist but it always returns error.
 */

#include "file68_vfs_null.h"
#include "file68_msg.h"

int vfs68_null_init(void) { return 0; }
void vfs68_null_shutdown(void) { }

#endif
