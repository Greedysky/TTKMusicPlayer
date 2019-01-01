#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

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

#include "musicsingleton.h"
#include "musicdownloadthreadabstract.h"

#define M_DOWNLOAD_QUERY_PTR (MusicSingleton<MusicDownLoadQueryFactory>::createInstance())

class MusicDownLoadSimilarThread;
class MusicDownLoadSongSuggestThread;
class MusicDownLoadCommentsThread;
class MusicDownLoadDiscoverListThread;
class MusicDownLoadQueryThreadAbstract;
class MusicDownloadBackgroundThread;
class MusicTranslationThreadAbstract;

/*! @brief The class to produce the download query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryFactory
{
    TTK_DECLARE_MODULE(MusicDownLoadQueryFactory)
public:
    /*!
     * Get query thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getQueryThread(QObject *parent = nullptr);
    /*!
     * Get movie thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getMovieThread(QObject *parent = nullptr);
    /*!
     * Get album thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getAlbumThread(QObject *parent = nullptr);
    /*!
     * Get artist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getArtistThread(QObject *parent = nullptr);
    /*!
     * Get artist category thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getArtistListThread(QObject *parent = nullptr);
    /*!
     * Get toplist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getToplistThread(QObject *parent = nullptr);
    /*!
     * Get playlist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getPlaylistThread(QObject *parent = nullptr);
    /*!
     * Get recommend thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getRecommendThread(QObject *parent = nullptr);
    /*!
     * Get similar song query thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getSimilarSongThread(QObject *parent = nullptr);

    /*!
     * Get similar query thread object by type.
     */
    MusicDownLoadSimilarThread *getSimilarArtistThread(QObject *parent = nullptr);
    /*!
     * Get suggest thread object by type.
     */
    MusicDownLoadSongSuggestThread *getSuggestThread(QObject *parent = nullptr);
    /*!
     * Get song comment thread object by type.
     */
    MusicDownLoadCommentsThread *getSongCommentThread(QObject *parent = nullptr);
    /*!
     * Get playlist comment thread object by type.
     */
    MusicDownLoadCommentsThread *getPlaylistCommentThread(QObject *parent = nullptr);
    /*!
     * Get discover list thread object by type.
     */
    MusicDownLoadDiscoverListThread *getDiscoverListThread(QObject *parent = nullptr);
    /*!
     * Get translation object by type.
     */
    MusicTranslationThreadAbstract *getTranslationThread(QObject *parent = nullptr);

    /*!
     * Get download small picture object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadSmallPicThread(const QString &url, const QString &save,
                                                           MusicObject::DownloadType type,
                                                           QObject *parent = nullptr);
    /*!
     * Get download lrc object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadLrcThread(const QString &url, const QString &save,
                                                      MusicObject::DownloadType type,
                                                      QObject *parent = nullptr);
    /*!
     * Get download big picture object by type.
     */
    MusicDownloadBackgroundThread *getDownloadBigPicThread(const QString &name, const QString &save,
                                                           QObject *parent = nullptr);

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#endif // MUSICDOWNLOADQUERYFACTORY_H
