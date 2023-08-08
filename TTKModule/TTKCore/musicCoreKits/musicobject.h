#ifndef MUSICOBJECT_H
#define MUSICOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "ttkqtglobal.h"
#include "ttkqtobject.h"

//
#define TKF_FILE_SUFFIX          "tkf"
#define TKX_FILE_SUFFIX          "tkx"
#define TKM_FILE_SUFFIX          "tkm"
//
#define SKN_FILE_SUFFIX          "skn"
#define JPG_FILE_SUFFIX          "jpg"
#define LRC_FILE_SUFFIX          "lrc"
#define KRC_FILE_SUFFIX          "krc"
#define XML_FILE_SUFFIX          "xml"
// playlist ext
#define TPL_FILE_SUFFIX          "tkpl"
#define M3U_FILE_SUFFIX          "m3u"
#define M3U8_FILE_SUFFIX         "m3u8"
#define PLS_FILE_SUFFIX          "pls"
#define WPL_FILE_SUFFIX          "wpl"
#define XSPF_FILE_SUFFIX         "xspf"
#define JSPF_FILE_SUFFIX         "jspf"
#define ASX_FILE_SUFFIX          "asx"
#define CSV_FILE_SUFFIX          "csv"
#define TXT_FILE_SUFFIX          "txt"
#define FPL_FILE_SUFFIX          "fpl"
#define DBPL_FILE_SUFFIX         "dbpl"
// file ext
#define MP3_FILE_SUFFIX          "mp3"
#define APE_FILE_SUFFIX          "ape"
#define FLAC_FILE_SUFFIX         "flac"


#define TKF_FILE                 TTK_STRCAT(TTK_DOT, TKF_FILE_SUFFIX)
#define TKX_FILE                 TTK_STRCAT(TTK_DOT, TKX_FILE_SUFFIX)
#define TKM_FILE                 TTK_STRCAT(TTK_DOT, TKM_FILE_SUFFIX)
//
#define SKN_FILE                 TTK_STRCAT(TTK_DOT, SKN_FILE_SUFFIX)
#define JPG_FILE                 TTK_STRCAT(TTK_DOT, JPG_FILE_SUFFIX)
#define LRC_FILE                 TTK_STRCAT(TTK_DOT, LRC_FILE_SUFFIX)
#define KRC_FILE                 TTK_STRCAT(TTK_DOT, KRC_FILE_SUFFIX)
#define MP3_FILE                 TTK_STRCAT(TTK_DOT, MP3_FILE_SUFFIX)
#define TPL_FILE                 TTK_STRCAT(TTK_DOT, TPL_FILE_SUFFIX)
#define XML_FILE                 TTK_STRCAT(TTK_DOT, XML_FILE_SUFFIX)
// file ext
#define MP3_FILE                 TTK_STRCAT(TTK_DOT, MP3_FILE_SUFFIX)
#define APE_FILE                 TTK_STRCAT(TTK_DOT, APE_FILE_SUFFIX)
#define FLAC_FILE                TTK_STRCAT(TTK_DOT, FLAC_FILE_SUFFIX)


#define APPDATA_DIR              TTK_STRCAT("AppData", TTK_SEPARATOR)
#define APPCACHE_DIR             TTK_STRCAT("AppCache", TTK_SEPARATOR)
#define DOWNLOAD_DIR             TTK_STRCAT("Downloads", TTK_SEPARATOR)
//
#define LRC_DIR                  TTK_STRCAT("Lyric", TTK_SEPARATOR)
#define MUSIC_DIR                TTK_STRCAT("Music", TTK_SEPARATOR)
#define MOVIE_DIR                TTK_STRCAT("Movie", TTK_SEPARATOR)
#define ARCHIVE_DIR              TTK_STRCAT("Archive", TTK_SEPARATOR)
//
#define LANGUAGE_DIR             TTK_STRCAT("GLanguage", TTK_SEPARATOR)
#define PLUGINS_DIR              TTK_STRCAT("GPlugins", TTK_SEPARATOR)
#define THEME_DIR                TTK_STRCAT("GTheme", TTK_SEPARATOR)
//
#define ART_DIR                  TTK_STRCAT("Art", TTK_SEPARATOR)
#define BACKGROUND_DIR           TTK_STRCAT("Background", TTK_SEPARATOR)
#define CACHE_DIR                TTK_STRCAT("Cache", TTK_SEPARATOR)
//
#define CONFIG_DIR               TTK_STRCAT("config", TTK_SEPARATOR)
#define USER_THEME_DIR           TTK_STRCAT("theme", TTK_SEPARATOR)


#define MAKE_TRANSFORM_NAME      TTK_STRCAT("avconv", TKX_FILE)
#define MAKE_PLAYER_NAME         TTK_STRCAT("avplayer", TKX_FILE)
#define MAKE_GAIN_NAME           TTK_STRCAT("avgain", TKX_FILE)


#define MAKE_CONFIG_DIR          TTK_STRCAT(PLUGINS_DIR, CONFIG_DIR)
#define MAKE_TRANSFORM_PATH      TTK_STRCAT(PLUGINS_DIR, MAKE_TRANSFORM_NAME)
#define MAKE_PLAYER_PATH         TTK_STRCAT(PLUGINS_DIR, MAKE_PLAYER_NAME)
#define MAKE_GAIN_PATH           TTK_STRCAT(PLUGINS_DIR, MAKE_GAIN_NAME)


#define COFIG_PATH               TTK_STRCAT("config", XML_FILE)
#define PLAYLIST_PATH            TTK_STRCAT("playlist", TPL_FILE)
#define NORMAL_DOWN_PATH         TTK_STRCAT("download", TKF_FILE)
#define CLOUD_DOWN_PATH          TTK_STRCAT("cdownload", TKF_FILE)
#define CLOUD_UP_PATH            TTK_STRCAT("cupload", TKF_FILE)
#define SEARCH_PATH              TTK_STRCAT("search", TKF_FILE)
#define FMRADIO_PATH             TTK_STRCAT("fmradio", TKF_FILE)


#define MAIN_DIR_FULL            TTK::applicationPath() + TTK_PDIR
//
#define DOWNLOAD_DIR_FULL        MAIN_DIR_FULL + DOWNLOAD_DIR
#define APPDATA_DIR_FULL         TTK::configPath() + APPDATA_DIR
#define APPCACHE_DIR_FULL        TTK::configPath() + APPCACHE_DIR
//
#define LRC_DIR_FULL             DOWNLOAD_DIR_FULL + LRC_DIR
#define MUSIC_DIR_FULL           DOWNLOAD_DIR_FULL + MUSIC_DIR
#define MOVIE_DIR_FULL           DOWNLOAD_DIR_FULL + MOVIE_DIR
#define ARCHIVE_DIR_DIR_FULL     DOWNLOAD_DIR_FULL + ARCHIVE_DIR
//
#define CACHE_DIR_FULL           APPCACHE_DIR_FULL + CACHE_DIR
#define ART_DIR_FULL             APPCACHE_DIR_FULL + ART_DIR
#define BACKGROUND_DIR_FULL      APPCACHE_DIR_FULL + BACKGROUND_DIR
//
#define COFIG_PATH_FULL          APPDATA_DIR_FULL + COFIG_PATH
#define PLAYLIST_PATH_FULL       APPDATA_DIR_FULL + PLAYLIST_PATH
#define NORMAL_DOWN_PATH_FULL    APPDATA_DIR_FULL + NORMAL_DOWN_PATH
#define CLOUD_DOWN_PATH_FULL     APPDATA_DIR_FULL + CLOUD_DOWN_PATH
#define CLOUD_UP_PATH_FULL       APPDATA_DIR_FULL + CLOUD_UP_PATH
#define SEARCH_PATH_FULL         APPDATA_DIR_FULL + SEARCH_PATH
#define FMRADIO_PATH_FULL        APPDATA_DIR_FULL + FMRADIO_PATH
#define USER_THEME_DIR_FULL      APPDATA_DIR_FULL + USER_THEME_DIR


#define THEME_DIR_FULL           TTK::applicationPath() + THEME_DIR
#define PLUGINS_DIR_FULL         TTK::applicationPath() + PLUGINS_DIR
#define LANGUAGE_DIR_FULL        TTK::applicationPath() + LANGUAGE_DIR


#define MAKE_CONFIG_DIR_FULL     TTK::applicationPath() + MAKE_CONFIG_DIR
#define MAKE_TRANSFORM_PATH_FULL TTK::applicationPath() + MAKE_TRANSFORM_PATH
#define MAKE_PLAYER_PATH_FULL    TTK::applicationPath() + MAKE_PLAYER_PATH
#define MAKE_GAIN_PATH_FULL      TTK::applicationPath() + MAKE_GAIN_PATH


#define WINDOW_WIDTH_MIN         1000
#define WINDOW_HEIGHT_MIN        665
#define LEFT_SIDE_WIDTH_MIN      320
#define CONCISE_WIDTH_MIN        LEFT_SIDE_WIDTH_MIN + 2


/*! @brief The namespace of the application object.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum class PlayState
    {
        Stopped,           /*!< stop state*/
        Playing,           /*!< play state*/
        Paused             /*!< pause state*/
    };

    enum class PlayMode
    {
        Order,             /*!< play order mode*/
        Random,            /*!< play random mode*/
        ListLoop,          /*!< play list loop mode*/
        OneLoop,           /*!< play single loop mode*/
        Once               /*!< play just once mode*/
    };

    enum class FontStyleMode
    {
        Bold =       0x0001,   /*!< font bold mode*/
        Italic =     0x0002,   /*!< font italic mode*/
        Underline =  0x0004,   /*!< font underline mode*/
        Overline =   0x0008,   /*!< font overline mode*/
        StrikeOut =  0x0010,   /*!< font strikeOut mode*/
        FixedPitch = 0x0020,   /*!< font fixedPitch mode*/
        Kerningt =   0x0040    /*!< font kerningt mode*/
    };

    enum class QueryQuality
    {
        None,            /*!< none quality*/
        Standard,        /*!< sd quality*/
        High,            /*!< hq quality*/
        Super,           /*!< sq quality*/
        Lossless         /*!< cd quality*/
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
            : m_bitrate(-1)
        {

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
        QString m_trackNumber;
        MusicSongPropertyList m_songProps;
    };
    TTK_DECLARE_LIST(MusicSongInformation);


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
