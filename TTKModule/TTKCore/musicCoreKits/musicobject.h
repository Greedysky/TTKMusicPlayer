#ifndef MUSICOBJECT_H
#define MUSICOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include <QDir>
#include <QCoreApplication>

#include "ttkglobal.h"

//
#define CFG_FILE_PREFIX         "ttk"
#define TEX_FILE_PREFIX         "tex"
#define TTS_FILE_PREFIX         "ttks"
//
#define SKN_FILE_PREFIX         "skn"
#define JPG_FILE_PREFIX         "jpg"
#define BMP_FILE_PREFIX         "bmp"
#define PNG_FILE_PREFIX         "png"
#define LRC_FILE_PREFIX         "lrc"
#define KRC_FILE_PREFIX         "krc"
#define EXE_FILE_PREFIX         "exe"
#define XML_FILE_PREFIX         "xml"
#define COM_FILE_PREFIX         "com"
// playlist ext
#define LST_FILE_PREFIX         "tkpl"
#define M3U_FILE_PREFIX         "m3u"
#define M3U8_FILE_PREFIX        "m3u8"
#define PLS_FILE_PREFIX         "pls"
#define WPL_FILE_PREFIX         "wpl"
#define XSPF_FILE_PREFIX        "xspf"
#define ASX_FILE_PREFIX         "asx"
#define CSV_FILE_PREFIX         "csv"
#define TXT_FILE_PREFIX         "txt"
#define FPL_FILE_PREFIX         "fpl"
#define DBPL_FILE_PREFIX        "dbpl"
// file ext
#define AAC_FILE_PREFIX         "aac"
#define WMA_FILE_PREFIX         "wma"
#define MP3_FILE_PREFIX         "mp3"
#define OGG_FILE_PREFIX         "ogg"
#define APE_FILE_PREFIX         "ape"
#define FLAC_FILE_PREFIX        "flac"
#define ZIP_FILE_PREFIX         "zip"


#define TTS_FILE                TTK_STRCAT(TTK_DOT, TTS_FILE_PREFIX)
#define CFG_FILE                TTK_STRCAT(TTK_DOT, CFG_FILE_PREFIX)
#define TEX_FILE                TTK_STRCAT(TTK_DOT, TEX_FILE_PREFIX)
//
#define SKN_FILE                TTK_STRCAT(TTK_DOT, SKN_FILE_PREFIX)
#define JPG_FILE                TTK_STRCAT(TTK_DOT, JPG_FILE_PREFIX)
#define BMP_FILE                TTK_STRCAT(TTK_DOT, BMP_FILE_PREFIX)
#define PNG_FILE                TTK_STRCAT(TTK_DOT, PNG_FILE_PREFIX)
#define LRC_FILE                TTK_STRCAT(TTK_DOT, LRC_FILE_PREFIX)
#define KRC_FILE                TTK_STRCAT(TTK_DOT, KRC_FILE_PREFIX)
#define MP3_FILE                TTK_STRCAT(TTK_DOT, MP3_FILE_PREFIX)
#define LST_FILE                TTK_STRCAT(TTK_DOT, LST_FILE_PREFIX)
#define EXE_FILE                TTK_STRCAT(TTK_DOT, EXE_FILE_PREFIX)
#define XML_FILE                TTK_STRCAT(TTK_DOT, XML_FILE_PREFIX)
#define COM_FILE                TTK_STRCAT(TTK_DOT, COM_FILE_PREFIX)
// file ext
#define AAC_FILE                TTK_STRCAT(TTK_DOT, AAC_FILE_PREFIX)
#define WMA_FILE                TTK_STRCAT(TTK_DOT, WMA_FILE_PREFIX)
#define MP3_FILE                TTK_STRCAT(TTK_DOT, MP3_FILE_PREFIX)
#define OGG_FILE                TTK_STRCAT(TTK_DOT, OGG_FILE_PREFIX)
#define APE_FILE                TTK_STRCAT(TTK_DOT, APE_FILE_PREFIX)
#define FLAC_FILE               TTK_STRCAT(TTK_DOT, FLAC_FILE_PREFIX)
#define ZIP_FILE                TTK_STRCAT(TTK_DOT, ZIP_FILE_PREFIX)


#define APP_NAME                "TTKMusicPlayer"
#define APP_DOT_NAME            TTK_STRCAT(APP_NAME, TTK_DOT)
#define APP_COME_NAME           TTK_STRCAT(APP_NAME, COM_FILE)
#define APP_EXE_NAME            TTK_STRCAT(APP_NAME, EXE_FILE)


#define APPDATA_DIR             TTK_STRCAT("AppData", TTK_SEPARATOR)
#define DOWNLOAD_DIR            TTK_STRCAT("Downloads", TTK_SEPARATOR)
#define APPCACHE_DIR            TTK_STRCAT("AppCache", TTK_SEPARATOR)
//
#define LRC_DIR                 TTK_STRCAT("Lyric", TTK_SEPARATOR)
#define MUSIC_DIR               TTK_STRCAT("Music", TTK_SEPARATOR)
#define MOVIE_DIR               TTK_STRCAT("Movie", TTK_SEPARATOR)
#define UPDATE_DIR              TTK_STRCAT("Update", TTK_SEPARATOR)
//
#define LANGUAGE_DIR            TTK_STRCAT("GLanguage", TTK_SEPARATOR)
#define PLUGINS_DIR             TTK_STRCAT("GPlugins", TTK_SEPARATOR)
#define THEME_DIR               TTK_STRCAT("GTheme", TTK_SEPARATOR)
//
#define ART_DIR                 TTK_STRCAT("Art", TTK_SEPARATOR)
#define BACKGROUND_DIR          TTK_STRCAT("Background", TTK_SEPARATOR)
#define CACHE_DIR               TTK_STRCAT("Cache", TTK_SEPARATOR)
//
#define USER_THEME_DIR          TTK_STRCAT("theme", TTK_SEPARATOR)


#define MAKE_TRANSFORM_PREFIX   TTK_STRCAT("avconv", TEX_FILE)
#define MAKE_KRC2LRC_PREFIX     TTK_STRCAT("avk2l", TEX_FILE)
#define MAKE_PLAYER_PREFIX      TTK_STRCAT("avplayer", TEX_FILE)
#define MAKE_GAIN_PREFIX        TTK_STRCAT("avgain", TEX_FILE)


#define MAKE_CONFIG_DIR         TTK_STRCAT(PLUGINS_DIR, TTK_STRCAT("config", TTK_SEPARATOR))
#define MAKE_TRANSFORM          TTK_STRCAT(PLUGINS_DIR, MAKE_TRANSFORM_PREFIX)
#define MAKE_KRC2LRC            TTK_STRCAT(PLUGINS_DIR, MAKE_KRC2LRC_PREFIX)
#define MAKE_PLAYER             TTK_STRCAT(PLUGINS_DIR, MAKE_PLAYER_PREFIX)
#define MAKE_GAIN               TTK_STRCAT(PLUGINS_DIR, MAKE_GAIN_PREFIX)


#define COFIG_PATH              "config.xml"
#define PLAYLIST_PATH           "playlist.tkpl"
#define NORMAL_DOWN_PATH        "download.ttk"
#define CLOUD_DOWN_PATH         "cdownload.ttk"
#define CLOUD_UP_PATH           "cupload.ttk"
#define SEARCH_PATH             "search.ttk"


#define MAIN_DIR_FULL           MusicObject::getAppDir() + TTK_PDIR
//
#define DOWNLOAD_DIR_FULL       MAIN_DIR_FULL + DOWNLOAD_DIR
#define APPDATA_DIR_FULL        MusicObject::getConfigDir() + APPDATA_DIR
#define APPCACHE_DIR_FULL       MusicObject::getConfigDir() + APPCACHE_DIR
//
#define LRC_DIR_FULL            DOWNLOAD_DIR_FULL + LRC_DIR
#define MUSIC_DIR_FULL          DOWNLOAD_DIR_FULL + MUSIC_DIR
#define MOVIE_DIR_FULL          DOWNLOAD_DIR_FULL + MOVIE_DIR
#define UPDATE_DIR_FULL         DOWNLOAD_DIR_FULL + UPDATE_DIR
//
#define CACHE_DIR_FULL          APPCACHE_DIR_FULL + CACHE_DIR
#define ART_DIR_FULL            APPCACHE_DIR_FULL + ART_DIR
#define BACKGROUND_DIR_FULL     APPCACHE_DIR_FULL + BACKGROUND_DIR
//
#define COFIG_PATH_FULL         APPDATA_DIR_FULL + COFIG_PATH
#define PLAYLIST_PATH_FULL      APPDATA_DIR_FULL + PLAYLIST_PATH
#define NORMAL_DOWN_PATH_FULL   APPDATA_DIR_FULL + NORMAL_DOWN_PATH
#define CLOUD_DOWN_PATH_FULL    APPDATA_DIR_FULL + CLOUD_DOWN_PATH
#define CLOUD_UP_PATH_FULL      APPDATA_DIR_FULL + CLOUD_UP_PATH
#define SEARCH_PATH_FULL        APPDATA_DIR_FULL + SEARCH_PATH
#define USER_THEME_DIR_FULL     APPDATA_DIR_FULL + USER_THEME_DIR


#define THEME_DIR_FULL          MusicObject::getAppDir() + THEME_DIR
#define PLUGINS_DIR_FULL        MusicObject::getAppDir() + PLUGINS_DIR
#define LANGUAGE_DIR_FULL       MusicObject::getAppDir() + LANGUAGE_DIR


#define MAKE_CONFIG_DIR_FULL    MusicObject::getAppDir() + MAKE_CONFIG_DIR
#define MAKE_TRANSFORM_FULL     MusicObject::getAppDir() + MAKE_TRANSFORM
#define MAKE_KRC2LRC_FULL       MusicObject::getAppDir() + MAKE_KRC2LRC
#define MAKE_PLAYER_FULL        MusicObject::getAppDir() + MAKE_PLAYER
#define MAKE_GAIN_FULL          MusicObject::getAppDir() + MAKE_GAIN


#define WINDOW_WIDTH_MIN        1000
#define WINDOW_HEIGHT_MIN       665
#define LEFT_SIDE_WIDTH_MIN     320
#define CONCISE_WIDTH_MIN       322


/*! @brief The namespace of the application object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicObject
{
    /*! @brief The class of the music song atrribute.
     * @author Greedysky <greedysky@163.com>
     */
    typedef struct MusicSongAttribute
    {
        int m_bitrate;
        QString m_format;
        QString m_url;
        QString m_size;

        MusicSongAttribute()
        {
            m_bitrate = -1;
        }

        inline bool operator< (const MusicSongAttribute &other) const
        {
            return m_bitrate < other.m_bitrate;
        }

        inline bool operator== (const MusicSongAttribute &other) const
        {
            return m_bitrate == other.m_bitrate || m_url == other.m_url;
        }
    }MusicSongAttribute;
    TTK_DECLARE_LIST(MusicSongAttribute)


    /*! @brief The class of the music song information.
     * @author Greedysky <greedysky@163.com>
     */
    typedef struct MusicSongInformation
    {
        MusicSongAttributes m_songAttrs;
        QString m_songId;
        QString m_albumId;
        QString m_artistId;
        QString m_lrcUrl;
        QString m_smallPicUrl;
        QString m_singerName;
        QString m_albumName;
        QString m_songName;
        QString m_duration;
        QString m_year;
        QString m_discNumber;
        QString m_trackNumber;
    }MusicSongInformation;
    TTK_DECLARE_LIST(MusicSongInformation)


    enum PlayState
    {
        StoppedState,           /*!< stop state*/
        PlayingState,           /*!< play state*/
        PausedState             /*!< pause state*/
    };

    enum PlayMode
    {
        PlayOrder,              /*!< play order*/
        PlayRandom,             /*!< play random*/
        PlaylistLoop,           /*!< play list loop*/
        PlayOneLoop,            /*!< play single loop*/
        PlayOnce                /*!< play just once*/
    };

    enum FontStyleMode
    {
        Bold =       0x00001,   /*!< font bold*/
        Italic =     0x00002,   /*!< font italic*/
        Underline =  0x00004,   /*!< font underline*/
        Overline =   0x00008,   /*!< font overline*/
        StrikeOut =  0x00010,   /*!< font strikeOut*/
        FixedPitch = 0x00020,   /*!< font fixedPitch*/
        Kerningt =   0x00040    /*!< font kerningt*/
    };


    /*!
     * Get application work dir.
     */
    static QString getAppDir()
    {
        return QCoreApplication::applicationDirPath() + TTK_SEPARATOR;
    }

    /*!
     * Get application config dir.
     */
    static QString getConfigDir()
    {
        const bool portable = QFile::exists(MAIN_DIR_FULL + "ttk_portable");
#ifdef Q_OS_WIN
        return (portable ? MAIN_DIR_FULL : QString::fromLocal8Bit(getenv("APPDATA")) + TTK_SEPARATOR) + "ttkmp/";
#else
        return (portable ? MAIN_DIR_FULL : QDir::homePath() + TTK_SEPARATOR) + ".config/ttkmp/";
#endif
    }
}

#endif // MUSICOBJECT_H
