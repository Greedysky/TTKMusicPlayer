#ifndef MUSICMOBILEGLOBALDEFINE_H
#define MUSICMOBILEGLOBALDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobal.h"

//////////////////////////////////////
///exoprt
///
///
#define MUSIC_EXPORT

#ifdef MUSIC_EXPORT
#  define MUSIC_MOBILE_EXPORT Q_DECL_EXPORT
#else
#  define MUSIC_MOBILE_EXPORT Q_DECL_IMPORT
#endif

#endif // MUSICMOBILEGLOBALDEFINE_H
