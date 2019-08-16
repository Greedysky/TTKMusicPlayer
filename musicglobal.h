#ifndef MUSICGLOBAL_H
#define MUSICGLOBAL_H

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

#include "ttkglobal.h"

///
#define DEFAULT_LEVEL_LOWER         -1
#define DEFAULT_LEVEL_LOW           0
#define DEFAULT_LEVEL_NORMAL        -999
#define DEFAULT_LEVEL_HIGH          999
#define DEFAULT_LEVEL_HIGHER        9999

///
#ifndef MUSIC_NAMESPACE
#define MUSTTK_DEBUGE MUSIC
#endif //MUSIC_NAMESPACE

#if QT_VERSION <= QT_VERSION_CHECK(4,6,0)
#define MUSIC_NO_NAMESPACE_SUPPORT
#endif

#ifndef MUSIC_NO_NAMESPACE_SUPPORT
#define MUSIC_BEGIN_NAMESPACE namespace MUSIC_NAMESPACE {
#define MUSIC_END_NAMESPACE }
#define MUSIC_USE_NAMESPACE using namespace ::MUSIC_NAMESPACE;
#else
#define MUSIC_BEGIN_NAMESPACE
#define MUSIC_END_NAMESPACE
#define MUSIC_USE_NAMESPACE
#endif //MUSIC_NAMESPACE_SUPPORT

#endif // MUSICGLOBAL_H
