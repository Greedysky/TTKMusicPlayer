#ifndef MUSICKWQUERYINTERFACE_H
#define MUSICKWQUERYINTERFACE_H

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

//TG5KazFERHlYc3hzS1JQd29ldXVEOWVRdGh2dmx0RWxma3I5cnRxekptRjdKM0NvQXo1bzV5c1ZpRDB2WGZ1UQ==
static constexpr const char *KW_SEARCH_URL = "azRpTk56R3ZhTXY5dk13YzFkaHJkMzRFY3NpRkRmUGdyUnQyWVY5NXNvOE82cUE4Mmg0VklKTTQrRFV0dEZHTExUSkJvTkRXMGdQVEpaK0Z4Rkp6ZUkrUnh1T29iSG1mTUxCNkRmMlVFT25zVzhjQUs1K0VCZk9UMWpTWGovMWR0czNlN0Z0Y1kyUCtxYlZSK3R3YnU3VTZ6dlkwanJIdGN6RWdjWEhxSi96ekRNWjBMMEMrL3JCUGxRQWJ6YkltQkRGeGljR3c4TmgxaXBXcGhzcnZlZ1p5cldlNzZTc1ZlZDFvUDcxTmtDdlJqY3JU";
static constexpr const char *KW_SONG_LRC_URL = "a2VZMjN2anovdkJJR2tmckthZmI2c1M4N0ZqOWN6WGcrbHBXbllJdnRWcS9JUnp4ZWIrTXpGODFETkVFN25QZ0lZc21HRC85bjdQWWI3b3I=";
static constexpr const char *KW_SONG_INFO_URL = "TjVRY3FwbGZRUkpITGYyczVUSGZ0MG14bEs4VEsyY3VJZE80S0lhSkF6b0p5UWsxSEl5aWpUNzdHY252aEhpSDAwZHhTR3UvM2J0ZlJ2dmc5SDRDa1ZBSGhLZy9SY2UrMllkUjJpUGtmWTA9";
static constexpr const char *KW_ALBUM_COVER_INFO_URL = "S0Z0Z0dTckV1TysxVnRGem43cU96Q0RRUVhiTS93RVhyNUFTNWNqOEduMDdMalZ1cCtGKzRudHRFcDB0enJ0bXI0VG5QVWMwQU1UKzRRdGc5clZPZ1NwKzRINjZoaDluWVAzVXpMWXlaemQvYnNjd1JwcmM5NGQ5VFhVU3k2SjE=";
static constexpr const char *KW_TOPLIST_URL = "T2QyOWl3WmttZlk1SnM4NFZ6S1BoZjhGQnV5Y0lHbm9nUDlEZUR1bDJRcmNITVZIVWxZT2YvYWtSQWxXQ1JjWE9DNjQydDdQRTQzZkZvclFOL25vd2JIUjl1T1RvRzFFOEFPSVFPVWQ0ZUR4dUVBbFYzbjhwUXdSQ09teFBucjA=";

class QNetworkRequest;

/*! @brief The namespace of the kuwo request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqKWInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request) noexcept;

    /*!
     * Make song artist name.
     */
    QString makeSongArtist(const QString &name);
    /*!
     * Make song artist name.
     */
    QString makeSongArtist(const QString &in, const QString &name);
    /*!
     * Make artist pixmap url.
     */
    QString makeArtistPixmapUrl(const QString &url);
    /*!
     * Make cover pixmap url.
     */
    QString makeCoverPixmapUrl(const QString &url, const QString &id);
    /*!
     * Read tags(size and bitrate and url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate);
    /*!
     * Read tags(size and bitrate and url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format);

}

#endif // MUSICKWQUERYINTERFACE_H
