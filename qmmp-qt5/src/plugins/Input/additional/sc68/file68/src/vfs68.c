/*
 * @file    istream68.c
 * @brief   VFS functions
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
#include "file68_vfs.h"
#include "file68_vfs_def.h"

const char * vfs68_filename(vfs68_t * vfs)
{
  const char * name =
    (!vfs || !vfs->name)
    ? 0
    : vfs->name(vfs)
    ;
  return name ? name : "<nul>";
}

int vfs68_open(vfs68_t *vfs)
{
  return (!vfs || !vfs->open)
    ? -1
    : vfs->open(vfs)
    ;
}

int vfs68_close(vfs68_t *vfs)
{
  int err = -1;

  if (vfs) {
    if (vfs->flush)
      vfs->flush(vfs);
    if (vfs->close)
      err = vfs->close(vfs);
  }
  return err;
}

int vfs68_read(vfs68_t *vfs, void * data, int len)
{
  return (!vfs || !vfs->read)
    ? -1
    : vfs->read(vfs, data, len);
}

int vfs68_write(vfs68_t *vfs, const void * data, int len)
{
  return (!vfs || !vfs->write)
    ? -1
    : vfs->write(vfs, data, len)
    ;
}

int vfs68_length(vfs68_t *vfs)
{
  return (!vfs || !vfs->length)
    ? -1
    : vfs->length(vfs)
    ;
}

int vfs68_tell(vfs68_t *vfs)
{
  return (!vfs || !vfs->tell)
    ? -1
    : vfs->tell(vfs)
    ;
}

int vfs68_flush(vfs68_t *vfs)
{
  return (!vfs || !vfs->flush)
    ? -1
    : vfs->flush(vfs)
    ;
}

static int isseek(vfs68_t *vfs, int pos, int offset)
{
  if (pos != -1) {
    if (offset) {
      vfs68_seek_t seek = (offset > 0) ? vfs->seekf : vfs->seekb;
      if (seek && seek(vfs, offset) != -1) {
        pos += offset;
      } else {
        pos = -1;
      }
    }
  }
  return pos;
}

int vfs68_seek(vfs68_t *vfs, int offset)
{
  return isseek(vfs, vfs68_tell(vfs), offset);
}

int vfs68_seek_to(vfs68_t *vfs, int pos)
{
  int cur = vfs68_tell(vfs);

  return isseek(vfs, cur, pos-cur);
}

void vfs68_destroy(vfs68_t *vfs)
{
  if (vfs) {
    if (vfs->close) {
      vfs->close(vfs);
    }
    if (vfs->destroy) {
      vfs->destroy(vfs);
    }
  }
}

int vfs68_gets(vfs68_t *vfs, char * buffer, int max)
{
  int i;

  if (!vfs || !vfs->read || !buffer || max <= 0) {
    return -1;
  }

  for (i=0, --max; i<max; ) {
    char c;
    int err;
    err = vfs->read(vfs, &c, 1);
    if (err == -1) {
      return -1;
    }
    if (err != 1) {
      break;
    }
    buffer[i++] = c;
    if (c == '\n') {
      break;
    }
  }
  buffer[i] = 0;
  return i;
}

int vfs68_getc(vfs68_t *vfs)
{
  unsigned char c;

  return vfs68_read(vfs,&c,1) != 1
    ? -1
    : c;
}

int vfs68_puts(vfs68_t *vfs, const char * s)
{
  int err = 0;
  if (s) {
    int c;
    while (!err && (c = *s++)) {
      err = vfs68_putc(vfs, c);
    }
  }
  return err;
}

int vfs68_putc(vfs68_t *vfs, const int c)
{
  unsigned char byte = c;
  return -(vfs68_write(vfs,&byte,1) != 1);
}
