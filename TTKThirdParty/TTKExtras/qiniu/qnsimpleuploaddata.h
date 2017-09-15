#ifndef QNSIMPLEUPLOADDATA_H
#define QNSIMPLEUPLOADDATA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtNetwork/QNetworkReply>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNSimpleUploadDataPrivate;

/*! @brief The namespace of the qiniu simple upload data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleUploadData : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QNSimpleUploadData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);

    /*!
     * Get uplaod data to server request.
     */
    void uploadDataToServer(const QByteArray &data, const QString &bucket,
                            const QString &key, const QString &name);
    /*!
     * Get download url request.
     */
    QString getDownloadUrl(const QString &domain, const QString &key);

Q_SIGNALS:
    /*!
     * Uplaod file finshed.
     */
    void uploadFileFinished(const QString &name);

private Q_SLOTS:
    /*!
     * Receive data from server finshed.
     */
    void receiveDataFromServer();
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);

private:
    MUSIC_DECLARE_PRIVATE(QNSimpleUploadData)

};

#endif // QNSIMPLEUPLOADDATA_H
