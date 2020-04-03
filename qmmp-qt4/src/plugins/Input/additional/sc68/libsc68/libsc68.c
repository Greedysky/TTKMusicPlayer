/*
 * @file    libsc68.c
 * @brief   sc68 API
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

#include "sc68.h"

#ifndef PACKAGE_STRING
# define PACKAGE_STRING "libsc68 n/a"
#endif

#ifndef PACKAGE_VERNUM
# define PACKAGE_VERNUM 0
#endif

const char * sc68_versionstr(void)
{
  return PACKAGE_STRING;
}

int sc68_version(void)
{
  return PACKAGE_VERNUM;
}
