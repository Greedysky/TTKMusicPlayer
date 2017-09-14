#ifndef MUSICDOWNLOADSOURCETHREAD_H
#define MUSICDOWNLOADSOURCETHREAD_H

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
class MUSIC_NETWORK_EXPORT MusicDownloadSourceThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadSourceThread(QObject *parent = 0);

    ~MusicDownloadSourceThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    void startToDownload(const QString &url);

Q_SIGNALS:
    /*!
     * Send download byte data from net.
     */
    void downLoadByteDataChanged(const QByteArray &data);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Download reply error.
     */
    virtual void replyError(QNetworkReply::NetworkError error) override;
#ifndef QT_NO_SSL
    /*!
     * Download ssl reply error.
     */
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) override;
#endif

};

#endif // MUSICDOWNLOADSOURCETHREAD_H
