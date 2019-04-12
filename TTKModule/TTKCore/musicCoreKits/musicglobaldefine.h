#ifndef MUSICGLOBALDEFINE_H
#define MUSICGLOBALDEFINE_H

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

#include <QtCore/qglobal.h>

///
#ifdef TTK_LIBRARY
#  define MUSIC_NETWORK
#  define MUSIC_CORE
#  define MUSIC_WIDGET
#  define MUSIC_SEARCH
#  define MUSIC_LRC
#  define MUSIC_REMOTE
#  define MUSIC_TOOL
#  define MUSIC_TOOLSET
#  define MUSIC_USER
#  define MUSIC_UTILS
#  define MUSIC_VIDEO
#  define MUSIC_GUI
#endif

///exoprt
#define MUSIC_EXPORT


#ifdef MUSIC_NETWORK
#  ifdef MUSIC_EXPORT
#    define MUSIC_NETWORK_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_NETWORK_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_CORE
#  ifdef MUSIC_EXPORT
#    define MUSIC_CORE_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_CORE_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_WIDGET
#  ifdef MUSIC_EXPORT
#    define MUSIC_WIDGET_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_WIDGET_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_SEARCH
#  ifdef MUSIC_EXPORT
#    define MUSIC_SEARCH_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_SEARCH_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_LRC
#  ifdef MUSIC_EXPORT
#    define MUSIC_LRC_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_LRC_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_REMOTE
#  ifdef MUSIC_EXPORT
#    define MUSIC_REMOTE_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_REMOTE_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_TOOL
#  ifdef MUSIC_EXPORT
#    define MUSIC_TOOL_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_TOOL_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_TOOLSET
#  ifdef MUSIC_EXPORT
#    define MUSIC_TOOLSET_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_TOOLSET_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_USER
#  ifdef MUSIC_EXPORT
#    define MUSIC_USER_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_USER_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_UTILS
#  ifdef MUSIC_EXPORT
#    define MUSIC_UTILS_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_UTILS_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_VIDEO
#  ifdef MUSIC_EXPORT
#    define MUSIC_VIDEO_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_VIDEO_IMPORT Q_DECL_IMPORT
#  endif
#endif

#ifdef MUSIC_GUI
#  ifdef MUSIC_EXPORT
#    define MUSIC_GUI_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_GUI_IMPORT Q_DECL_IMPORT
#  endif
#endif

///
#include "musiclogger.h"
#include "musicglobal.h"
///

#endif // MUSICGLOBALDEFINE_H
