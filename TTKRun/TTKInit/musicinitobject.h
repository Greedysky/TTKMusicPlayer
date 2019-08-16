#ifndef MUSICINITOBJECT_H
#define MUSICINITOBJECT_H

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

#include <QDebug>
#include "musicobject.h"
#include "ttkversion.h"
#include "musicrunglobaldefine.h"


#define S_APPDATA_DIR_FULL        MusicObject::getAppDir() + APPDATA_DIR
#define S_DOWNLOADS_DIR_FULL      MusicObject::getAppDir() + DOWNLOADS_DIR


#define S_LRC_DIR_FULL            S_DOWNLOADS_DIR_FULL + LRC_DIR
#define S_MUSIC_DIR_FULL          S_DOWNLOADS_DIR_FULL + MUSIC_DIR
#define S_MOVIE_DIR_FULL          S_DOWNLOADS_DIR_FULL + MOVIE_DIR
#define S_CACHE_DIR_FULL          S_DOWNLOADS_DIR_FULL + CACHE_DIR
#define S_ART_DIR_FULL            S_DOWNLOADS_DIR_FULL + ART_DIR
#define S_BACKGROUND_DIR_FULL     S_DOWNLOADS_DIR_FULL + BACKGROUND_DIR
#define S_UPDATE_DIR_FULL         S_DOWNLOADS_DIR_FULL + UPDATE_DIR


#define S_COFIGPATH_FULL          S_APPDATA_DIR_FULL + COFIGPATH
#define S_MUSICPATH_FULL          S_APPDATA_DIR_FULL + MUSICPATH
#define S_NORMALDOWNPATH_FULL     S_APPDATA_DIR_FULL + NORMALDOWNPATH
#define S_CLOUDDOWNPATH_FULL      S_APPDATA_DIR_FULL + CLOUDDOWNPATH
#define S_CLOUDUPPATH_FULL        S_APPDATA_DIR_FULL + CLOUDUPPATH
#define S_MUSICSEARCH_FULL        S_APPDATA_DIR_FULL + MUSICSEARCH
#define S_DARABASEPATH_FULL       S_APPDATA_DIR_FULL + DARABASEPATH
#define S_USERPATH_FULL           S_APPDATA_DIR_FULL + USERPATH
#define S_BARRAGEPATH_FULL        S_APPDATA_DIR_FULL + BARRAGEPATH
#define S_AVATAR_DIR_FULL         S_APPDATA_DIR_FULL + AVATAR_DIR
#define S_USER_THEME_DIR_FULL     S_APPDATA_DIR_FULL + USER_THEME_DIR


#define S_TEMPPATH_FULL           MusicObject::getAppDir() + TEMPPATH
#define S_THEME_DIR_FULL          MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + THEME_DIR
#define S_PLUGINS_DIR_FULL        MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + PLUGINS_DIR
#define S_LANGUAGE_DIR_FULL       MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + LANGUAGE_DIR


#ifdef Q_OS_WIN
#define S_TTKSERVICE_FULL         MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/TTKService.exe"
#else
#define S_TTKINIT_FULL            MusicObject::getAppDir() + "TTKInit.sh"
#define S_TTKMUSICPLAYER_FULL     MusicObject::getAppDir() + "TTKMusicPlayer.sh"
#define S_TTKROUTINE_FULL         MusicObject::getAppDir() + "TTKRoutine.sh"
#define S_TTKCONSOLE_FULL         MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/TTKConsole.sh"
#define S_TTKSERVICE_FULL         MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/TTKService.sh"
#define S_TTKROUTINECOPY_FULL     MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/TTKRoutineCopy.sh"
#endif


/*! @brief The class of the music init object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicInitObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicInitObject(QObject *parent = nullptr);

    /*!
     * Check current setting file's validation.
     */
    void checkValid();

    /*!
     * Init all parameter.
     */
    void init();
    /*!
     * Check current dir is exist, no, just create it.
     */
    void dirIsExist(const QString &name);
    /*!
     * Check the related dir is exist.
     */
    void checkTheDirectoryExist();
    /*!
     * Check the related file is exist.
     */
    void checkTheFileNeededExist();
    /*!
     * Copy file by overwrite.
     */
    void copyFileOverwrite(const QString &origin, const QString &des);
    /*!
     * Copy file.
     */
    void copyFile(const QString &origin, const QString &des);
    /*!
     * Copy linux shell file.
     */
    void copyLinuxShellFile(const QString &name, const QString &path);

};

#endif // MUSICINITOBJECT_H
