#ifndef MUSICDOWNLOADQUERYPLAYLISTTHREAD_H
#define MUSICDOWNLOADQUERYPLAYLISTTHREAD_H

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

#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryPlaylistThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownLoadQueryPlaylistThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryPlaylistThread(QObject *parent = nullptr);

    /*!
     * Start to Search data.
     */
    virtual void startToSearch(const QString &playlist) = 0;
    /*!
     * Get playlist info.
     */
    virtual void getPlaylistInfo(MusicResultsItem &item) = 0;

Q_SIGNALS:
    /*!
     * Create the current playlist item.
     */
    void createPlaylistItem(const MusicResultsItem &item);

};

#endif // MUSICDOWNLOADQUERYPLAYLISTTHREAD_H
