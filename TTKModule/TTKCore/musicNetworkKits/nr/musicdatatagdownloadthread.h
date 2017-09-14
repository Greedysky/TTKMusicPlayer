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
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    MusicDataTagDownloadThread(const QString &url, const QString &save,
                               Download_Type type, QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set custom tags.
     */
    void setTags(const QString &smlUrl, const QString &title, const QString &artist);
    /*!
     * Start to download data.
     */
    virtual void startToDownload() override;

Q_SIGNALS:
    /*!
     * Modify tag finished.
     */
    void finished();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Send download byte data from net.
     */
    void downLoadFinished(const QByteArray &data);

protected:
    QString m_smallPicUrl;
    QString m_title, m_artist;

};

#endif // MUSICDATATAGDOWNLOADTHREAD_H
