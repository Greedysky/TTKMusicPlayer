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
static constexpr const char *BL_MOVIE_DATA_URL = "ak5VTmcwemNhNXNpU1NCcXFTb28rWmo5TEdmMmtHNkh1ZUdHWWEveHFCWG80MnRyVkpHWHVNQ2VIN0dhUDJvOGRGVmdDei9paTFNPQ==";
static constexpr const char *BL_MOVIE_PLAY_URL = "REMzSnh1WU1mZ0dkTnIvUzFpZTZDOHJ1QjRFb3IxSituTHkrMGRmRlJHRERwTHd1bFJsOEJCR2JYcnNlMDdCQk1VR2JlMzJNVUhJYWxtNS84TTFzY2h4T0w1a3pReXJXVk50WTBzQlBMSjhZZGlyU3Z6aElLbTdhVUhrMmc0RnM=";
static constexpr const char *BL_COOKIE_URL = "R0FJc1BoY0MzL0ZtdlUzbE5UcXNWY0xqZCszUE9ReDJEZ254bDkxaTdnOFQ1REllUEpFUkJUV21ac0xNNitkYXZudDc5RTNaTHdPSzFrQ25OZFYvc2RrQVpOdVU1dDRDa3VJeEp3aStuV2hwazZoNVdYSFAxeGJ4L2MrV3hqSzd1RVJSZlZwbUdQazNIUmt3TlMvT0xSQkFUUXBicUp2aDg0OU5YS0ZCcFQvMk8xTGtiZzFsc2paeEI3WnpTZkx3QUJvai82VytwYmQrelpWbUljbTdEaVZXejg0UmlHUWZNM0xmMjc4QzlUZ2VSRi9mM1FEZzQwUTFPcjRyWDU0L0t5cWl3QmZ6K0s5SEtmeW44R1l5RHRUOXZnZHNXVlpvc09nNEJ3V0tTUk1ZQzFhQWZ4N2tHS1k4Skw0PQ==";

class QNetworkRequest;

/*! @brief The namespace of the bl request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqBLInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request);

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
