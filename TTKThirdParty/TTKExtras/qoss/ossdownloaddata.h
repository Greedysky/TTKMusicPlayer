#ifndef OSSDOWNLOADDATA_H
#define OSSDOWNLOADDATA_H

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

#include "ossdatainterface.h"

/*! @brief The class of the alioss cloud data item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT OSSDownloadData : public OSSDataInterface
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit OSSDownloadData(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

    /*!
     * Get download data operator.
     */
    void downloadDataOperator(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath);

    /*!
     * Get download url request.
     */
    QString getDownloadUrl(const QString &bucket, const QString &fileName);
    /*!
     * Get download url request by content type.
     */
    QString getDownloadUrl(const QString &bucket, const QString &fileName, const QString &contentType);

Q_SIGNALS:
    /*!
     * download file finshed.
     */
    void downloadFileFinished(const QString &time);
    /*!
     * Show download progress.
     */
    void downloadProgressChanged(const QString &time, qint64 bytesSent, qint64 bytesTotal);

protected Q_SLOTS:
    /*!
     * Receive data from server finshed.
     */
    virtual void receiveDataFromServer() override;
    /*!
     * Show download progress.
     */
    void downloadProgress(qint64 bytesSent, qint64 bytesTotal);

};

#endif // OSSDOWNLOADDATA_H
