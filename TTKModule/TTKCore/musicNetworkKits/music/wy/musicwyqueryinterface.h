#ifndef MUSICWYQUERYINTERFACE_H
#define MUSICWYQUERYINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musicglobaldefine.h"

static constexpr const char *WY_SEARCH_URL = "STg2eHNFN0FLWnRzeDNlQXRlaHIvVlJ6b2k5a2s3Y2JmMG9lUEQzTytuNG5JenowY3ZleC9SbXFyak1ta29pYzA3dytZZz09";
static constexpr const char *WY_SEARCH_DATA_URL = "enoyd2tlWk4xYk1JUUIraXg5bzBUV2FvMTBvVWNVMWVxUmh2YUJqcVhoMW44cldwRldISVNqK09WVml0dDg4d2YwdllwZz09";
static constexpr const char *WY_SONG_LRC_OLD_URL = "eEZRWnA5eWZZZWFvakpnQ2F2Sml1aTZQa1crekFlZUJoTXUwRUgwekRtOXFsU2l2S1BKZFdwTUVaNGRzWUJ2ZTNpdlRDNnVuTUt2M0xtUTRsaWh2NTZBQ0JacnNTbXp6";
static constexpr const char *WY_TOPLIST_URL = "VUdTa0xMeG1Nblc4cThoS2E1QzhJK0lrTHJBRmk4TUJZMC9NQW1GanZydDJmR0VJVVRZNmJQSEhOZU96aE55bTJxdW55RldJOHl6aElNdTlHbkZkSmc9PQ==";
static constexpr const char *WY_TOPLIST_DATA_URL = "dTJrZloxZDVFSU9GSEowWjFoaGI1Z2FlMFd1alE4aFRiS05zUFZHMDNJd0lJejFMSVIwWXVndWhRU2ZZRXA5WnY3b0dVcklGZHd1K3R2aEJqRVo2c0JvTkdKWkNEL3Qz";

class QNetworkRequest;

/*! @brief The namespace of the wangyi request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqWYInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request) noexcept;

    /*!
     * Make song artist name.
     */
    QString makeSongArtist(const QString &in, const QString &name);
    /*!
     * Make cover pixmap url.
     */
    QString makeCoverPixmapUrl(const QString &url);

    /*!
     * Make token query request.
     */
    QByteArray makeTokenRequest(QNetworkRequest *request, const QString &query, const QString &data);
    /*!
     * Make token query request.
     */
    QByteArray makeTokenRequest(QNetworkRequest *request, const QString &url, const QString &query, const QString &data);

    /*!
     * Read tags(size and bitrate and url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate);
    /*!
     * Read tags(size and bitrate and url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key);

}

#endif // MUSICWYQUERYINTERFACE_H
