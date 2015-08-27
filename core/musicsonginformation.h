#ifndef QMUSICSONG_H
#define QMUSICSONG_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStringList>
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT QMusicSong
{
public:
    QMusicSong():m_musicName(""),m_musicType(""),m_musicSize(0){}
    explicit QMusicSong(const QString &musicName,
                        const QString &musicType,
                        const qint64 musicSize):
        m_musicName(musicName),m_musicType(musicType),
        m_musicSize(musicSize){}

    inline QString getMusicArtistFront() const
    {
        return m_musicName.split('-').front().trimmed();
    }
    inline QString getMusicArtistBack() const
    {
        return m_musicName.split('-').back().trimmed();
    }
    inline QString getMusicAbName() const
    {
        return m_musicName.split('-').back().trimmed();
    }

    QString m_musicName;
    QString m_musicType;
    qint64  m_musicSize;

};

#endif // QMUSICSONG_H
