#ifndef MUSICDATATAGDOWNLOADTHREAD_H
#define MUSICDATATAGDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdatadownloadthread.h"

/*! @brief The class of downloading the type of data by custom tags.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDataTagDownloadThread : public MusicDataDownloadThread
{
    Q_OBJECT
public:
    MusicDataTagDownloadThread(const QString &url, const QString &save,
                               Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setTags(const QString &smlUrl, const QString &title, const QString &artist);
    /*!
     * Set custom tags.
     */
    virtual void startToDownload() override;
    /*!
     * Start to download data.
     */

signals:
    void finished();
    /*!
     * Modify tag finished.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Send download byte data from net.
     */

protected:
    QString m_smallPicUrl;
    QString m_title, m_artist;

};

#endif // MUSICDATATAGDOWNLOADTHREAD_H
