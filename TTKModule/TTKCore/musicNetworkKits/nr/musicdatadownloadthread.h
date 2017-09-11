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
    MusicDataDownloadThread(const QString &url, const QString &save,
                            Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToDownload() override;
    /*!
     * Start to download data.
     */

Q_SIGNALS:
    void downloadUrlChanged(const QString &url);
    /*!
     * Update download url finished.
     */
    void downloadSpeedLabelChanged(const QString &speed, qint64 timeLeft);
    /*!
     * Update download speed label.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Update download percent\ total time and current time progress.
     */
    void createDownloadItem(const QString &name, qint64 time);
    /*!
     * Create download item from download name and total time.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) override;
    /*!
     * Get download received and total data.
     */
    virtual void updateDownloadSpeed() override;
    /*!
     * Updata download speed due the user mod the net speed limited.
     */
    void downLoadReadyRead();
    /*!
     * Download received data ready.
     */

protected:
    void startRequest(const QUrl &url);
    /*!
     * Start to download data from url.
     */

    qint64 m_createItemTime;
    bool m_redirection, m_needUpdate;

};

#endif // MUSICDATADOWNLOADTHREAD_H
