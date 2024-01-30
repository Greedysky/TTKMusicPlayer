#ifndef MUSICDOWNLOADDATAREQUEST_H
#define MUSICDOWNLOADDATAREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "musicabstractdownloadrequest.h"

/*! @brief The class of the download the type of data.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadDataRequest : public MusicAbstractDownLoadRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadDataRequest)
public:
    /*!
     * Object constructor provide download url save local path and download type.
     */
    MusicDownloadDataRequest(const QString &url, const QString &path, TTK::Download type, QObject *parent = nullptr);
    MusicDownloadDataRequest(const QString &url, const QString &path, TTK::Download type, TTK::Record record, QObject *parent = nullptr);

    /*!
     * Start to download data.
     */
    virtual void startToRequest() override;

Q_SIGNALS:
    /*!
     * Update download url finished.
     */
    void downloadUrlChanged(const QString &url);
    /*!
     * Update download speed label.
     */
    void downloadSpeedLabelChanged(const QString &speed, qint64 timeLeft);
    /*!
     * Update download percent\ total time and current time progress.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Create download item from download name and total time.
     */
    void createDownloadItem(const QString &name, qint64 time);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Get download received and total data.
     */
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) override final;
    /*!
     * Updata download speed due the user mod the net speed limited.
     */
    virtual void updateDownloadSpeed() override final;
    /*!
     * Download received data ready.
     */
    void handleReadyRead();

protected:
    /*!
     * Start to download data by url.
     */
    void startToRequest(const QString &url);

    qint64 m_createTime;
    bool m_redirection, m_needUpdate;
    TTK::Record m_recordType;

};

#endif // MUSICDOWNLOADDATAREQUEST_H
