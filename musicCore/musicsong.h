#ifndef MUSICSONG_H
#define MUSICSONG_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicSong
{
public:
    MusicSong();
    explicit MusicSong(const QString &musicPath,
                       const QString &musicName = QString());
    MusicSong(const QString &musicPath, int playCount,
              const QString &musicName = QString());
    MusicSong(const QString &musicPath, const QString &type,
              int playCount, const QString &musicName = QString());
    MusicSong(const QString &musicPath, const QString &type,
              const QString &time, int playCount,
              const QString &musicName = QString());
    MusicSong(const QString &musicPath, int playCount, const QString &time,
              const QString &musicName);

    QString getMusicArtistFront() const;
    QString getMusicArtistBack() const;

    inline void setMusicName(const QString &n) { m_musicName = n;}
    inline QString getMusicName() const { return m_musicName;}

    inline void setMusicPath(const QString &p) { m_musicPath = p;}
    inline QString getMusicPath() const { return m_musicPath;}

    inline void setMusicType(const QString &t) { m_musicType = t;}
    inline QString getMusicType() const { return m_musicType;}

    inline void setMusicTime(const QString &t) { m_musicTime = t;}
    inline QString getMusicTime() const { return m_musicTime;}

    inline void setMusicSize(const qint64 s) { m_musicSize = s;}
    inline qint64 getMusicSize() const { return m_musicSize;}

    inline void setMusicPlayCount(const int c) { m_musicPlayCount = c;}
    inline int getMusicPlayCount() const { return m_musicPlayCount;}

protected:
    QString m_musicName, m_musicPath, m_musicType, m_musicTime;
    qint64  m_musicSize;
    int m_musicPlayCount;

};

typedef QList<MusicSong>    MusicSongs;
typedef QList<MusicSongs>   MusicSongsList;

#endif // MUSICSONG_H
