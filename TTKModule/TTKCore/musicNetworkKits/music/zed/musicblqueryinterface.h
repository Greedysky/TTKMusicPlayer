#ifndef MUSICBLQUERYINTERFACE_H
#define MUSICBLQUERYINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

///song url
static constexpr const char *BL_MOVIE_URL = "ZTRrNzB2YnIzbTVvRzZUdDZVeGllN09MM3J6TjhaVGplNUZUOHBFWnBtZ0FFWEh2MUN2STBWb2ZJUzlXRE5HcHo4RlhMQWZjRitha04rczNIcnlOVXpxcWVRQlp6YzBsNE9pVExuNWRDNS9zSnNKSjlyN0pOQUs2S3JOQjVkWmo=";

class QNetworkRequest;

/*! @brief The namespace of the bl request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqBLInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request) noexcept;

    /*!
     * Read mv cid from query results.
     */
    void parseFromMovieInfo(TTK::MusicSongInformation *info, QString &cid);
    /*!
     * Read mv tags(size and bitrate and url) from query results.
     */
    void parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &cid);

}

#endif // MUSICBLQUERYINTERFACE_H
