#ifndef MUSICDATADOWNLOADTHREAD_H
#define MUSICDATADOWNLOADTHREAD_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadthreadabstract.h"

class MUSIC_NETWORK_EXPORT MusicDataDownloadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    MusicDataDownloadThread(const QString &url, const QString &save,
                            Download_Type type, QObject *parent = 0);
    virtual void startToDownload() override;

signals:
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    void createDownloadItem(const QString &name, qint64 time);

public slots:
    virtual void downLoadFinished() override;
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) override;
    void downLoadReadyRead();

protected:
    virtual void startRequest(const QUrl &url);
    qint64 m_createItemTime;

};

#endif // MUSICDATADOWNLOADTHREAD_H
