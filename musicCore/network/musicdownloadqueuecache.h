#ifndef MUSICDOWNLOADQUEUECACHE_H
#define MUSICDOWNLOADQUEUECACHE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadthreadabstract.h"

typedef struct DownloadData
{
    QString url;
    QString savePath;
}DownloadData;

class MUSIC_NETWORK_EXPORT MusicDownloadQueueCache : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    MusicDownloadQueueCache(const QString &url, const QString &save,
                            Download_Type type, QObject *parent = 0);
    MusicDownloadQueueCache(const QStringList &url, const QStringList &save,
                            Download_Type type, QObject *parent = 0);
    ~MusicDownloadQueueCache();

    void addImageQueue(const QStringList &url, const QStringList &savePath);
    virtual void startToDownload() override;
    void abort();

public slots:
    virtual void downLoadFinished() override;
    void readyReadSlot();
    void errorSlot(QNetworkReply::NetworkError code);

protected:
    void startDownload(const QString &url);
    void startOrderImageQueue();

    bool m_isDownload;
    bool m_isAbort;
    QList<DownloadData> m_imageQueue;
    QNetworkRequest *m_request;

};

#endif // MUSICDOWNLOADQUEUECACHE_H
