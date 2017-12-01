#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

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

#include "musicsingleton.h"
#include "musicdownloadthreadabstract.h"

#define M_DOWNLOAD_QUERY_PTR (MusicSingleton<MusicDownLoadQueryFactory>::createInstance())

class MusicDownLoadCommentsThread;
class MusicDownLoadDiscoverListThread;
class MusicDownLoadQueryThreadAbstract;
class MusicDownloadBackgroundThread;

/*! @brief The class to produce the downlaod query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryFactory
{
public:
    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get query thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getQueryThread(QObject *parent = 0);
    /*!
     * Get movie thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getMovieThread(QObject *parent = 0);
    /*!
     * Get album thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getAlbumThread(QObject *parent = 0);
    /*!
     * Get artist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getArtistThread(QObject *parent = 0);
    /*!
     * Get toplist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getToplistThread(QObject *parent = 0);
    /*!
     * Get playlist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getPlaylistThread(QObject *parent = 0);
    /*!
     * Get recommend thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getRecommendThread(QObject *parent = 0);

    /*!
     * Get song comment thread object by type.
     */
    MusicDownLoadCommentsThread *getSongCommentThread(QObject *parent = 0);
    /*!
     * Get playlist comment thread object by type.
     */
    MusicDownLoadCommentsThread *getPlaylistCommentThread(QObject *parent = 0);

    /*!
     * Get discover list thread object by type.
     */
    MusicDownLoadDiscoverListThread *getDiscoverListThread(QObject *parent = 0);

    /*!
     * Get download small picture object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadSmallPicThread(const QString &url, const QString &save,
                                                           MusicDownLoadThreadAbstract::Download_Type type,
                                                           QObject *parent = 0);
    /*!
     * Get download lrc object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadLrcThread(const QString &url, const QString &save,
                                                      MusicDownLoadThreadAbstract::Download_Type type,
                                                      QObject *parent = 0);

    /*!
     * Get download big picture object by type.
     */

    MusicDownloadBackgroundThread *getDownloadBigPicThread(const QString &name, const QString &save,
                                                           QObject *parent = 0);

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#endif // MUSICDOWNLOADQUERYFACTORY_H
