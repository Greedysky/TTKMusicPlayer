/*
 * @file    emu68/error68.c
 * @brief   error message handler
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error68.h"

#ifdef MAXERROR
#undef MAXERROR
#endif
#ifdef MAXERRORSTR
#undef MAXERRORSTR
#endif

#define MAXERROR    (sizeof(emu68->err)/sizeof(emu68->err[0]))
#define MAXERRORSTR sizeof(emu68->err[0])

/*  Push error message
 */
int emu68_error_add(emu68_t * const emu68, const char * format, ...)
{
  if (emu68 && format) {
    va_list list;
    int n = emu68->nerr;

    if (n >= MAXERROR) {
      memmove(emu68->err[0],emu68->err[1],MAXERRORSTR*(MAXERROR-1));
      n = MAXERROR-1;
    }

    va_start(list,format);
    vsnprintf(emu68->err[n],MAXERRORSTR,format,list);
    va_end(list);
    emu68->err[n][MAXERRORSTR-1] = 0;
    emu68->nerr = ++n;
  }
  return -1;
}

/*  Pop error message
 */
const char * emu68_error_get(emu68_t * const emu68)
{
  const char * res = 0;
  if (emu68 && emu68->nerr > 0) {
    res = emu68->err[--emu68->nerr];
  }
  return res;
}
