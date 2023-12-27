#ifndef MUSICDOWNLOADQUEUEREQUEST_H
#define MUSICDOWNLOADQUEUEREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

/*! @brief The class of the download queue data.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicDownloadQueueData
{
    QString m_url;    ///*download url*/
    QString m_path;   ///*save local path*/
};
TTK_DECLARE_LIST(MusicDownloadQueueData);


/*! @brief The class of the download data from queue request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadQueueRequest : public MusicAbstractDownLoadRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadQueueRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadQueueRequest(TTK::Download type, QObject *parent = nullptr);
    MusicDownloadQueueRequest(const MusicDownloadQueueData &data, TTK::Download type, QObject *parent = nullptr);
    MusicDownloadQueueRequest(const MusicDownloadQueueDataList &datas, TTK::Download type, QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicDownloadQueueRequest();

    /*!
     * Add image download url and save path to download queue.
     */
    void addImageQueue(const MusicDownloadQueueDataList &datas);
    /*!
     * Start to download queue data.
     */
    virtual void startRequest() override final;

    /*!
     * Abort current download request.
     */
    void abort();
    /*!
     * Clear image download url queue.
     */
    void clear();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Download received data ready.
     */
    void handleReadyRead();
    /*!
     * Download reply error.
     */
    void handleError(QNetworkReply::NetworkError code);

private:
    /*!
     * Start to download data from net.
     */
    void startDownload(const QString &url);
    /*!
     * Start to download data in order.
     */
    void startOrderImageQueue();

    bool m_isDownload, m_isAbort;
    MusicDownloadQueueDataList m_imageQueue;
    QNetworkRequest *m_request;

};

#endif // MUSICDOWNLOADQUEUEREQUEST_H
