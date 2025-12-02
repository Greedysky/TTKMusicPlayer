#ifndef TTKOBJECT_H
#define TTKOBJECT_H

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

#include "ttkglobal.h"

#define TTK_SERVICE_NAME        "TTKService"
#define TTK_APP_NAME            "TTKMusicPlayer"
#define TTK_APP_COME_NAME       TTK_STR_CAT(TTK_APP_NAME, COM_FILE)

#ifdef _WIN32
#  define TTK_APP_FILE_NAME     TTK_STR_CAT(TTK_APP_NAME, EXE_FILE)
#  define TTK_APP_RUN_NAME      TTK_APP_FILE_NAME
#  define TTK_SERVICE_RUN_NAME  TTK_STR_CAT(TTK_SERVICE_NAME, EXE_FILE)
#else
#  define TTK_APP_FILE_NAME     TTK_APP_NAME
#  define TTK_APP_RUN_NAME      TTK_STR_CAT(TTK_APP_NAME, SHL_FILE)
#  define TTK_SERVICE_RUN_NAME  TTK_STR_CAT(TTK_SERVICE_NAME, SHL_FILE)
#endif

#endif // TTKOBJECT_H
