#ifndef MUSICSONGDOWNLOADTHREAD_H
#define MUSICSONGDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdatadownloadthread.h"

class MUSIC_NETWORK_EXPORT MusicSongDownloadThread : public MusicDataDownloadThread
{
    Q_OBJECT
public:
    explicit MusicSongDownloadThread(const QString &url, const QString &save,
                                     Download_Type type, QObject *parent = 0);

signals:
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    void createDownloadItem(const QString &name, qint64 time);

public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    virtual void startRequest(const QUrl &url);
    qint64 m_createItemTime;

};

#endif // MUSICSONGDOWNLOADTHREAD_H
