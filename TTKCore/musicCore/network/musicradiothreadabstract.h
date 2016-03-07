#ifndef MUSICRADIOTHREADABSTRACT_H
#define MUSICRADIOTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include "musicobject.h"
#include "musicglobaldefine.h"

const QString channelUrl = "http://fm.baidu.com/dev/api/?tn=channellist";
const QString playListUrl = "http://fm.baidu.com/dev/api/?tn=playlist&format=json&id=";
const QString songsUrl = "http://music.baidu.com/data/music/fmlink?type=mp3&rate=320&songIds=";

/*! @brief The class of music radio thread base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioThreadAbstract : public QObject
{
    Q_OBJECT
public:
    explicit MusicRadioThreadAbstract(QObject *parent = 0, QNetworkCookieJar *cookie = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRadioThreadAbstract();

    void deleteAll();
    /*!
     * Release the network object.
     */
    virtual void startToDownload(const QString &data) = 0;
    /*!
     * Start to download data.
     * Subclass should implement this function.
     */

Q_SIGNALS:
    void networkReplyFinished(const QString &info);
    /*!
     * Download data from net finished emit and info.
     */

public Q_SLOTS:
    virtual void downLoadFinished() = 0;
    /*!
     * Download data from net finished.
     */
    void replyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */

protected:
    QNetworkReply *m_reply;
    QNetworkCookieJar *m_cookJar;
    QNetworkAccessManager *m_manager;

};

#endif // MUSICRADIOTHREADABSTRACT_H
