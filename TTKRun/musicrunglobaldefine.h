#ifndef MUSICRUNGLOBALDEFINE_H
#define MUSICRUNGLOBALDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicglobal.h"

///exoprt
#define MUSIC_EXPORT

#ifdef MUSIC_EXPORT
#  define MUSIC_RUN_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_RUN_EXPORT Q_DECL_IMPORT
#endif

#endif // MUSICRUNGLOBALDEFINE_H
