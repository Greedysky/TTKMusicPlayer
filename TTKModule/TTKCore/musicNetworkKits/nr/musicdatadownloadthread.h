#ifndef MUSICDATADOWNLOADTHREAD_H
#define MUSICDATADOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadthreadabstract.h"

/*! @brief The class of downloading the type of data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDataDownloadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    MusicDataDownloadThread(const QString &url, const QString &save,
                            Download_Type type, QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    virtual void startToDownload() override;

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

};

#endif // MUSICDATADOWNLOADTHREAD_H
