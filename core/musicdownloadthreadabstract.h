#ifndef MUSICDOWNLOADTHREADABSTRACT_H
#define MUSICDOWNLOADTHREADABSTRACT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include <QFile>
#include "musiclibexportglobal.h"

class QNetworkAccessManager;

class MUSIC_CORE_EXPORT MusicDownLoadThreadAbstract : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownLoadThreadAbstract(const QString &url, const QString &save,
                                         QObject *parent = 0);
    void deleteAll();
    virtual void startToDownload() = 0;

signals:
    void musicDownLoadFinished(const QString &name);

public slots:
    virtual void downLoadFinished() = 0;
    void replyError(QNetworkReply::NetworkError error);

protected:
    QNetworkAccessManager *m_manager;
    QNetworkReply* m_reply;
    QFile *m_file;
    QString m_url;
    QString m_savePathName;

};

#endif // MUSICDOWNLOADTHREADABSTRACT_H
