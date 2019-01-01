#ifndef MUSICDATADOWNLOADTHREAD_H
#define MUSICDATADOWNLOADTHREAD_H

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

#include "musicdownloadthreadabstract.h"

/*! @brief The class of downloading the type of data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDataDownloadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDataDownloadThread)
public:
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    MusicDataDownloadThread(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent = nullptr);

    /*!
     * Start to download data.
     */
    virtual void startToDownload() override;

    /*!
     * Set record type.
     */
    void setRecordType(MusicObject::RecordType type);

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
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) override;
    /*!
     * Updata download speed due the user mod the net speed limited.
     */
    virtual void updateDownloadSpeed() override;
    /*!
     * Download received data ready.
     */
    void downLoadReadyRead();

protected:
    /*!
     * Start to download data from url.
     */
    void startRequest(const QUrl &url);

    qint64 m_createItemTime;
    bool m_redirection, m_needUpdate;
    MusicObject::RecordType m_recordType;
};

#endif // MUSICDATADOWNLOADTHREAD_H
