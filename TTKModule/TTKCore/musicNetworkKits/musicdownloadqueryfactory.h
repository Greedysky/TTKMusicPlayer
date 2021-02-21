#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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
#include "musicabstractdownloadrequest.h"

class MusicSimilarRequest;
class MusicSongSuggestRequest;
class MusicCommentsRequest;
class MusicDiscoverListRequest;
class MusicAbstractQueryRequest;
class MusicDownloadBackgroundRequest;
class MusicTranslationRequest;

/*! @brief The class to produce the download query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryFactory
{
    TTK_DECLARE_MODULE(MusicDownLoadQueryFactory)
public:
    /*!
     * Get query request object by type.
     */
    MusicAbstractQueryRequest *getQueryRequest(QObject *parent = nullptr);
    /*!
     * Get movie request object by type.
     */
    MusicAbstractQueryRequest *getMovieRequest(QObject *parent = nullptr);
    /*!
     * Get album request object by type.
     */
    MusicAbstractQueryRequest *getAlbumRequest(QObject *parent = nullptr);
    /*!
     * Get artist request object by type.
     */
    MusicAbstractQueryRequest *getArtistRequest(QObject *parent = nullptr);
    /*!
     * Get artist category request object by type.
     */
    MusicAbstractQueryRequest *getArtistListRequest(QObject *parent = nullptr);
    /*!
     * Get toplist request object by type.
     */
    MusicAbstractQueryRequest *getToplistRequest(QObject *parent = nullptr);
    /*!
     * Get playlist request object by type.
     */
    MusicAbstractQueryRequest *getPlaylistRequest(QObject *parent = nullptr);
    /*!
     * Get recommend request object by type.
     */
    MusicAbstractQueryRequest *getRecommendRequest(QObject *parent = nullptr);
    /*!
     * Get similar song query request object by type.
     */
    MusicAbstractQueryRequest *getSimilarSongRequest(QObject *parent = nullptr);

    /*!
     * Get similar query request object by type.
     */
    MusicSimilarRequest *getSimilarArtistRequest(QObject *parent = nullptr);
    /*!
     * Get suggest request object by type.
     */
    MusicSongSuggestRequest *getSuggestRequest(QObject *parent = nullptr);
    /*!
     * Get song comment request object by type.
     */
    MusicCommentsRequest *getSongCommentRequest(QObject *parent = nullptr);
    /*!
     * Get playlist comment request object by type.
     */
    MusicCommentsRequest *getPlaylistCommentRequest(QObject *parent = nullptr);
    /*!
     * Get discover list request object by type.
     */
    MusicDiscoverListRequest *getDiscoverListRequest(QObject *parent = nullptr);
    /*!
     * Get translation object by type.
     */
    MusicTranslationRequest *getTranslationRequest(QObject *parent = nullptr);

    /*!
     * Get download small picture object by type.
     */
    MusicAbstractDownLoadRequest *getDownloadSmallPictureRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent = nullptr);
    /*!
     * Get download lrc object by type.
     */
    MusicAbstractDownLoadRequest *getDownloadLrcRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent = nullptr);
    /*!
     * Get download big picture object by type.
     */
    MusicDownloadBackgroundRequest *getDownloadBigPictureRequest(const QString &name, const QString &save, QObject *parent = nullptr);

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#define G_DOWNLOAD_QUERY_PTR GetMusicDownLoadQueryFactory()
MUSIC_NETWORK_EXPORT MusicDownLoadQueryFactory* GetMusicDownLoadQueryFactory();

#endif // MUSICDOWNLOADQUERYFACTORY_H
