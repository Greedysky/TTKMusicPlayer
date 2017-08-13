#ifndef MUSICDOWNLOADQUEUECACHE_H
#define MUSICDOWNLOADQUEUECACHE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    explicit MusicDownloadQueueCache(Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    MusicDownloadQueueCache(const MusicDownloadQueueData &data,
                            Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    MusicDownloadQueueCache(const MusicDownloadQueueDatas &datas,
                            Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicDownloadQueueCache();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void addImageQueue(const MusicDownloadQueueDatas &datas);
    /*!
     * Add image download url and save path to download queue.
     */
    virtual void startToDownload() override;
    /*!
     * Start to download data.
     */
    void abort();
    /*!
     * Abort current download thread.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void readyReadSlot();
    /*!
     * Download received data ready.
     */
    void errorSlot(QNetworkReply::NetworkError code);
    /*!
     * Download reply error.
     */

protected:
    void startDownload(const QString &url);
    /*!
     * Start to download data from url.
     */
    void startOrderImageQueue();
    /*!
     * Start to download data in order.
     */

    bool m_isDownload;
    bool m_isAbort;
    QList<MusicDownloadQueueData> m_imageQueue;
    QNetworkRequest *m_request;

};

#endif // MUSICDOWNLOADQUEUECACHE_H
