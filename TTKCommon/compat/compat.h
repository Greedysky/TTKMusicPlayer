#ifndef COMPAT_H
#define COMPAT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <string.h>
#include "ttkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t dsize);
#endif

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t dsize);
#endif

#ifdef __cplusplus
}
#endif

#endif // COMPAT_H
