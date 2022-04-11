#ifndef MUSICOBJECT_H
#define MUSICOBJECT_H

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

#include <QDir>
#include <QCoreApplication>

#include "ttkglobal.h"

//
#define TKF_FILE_PREFIX         "tkf"
#define TKX_FILE_PREFIX         "tkx"
#define TKM_FILE_PREFIX         "tkm"
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
#define TPL_FILE_PREFIX         "tkpl"
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


#define TKF_FILE                TTK_STRCAT(TTK_DOT, TKF_FILE_PREFIX)
#define TKX_FILE                TTK_STRCAT(TTK_DOT, TKX_FILE_PREFIX)
#define TKM_FILE                TTK_STRCAT(TTK_DOT, TKM_FILE_PREFIX)
//
#define SKN_FILE                TTK_STRCAT(TTK_DOT, SKN_FILE_PREFIX)
#define JPG_FILE                TTK_STRCAT(TTK_DOT, JPG_FILE_PREFIX)
#define BMP_FILE                TTK_STRCAT(TTK_DOT, BMP_FILE_PREFIX)
#define PNG_FILE                TTK_STRCAT(TTK_DOT, PNG_FILE_PREFIX)
#define LRC_FILE                TTK_STRCAT(TTK_DOT, LRC_FILE_PREFIX)
#define KRC_FILE                TTK_STRCAT(TTK_DOT, KRC_FILE_PREFIX)
#define MP3_FILE                TTK_STRCAT(TTK_DOT, MP3_FILE_PREFIX)
#define TPL_FILE                TTK_STRCAT(TTK_DOT, TPL_FILE_PREFIX)
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
#define ARCHIVE_DIR             TTK_STRCAT("Archive", TTK_SEPARATOR)
//
#define LANGUAGE_DIR            TTK_STRCAT("GLanguage", TTK_SEPARATOR)
#define PLUGINS_DIR             TTK_STRCAT("GPlugins", TTK_SEPARATOR)
#define THEME_DIR               TTK_STRCAT("GTheme", TTK_SEPARATOR)
//
#define ART_DIR                 TTK_STRCAT("Art", TTK_SEPARATOR)
#define BACKGROUND_DIR          TTK_STRCAT("Background", TTK_SEPARATOR)
#define CACHE_DIR               TTK_STRCAT("Cache", TTK_SEPARATOR)
//
#define CONFIG_DIR              TTK_STRCAT("config", TTK_SEPARATOR)
#define USER_THEME_DIR          TTK_STRCAT("theme", TTK_SEPARATOR)


#define MAKE_TRANSFORM_PREFIX   TTK_STRCAT("avconv", TKX_FILE)
#define MAKE_KRC2LRC_PREFIX     TTK_STRCAT("avk2l", TKX_FILE)
#define MAKE_PLAYER_PREFIX      TTK_STRCAT("avplayer", TKX_FILE)
#define MAKE_GAIN_PREFIX        TTK_STRCAT("avgain", TKX_FILE)


#define MAKE_CONFIG_DIR         TTK_STRCAT(PLUGINS_DIR, CONFIG_DIR)
#define MAKE_TRANSFORM          TTK_STRCAT(PLUGINS_DIR, MAKE_TRANSFORM_PREFIX)
#define MAKE_KRC2LRC            TTK_STRCAT(PLUGINS_DIR, MAKE_KRC2LRC_PREFIX)
#define MAKE_PLAYER             TTK_STRCAT(PLUGINS_DIR, MAKE_PLAYER_PREFIX)
#define MAKE_GAIN               TTK_STRCAT(PLUGINS_DIR, MAKE_GAIN_PREFIX)


#define COFIG_PATH              TTK_STRCAT("config", XML_FILE)
#define PLAYLIST_PATH           TTK_STRCAT("playlist", TPL_FILE)
#define NORMAL_DOWN_PATH        TTK_STRCAT("download", TKF_FILE)
#define CLOUD_DOWN_PATH         TTK_STRCAT("cdownload", TKF_FILE)
#define CLOUD_UP_PATH           TTK_STRCAT("cupload", TKF_FILE)
#define SEARCH_PATH             TTK_STRCAT("search", TKF_FILE)
#define FMRADIO_PATH            TTK_STRCAT("fmradio", TKF_FILE)


#define MAIN_DIR_FULL           MusicObject::applicationPath() + TTK_PDIR
//
#define DOWNLOAD_DIR_FULL       MAIN_DIR_FULL + DOWNLOAD_DIR
#define APPDATA_DIR_FULL        MusicObject::configPath() + APPDATA_DIR
#define APPCACHE_DIR_FULL       MusicObject::configPath() + APPCACHE_DIR
//
#define LRC_DIR_FULL            DOWNLOAD_DIR_FULL + LRC_DIR
#define MUSIC_DIR_FULL          DOWNLOAD_DIR_FULL + MUSIC_DIR
#define MOVIE_DIR_FULL          DOWNLOAD_DIR_FULL + MOVIE_DIR
#define ARCHIVE_DIR_DIR_FULL    DOWNLOAD_DIR_FULL + ARCHIVE_DIR
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
#define FMRADIO_PATH_FULL       APPDATA_DIR_FULL + FMRADIO_PATH
#define USER_THEME_DIR_FULL     APPDATA_DIR_FULL + USER_THEME_DIR


#define THEME_DIR_FULL          MusicObject::applicationPath() + THEME_DIR
#define PLUGINS_DIR_FULL        MusicObject::applicationPath() + PLUGINS_DIR
#define LANGUAGE_DIR_FULL       MusicObject::applicationPath() + LANGUAGE_DIR


#define MAKE_CONFIG_DIR_FULL    MusicObject::applicationPath() + MAKE_CONFIG_DIR
#define MAKE_TRANSFORM_FULL     MusicObject::applicationPath() + MAKE_TRANSFORM
#define MAKE_KRC2LRC_FULL       MusicObject::applicationPath() + MAKE_KRC2LRC
#define MAKE_PLAYER_FULL        MusicObject::applicationPath() + MAKE_PLAYER
#define MAKE_GAIN_FULL          MusicObject::applicationPath() + MAKE_GAIN


#define WINDOW_WIDTH_MIN        1000
#define WINDOW_HEIGHT_MIN       665
#define LEFT_SIDE_WIDTH_MIN     320
#define CONCISE_WIDTH_MIN       322

#define HTTP_PREFIX             "http://"
#define HTTPS_PREFIX            "https://"

#define FILE_SUFFIX(fin)        fin.suffix().toLower()

/*! @brief The namespace of the application object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicObject
{
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

    enum QueryQuality
    {
        NoneQuality,            /*!< none*/
        StandardQuality,        /*!< sd*/
        HighQuality,            /*!< hq*/
        SuperQuality,           /*!< sq*/
        LosslessQuality         /*!< cd*/
    };


    /*! @brief The class of the music song property.
     * @author Greedysky <greedysky@163.com>
     */
    struct MusicSongProperty
    {
        int m_bitrate;
        QString m_format;
        QString m_url;
        QString m_size;

        MusicSongProperty()
        {
            m_bitrate = -1;
        }

        inline bool operator< (const MusicSongProperty &other) const
        {
            return m_bitrate < other.m_bitrate;
        }

        inline bool operator== (const MusicSongProperty &other) const
        {
            return m_bitrate == other.m_bitrate || m_url == other.m_url;
        }
    };
    TTK_DECLARE_LIST(MusicSongProperty);


    /*! @brief The class of the music artist property.
     * @author Greedysky <greedysky@163.com>
     */
    struct MusicArtistProperty
    {
        QString m_artistId;
        QString m_artistName;
    };
    TTK_DECLARE_LIST(MusicArtistProperty);


    /*! @brief The class of the music song information.
     * @author Greedysky <greedysky@163.com>
     */
    struct MusicSongInformation
    {
        QString m_artistId;
        QString m_singerName;
        QString m_songId;
        QString m_songName;
        QString m_albumId;
        QString m_albumName;
        QString m_lrcUrl;
        QString m_coverUrl;
        QString m_duration;
        QString m_year;
        QString m_discNumber;
        QString m_trackNumber;
        MusicSongPropertyList m_songProps;
    };
    TTK_DECLARE_LIST(MusicSongInformation);


    /*!
     * Get application work dir.
     */
    static QString applicationPath()
    {
        return QCoreApplication::applicationDirPath() + TTK_SEPARATOR;
    }

    /*!
     * Get application config dir.
     */
    static QString configPath()
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
