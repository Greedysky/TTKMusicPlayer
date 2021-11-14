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

#include "ttksingleton.h"
#include "musicabstractdownloadrequest.h"

class MusicSimilarRequest;
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
     * Generate query request object by type.
     */
    MusicAbstractQueryRequest *generateQueryRequest(QObject *parent = nullptr);
    /*!
     * Generate movie request object by type.
     */
    MusicAbstractQueryRequest *generateMovieRequest(QObject *parent = nullptr);
    /*!
     * Generate album request object by type.
     */
    MusicAbstractQueryRequest *generateAlbumRequest(QObject *parent = nullptr);
    /*!
     * Generate artist request object by type.
     */
    MusicAbstractQueryRequest *generateArtistRequest(QObject *parent = nullptr);
    /*!
     * Generate artist category request object by type.
     */
    MusicAbstractQueryRequest *generateArtistListRequest(QObject *parent = nullptr);
    /*!
     * Generate toplist request object by type.
     */
    MusicAbstractQueryRequest *generateToplistRequest(QObject *parent = nullptr);
    /*!
     * Generate playlist request object by type.
     */
    MusicAbstractQueryRequest *generatePlaylistRequest(QObject *parent = nullptr);
    /*!
     * Generate recommend request object by type.
     */
    MusicAbstractQueryRequest *generateRecommendRequest(QObject *parent = nullptr);
    /*!
     * Generate similar song query request object by type.
     */
    MusicAbstractQueryRequest *generateSimilarSongRequest(QObject *parent = nullptr);

    /*!
     * Generate similar query request object by type.
     */
    MusicSimilarRequest *generateSimilarArtistRequest(QObject *parent = nullptr);
    /*!
     * Generate suggest request object by type.
     */
    MusicSongSuggestRequest *generateSuggestRequest(QObject *parent = nullptr);
    /*!
     * Generate song comment request object by type.
     */
    MusicCommentsRequest *generateSongCommentRequest(QObject *parent = nullptr);
    /*!
     * Generate playlist comment request object by type.
     */
    MusicCommentsRequest *generatePlaylistCommentRequest(QObject *parent = nullptr);
    /*!
     * Generate discover list request object by type.
     */
    MusicDiscoverListRequest *generateDiscoverListRequest(QObject *parent = nullptr);
    /*!
     * Generate translation object by type.
     */
    MusicTranslationRequest *generateTranslationRequest(QObject *parent = nullptr);

    /*!
     * Generate download small picture object by type.
     */
    MusicAbstractDownLoadRequest *generateSmallPictureRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent = nullptr);
    /*!
     * Generate download lrc object by type.
     */
    MusicAbstractDownLoadRequest *generateLrcRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent = nullptr);
    /*!
     * Generate download big picture object by type.
     */
    MusicDownloadImageRequest *generateBigPictureRequest(const QString &name, const QString &save, QObject *parent = nullptr);

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#define G_DOWNLOAD_QUERY_PTR GetMusicDownLoadQueryFactory()
TTK_MODULE_EXPORT MusicDownLoadQueryFactory* GetMusicDownLoadQueryFactory();

#endif // MUSICDOWNLOADQUERYFACTORY_H
