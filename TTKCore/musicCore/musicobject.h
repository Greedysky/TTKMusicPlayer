#ifndef MUSICOBJECT_H
#define MUSICOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDir>
#include <QMap>
#include <QSet>
#include <QApplication>

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#  define MUSIC_QT_5
#  if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
#    define MUSIC_WINEXTRAS
#  endif
#endif


///////////////////////////////////////
#define LRC_DOWNLOAD       "MLrc/"
#define MUSIC_DOWNLOAD     "Music/"
#define MOVIE_DOWNLOAD     "Movie/"
#define THEME_DOWNLOAD     "MTheme/"
#define DATA_CACHED        "MCached/"
#define ART_DOWNLOAD       "MArt/"
#define TRANS_PLUGINS      "MPlugins/"
#define ART_BG             "MArt/background/"
#define TR_LANGUAGE        "MLanguage/"
#define TMP_DOWNLOAD       "temporary"

#define SKN_FILE           ".skn"
#define JPG_FILE           ".jpg"
#define BMP_FILE           ".bmp"
#define PNG_FILE           ".png"
#define LRC_FILE           ".lrc"
#define KRC_FILE           ".krc"
#define MP3_FILE           ".mp3"

#define MAKE_TRANSFORM     "MPlugins/avconv.dll"
#define MAKE_KRC2LRC       "MPlugins/avk2l.dll"
#define MAKE_RING          "MPlugins/avring.dll"
#define MAKE_PLAYER        "MPlugins/avplayer.dll"
#define MAKE_GAIN          "MPlugins/avgain.dll"
#ifdef Q_OS_UNIX
#define MAKE_NETS          "MPlugins/avnets.dll"
#endif

#define COFIGPATH          "musicconfig.xml"
#define NETDADIOPATH       "musicradio.dll"
#define MUSICPATH          "music.dll"
#define DOWNLOADINFO       "music_1.dll"
#define MUSICSEARCH        "music_2.dll"
#define DARABASEPATH       "musicuser_1.dll"
#define USERPATH           "musicuser_2.dll"
#define BARRAGEPATH        "musicbarrage"

#define SQLITE_DATABASE    "QSQLITE"
#define MYSQL_DATABASE     "QMYSQL"
#define OCI_DATABASE       "QOCI"

///////////////////////////////////////
#define LRC_DOWNLOAD_AL    MusicObject::getAppDir() + LRC_DOWNLOAD
#define MUSIC_DOWNLOAD_AL  MusicObject::getAppDir() + MUSIC_DOWNLOAD
#define MOVIE_DOWNLOAD_AL  MusicObject::getAppDir() + MOVIE_DOWNLOAD
#define THEME_DOWNLOAD_AL  MusicObject::getAppDir() + THEME_DOWNLOAD
#define DATA_CACHED_AL     MusicObject::getAppDir() + DATA_CACHED
#define ART_DOWNLOAD_AL    MusicObject::getAppDir() + ART_DOWNLOAD
#define TRANS_PLUGINS_AL   MusicObject::getAppDir() + TRANS_PLUGINS
#define ART_BG_AL          MusicObject::getAppDir() + ART_BG
#define TR_LANGUAGE_AL     MusicObject::getAppDir() + TR_LANGUAGE
#define TMP_DOWNLOAD_AL    MusicObject::getAppDir() + TMP_DOWNLOAD

#define MAKE_TRANSFORM_AL  MusicObject::getAppDir() + MAKE_TRANSFORM
#define MAKE_KRC2LRC_AL    MusicObject::getAppDir() + MAKE_KRC2LRC
#define MAKE_RING_AL       MusicObject::getAppDir() + MAKE_RING
#define MAKE_PLAYER_AL     MusicObject::getAppDir() + MAKE_PLAYER
#define MAKE_GAIN_AL       MusicObject::getAppDir() + MAKE_GAIN
#ifdef Q_OS_UNIX
#define MAKE_NETS_AL       MusicObject::getAppDir() + MAKE_NETS
#endif

#define COFIGPATH_AL       MusicObject::getAppDir() + COFIGPATH
#define NETDADIOPATH_AL    MusicObject::getAppDir() + NETDADIOPATH
#define MUSICPATH_AL       MusicObject::getAppDir() + MUSICPATH
#define DOWNLOADINFO_AL    MusicObject::getAppDir() + DOWNLOADINFO
#define MUSICSEARCH_AL     MusicObject::getAppDir() + MUSICSEARCH
#define DARABASEPATH_AL    MusicObject::getAppDir() + DARABASEPATH
#define USERPATH_AL        MusicObject::getAppDir() + USERPATH
#define BARRAGEPATH_AL     MusicObject::getAppDir() + BARRAGEPATH

///////////////////////////////////////
#define USERID             20
#define PASSWD             100
#define EMAIL              20
#define USERNAME           20
#define LOGINTIME          20
///////////////////////////////////////

typedef signed char        MInt8;         /* 8 bit signed */
typedef unsigned char      MUint8;        /* 8 bit unsigned */
typedef short              MInt16;        /* 16 bit signed */
typedef unsigned short     MUint16;       /* 16 bit unsigned */
typedef int                MInt32;        /* 32 bit signed */
typedef unsigned int       MUint32;       /* 32 bit unsigned */
typedef long long          MInt64;        /* 64 bit signed */
typedef unsigned long long MUint64;       /* 64 bit unsigned */

typedef double             MDouble;       /* double */
typedef float              MFloat;        /* float */
typedef bool               MBool;         /* bool */


typedef QList<QStringList>                 MStringLists;      /* stringlists */
typedef QList<int>                         MIntList;          /* intlist */
typedef QList<MIntList>                    MIntLists;         /* intlists */
typedef QSet<int>                          MIntSet;           /* intset */
typedef QSet<MIntSet>                      MIntSets;          /* intsets */
typedef QMap<QString, QVariant>            MStriantMap;       /* stringVariantMap */
typedef QMap<QString, QStringList>         MStringsListMap;   /* stringStrlistsMap */
typedef QMap<int, MIntList>                MIntsListMap;      /* intIntlistMap */
typedef QMap<qint64, QString>              MIntStringMap;     /* intStrMap */
typedef QMapIterator<QString, QVariant>    MStriantMapIt;     /* stringVariantMapIt */
typedef QMapIterator<QString, QStringList> MStringsListMapIt; /* stringStrlistsMapIt */
typedef QMapIterator<int, MIntList>        MIntsListMapIt;    /* intIntlistMapIt */
typedef QMapIterator<qint64, QString>      MIntStringMapIt;   /* intStrMapIt */


typedef struct MusicSongAttribute
{
    int m_bitrate;
    QString m_format;
    QString m_url;
    QString m_size;
}MusicSongAttribute;
typedef QList<MusicSongAttribute> MusicSongAttributes;


typedef struct MusicSongInfomation
{
    MusicSongAttributes m_songAttrs;
    QString m_lrcUrl;
    QString m_smallPicUrl;
    QString m_singerName;
    QString m_songName;
}MusicSongInfomation;
typedef QList<MusicSongInfomation> MusicSongInfomations;

/*! @brief The namespace of the application object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicObject
{
    enum DownLoadType
    {
        Null,           ///*network null*/
        DisConnection,  ///*network disable*/
        DownLoading,    ///*network download*/
        Buffing,        ///*network buffing*/
        Waiting         ///*network waiting*/
    };

    enum SongPlayType
    {
        MC_PlayOrder = 1,   ///*play order*/
        MC_PlayRandom,      ///*play random*/
        MC_PlayListLoop,    ///*play list loop*/
        MC_PlayOneLoop,     ///*play single loop*/
        MC_PlayOnce         ///*play just once*/
    };

    static QString getAppDir()
    {
        return QApplication::applicationDirPath() + "/";
    }
}

#endif // MUSICOBJECT_H
