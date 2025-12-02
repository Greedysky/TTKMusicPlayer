#ifndef INTEGER_H
#define INTEGER_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#if (defined __SIZEOF_INT128__ && __SIZEOF_INT128__ == 16) || defined _GLIBCXX_USE_INT128
using TTKInt128  = __extension__ __int128;
using TTKUInt128 = __extension__ unsigned __int128;
#else
#include "int128.h"
using TTKInt128  = int128_t;
using TTKUInt128 = uint128_t;
#endif

#endif // INTEGER_H
