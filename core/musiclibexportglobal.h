#ifndef MUSICLIBEXPORTGLOBAL_H
#define MUSICLIBEXPORTGLOBAL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtCore/qglobal.h>

#ifdef QT_DEBUG
#   define MUSIC_DEBUG
#endif

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


#ifdef MUSIC_GUI
#  define MUSIC_GUI_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_GUI_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_NETWORK
#  define MUSIC_NETWORK_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_NETWORK_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_CORE
#  define MUSIC_CORE_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_CORE_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_WIDGET
#  define MUSIC_WIDGET_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_WIDGET_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_SEARCH
#  define MUSIC_SEARCH_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_SEARCH_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_LRC
#  define MUSIC_LRC_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_LRC_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_REMOTE
#  define MUSIC_REMOTE_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_REMOTE_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_TOOL
#  define MUSIC_TOOL_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_TOOL_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_TOOLSET
#  define MUSIC_TOOLSET_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_TOOLSET_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_USER
#  define MUSIC_USER_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_USER_IMPORT Q_DECL_IMPORT
#endif

#ifdef MUSIC_VIDEO
#  define MUSIC_VIDEO_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_VIDEO_IMPORT Q_DECL_IMPORT
#endif

///load logger
#include "musiclogger.h"

#endif // MUSICLIBEXPORTGLOBAL_H
