#ifndef QNSIMPLEUPLOADDATA_H
#define QNSIMPLEUPLOADDATA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QtNetwork/QNetworkReply>
#include "ttkprivate.h"
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
    explicit QNSimpleUploadData(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

    /*!
     * Get uplaod data to server request.
     */
    void uploadDataToServer(const QString &time, const QByteArray &data, const QString &bucket, const QString &key, const QString &name);
    /*!
     * Get download url request.
     */
    QString getDownloadUrl(const QString &domain, const QString &key);

Q_SIGNALS:
    /*!
     * Uplaod file finshed.
     */
    void uploadFileFinished(const QString &time);
    /*!
     * Show upload progress.
     */
    void uploadProgressChanged(const QString &time, qint64 bytesSent, qint64 bytesTotal);

private Q_SLOTS:
    /*!
     * Receive data from server finshed.
     */
    void receiveDataFromServer();
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);
    /*!
     * Show upload progress.
     */
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    TTK_DECLARE_PRIVATE(QNSimpleUploadData)

};

#endif // QNSIMPLEUPLOADDATA_H
