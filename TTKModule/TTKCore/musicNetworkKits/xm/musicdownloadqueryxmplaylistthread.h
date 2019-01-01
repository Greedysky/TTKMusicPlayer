#ifndef MUSICDOWNLOADQUERYXMPLAYLISTTHREAD_H
#define MUSICDOWNLOADQUERYXMPLAYLISTTHREAD_H

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

#include "musicdownloadxminterface.h"
#include "musicdownloadqueryplaylistthread.h"

/*! @brief The class to xiami query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryXMPlaylistThread : public MusicDownLoadQueryPlaylistThread,
                                                                private MusicDownLoadXMInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownLoadQueryXMPlaylistThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryXMPlaylistThread(QObject *parent = nullptr);

    /*!
     * Start to Search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &playlist) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset) override;
    /*!
     * Start to Search data.
     */
    virtual void startToSearch(const QString &playlist) override;
    /*!
     * Get playlist info.
     */
    virtual void getPlaylistInfo(MusicResultsItem &item) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Download details data from net finished.
     */
    void getDetailsFinished();

};

#endif // MUSICDOWNLOADQUERYXMPLAYLISTTHREAD_H
