#ifndef MUSICRADIOPLAYLISTTHREAD_H
#define MUSICRADIOPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include <QStringList>
#include "musicradiothreadabstract.h"

/*! @brief The class of music radio thread of playlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioPlayListThread : public MusicRadioThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRadioPlayListThread(QObject *parent = 0, QNetworkCookieJar *cookie = 0);

    virtual ~MusicRadioPlayListThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Get music playlist.
     */
    QStringList getMusicPlayList();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    QStringList m_playList;

};

#endif // MUSICRADIOPLAYLISTTHREAD_H
