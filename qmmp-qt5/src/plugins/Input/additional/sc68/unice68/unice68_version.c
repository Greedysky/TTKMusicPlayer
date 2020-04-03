/*
 * @file    unice68_version.c
 * @brief   library, packer and depacker version
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifndef PACKAGE_NAME
# define PACKAGE_NAME "unice68"
#endif
#ifndef PACKAGE_VERSION
# define PACKAGE_VERSION __DATE__
#endif
#ifndef PACKAGE_VERNUM
# define PACKAGE_VERNUM 0
#endif

#include "unice68.h"

int unice68_unice_version(void)
{
  return 240;
}

int unice68_ice_version(void)
{
  return 235;
}

const char * unice68_versionstr(void)
{
  return PACKAGE_NAME " " PACKAGE_VERSION " ICE! 2.25/2.40" ;
}

int unice68_version(void)
{
  return PACKAGE_VERNUM;
}
