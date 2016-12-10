#ifndef MUSICSOURCEDOWNLOADTHREAD_H
#define MUSICSOURCEDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnetworkabstract.h"

/*! @brief The class of source data download thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSourceDownloadThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    explicit MusicSourceDownloadThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSourceDownloadThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void startToDownload(const QString &url);
    /*!
     * Start to download data.
     */

Q_SIGNALS:
    void downLoadByteDataChanged(const QByteArray &data);
    /*!
     * Send download byte data from net.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    virtual void replyError(QNetworkReply::NetworkError error) override;
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) override;
    /*!
     * Download ssl reply error.
     */
#endif

};

#endif // MUSICSOURCEDOWNLOADTHREAD_H
