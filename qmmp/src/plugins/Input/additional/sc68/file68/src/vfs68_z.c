/*
 * @file    istream68_z.c
 * @brief   implements vfs68 VFS for gzip
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
#include "file68_vfs_z.h"
#include "file68_msg.h"

/** Default gzip option. */
const vfs68_z_option_t vfs68_z_default_option = {
  1,   /* gzip      */
  3,   /* level     */
  0,   /* strategy  */
  1    /* name      */
};

#ifdef USE_Z

#include "file68_vfs_def.h"
#include "file68_str.h"

#include <zlib.h>
#include <string.h>
#include <stdlib.h>

#ifndef DEBUG_ZLIB_O
# define DEBUG_ZLIB_O 0
#endif
static int zlib_cat = msg68_DEFAULT;

/* defined in zutil.h */
#ifndef DEF_MEM_LEVEL
# define DEF_MEM_LEVEL 8
#endif

/* gzip flag byte (from gzio.c) */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text   */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present         */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present        */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present       */
#define RESERVED     0xE0 /* bits 5..7: reserved                   */

/** vfs Z structure. */
typedef struct {
  vfs68_t vfs;     /**< vfs function.     */
  vfs68_t * is;        /**< Wrapped stream.       */
  unsigned int mode:2;     /**< Available open modes. */
  unsigned int is_slave:1; /**< slave mode.           */
  unsigned int is_err:1;   /**< Last is op failed.    */
  unsigned int is_eof:1;   /**< is EOF.               */
  unsigned int gzip:1;     /**< Gzip stream.          */
  unsigned int gz_name:1;  /**< Add filename.         */
  unsigned int level:4;    /**< Compression level.    */
  unsigned int strategy:2; /**< Compression startegy. */
  unsigned int inflate:1;  /**< Inflate inited.       */
  unsigned int deflate:1;  /**< Deflate inited.       */
  unsigned int hd_crc:17;  /**< Header CRC16 value.   */
  unsigned int c_crc32;    /**< Computed CRC32 value. */
  unsigned int gz_crc32;   /**< Read CRC32 value.     */
  unsigned int gz_len;     /**< Read length value.    */

  int pos;                 /**< Current R/W position. */
  int org_pos;             /**< Z stream start pos.   */
  int length;              /**< Known length.         */
  z_stream c_stream;       /**< Z compression stream. */
  Byte * write_in;         /**< Write buffer_in pos.  */
  Byte * read_out;         /**< Read buffer_out pos.  */
  Byte buffer_in[512];     /**< Input buffer.         */
  Byte buffer_out[512];    /**< Output buffer.        */
} vfs68_z_t;

static voidpf isf_zcalloc(voidpf opaque, unsigned items, unsigned size)
{
  opaque = opaque;
  return calloc(items*size,1);
}

static void isf_zcfree(voidpf opaque, voidpf ptr)
{
  opaque = opaque;
  free(ptr);
}

static const char * isf_name(vfs68_t * vfs)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;

  return (!isf)
    ? 0
    : vfs68_filename(isf->is);
}

/* Fill input buffer.
 *
 * @return number of bytes read or -1
 */
static int isf_fill_input_buffer(vfs68_z_t * isf, const int max)
{
  int n;

  /* Try to fill whole buffer */
  n = vfs68_read(isf->is, isf->buffer_in, max);
  isf->is_eof = (n != max);
  isf->is_err = (n == -1);
  isf->c_stream.next_in = isf->buffer_in;
  isf->c_stream.avail_in = n; /* -1: generates error in isf_read_buffer() */
  return n;
}

/* Flush output buffer.
 *
 * @return number of bytes written or -1
 */
static int isf_flush_output_buffer(vfs68_z_t * isf)
{
  int n, w;

  w = n = isf->c_stream.next_out - isf->buffer_out;
  /*   TRACE68(zlib_cat," FLUSH:%d",n); */
  if (n > 0) {
    w = vfs68_write(isf->is, isf->buffer_out, n);
    isf->is_eof = (w != n);
    isf->is_err = (w == -1);
    if (isf->is_err) {
      /*       TRACE68(zlib_cat," WRITE-ERROR"); */
      w = 0;
      n = -1;
    } else {
      int rem = n - w;

      if (rem) {
        /* Damned! ... Could be an error ... Trust vfs & Pray */
        int i;
        /*      TRACE68(zlib_cat," FLUSH-MISS:%d", rem); */
        for (i=0; i<rem; ++i) {
          isf->buffer_out[i] = isf->buffer_out[i+w];
        }
      }
      n = w;
      w = rem;
    }
    isf->c_stream.next_out = isf->buffer_out + w;
  }
  isf->c_stream.avail_out = sizeof(isf->buffer_out) - w;
  /*   TRACE68(zlib_cat,"->%d ", n); */

  return n;
}

/* Read data buffer_in data */
static int isf_read_buffer(vfs68_z_t * isf, void *buffer, int n)
{
  int bytes;

  if (!n) {
    return 0;
  }

  bytes = n;
  while (bytes > 0) {
    int n;

    n = isf->c_stream.avail_in;
    if (!n) {
      n = isf_fill_input_buffer(isf,bytes);
    }
    if (n == -1) {
      return -1;
    }
    if (!n) {
      break;
    }
    if (n > bytes) {
      n = bytes;
    }
    bytes -= n;
    memcpy(buffer, isf->c_stream.next_in, n);
    isf->c_stream.next_in += n;
    isf->c_stream.avail_in -= n;
    buffer = (char *)buffer + n;
  }
  return n - bytes;
}

static int isf_seek_buffer(vfs68_z_t * isf, int offset)
{
  Byte * next_in;
  int avail_in, n;

  if (!offset) {
    return 0;
  }

  next_in = isf->c_stream.next_in;
  avail_in = isf->c_stream.avail_in;
  n = offset;

  if (offset < 0) {
    if (-offset <= next_in - isf->buffer_in) {
      offset = 0;
    }
  } else if (offset <= avail_in) {
    offset = 0;
  }

  if (!offset) {
    isf->c_stream.next_in = next_in + n;
    isf->c_stream.avail_in = avail_in - n;
  } else {
    if (vfs68_seek(isf->is, offset) == -1) {
      isf->is_err = 1;
      return -1;
    }
    isf->c_stream.next_in = isf->buffer_in;
    isf->c_stream.avail_in = 0;
  }

  return 0;
}

static const Byte gz_magic[2] = {0x1f, 0x8b};

static int isf_write_gz_header(vfs68_z_t * isf)
{
  struct {
    Byte magic[2]; /* gz header                */
    Byte method;   /* method (Z_DEFLATED)      */
    Byte flags;    /* Xtra,name,comment...     */
    Byte info[6];  /* time, xflags and OS code */
  } header;
  int i;
  const char * name = isf->gz_name ? vfs68_filename(isf->is) : 0;

  header.magic[0] = gz_magic[0];
  header.magic[1] = gz_magic[1];
  header.method   = Z_DEFLATED;
  header.flags    = name ? ORIG_NAME : 0;
  for (i=0; i<sizeof(header.info); ++i) {
    header.info[i] = 0;
  }
  if (vfs68_write(isf->is, &header, sizeof(header)) != sizeof(header)) {
    return -1;
  }
  if (name) {
    const char * s1, * s2;
    int len;
    s1 = strrchr(name,'/');
    s2 = strrchr(name,'\\');
    name = (s1 > name) ? s1+1 : name;
    name = (s2 > name) ? s2+1 : name;
    len = strlen(name) + 1;
    if (vfs68_write(isf->is, name, len) != len) {
      return -1;
    }
  }
  return 0;
}

static int isf_read_gz_header(vfs68_z_t * isf)
{
  int len;
  struct {
    Byte magic[2]; /* gz header                */
    Byte method;   /* method (Z_DEFLATED)      */
    Byte flags;    /* Xtra,name,comment...     */
    Byte info[6];  /* time, xflags and OS code */
  } header;

  /* Read gzip header. */
  len = isf_read_buffer(isf, &header, sizeof(header));
  if (len != sizeof(header)) {
    goto error;
  }
  /* Preliminary tests. */
  if (0
      || gz_magic[0] != header.magic[0]
      || gz_magic[1] != header.magic[1]
      || header.method != Z_DEFLATED
      || (header.flags & RESERVED) != 0) {
    goto error;
  }
  /* Skip the extra field. */
  if ((header.flags & EXTRA_FIELD) != 0) {
    unsigned char xtraLen[2];
    int xlen = isf_read_buffer(isf,&xtraLen,2);
    if (xlen == -1) {
      goto error;
    }
    len += xlen;
    if (xlen != 2) {
      goto error;
    }
    xlen = xtraLen[0] + (xtraLen[1]<<8);
    if (isf_seek_buffer(isf,xlen) == -1) {
      goto error;
    }
    len += xlen;
  }

  /* Skip the original file name & .gz file comment. */
  while (header.flags & (ORIG_NAME|COMMENT)) {
    char c;
    int e;
    if (header.flags & ORIG_NAME) {
      header.flags &= ~ORIG_NAME;
    } else {
      header.flags &= ~COMMENT;
    }
    while (e=isf_read_buffer(isf, &c, 1), e == 1) {
      ++len;
      if (!c) break;
    }
    if (e != 1) {
      goto error;
    }
  }

  /* Skip the header crc */
  if (header.flags & HEAD_CRC) {
    unsigned char crc[2];
    int e = isf_read_buffer(isf,crc,2);
    if (e == -1) {
      goto error;
    }
    len += e;
    if (e != 2) {
      goto error;
    }
    /* Add 0x10000 so that the value could not be 0. */
    isf->hd_crc = crc[0] | (crc[1]<<8) | (1<<16);
  }
  return 0;

error:
  /* Try to seek back to starting position. */
  isf_seek_buffer(isf,-len);
  return -1;
}

/* Inflate as much as possible.
 * returns number of byte available in out buffer
 */
static int isf_inflate_buffer(vfs68_z_t * isf)
{
  int err, n;
  err = 0;
  isf->c_stream.avail_out = sizeof(isf->buffer_out);
  isf->c_stream.next_out = isf->read_out = isf->buffer_out;

  while (isf->c_stream.avail_out) {

    /*     TRACE68(zlib_cat,"INFLATE: (%d,%d)", */
    /*         isf->c_stream.avail_in, isf->c_stream.avail_out); */
    if (!isf->c_stream.avail_in) {
      err = isf_fill_input_buffer(isf,sizeof(isf->buffer_in));
      /*       TRACE68(zlib_cat," fill_in(%d) ",err); */
      if (err <= 0) {
        break;
      }
    }
    /*     TRACE68(zlib_cat," (%d,%d), inflate->", */
    /*         isf->c_stream.avail_in, isf->c_stream.avail_out); */
    err = inflate(&isf->c_stream, Z_NO_FLUSH);
    /*     TRACE68(zlib_cat,"(%d,%d)", isf->c_stream.avail_in, isf->c_stream.avail_out); */

    if (err == Z_STREAM_END) {
      /*       TRACE68(zlib_cat," Z_STREAM_END\n"); */
      if (isf->gzip) {
        unsigned char data[8];

        err = isf_read_buffer(isf, data, 8);
        if (err >= 4) {
          isf->gz_crc32 = data[0]
            | (data[1]<<8)
            | (data[2]<<16)
            | (data[3]<<24);
        }
        if (err >= 8) {
          isf->gz_len = data[4]
            | (data[5]<<8)
            | (data[6]<<16)
            | (data[7]<<24);
        }

        /* TRACE68(zlib_cat,"Total In  : %8d\n" */
        /*         "Total Out : %8d\n" */
        /*         "Crc32     : %08X\n" */
        /*         "Gzip-Size : %8d\n", */
        /*         isf->c_stream.total_in, */
        /*         isf->c_stream.total_out, */
        /*         isf->gz_crc32, */
        /*         isf->gz_len); */
      }
      err = 0;
      break;
    } else if (err == Z_OK) {
      /*       TRACE68(zlib_cat," Z_STREAM_OK\n"); */
      err = 0;
    } else {
      /* TRACE68(zlib_cat,"Z_ERROR:[%s]\n", isf->c_stream.msg); */
      break;
    }

  }

  n = isf->c_stream.next_out - isf->buffer_out;
  if (isf->gzip) {
    isf->c_crc32 = crc32(isf->c_crc32, isf->buffer_out, n);
    /*     TRACE68(zlib_cat,"CRC32:%08X\n",isf->c_crc32); */
  }

  return err
    ? -1
    : n;
}

/* Deflate as much as possible.
 * @return  Number of byte
 */
static int isf_deflate_buffer(vfs68_z_t * isf, int finish)
{
  int err = 0, mia, zeof;
  const int z_flush_mode = finish ? Z_FINISH : Z_NO_FLUSH;

  /* fake zeof to avoid flushing */
  zeof = (z_flush_mode != Z_FINISH);

  isf->c_stream.next_in = isf->buffer_in;
  isf->c_stream.avail_in = isf->write_in - isf->buffer_in;

  /* Loop while there is data to compress ... */
  while (!zeof || isf->c_stream.avail_in) {
    /*     TRACE68(zlib_cat,"%sDEFLATE: (%d:%d/%d:%d) ", */
    /*         finish ? "F-" : "", */
    /*         isf->c_stream.next_in - isf->buffer_in, */
    /*         isf->c_stream.avail_in, */
    /*         isf->c_stream.next_out - isf->buffer_out, */
    /*         isf->c_stream.avail_out); */

    /* Flush output buffer and win a fresh one. */
    err = isf_flush_output_buffer(isf);
    if (err == -1) {
      /*       TRACE68(zlib_cat,"\n"); */
      return -1;
    }

    /* Do the deflate thing */
    /*     TRACE68(zlib_cat,"(%d:%d/%d:%d), deflate->", */
    /*         isf->c_stream.next_in - isf->buffer_in, */
    /*         isf->c_stream.avail_in, */
    /*         isf->c_stream.next_out - isf->buffer_out, */
    /*         isf->c_stream.avail_out); */
    {
      Byte * start = isf->c_stream.next_in;
      int len = isf->c_stream.avail_in;
      err = deflate(&isf->c_stream, z_flush_mode);
      if (isf->gzip) {
        isf->c_crc32 = crc32(isf->c_crc32, start, len-isf->c_stream.avail_in);
      }
    }
    /*     TRACE68(zlib_cat,"(%d:%d/%d:%d) ", */
    /*         isf->c_stream.next_in - isf->buffer_in, */
    /*         isf->c_stream.avail_in, */
    /*         isf->c_stream.next_out - isf->buffer_out, */
    /*         isf->c_stream.avail_out); */

    if (err == Z_STREAM_END) {
      /* Zlib tell us this is the end my friend ?? */
      /*       TRACE68(zlib_cat,"Z_STREAM_END!! "); */
      err = isf_flush_output_buffer(isf) == -1;
      zeof = 1;
      break;
    }
    if (err != Z_OK) {
      /* TRACE68(zlib_cat,"Z_ERROR(%d:[%s])\n", err, isf->c_stream.msg); */
      err = -1;
      break;
    } else {
      err = 0;
      /*       TRACE68(zlib_cat,"Z_OK "); */
    }
  }

  /* Are there "Missing In Action" bytes in the input buffer ? */
  mia = isf->c_stream.avail_in;
  /*   TRACE68(zlib_cat,"MIA:%d\n", mia); */

  isf->write_in = isf->buffer_in + mia;
  if (mia > 0) {
    Byte * src = isf->c_stream.next_in;
    int i;
    for (i=0; i<mia; ++i) {
      isf->buffer_in[i] = src[i];
    }
  }

  return err
    ? -1
    : sizeof(isf->buffer_in) - mia;
}


static int isf_close(vfs68_t * vfs)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  int err = -1;

  /* TRACE68(zlib_cat,"vfs68_z: close(%s) {\n", */
  /*         vfs68_filename(vfs)); */

  if (isf) {
    err = isf->is_err;

    if (isf->deflate) {
      /* TRACE68(zlib_cat,"DEFLATED "); */
      isf->deflate = 0;
      if (!err) {
        err = isf_deflate_buffer(isf, 1) == -1;
      }
      /* TRACE68(zlib_cat,"in:%d out:%d ", */
      /*         isf->c_stream.total_in,isf->c_stream.total_out); */
      /* if (isf->gzip) { */
      /*   TRACE68(zlib_cat,"crc:%08X ",isf->c_crc32); */
      /* } */
      if (!err) {
        int i, c, t;
        unsigned char hd[8];
        for (i=0, c=isf->c_crc32, t=isf->c_stream.total_in;
             i<4;
             ++i, c>>=8, t>>=8) {
          hd[0+i] = (unsigned char)c;
          hd[4+i] = (unsigned char)t;
        }
        err = vfs68_write(isf->is, hd, 8) != 8;
      }
      deflateEnd(&isf->c_stream);
    }

    if (isf->inflate) {
      /* TRACE68(zlib_cat,"INFLATED "); */
      /* if (isf->gzip) { */
      /*   TRACE68(zlib_cat,"crc:%08X/%08X ", */
      /*           isf->c_crc32,isf->gz_crc32); */
      /* } */
      inflateEnd(&isf->c_stream);
      isf->inflate = 0;
    }

    if (isf->is_slave)
      err |= vfs68_close(isf->is);
  }

  /* TRACE68(zlib_cat, " => [%s]\n", strok68(err)); */
  /* TRACE68(zlib_cat, "vfs_z: close '%s' => [%s]\n", */
  /*         vfs68_filename(vfs), strok68(err)); */
  return err;
}

static int isf_open(vfs68_t * vfs)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  int err;

  if (!isf->is || isf->inflate || isf->deflate) {
    return -1;
  }

  memset(&isf->c_stream, 0 , sizeof(isf->c_stream));
  isf->c_stream.zalloc = isf_zcalloc;
  isf->c_stream.zfree  = isf_zcfree;
  isf->c_stream.opaque = 0;/* vfs->context; */

  isf->c_crc32 = crc32(0, 0, 0);
  isf->pos = 0;
  isf->length = -1;
  isf->write_in = isf->c_stream.next_in  = isf->buffer_in;
  isf->read_out = isf->c_stream.next_out = isf->buffer_out;

  err = (isf->is_slave)
    ? vfs68_open(isf->is)
    : 0
    ;

  if (!err) {
    switch (isf->mode) {

    case VFS68_OPEN_READ:
      if (isf->gzip) {
        err = isf_read_gz_header(isf)
          || inflateInit2(&isf->c_stream, -MAX_WBITS) != Z_OK;
      } else {
        err = inflateInit(&isf->c_stream) != Z_OK;
      }
      isf->inflate = !err;

      break;

    case VFS68_OPEN_WRITE: {
      int level = isf->level;
      if (level > Z_BEST_COMPRESSION) {
        level = Z_DEFAULT_COMPRESSION;
      }
      if (isf->gzip) {
        err = Z_OK != deflateInit2(&isf->c_stream, level, Z_DEFLATED,
                                   -MAX_WBITS, DEF_MEM_LEVEL, isf->strategy)
          || isf_write_gz_header(isf);
      } else {
        err = Z_OK != deflateInit2(&isf->c_stream, level, Z_DEFLATED,
                                   MAX_WBITS, DEF_MEM_LEVEL, isf->strategy);
      }
      isf->deflate = !err;
      isf->length = 0;
    } break;
    default:
      err = -1;
    }
  }

  if (err) {
    isf_close(vfs);
    err = -1;
  }

  /* msg68_info("vfs_z: open(%s) => [%s]\n", */
  /*            vfs68_filename(vfs), strok68(err)); */
  return err;
}


static int isf_read(vfs68_t * vfs, void * data, int n)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  int bytes;

  if (!isf->inflate) {
    return -1;
  } else if (!n) {
    return 0;
  }

  for (bytes=n; bytes>0;) {
    int n;

    n = isf->c_stream.next_out - isf->read_out;

    if (!n) {
      n = isf_inflate_buffer(isf);
      if (n == -1) {
        return -1;
      }
      if (!n) {
        break;
      }
    }
    if (n > bytes) {
      n = bytes;
    }
    memcpy (data, isf->read_out, n);
    data = (char *) data + n;
    isf->read_out += n;
    isf->pos += n;
    bytes -= n;
  }
  return n - bytes;
}

static int isf_write(vfs68_t * vfs, const void * data, int n)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  int bytes;

  if (!isf->deflate) {
    return -1;
  } else if (!n) {
    return 0;
  }

  for (bytes=n; bytes>0;) {
    int n;

    /* Get number of byte free in the input buffer. */
    n = isf->buffer_in + sizeof(isf->buffer_in) - isf->write_in;
    if (!n) {
      /* No more bytes, it is time to compress and write some. */
      n = isf_deflate_buffer(isf,0);
      if (n == -1) {
        return -1;
      }
      if (!n) {
        break;
      }
    }
    if (n > bytes) {
      n = bytes;
    }
    memcpy (isf->write_in, data, n);
    data = (char *) data + n;
    isf->write_in += n;
    isf->pos += n;
    bytes -= n;
  }

  return n - bytes;
}

static int isf_flush(vfs68_t * vfs)
{
  /* vfs68_z_t * isf = (vfs68_z_t *)vfs; */
  /* $$$ TODO */
  return 0;
}

static int isf_length(vfs68_t * vfs)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;

  return (!isf)
    ? -1
    : isf->length;
}

static int isf_tell(vfs68_t * vfs)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  return (!isf)
    ? -1
    : isf->pos;
}

static int isf_seek(vfs68_t * vfs, int offset)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  int pos;

  if (!isf ) {
    return -1;
  }
  pos = isf->pos + offset;
  if (pos < 0 || (isf->length != -1 && pos > isf->length)) {
    return -1;
  }
  isf->pos = pos;
  return 0;
}

static void isf_destroy(vfs68_t * vfs)
{
  vfs68_z_t * isf = (vfs68_z_t *)vfs;
  if (isf->is_slave)
    vfs68_destroy(isf->is);

  /* call free handler even with NUL pointer since we
     never know what free68() do. */
  free(vfs);
}

static const vfs68_t vfs68_z = {
  isf_name,
  isf_open, isf_close,
  isf_read, isf_write, isf_flush,
  isf_length, isf_tell, isf_seek, isf_seek,
  isf_destroy
};


vfs68_t * vfs68_z_create(vfs68_t * is, int mode,
                                 const vfs68_z_option_t opt)
{
  vfs68_z_t * isf = 0;

  if (!is) {
    msg68_error("vfs68_z: create -- no slave stream\n");
    goto out;
  }

  if (!(1&(mode^(mode>>1)))) {
    msg68_error("vfs68_z: create  -- invalid mode (%c%c)\n",
                (mode&1)?'R':'.', (mode&2)?'W':'.');
    goto out;
  }

  isf = calloc(sizeof(vfs68_z_t),1);
  if (!isf) {
    msg68_error("vfs68_z: create  -- alloc error\n");
    goto out;
  }

  /* Copy vfs functions. */
  memcpy(&isf->vfs, &vfs68_z, sizeof(vfs68_z));
  /* Setup */
  isf->is       = is;
  isf->mode     = mode & VFS68_OPEN_MASK;
  isf->is_slave = mode >> VFS68_SLAVE_BIT;
  isf->length   = -1;
  isf->org_pos  = vfs68_tell(is);
  /* Setup gzip option. */
  isf->gzip     = opt.gzip;
  isf->level    = opt.level;
  isf->strategy = opt.strategy;
  isf->gz_name  = opt.name;

  /* TRACE68(zlib_cat, */
  /*         "vfs68_z: create '%s' mode:%c gz:%c level:%d name:%c\n", */
  /*         vfs68_filename(isf->is), (mode&1)?'R':'W', */
  /*         isf->gzip?'Y':'N', isf->level, isf->gz_name?'Y':'N'); */

out:
  return (vfs68_t *) isf;
}

#else /* #ifndef USE_Z */

# include "file68_vfs_z.h"

/* vfs Z must not be include in this package. Anyway the creation
 * function still exists but it always returns an error.
 */

vfs68_t * vfs68_z_create(vfs68_t * is, int mode,
                                 const vfs68_z_option_t opt)
{
  msg68_error("zlib68: create -- *NOT SUPPORTED*\n");
  return 0;
}

#endif /* #ifndef USE_Z */

int vfs68_z_init(void)
{
#ifdef USE_Z
  zlib_cat = msg68_cat("zlib","Zlib VFS",DEBUG_ZLIB_O);
#endif
  return 0;
}

void vfs68_z_shutdown(void)
{
#ifdef USE_Z
  if (zlib_cat > 0) {
    msg68_cat_free(zlib_cat);
    zlib_cat = msg68_NEVER;
  }
#endif
}
