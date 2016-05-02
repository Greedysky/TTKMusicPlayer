#ifndef MUSICSOURCEDOWNLOADTHREAD_H
#define MUSICSOURCEDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include "musicglobaldefine.h"

/*! @brief The class of source data download thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSourceDownloadThread : public QObject
{
    Q_OBJECT
public:
    explicit MusicSourceDownloadThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSourceDownloadThread();

    void deleteAll();
    /*!
     * Release the network object.
     */
    void startToDownload(const QString &url);
    /*!
     * Start to download data.
     */

Q_SIGNALS:
    void recievedData(const QByteArray &data);
    /*!
     * Send recieved data from net.
     */

public Q_SLOTS:
    void downLoadFinished();
    /*!
     * Download data from net finished.
     */
    void replyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    /*!
     * Download ssl reply error.
     */
#endif

protected:
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_manager;

};

#endif // MUSICSOURCEDOWNLOADTHREAD_H
