/*
 * @file    error68.c
 * @brief   error messages
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
#include "file68_err.h"
#include "file68_msg.h"

#include <string.h>

/* Process error message (printf like fmt). */
int error68_va(const char * fmt, va_list list)
{
  if (fmt) {
    int len = strlen(fmt);
    msg68_va(msg68_ERROR, fmt, list);
    if (len > 0 && fmt[len-1] != '\n') {
      msg68(msg68_ERROR,"\n");
    }
  }
  return -1;
}

int error68x_va(void * cookie, const char * fmt, va_list list)
{
  if (fmt) {
    int len = strlen(fmt);
    msg68x_va(msg68_ERROR, cookie, fmt, list);
    if (len > 0 && fmt[len-1] != '\n') {
      msg68x(msg68_ERROR, cookie, "\n");
    }
  }
  return -1;
}

/* Process error message (printf like fmt). */
int error68(const char * fmt, ...)
{
  int err;
  va_list list;

  va_start(list, fmt);
  err = error68_va(fmt, list);
  va_end(list);

  return err;
}

/* Process error message (printf like fmt). */
int error68x(void * cookie, const char * fmt, ...)
{
  int err;
  va_list list;

  va_start(list, fmt);
  err = error68x_va(cookie, fmt, list);
  va_end(list);

  return err;
}
