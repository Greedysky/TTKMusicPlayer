#ifndef MUSICDOWNLOADQUEUECACHE_H
#define MUSICDOWNLOADQUEUECACHE_H

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

/*! @brief The class of the download queue data.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicDownloadQueueData
{
    QString m_url;        ///*download url*/
    QString m_savePath;   ///*save local path*/
}MusicDownloadQueueData;
TTK_DECLARE_LISTS(MusicDownloadQueueData)

/*! @brief The class to download data from cache queue.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadQueueCache : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadQueueCache)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadQueueCache(MusicObject::DownloadType  type, QObject *parent = nullptr);

    /*!
     * Object contsructor.
     */
    MusicDownloadQueueCache(const MusicDownloadQueueData &data,
                            MusicObject::DownloadType  type, QObject *parent = nullptr);
    /*!
     * Object contsructor.
     */
    MusicDownloadQueueCache(const MusicDownloadQueueDatas &datas,
                            MusicObject::DownloadType  type, QObject *parent = nullptr);

    ~MusicDownloadQueueCache();

    /*!
     * Add image download url and save path to download queue.
     */
    void addImageQueue(const MusicDownloadQueueDatas &datas);
    /*!
     * Start to download data.
     */
    virtual void startToDownload() override;
    /*!
     * Abort current download thread.
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
    virtual void downLoadFinished() override;
    /*!
     * Download received data ready.
     */
    void readyReadSlot();
    /*!
     * Download reply error.
     */
    void errorSlot(QNetworkReply::NetworkError code);

protected:
    /*!
     * Start to download data from url.
     */
    void startDownload(const QString &url);
    /*!
     * Start to download data in order.
     */
    void startOrderImageQueue();

    bool m_isDownload, m_isAbort;
    QList<MusicDownloadQueueData> m_imageQueue;
    QNetworkRequest *m_request;

};

#endif // MUSICDOWNLOADQUEUECACHE_H
