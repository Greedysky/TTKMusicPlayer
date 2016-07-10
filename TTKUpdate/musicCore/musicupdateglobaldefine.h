#ifndef MUSICUPDATEGLOBALDEFINE_H
#define MUSICUPDATEGLOBALDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtCore/qglobal.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#  define MUSIC_GREATER_NEW
#  if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
#    define MUSIC_WINEXTRAS
#  endif
#endif

#ifdef QT_DEBUG
#   define MUSIC_DEBUG
#endif

//////////////////////////////////////
///exoprt
///
///
#define MUSIC_EXPORT

#ifdef MUSIC_EXPORT
#  define MUSIC_UPDATE_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_UPDATE_IMPORT Q_DECL_IMPORT
#endif

#endif // MUSICUPDATEGLOBALDEFINE_H
