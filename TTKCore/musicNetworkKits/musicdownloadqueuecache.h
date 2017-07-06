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

typedef struct MUSIC_NETWORK_EXPORT MusicDownloadQueueData
{
    QString m_url;        ///*download url*/
    QString m_savePath;   ///*save local path*/
}MusicDownloadQueueData;

/*! @brief The class to download data from cache queue.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadQueueCache : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    MusicDownloadQueueCache(const QString &url, const QString &save,
                            Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    MusicDownloadQueueCache(const QStringList &url, const QStringList &save,
                            Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URLs\ save local paths and download type.
     */
    ~MusicDownloadQueueCache();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void addImageQueue(const QStringList &url, const QStringList &savePath);
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
