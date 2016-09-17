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

/*! @brief The namespace of the qiniu simple upload data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleUploadData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleUploadData(QNetworkAccessManager *networkManager,
                                QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    void uploadDataToServer(const QByteArray &data, const QString &bucket,
                            const QString &key, const QString &name);
    /*!
     * Get uplaod data to server request.
     */
    QString getDownloadUrl(const QString &domain, const QString &key);
    /*!
     * Get download url request.
     */

Q_SIGNALS:
    void uploadFileFinished(const QString &name);
    /*!
     * Uplaod file finshed.
     */

private Q_SLOTS:
    void receiveDataFromServer();
    /*!
     * Receive data from server finshed.
     */
    void handleError(QNetworkReply::NetworkError error);
    /*!
     * Get handle error.
     */

private:
    TTK_DECLARE_PRIVATE(QNSimpleUploadData)

};

#endif // QNSIMPLEUPLOADDATA_H
