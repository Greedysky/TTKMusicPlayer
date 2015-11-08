#ifndef MUSICOBJECT_H
#define MUSICOBJECT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDir>
#include <QMap>
#include <QSet>
#include <QApplication>

#define LRC_DOWNLOAD   "MLrc/"
#define MUSIC_DOWNLOAD "Music/"
#define MOVIE_DOWNLOAD "Movie/"
#define THEME_DOWNLOAD "MTheme/"
#define DATA_CACHED    "MCached/"
#define ART_DOWNLOAD   "MArt/"
#define TRANS_PLUGINS  "MPlugins/"
#define ART_BG         "MArt/background/"
#define TR_LANGUAGE    "MLanguage/"
#define TMP_DOWNLOAD   "temporary"

#define MUSIC_FILE     ".mp3"
#define SKN_FILE       ".skn"
#define JPG_FILE       ".jpg"
#define LRC_FILE       ".lrc"

#define MAKE_TRANSFORM "MPlugins/avconv.dll"
#define MAKE_RING      "MPlugins/avring.dll"
#define MAKE_RADIO     "MPlugins/avradio.dll"

#define COFIGPATH      "musicconfig.xml"
#define DATABASETYPE   "QSQLITE"
#define NETDADIOPATH   "musicradio.dll"
#define MUSICPATH      "music.dll"
#define DOWNLOADINFO   "music_1.dll"
#define MUSICSEARCH    "music_2.dll"
#define DARABASEPATH   "musicuser_1.dll"
#define USERPATH       "musicuser_2.dll"

#define USERID         20
#define PASSWD         100
#define EMAIL          20
#define USERNAME       20
#define LOGINTIME      20

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

typedef struct SongUrlFormat
{
    QString m_format;                      /* 500p or 720p */
    QString m_url;
}SongUrlFormat;
typedef QList<SongUrlFormat> SongUrlFormats;

typedef struct DownloadSongInfo
{
    SongUrlFormats m_songUrl;
    QString m_lrcUrl;
    QString m_smallPicUrl;
    QString m_singerName;
    QString m_songName;
    QString m_size;
    QString m_format;
}DownloadSongInfo;
typedef QList<DownloadSongInfo> DownloadSongInfos;

namespace MusicObject
{
    enum DownLoadType{ DisConnection, DownLoading, Buffing, Waiting };

    enum SongPlayType{ MC_PlayOrder = 1, MC_PlayRandom,  MC_PlayListLoop,
                       MC_PlayOneLoop, MC_PlayOnce };

    enum SpecialEQ { EQ_NULL = -1, EQ_EchoEffect, EQ_MixChannelEffect, EQ_ReverseEffect,
                     EQ_SideCutEffect, EQ_CenterCutEffect, EQ_RateUpEffect, EQ_RateDownEffect,
                     EQ_PitchUpEffect, EQ_PitchDownEffect, EQ_TempoUpEffect, EQ_TempoDownEffect,
                     EQ_FadeOutEffect, EQ_FadeInEffect };

    static void dirIsExist(const QString& name)
    {
        QDir dir;
        if(!dir.exists(name))
        {
            dir.mkdir(name);
        }
    }

    static QString getAppDir()
    {
        QString path = QApplication::applicationDirPath();
#ifdef QT_DEBUG
        path.chop(5);
        return path;
#else
        return path + "/";
#endif
    }

    static void checkTheDirectoryExist()
    {
        dirIsExist(getAppDir() + LRC_DOWNLOAD);
        dirIsExist(getAppDir() + MUSIC_DOWNLOAD);
        dirIsExist(getAppDir() + MOVIE_DOWNLOAD);
        dirIsExist(getAppDir() + DATA_CACHED);
        dirIsExist(getAppDir() + THEME_DOWNLOAD);
        dirIsExist(getAppDir() + ART_DOWNLOAD);
        dirIsExist(getAppDir() + TRANS_PLUGINS);
        dirIsExist(getAppDir() + ART_BG);
        dirIsExist(getAppDir() + TR_LANGUAGE);
    }

    static bool checkTheFileExist()
    {
        return QFile::exists(getAppDir() + DOWNLOADINFO) &&
               QFile::exists(getAppDir() + DARABASEPATH) &&
               QFile::exists(getAppDir() + USERPATH) &&
               QFile::exists(getAppDir() + COFIGPATH) &&
               QFile::exists(getAppDir() + MUSICPATH) &&
               QFile::exists(getAppDir() + MUSICSEARCH) &&
               QFile::exists(getAppDir() + NETDADIOPATH);
    }

    static QString getLanguageName(int index)
    {
        QString lan(getAppDir() + TR_LANGUAGE);
        switch(index)
        {
            case 0 : return lan.append("cn.ln");
            case 1 : return lan.append("cn_c.ln");
            case 2 : return lan.append("en.ln");
            default: return QString();
        }
    }

}

#endif // MUSICOBJECT_H
