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
#  define MUSIC_WIDGET_EXPORT Q_DECL_EXPORT
#  define MUSIC_CORE_EXPORT Q_DECL_EXPORT
#  define MUSIC_GUI_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_WIDGET_EXPORT Q_DECL_IMPORT
#  define MUSIC_CORE_EXPORT Q_DECL_IMPORT
#  define MUSIC_GUI_EXPORT Q_DECL_IMPORT
#endif

#endif // MUSICLIBEXPORTGLOBAL_H
