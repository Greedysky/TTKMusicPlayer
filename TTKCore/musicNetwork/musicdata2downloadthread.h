#ifndef MUSICDATA2DOWNLOADTHREAD_H
#define MUSICDATA2DOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdatadownloadthread.h"

/*! @brief The class of downloading the type of data2.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicData2DownloadThread : public MusicDataDownloadThread
{
    Q_OBJECT
public:
    MusicData2DownloadThread(const QString &url, const QString &save,
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

Q_SIGNALS:
    void data2urlHasChanged(const QString &url);
    /*!
     * Emit data2 download finished.
     */

public Q_SLOTS:
    void dataGetFinished();
    /*!
     * Data2 reply finished.
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

#endif // MUSICDATA2DOWNLOADTHREAD_H
