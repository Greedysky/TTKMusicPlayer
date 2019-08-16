#ifndef MUSICDOWNLOADQUERYWYMOVIETHREAD_H
#define MUSICDOWNLOADQUERYWYMOVIETHREAD_H

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

#include "musicdownloadwyinterface.h"
#include "musicdownloadquerymoviethread.h"

/*! @brief The class to wangyi mv query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYMovieThread : public MusicDownLoadQueryMovieThread,
                                                             private MusicDownLoadWYInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownLoadQueryWYMovieThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryWYMovieThread(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset) override;
    /*!
     * Start to search data by given id.
     */
    virtual void startToSingleSearch(const QString &text) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Download page data from net finished.
     */
    void pageDownLoadFinished();
    /*!
     * Download single data from net finished.
     */
    void singleDownLoadFinished();

protected:
    /*!
     * Start to search mv list by id.
     */
    void startMVListQuery(qint64 id);
    /*!
     * Get artist mvs count.
     */
    void getArtistMvsCount(qint64 id);

};

#endif // MUSICDOWNLOADQUERYWYMOVIETHREAD_H
