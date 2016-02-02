#ifndef MUSICGLOBALDEFINE_H
#define MUSICGLOBALDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtCore/qglobal.h>

///load logger
#include "musiclogger.h"

#ifdef QT_DEBUG
#   define MUSIC_DEBUG
#endif
//////////////////////////////////////
#ifdef MUSIC_LIBRARY
#  define MUSIC_NETWORK
#  define MUSIC_CORE
#  define MUSIC_WIDGET
#  define MUSIC_SEARCH
#  define MUSIC_LRC
#  define MUSIC_REMOTE
#  define MUSIC_TOOL
#  define MUSIC_TOOLSET
#  define MUSIC_USER
#  define MUSIC_VIDEO
#  define MUSIC_GUI
#endif
//////////////////////////////////////
#define MUSIC_CAST
//////////////////////////////////////


//////////////////////////////////////
///exoprt
///
///
#define MUSIC_EXPORT

#ifdef MUSIC_GUI
#  ifdef MUSIC_EXPORT
#    define MUSIC_GUI_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_GUI_IMPORT Q_DECL_IMPORT
#  endif
#endif

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

#ifdef MUSIC_VIDEO
#  ifdef MUSIC_EXPORT
#    define MUSIC_VIDEO_EXPORT Q_DECL_EXPORT
#  else
#    define MUSIC_VIDEO_IMPORT Q_DECL_IMPORT
#  endif
#endif


//////////////////////////////////////
///cast
///
#ifdef MUSIC_CAST
#  define MSconst_cast(x,y) (const_cast< x >(y))
#else
#  define MSconst_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MSdynamic_cast(x,y) (dynamic_cast< x >(y))
#else
#  define MSdynamic_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MSreinterpret_cast(x,y) (reinterpret_cast< x >(y))
#else
#  define MSreinterpret_cast(x,y) ((x)(y))
#endif

#ifdef MUSIC_CAST
#  define MSstatic_cast(x,y) (static_cast< x >(y))
#else
#  define MSstatic_cast(x,y) ((x)(y))
#endif


#endif // MUSICGLOBALDEFINE_H
