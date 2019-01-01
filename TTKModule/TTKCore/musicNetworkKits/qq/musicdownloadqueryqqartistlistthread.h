#ifndef MUSICDOWNLOADQUERYQQARTISTLISTTHREAD_H
#define MUSICDOWNLOADQUERYQQARTISTLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musicdownloadqqinterface.h"
#include "musicdownloadqueryartistlistthread.h"

/*! @brief The class to qq query artist list download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryQQArtistListThread : public MusicDownLoadQueryArtistListThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownLoadQueryQQArtistListThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryQQArtistListThread(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset) override;
    /*!
     * Start to Search data.
     */
    virtual void startToSearch(const QString &artistlist) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

};

#endif // MUSICDOWNLOADQUERYQQARTISTLISTTHREAD_H
