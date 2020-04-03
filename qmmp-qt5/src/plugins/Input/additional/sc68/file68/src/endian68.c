/*
 * @file    endian68.c
 * @brief   machine endianess
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
#include "file68_ord.h"

int endian68_byte_order(void)
{
  static int endian = 0;

  if (!endian) {
    char * s;
    int i;
    for (i=0, s = (char *)&endian; i<sizeof(int); ++i) {
      s[i] = i;
    }
  }
  return endian;
}

int endian68_is_little(void)
{
  return !(endian68_byte_order() & 255);
}

int endian68_is_big(void)
{
  return !endian68_is_little();
}
