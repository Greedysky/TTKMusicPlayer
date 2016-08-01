#ifndef QNSIMPLEUPLOADDATA_H
#define QNSIMPLEUPLOADDATA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNSimpleUploadDataPrivate;

class MUSIC_EXTRAS_EXPORT QNSimpleUploadData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleUploadData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);

    void uploadDataToServer(const QByteArray &data, const QString &bucket,
                            const QString &key, const QString &name);
    QString getDownloadUrl(const QString &domain, const QString &key);

Q_SIGNALS:
    void uploadFileFinished(const QString &name);

private Q_SLOTS:
    void receiveDataFromServer();
    void handleError(QNetworkReply::NetworkError error);

private:
    TTK_DECLARE_PRIVATE(QNSimpleUploadData)

};

#endif // QNSIMPLEUPLOADDATA_H
