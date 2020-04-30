/*
 * @file    ice68.c
 * @brief   ICE! file loader
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
#include "file68_ice.h"
#include "file68_vfs_def.h"
#include "file68_err.h"
#ifdef USE_UNICE68
# include "file68_msg.h"
# include "file68_uri.h"
# include "unice68.h"
# include <string.h>
# include <stdlib.h>

int file68_ice_version(void)
{
  return unice68_ice_version();
}

int file68_ice_is_magic(const void * buffer)
{
  return unice68_depacked_size(buffer,0) > 0;
}


#define TERROR(S) do { errstr = S; goto error; } while(0)


void * file68_ice_load(vfs68_t *is, int *ulen)
{
  char header[12], *inbuf = 0, * outbuf = 0;
  int dsize, csize;
  const char * fname;
  const char * errstr = 0;

  fname = vfs68_filename(is);

  if (vfs68_read(is, header, 12) != 12)
    TERROR("not ICE! (too small)");

  csize = 0;
  dsize = unice68_depacked_size(header, &csize);

  if (dsize < 0)
    TERROR("not ICE! (not magic)");

  if(inbuf = malloc(csize), !inbuf)
    TERROR("input alloc failed");

  csize -= 12;
  memcpy(inbuf, header, 12);
  if (vfs68_read(is, inbuf+12, csize) != csize)
    TERROR("read error");

  if (outbuf = malloc(dsize), !outbuf)
    TERROR("output alloc failed");

  if (unice68_depacker(outbuf, inbuf))
    TERROR("depack failed");

  goto success;

error:
  error68("ice68: load: %s -- %s", errstr, fname);
  free(outbuf);
  outbuf = 0;
  dsize = 0;

success:
  free(inbuf);
  if (ulen) {
    *ulen = dsize;
  }
  return outbuf;
}

void * file68_ice_load_file(const char * fname, int * ulen)
{
  void * ret = 0;
  vfs68_t * is;

  is = uri68_vfs(fname, 1, 0);
  if (vfs68_open(is) != -1) {
    ret = file68_ice_load(is, ulen);
    vfs68_close(is);
  }
  vfs68_destroy(is);

  return ret;
}

#else /* #ifdef USE_UNICE68 */

int file68_ice_version(void)
{
  return 0;
}

int file68_ice_is_magic(const void * buffer)
{
  return buffer
    && 0[(char *)buffer] == 'I'
    && (1[(char *)buffer] | 0x20) == 'c'
    && (2[(char *)buffer] | 0x20) == 'e'
    && 3[(char *)buffer] == '!';
}

void * file68_ice_load(vfs68_t * is, int * ulen)
{
  const char * fname = vfs68_filename(is);
  error68("ice68: *NOT SUPPORTED*");
  return 0;
}

void * file68_ice_load_file(const char * fname, int * ulen)
{
  error68("ice68: *NOT SUPPORTED*");
  return 0;
}

#endif /* #ifdef USE_UNICE68 */
