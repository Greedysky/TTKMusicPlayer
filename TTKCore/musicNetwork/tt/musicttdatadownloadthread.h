#ifndef MUSICTTDATADOWNLOADTHREAD_H
#define MUSICTTDATADOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdatadownloadthread.h"

/*! @brief The class of downloading the type of ttpod data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicTTDataDownloadThread : public MusicDataDownloadThread
{
    Q_OBJECT
public:
    MusicTTDataDownloadThread(const QString &url, const QString &save,
                              Download_Type type, QObject *parent = 0);
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void deleteAll() override;
    /*!
     * Release the network object.
     */
    virtual void startToDownload() override;
    /*!
     * Start to download data.
     */

public Q_SLOTS:
    void dataGetFinished();
    /*!
     * Data reply finished.
     */
    void dataReplyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    void dataSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    /*!
     * Download ssl reply error.
     */
#endif

protected:
    QNetworkAccessManager *m_dataManager;
    QNetworkReply *m_dataReply;

};

#endif // MUSICTTDATADOWNLOADTHREAD_H
