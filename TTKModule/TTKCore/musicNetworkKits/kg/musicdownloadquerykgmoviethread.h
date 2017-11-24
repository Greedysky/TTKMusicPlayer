#ifndef MUSICDOWNLOADQUERYKGMOVIETHREAD_H
#define MUSICDOWNLOADQUERYKGMOVIETHREAD_H

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

#include "musicdownloadkginterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to kugou mv query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryKGMovieThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryKGMovieThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;
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
     * Download single data from net finished.
     */
    void singleDownLoadFinished();

protected:
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    void readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, bool more);
    /*!
     * Read mv infos from query results.
     */
    void readFromMusicMVInfo(MusicObject::MusicSongInformation *info);
    /*!
     * Read mv info attribute from query results.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key);
    /*!
     * Find time string by attrs.
     */
    QString findTimeStringByAttrs(const MusicObject::MusicSongAttributes &attrs);

};

#endif // MUSICDOWNLOADQUERYKGMOVIETHREAD_H
