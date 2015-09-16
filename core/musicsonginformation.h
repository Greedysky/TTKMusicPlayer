#ifndef QMUSICSONG_H
#define QMUSICSONG_H

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
    explicit MusicSong();
    explicit MusicSong(const QString &musicName,
                       const QString &musicPath);

    QString getMusicArtistFront() const;
    QString getMusicArtistBack() const;

    inline void setMusicName(const QString &n) { m_musicName = n;}
    inline QString getMusicName() const { return m_musicName;}

    inline void setMusicPath(const QString &p) { m_musicPath = p;}
    inline QString getMusicPath() const { return m_musicPath;}

    inline void setMusicType(const QString &t) { m_musicType = t;}
    inline QString getMusicType() const { return m_musicType;}

    inline void setMusicSize(const qint64 s) { m_musicSize = s;}
    inline qint64 getMusicSize() const { return m_musicSize;}

    inline void setMusicPlayCount(const int c) { m_musicPlayCount = c;}
    inline int getMusicPlayCount() const { return m_musicPlayCount;}

protected:
    QString m_musicName;
    QString m_musicPath;
    QString m_musicType;
    qint64  m_musicSize;
    int m_musicPlayCount;

};

#endif // QMUSICSONG_H
