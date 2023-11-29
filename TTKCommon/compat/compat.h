#ifndef COMPAT_H
#define COMPAT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

// declare
#define strlcat _strlcat
#define strlcpy _strlcpy

# ifdef  __cplusplus
extern "C" {
# endif
// implementation
size_t _strlcat(char *dst, const char *src, size_t dsize);
size_t _strlcpy(char *dst, const char *src, size_t dsize);
# ifdef  __cplusplus
}
# endif

#endif // COMPAT_H
