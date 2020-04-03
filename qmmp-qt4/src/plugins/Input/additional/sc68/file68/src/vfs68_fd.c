/*
 * @file    istream68_fd.c
 * @brief   implements istream68 VFS for file descriptor
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
#include "file68_vfs_fd.h"

/* define this if you don't want file descriptor support. */
#ifndef ISTREAM68_NO_FD

#include "file68_vfs_def.h"
#include "file68_uri.h"
#include "file68_str.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(_MSC_VER)
# include <stdio.h>
# include <io.h>
#else
# include <unistd.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/** vfs file structure. */
typedef struct {
  vfs68_t vfs;            /**< vfs function.            */
  int fd;                         /**< File descriptor (-1:closed). */
  int org_fd;                     /**< Original file descriptor.    */
  int mode;                       /**< Open modes.                  */

  /* MUST BE at the end of the structure because supplemental bytes will
   * be allocated to store filename.
   */
  char name[1];                   /**< filename.                    */

} vfs68_fd_t;

static int fd_ismine(const char *);
static vfs68_t * fd_create(const char *, int, int, va_list);
static scheme68_t fd_scheme = {
  0, "vfs-fd", fd_ismine, fd_create
};

static struct {
  const char * name;  int len;  int mode;
} myshemes[] =
{
  { "fd:",      3, SCHEME68_ISMINE|SCHEME68_READ|SCHEME68_WRITE },
  { "file://",  7, SCHEME68_ISMINE|SCHEME68_READ|SCHEME68_WRITE },
  { "local://", 8, SCHEME68_ISMINE|SCHEME68_READ|SCHEME68_WRITE },
  { "stdout:",  7, SCHEME68_ISMINE|              SCHEME68_WRITE },
  { "stderr:",  7, SCHEME68_ISMINE|              SCHEME68_WRITE },
  { "stdin:",   7, SCHEME68_ISMINE|SCHEME68_READ                },
};

static int fd_ismine(const char * uri)
{
  int i = uri68_get_scheme(0, 0, uri);
  if (!i)
    return SCHEME68_ISMINE|SCHEME68_READ|SCHEME68_WRITE;
  else if (i > 0) {
#ifdef NATIVE_WIN32
    if (i == 2 && isalpha((int)uri[0]))
      return SCHEME68_ISMINE|SCHEME68_READ|SCHEME68_WRITE;
#endif
    for (i = 0; i < sizeof(myshemes)/sizeof(*myshemes); ++i)
      if (!strncmp68(uri, myshemes[i].name, myshemes[i].len))
        return myshemes[i].mode;
  }
  return 0;
}

static const char * ifdname(vfs68_t * vfs)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;

  return (!isf->name[0])
    ? 0
    : isf->name;
}

static int ifdopen(vfs68_t * vfs)
{
  int imode;
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;

  if (!isf->name || isf->fd != -1) {
    return -1;
  }

  if (isf->org_fd != -1) {
    isf->fd = isf->org_fd;
    return 0;
  }

  switch (isf->mode) {
  case VFS68_OPEN_READ:
    imode = O_RDONLY;
    break;
  case VFS68_OPEN_WRITE:
    imode = O_WRONLY | O_CREAT | O_TRUNC;
    break;
  case VFS68_OPEN_READ|VFS68_OPEN_WRITE:
    imode = O_RDWR | O_CREAT;
    break;
  default:
    return -1;
  }
#ifdef _O_BINARY
  imode |= _O_BINARY;
#endif

  isf->fd = open(isf->name, imode, 0644);
  return -(isf->fd == -1);
}

static int ifdclose(vfs68_t * vfs)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;
  int fd;

  if (isf->fd == -1) {
    return -1;
  }
  fd = isf->fd;
  isf->fd = -1;
  /* $$$ Close even if org_fd was given. Is it a good thing ? */
  return close(fd);
}

static int ifdread(vfs68_t * vfs, void * data, int n)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;
  return  (isf->fd == -1)
    ? -1
    : read(isf->fd, data, n);
}

static int ifdwrite(vfs68_t * vfs, const void * data, int n)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;
  return isf->fd == -1
    ? -1
    : write(isf->fd, data, n)
    ;
}

#if defined(HAVE_FDATASYNC)
# define MY_FSYNC(fd) fdatasync(fd)
#elif defined(HAVE_FSYNC)
# define MY_FSYNC(fd) fsync(fd)
#else
# define MY_FSYNC(fd) 0
#endif

static int ifdflush(vfs68_t * vfs)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;
  return isf->fd == -1
    ? -1
    : MY_FSYNC(isf->fd)
    ;
}

/* We could have store the length value at opening, but this way it handles
 * dynamic changes of file size.
 */
static int ifdlength(vfs68_t * vfs)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;
  off_t pos,len = -1;

  if (isf->fd == -1) {
    return -1;
  }

  /* save current position. */
  pos = lseek(isf->fd,0,SEEK_CUR);
  if (pos != (off_t)-1) {
    /* seek to end of file to get file position (AKA length) */
    len = lseek(isf->fd, 0, SEEK_END);
    /* restore saved position. */
    lseek(isf->fd, pos, SEEK_SET);
  }
  return len;
}

static int ifdtell(vfs68_t * vfs)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;

  return isf->fd == -1
    ? -1
    : lseek(isf->fd,0,SEEK_CUR)
    ;
}

static int ifdseek(vfs68_t * vfs, int offset)
{
  vfs68_fd_t * isf = (vfs68_fd_t *)vfs;

  return isf->fd == -1
    ? -1
    : lseek(isf->fd, offset, SEEK_CUR)
    ;
}

static void ifddestroy(vfs68_t * vfs)
{
  free(vfs);
}

static const vfs68_t vfs68_fd = {
  ifdname,
  ifdopen, ifdclose,
  ifdread, ifdwrite, ifdflush,
  ifdlength, ifdtell,
  ifdseek, ifdseek,
  ifddestroy
};

static vfs68_t * create(const char * fname, int fd, int mode)
{
  vfs68_fd_t *isf;
  int len;

  if (fname && !strncmp68(fname,"fd:",3) && isdigit((int)fname[3])) {
    fd = strtoul(fname+3, 0, 10);
    fname = 0;
  }

  if (fname)
    len = strlen(fname);
  else if (fd >= 0)
    len = 40;
  else
    return 0;

  /* Don't need 0, because 1 byte already allocated in the
   * vfs68_fd_t::fname.
   */

  isf = malloc(sizeof(vfs68_fd_t) + len);
  if (!isf)
    return 0;

  /* Copy vfs functions. */
  memcpy(&isf->vfs, &vfs68_fd, sizeof(vfs68_fd));
  /* Clean init. */
  isf->fd     = -1;
  isf->org_fd = fd;
  isf->mode   = mode & (VFS68_OPEN_READ|VFS68_OPEN_WRITE);

  if (!fname)
    sprintf(isf->name, "fd:%u", fd);
  else
    /* Copy filename. */
    /* $$$ May be later, we should add a check for relative path and add
     * CWD ... or make it URL compatible */
    strcpy(isf->name, fname);

  return &isf->vfs;
}

static vfs68_t * fd_create(const char * uri, int mode,
                           int argc , va_list list)
{
  int fd = -1;

  if (!strncmp68(uri,"fd:", 3))
    fd = strtoul(uri+3, 0, 10);
  else if (!strncmp68(uri,"file://", 7))
    uri += 7;
  else if (!strncmp68(uri,"local://", 8))
    uri += 8;
  else if (!strncmp68(uri,"stdin:", 6))
    return (mode & 3) == 1
      ? create(0, 0, mode)
      : 0
      ;
  else if (!strncmp68(uri,"stdout:", 7))
    return (mode & 3) == 2
      ? create(0, 1, mode)
      : 0
      ;
  else if (!strncmp68(uri,"stderr:", 7))
    return (mode & 3) == 2
      ? create(0, 2, mode)
      : 0
      ;

  if (fd == -1 && argc == 1) {
    fd = va_arg(list,int);
    uri = 0;
  }

  return create(uri, fd, mode);
}

int vfs68_fd_init(void)
{
  return uri68_register(&fd_scheme);
}

void vfs68_fd_shutdown(void)
{
  uri68_unregister(&fd_scheme);
}

#else /* #ifndef VFS68_NO_FILE */

/* vfs fd must not be include in this package. Anyway the creation
 * still exist but it always returns error.
 */

#include "file68_msg.h"

int vfs68_fd_init(void) { return 0; }
void vfs68_fd_shutdown(void) { }

#endif

