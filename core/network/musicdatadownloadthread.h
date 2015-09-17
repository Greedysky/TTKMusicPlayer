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
    explicit MusicDataDownloadThread(const QString &url, const QString &save,
                                     Download_Type type, QObject *parent = 0);
    virtual void startToDownload();

public slots:
    virtual void downLoadFinished();
    void downLoadReadyRead();

protected:
    virtual void startRequest(const QUrl &url);

};

#endif // MUSICDATADOWNLOADTHREAD_H
