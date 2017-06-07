#ifndef MUSICRADIOTHREADABSTRACT_H
#define MUSICRADIOTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicnetworkabstract.h"

const QString CHANNEL_URL  = "bkNHdDVjMFdFMllITEJKTXBSSEsyNXhHWGJ2d0RjdUtJV0hwVk0yMzVUNy9wOTBxSXBkWFQ2MDBlcmtVcHoxeg==";
const QString PLAYLIST_URL = "RzAwMGlXbFVFTC9NY29YUUo5ZlR3VjNMZmpicFkybkpZVGRtL2FIMDhaeGpmbmN3Q0h5QmhQRCtnVm5xTkRENVN4TTQ0WS9QT0Nvc1haUEY=";
const QString SONG_URL     = "Z1pzMFpFZytLQnAzaG95SHR4SkYwOUk0Z3BBTm1wVkZYTUczK1hwT0REM0tXd0hueGVFeVpVMVRmdXhVbDMrMm9KRzBleGRmMnZIN3RCWmpwOU9YWUcrQlgvMG4yWHJC";
const QString LRC_URL      = "NmdYZm9tYkZjdHJOc1ZNNjIxSFdsc0NZc0xWa1RsM0dhMCtSaTdveGZPaz0=";

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
