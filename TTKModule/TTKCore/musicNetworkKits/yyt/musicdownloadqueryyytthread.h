#ifndef MUSICDOWNLOADQUERYYYTTHREAD_H
#define MUSICDOWNLOADQUERYYYTTHREAD_H

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

#include "musicdownloadquerymoviethread.h"

/*! @brief The class to yinyuetai query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryYYTThread : public MusicDownLoadQueryMovieThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownLoadQueryYYTThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryYYTThread(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    void readFromMusicMVAttribute(MusicObject::MusicSongInformation *info);
    /*!
     * Read mv info attribute from query results.
     */
    void readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, const QString &url, const QString &size, const QString &duration);

};

#endif // MUSICDOWNLOADQUERYYYTTHREAD_H
