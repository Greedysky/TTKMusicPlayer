#ifndef MUSICCONFIGDEFINE_H
#define MUSICCONFIGDEFINE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include "musicobject.h"
#include "ttkversion.h"
#include "ttkglobaldefine.h"

#ifdef CONFIG_OUT_BUILD
#  define APP_PATH_DIR              MusicObject::applicationPath() + TTK_VERSION_STR + TTK_SEPARATOR
#else
#  define APP_PATH_DIR              MusicObject::applicationPath()
#endif
//
#define TTK_DOWNLOAD_DIR_FULL       APP_PATH_DIR + TTK_PDIR + DOWNLOAD_DIR
#define TTK_LRC_DIR_FULL            TTK_DOWNLOAD_DIR_FULL + LRC_DIR
#define TTK_MUSIC_DIR_FULL          TTK_DOWNLOAD_DIR_FULL + MUSIC_DIR
#define TTK_MOVIE_DIR_FULL          TTK_DOWNLOAD_DIR_FULL + MOVIE_DIR
//
#define TTK_CACHE_DIR_FULL          CACHE_DIR_FULL
#define TTK_ART_DIR_FULL            ART_DIR_FULL
#define TTK_BACKGROUND_DIR_FULL     BACKGROUND_DIR_FULL
//
#define TTK_COFIG_PATH_FULL         COFIG_PATH_FULL
#define TTK_PLAYLIST_PATH_FULL      PLAYLIST_PATH_FULL
#define TTK_NORMAL_DOWN_PATH_FULL   NORMAL_DOWN_PATH_FULL
#define TTK_CLOUD_DOWN_PATH_FULL    CLOUD_DOWN_PATH_FULL
#define TTK_CLOUD_UP_PATH_FULL      CLOUD_UP_PATH_FULL
#define TTK_SEARCH_PATH_FULL        SEARCH_PATH_FULL
#define TTK_USER_THEME_DIR_FULL     USER_THEME_DIR_FULL
//
#define TTK_THEME_DIR_FULL          APP_PATH_DIR + THEME_DIR
#define TTK_PLUGINS_DIR_FULL        APP_PATH_DIR + PLUGINS_DIR
#define TTK_LANGUAGE_DIR_FULL       APP_PATH_DIR + LANGUAGE_DIR


#ifdef Q_OS_WIN
#  define TTK_SERVICE_FULL          APP_PATH_DIR + "TTKService.exe"
#else
#  define TTK_ROUTINE_FULL          APP_PATH_DIR + TTK_PDIR + "TTKRoutine.sh"
#  define TTK_MUSICPLAYER_FULL      APP_PATH_DIR + TTK_PDIR + "TTKMusicPlayer.sh"
#  define TTK_INIT_FULL             APP_PATH_DIR + "TTKInit.sh"
#  define TTK_CONSOLE_FULL          APP_PATH_DIR + "TTKConsole.sh"
#  define TTK_SERVICE_FULL          APP_PATH_DIR + "TTKService.sh"
#  define TTK_ROUTINECOPY_FULL      APP_PATH_DIR + "TTKRoutineCopy.sh"
#endif

#endif
