#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
class MusicTranslationRequest;
class MusicAbstractQueryRequest;
class MusicDownloadBackgroundRequest;

/*! @brief The class of the produce the download query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownLoadQueryFactory
{
    TTK_DECLARE_MODULE(MusicDownLoadQueryFactory)
public:
    /*!
     * Make query request object by type.
     */
    MusicAbstractQueryRequest *makeQueryRequest(QObject *parent);
    /*!
     * Make movie request object by type.
     */
    MusicAbstractQueryRequest *makeMovieRequest(QObject *parent);
    /*!
     * Make album request object by type.
     */
    MusicAbstractQueryRequest *makeAlbumRequest(QObject *parent);
    /*!
     * Make artist request object by type.
     */
    MusicAbstractQueryRequest *makeArtistRequest(QObject *parent);
    /*!
     * Make artist category request object by type.
     */
    MusicAbstractQueryRequest *makeArtistListRequest(QObject *parent);
    /*!
     * Make toplist request object by type.
     */
    MusicAbstractQueryRequest *makeToplistRequest(QObject *parent);
    /*!
     * Make playlist request object by type.
     */
    MusicAbstractQueryRequest *makePlaylistRequest(QObject *parent);
    /*!
     * Make recommend request object by type.
     */
    MusicAbstractQueryRequest *makeRecommendRequest(QObject *parent);
    /*!
     * Make similar song query request object by type.
     */
    MusicAbstractQueryRequest *makeSimilarSongRequest(QObject *parent);
    /*!
     * Make suggest request object by type.
     */
    MusicSongSuggestRequest *makeSuggestRequest(QObject *parent);
    /*!
     * Make song comment request object by type.
     */
    MusicCommentsRequest *makeSongCommentRequest(QObject *parent);
    /*!
     * Make playlist comment request object by type.
     */
    MusicCommentsRequest *makePlaylistCommentRequest(QObject *parent);
    /*!
     * Make discover list request object by type.
     */
    MusicDiscoverListRequest *makeDiscoverListRequest(QObject *parent);

    /*!
     * Make translation object by type.
     */
    MusicTranslationRequest *makeTranslationRequest(const QString &data, QObject *parent);
    /*!
     * Make download lrc object by type.
     */
    MusicAbstractDownLoadRequest *makeLrcRequest(const QString &url, const QString &path, QObject *parent);
    /*!
     * Make download art cover object by type.
     */
    MusicAbstractDownLoadRequest *makeCoverRequest(const QString &url, const QString &path, QObject *parent);
    /*!
     * Make download art background object by type.
     */
    MusicDownloadBackgroundRequest *makeBackgroundRequest(const QString &name, const QString &path, QObject *parent);

private:
    TTK_DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#define G_DOWNLOAD_QUERY_PTR makeMusicDownLoadQueryFactory()
TTK_MODULE_EXPORT MusicDownLoadQueryFactory* makeMusicDownLoadQueryFactory();

#endif // MUSICDOWNLOADQUERYFACTORY_H
