#ifndef TTKLIBRARYVERSION_H
#define TTKLIBRARYVERSION_H

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

#include "../ttkversion.h"

//update time 2025.09.08
#define TTK_LIBRARY_MAJOR_VERSION 1
#define TTK_LIBRARY_MINOR_VERSION 4
#define TTK_LIBRARY_PATCH_VERSION 2

#define TTK_LIBRARY_VERSION       TTK_VERSION_CHECK(TTK_LIBRARY_MAJOR_VERSION, TTK_LIBRARY_MINOR_VERSION, TTK_LIBRARY_PATCH_VERSION, 0)
#define TTK_LIBRARY_VERSION_STR   TTK_VERSION_CHECK_STR(TTK_LIBRARY_MAJOR_VERSION, TTK_LIBRARY_MINOR_VERSION, TTK_LIBRARY_PATCH_VERSION, 0)

#undef TTK_RC_FILEVERSION
#undef TTK_RC_PRODUCTVERSION

#define TTK_RC_FILEVERSION        TTK_LIBRARY_MAJOR_VERSION, TTK_LIBRARY_MINOR_VERSION, TTK_LIBRARY_PATCH_VERSION, 0
#define TTK_RC_PRODUCTVERSION     TTK_LIBRARY_VERSION_STR

#endif // TTKLIBRARYVERSION_H
