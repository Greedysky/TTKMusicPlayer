/*
 * @file    gzip68.c
 * @brief   gzip file loader
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
#include "file68_zip.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
typedef uint8_t byte;
#else
typedef unsigned char byte;
#endif

#ifdef USE_Z
# include <zlib.h>
/* gzip flag byte (from gzio.c) */
# define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text   */
# define HEAD_CRC     0x02 /* bit 1 set: header CRC present         */
# define EXTRA_FIELD  0x04 /* bit 2 set: extra field present        */
# define ORIG_NAME    0x08 /* bit 3 set: original file name present */
# define COMMENT      0x10 /* bit 4 set: file comment present       */
# define RESERVED     0xE0 /* bits 5..7: reserved                   */
#else
# define Z_DEFLATED   8 /* From zlib.h */
#endif

/* gzip magic header */
static byte gz_magic[] = {0x1f, (char)0x8b, Z_DEFLATED};

int gzip68_is_magic(const void * buffer)
{
  return !memcmp(gz_magic, buffer, sizeof(gz_magic));
}

#ifdef USE_Z

#include "file68_err.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef _MSC_VER
# include <stdio.h>
# define __STDC__ 0
# include <io.h>
#elif defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif


static int is_gz(int fd, int len)
{
  byte magic[sizeof(gz_magic)];
  int inflate_len = -1;

  /* header + inflate len */
  if (len < 10+4) {
    goto error;
  }

  /* Read and check magic. */
  if (read(fd, magic, sizeof(magic)) != sizeof(magic) ||
      memcmp(magic, gz_magic, sizeof(magic))) {
    goto error;
  }

  /* Get uncompressed size at the end of file.
   */
  if (lseek(fd, -4, SEEK_END) == (off_t) -1) {
    goto error;
  } else {
    unsigned char buffer[4];
    if(read(fd, buffer, 4) != 4) {
      goto error;
    }
    inflate_len =
      buffer[0]
      | (buffer[1] << 8)
      | (buffer[2] << 16)
      | (buffer[3] << 24);
    if (inflate_len < 0) {
      inflate_len = -1;
    }
  }
error:

  /* Rewind */
  lseek(fd, 0, SEEK_SET);
  return inflate_len;
}

/* Windows opens streams in Text mode by default. */
#ifndef _O_BINARY
# ifdef O_BINARY
#  define _O_BINARY O_BINARY
# else
#  define _O_BINARY 0
# endif
#endif

void *gzip68_load(const char *fname, int *ptr_ulen)
{
  int fd, err;
  gzFile f = 0;
  int ulen = 0;
  void * uncompr = 0;
  off_t len;
  const int omode = O_RDONLY | _O_BINARY;

  fd = open(fname, omode);
  if (fd == -1) {
    error68("gzip68: load '%s' -- %s", fname, strerror(errno));
    goto error;
  }

  len = lseek(fd, 0, SEEK_END);
  if (len == (off_t) -1) {
    error68("gzip68: load '%s' -- %s", fname, strerror(errno));
    goto error;
  }

  if (lseek(fd, 0, SEEK_SET) != 0) {
    error68("gzip68: load '%s' -- %s", fname, strerror(errno));
    goto error;
  }

  ulen = is_gz(fd, len);
  if (ulen == -1) {
    ulen = len;
  }

  f = gzdopen(fd, "rb");
  if (!f) {
    error68("gzip68: load '%s' -- %s", fname, gzerror(f, &err));
    goto error;
  }
  fd = 0; /* $$$ Closed by gzclose(). Verify fdopen() rules. */

  uncompr = malloc(ulen);
  if (!uncompr) {
    error68("gzip68: load '%s' -- alloc error", fname);
    goto error;
  }
  len = gzread(f, uncompr, ulen);

  if (len != ulen) {
    error68("gzip68: load '%s' -- read error (%s)",fname, gzerror(f, &err));
    goto error;
  }
  goto end;

error:
  if (uncompr) {
    free(uncompr);
    uncompr = 0;
    ulen = 0;
  }

end:
  if (fd) {
    close(fd);
  }
  if (f) {
    gzclose(f);
  }
  if (ptr_ulen) {
    *ptr_ulen = ulen;
  }

  return uncompr;
}

int gzip68_buffer(void * dst, int dsize, const void * src, int csize)
{
  const struct {
    byte magic[2]; /* gz header                */
    byte method;   /* method (Z_DEFLATED)      */
    byte flags;    /* Xtra,name,comment...     */
    byte info[6];  /* time, xflags and OS code */
  } * header = src;
  int err = -(!dst || !src || csize < sizeof(*header) || dsize <= 0);
  byte crc[2];
  z_stream c_stream;

  for (;;) {
    int len = sizeof(*header), flags;
    byte * d = (byte *)dst;
    const byte * s = (const byte *) src;

    /* check gzip header */
    if (0
        || gz_magic[0] != header->magic[0]
        || gz_magic[1] != header->magic[1]
        || header->method != Z_DEFLATED
        || (header->flags & RESERVED) != 0) {
      err = -1;
      break;
    }

    /* Skip the extra field. */
    if ((header->flags & EXTRA_FIELD) != 0) {
      if (len + 2 < csize)
        len += s[len] + (s[len+1]<<8);
      len += 2;
    }

    /* Skip the original file name & .gz file comment. */
    flags = header->flags & (ORIG_NAME|COMMENT);
    while (len < csize && flags) {
      if (flags & ORIG_NAME) {
        flags &= ~ORIG_NAME;
      } else {
        flags &= ~COMMENT;
      }
      while (len < csize && !!s[len++])
        ;
    }

    /* Skip the header crc */
    if (header->flags & HEAD_CRC) {
      if (len + 2 < csize) {
        crc[0] = s[len+0];
        crc[1] = s[len+1];
      } else {
        crc[0] = crc[1] = 0;
      }
      len += 2;
    }

    /* Deflate now  */
    if (len < csize) {
      const int z_flush_mode = Z_FINISH;

      memset(&c_stream,0,sizeof(c_stream));
      err = -(Z_OK != inflateInit2(&c_stream, -MAX_WBITS));
      if (err)
        break;

      c_stream.next_in   = (byte *)(s + len);
      c_stream.avail_in  = csize - len;
      c_stream.next_out  = d;
      c_stream.avail_out = dsize;
      err = inflate(&c_stream, z_flush_mode);
      inflateEnd(&c_stream);
      if (err != Z_STREAM_END) {
        err = -1;
      } else {
        err = c_stream.total_out;
      }
    } else {
      err = -1;
    }
    break;
  }

  return err;
}

#else

#include "file68_err.h"

void *gzip68_load(const char *fname, int *ptr_ulen)
{
  if (ptr_ulen) *ptr_ulen=0;
  error68("gzip68: *NOT SUPPORTED*");
  return 0;
}

int gzip68_buffer(void * dst, int dsize, const void * src, int csize)
{
  error68("gzip68: *NOT SUPPORTED*");
  return -1;
}

#endif /* #ifdef HAVE_ZLIB_H */
