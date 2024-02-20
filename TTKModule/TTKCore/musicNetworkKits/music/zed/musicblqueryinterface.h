#ifndef MUSICBLQUERYINTERFACE_H
#define MUSICBLQUERYINTERFACE_H

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

#include "musicglobaldefine.h"

///song url
static const QString BL_BUVID_URL      = "NmozU2YzaW5JbFE0QnRHVHJheWFaWk5JbjE1QWdWdFpTWFU4aFoyZEt1RU5FdVZ0T1JsV3pscUlkbFdMYVVjT1ArYStiOG9yR1RZMFZ0UVY=";
static const QString BL_MOVIE_URL      = "ZTRrNzB2YnIzbTVvRzZUdDZVeGllN09MM3J6TjhaVGplNUZUOHBFWnBtZ0FFWEh2MUN2STBWb2ZJUzlXRE5HcHo4RlhMQWZjRitha04rczNIcnlOVXpxcWVRQlp6YzBsNE9pVExuNWRDNS9zSnNKSjlyN0pOQUs2S3JOQjVkWmo=";
static const QString BL_MOVIE_DATA_URL = "ak5VTmcwemNhNXNpU1NCcXFTb28rWmo5TEdmMmtHNkh1ZUdHWWEveHFCWG80MnRyVkpHWHVNQ2VIN0dhUDJvOGRGVmdDei9paTFNPQ==";
static const QString BL_MOVIE_PLAY_URL = "a0Y2MzNmVEFOd2JHN0xuaDBjVmJEYndRNkRmb2w1YUs3K0ZoNExpYXVYR3BjaEtSemgwdXVmem5GSDhpS29Cc0RDMStIR1RuWmF5RENKYWVxcmhCRXI0dE5JdGgwVTh5YW9YWVMvK1B3bWdNZFBMag==";

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
