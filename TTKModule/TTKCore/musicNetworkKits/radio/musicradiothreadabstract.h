#ifndef MUSICRADIOTHREADABSTRACT_H
#define MUSICRADIOTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    /*!
     * Object contsructor.
     */
    explicit MusicRadioThreadAbstract(QObject *parent = 0, QNetworkCookieJar *cookie = 0);

    virtual ~MusicRadioThreadAbstract();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     * Subclass should implement this function.
     */
    virtual void startToDownload(const QString &data) = 0;

protected:
    QNetworkCookieJar *m_cookJar;

};

#endif // MUSICRADIOTHREADABSTRACT_H
