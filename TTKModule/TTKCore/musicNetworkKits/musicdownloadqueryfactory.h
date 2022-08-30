#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include "ttksingleton.h"
#include "musicabstractdownloadrequest.h"

class MusicSongSuggestRequest;
class MusicCommentsRequest;
class MusicDiscoverListRequest;
class MusicAbstractQueryRequest;
class MusicDownloadImageRequest;
class MusicTranslationRequest;

/*! @brief The class of produce the download query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownLoadQueryFactory
{
    TTK_DECLARE_MODULE(MusicDownLoadQueryFactory)
public:
    /*!
     * Make query request object by type.
     */
    MusicAbstractQueryRequest *makeQueryRequest(QObject *parent = nullptr);
    /*!
     * Make movie request object by type.
     */
    MusicAbstractQueryRequest *makeMovieRequest(QObject *parent = nullptr);
    /*!
     * Make album request object by type.
     */
    MusicAbstractQueryRequest *makeAlbumRequest(QObject *parent = nullptr);
    /*!
     * Make artist request object by type.
     */
    MusicAbstractQueryRequest *makeArtistRequest(QObject *parent = nullptr);
    /*!
     * Make artist category request object by type.
     */
    MusicAbstractQueryRequest *makeArtistListRequest(QObject *parent = nullptr);
    /*!
     * Make toplist request object by type.
     */
    MusicAbstractQueryRequest *makeToplistRequest(QObject *parent = nullptr);
    /*!
     * Make playlist request object by type.
     */
    MusicAbstractQueryRequest *makePlaylistRequest(QObject *parent = nullptr);
    /*!
     * Make recommend request object by type.
     */
    MusicAbstractQueryRequest *makeRecommendRequest(QObject *parent = nullptr);
    /*!
     * Make similar song query request object by type.
     */
    MusicAbstractQueryRequest *makeSimilarSongRequest(QObject *parent = nullptr);

    /*!
     * Make suggest request object by type.
     */
    MusicSongSuggestRequest *makeSuggestRequest(QObject *parent = nullptr);
    /*!
     * Make song comment request object by type.
     */
    MusicCommentsRequest *makeSongCommentRequest(QObject *parent = nullptr);
    /*!
     * Make playlist comment request object by type.
     */
    MusicCommentsRequest *makePlaylistCommentRequest(QObject *parent = nullptr);
    /*!
     * Make discover list request object by type.
     */
    MusicDiscoverListRequest *makeDiscoverListRequest(QObject *parent = nullptr);
    /*!
     * Make translation object by type.
     */
    MusicTranslationRequest *makeTranslationRequest(QObject *parent = nullptr);

    /*!
     * Make download small picture object by type.
     */
    MusicAbstractDownLoadRequest *makeSmallPictureRequest(const QString &url, const QString &save, MusicObject::Download type, QObject *parent = nullptr);
    /*!
     * Make download lrc object by type.
     */
    MusicAbstractDownLoadRequest *makeLrcRequest(const QString &url, const QString &save, MusicObject::Download type, QObject *parent = nullptr);
    /*!
     * Make download big picture object by type.
     */
    MusicDownloadImageRequest *makeBigPictureRequest(const QString &name, const QString &save, QObject *parent = nullptr);

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#define G_DOWNLOAD_QUERY_PTR makeMusicDownLoadQueryFactory()
TTK_MODULE_EXPORT MusicDownLoadQueryFactory* makeMusicDownLoadQueryFactory();

#endif // MUSICDOWNLOADQUERYFACTORY_H
