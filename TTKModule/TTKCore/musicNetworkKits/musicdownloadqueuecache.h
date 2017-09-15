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
MUSIC_DECLARE_LISTS(MusicDownloadQueueData)

/*! @brief The class to download data from cache queue.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadQueueCache : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
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

    ~MusicDownloadQueueCache();

    /*!
     * Get class object name.
     */
    static QString getClassName();

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
