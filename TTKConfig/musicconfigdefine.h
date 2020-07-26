#ifndef MUSICCONFIGDEFINE_H
#define MUSICCONFIGDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "musicobject.h"
#include "ttkversion.h"

#ifdef CONFIG_OUT_BUILD
#define CONFIG_DLL_DIR              MusicObject::getAppDir() + "/" + TTKMUSIC_VERSION_STR + "/"
#else
#define CONFIG_DLL_DIR              MusicObject::getAppDir() + "/"
#endif
//
#define TTK_DOWNLOADS_DIR_FULL      CONFIG_DLL_DIR + "../" + DOWNLOADS_DIR
#define TTK_LRC_DIR_FULL            TTK_DOWNLOADS_DIR_FULL + LRC_DIR
#define TTK_MUSIC_DIR_FULL          TTK_DOWNLOADS_DIR_FULL + MUSIC_DIR
#define TTK_MOVIE_DIR_FULL          TTK_DOWNLOADS_DIR_FULL + MOVIE_DIR
#define TTK_UPDATE_DIR_FULL         TTK_DOWNLOADS_DIR_FULL + UPDATE_DIR
//
#define TTK_CACHE_DIR_FULL          CACHE_DIR_FULL
#define TTK_ART_DIR_FULL            ART_DIR_FULL
#define TTK_BACKGROUND_DIR_FULL     BACKGROUND_DIR_FULL
#define TTK_SCREEN_DIR_FULL         SCREEN_DIR_FULL
//
#define TTK_COFIGPATH_FULL          COFIGPATH_FULL
#define TTK_MUSICPATH_FULL          MUSICPATH_FULL
#define TTK_NORMALDOWNPATH_FULL     NORMALDOWNPATH_FULL
#define TTK_CLOUDDOWNPATH_FULL      CLOUDDOWNPATH_FULL
#define TTK_CLOUDUPPATH_FULL        CLOUDUPPATH_FULL
#define TTK_MUSICSEARCH_FULL        MUSICSEARCH_FULL
#define TTK_DARABASEPATH_FULL       DARABASEPATH_FULL
#define TTK_USERPATH_FULL           USERPATH_FULL
#define TTK_BARRAGEPATH_FULL        BARRAGEPATH_FULL
#define TTK_AVATAR_DIR_FULL         AVATAR_DIR_FULL
#define TTK_USER_THEME_DIR_FULL     USER_THEME_DIR_FULL
//
#define TTK_TEMPPATH_FULL           CONFIG_DLL_DIR + TEMPPATH
#define TTK_THEME_DIR_FULL          CONFIG_DLL_DIR + THEME_DIR
#define TTK_PLUGINS_DIR_FULL        CONFIG_DLL_DIR + PLUGINS_DIR
#define TTK_LANGUAGE_DIR_FULL       CONFIG_DLL_DIR + LANGUAGE_DIR


#ifdef Q_OS_WIN
#define TTK_SERVICE_FULL            CONFIG_DLL_DIR + "TTKService.exe"
#else
#define TTK_ROUTINE_FULL            CONFIG_DLL_DIR + "../TTKRoutine.sh"
#define TTK_MUSICPLAYER_FULL        CONFIG_DLL_DIR + "../TTKMusicPlayer.sh"
#define TTK_INIT_FULL               CONFIG_DLL_DIR + "TTKInit.sh"
#define TTK_CONSOLE_FULL            CONFIG_DLL_DIR + "TTKConsole.sh"
#define TTK_SERVICE_FULL            CONFIG_DLL_DIR + "TTKService.sh"
#define TTK_ROUTINECOPY_FULL        CONFIG_DLL_DIR + "TTKRoutineCopy.sh"
#endif

#endif
