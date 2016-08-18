#ifndef MUSICRADIOTHREADABSTRACT_H
#define MUSICRADIOTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicnetworkabstract.h"

const QString channelUrl = "http://fm.baidu.com/dev/api/?tn=channellist";
const QString playListUrl = "http://fm.baidu.com/dev/api/?tn=playlist&format=json&id=";
const QString songsUrl = "http://music.baidu.com/data/music/fmlink?type=mp3&rate=320&songIds=";

/*! @brief The class of music radio thread base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioThreadAbstract : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    explicit MusicRadioThreadAbstract(QObject *parent = 0, QNetworkCookieJar *cookie = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRadioThreadAbstract();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToDownload(const QString &data) = 0;
    /*!
     * Start to download data.
     * Subclass should implement this function.
     */

protected:
    QNetworkCookieJar *m_cookJar;

};

#endif // MUSICRADIOTHREADABSTRACT_H
