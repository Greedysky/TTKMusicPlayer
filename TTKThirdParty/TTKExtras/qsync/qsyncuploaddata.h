#ifndef QSYNCUPLOADDATA_H
#define QSYNCUPLOADDATA_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include "qsyncdatainterface.h"

/*! @brief The class of the sync cloud upload data.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QSyncUploadData : public QSyncDataInterface
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit QSyncUploadData(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

    /*!
     * Get uplaod data operator.
     */
    void request(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath);

Q_SIGNALS:
    /*!
     * Uplaod file finshed.
     */
    void uploadFileFinished(const QString &time, bool state);
    /*!
     * Show upload progress.
     */
    void uploadProgressChanged(const QString &time, qint64 bytesSent, qint64 bytesTotal);

private Q_SLOTS:
    /*!
     * Receive data from server finshed.
     */
    virtual void receiveDataFromServer() override final;
    /*!
     * Show upload progress.
     */
    void uploadProgress(qint64 percent, qint64 total);

};

#endif // QSYNCUPLOADDATA_H
