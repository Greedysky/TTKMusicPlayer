#ifndef MUSICKGQUERYINTERFACE_H
#define MUSICKGQUERYINTERFACE_H

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

static constexpr const char *KG_SEARCH_URL = "ZGdSV3NWWE1CY3hDT2FMMXNqOW5uTXExZzc2Q1RFN1FMcUp2ZEdnRFpOMVE5QjloaUlXSjIyVmZaZHFsRnd2WDFxbVFjRUNzVVZhMU5HbzFUd3R5NU5aSHVoMzVMS0NOQ3NleStXUGNiU0RZNjZtdU5zb2N1Z2FBSEtVL2JGa2JqcllOekZrMHQ2QmpWREgvN2lKaFk3d2NRUkVpYkJ5UkxETDhuUlVUSVhaODFNN3dyUGhRbVZUN01CNnFxZmlqRE5wS2l3PT0=";
static constexpr const char *KG_SONG_LRC_URL = "Yld5K1Rac2JFc3d0YThTbnluN2RkQ0U0bjBWQkxSQWJGNDZoT2xvUERFdEUzV0RkalhjbVpYT1FGcmY0UlZJTDRoTHFMMklXNWRsdyswcG5XYzArUlJ6bHhoWXdyQ3JtSXRiWEdXbE53dXNjcU1QMzRBNFBhMDdWOE95eVVURG5KRHRPNFE9PQ==";
static constexpr const char *KG_SONG_INFO_URL = "dnVZTEYrYXgxcWkvT2dmWHdLNE1oMUZURStIKytVSnBIMzVXUlk2cVkyaURGcEl1WVRZa2lIRE13dTVSQ0JUcWwwM0hkUT09";
static constexpr const char *KG_MOVIE_INFO_URL = "ajJaR0tsRUUzcUE0ZnZGYmhhR205TkwvUGFMcSt3RU5RaGhqQVFEelRjemRVR3RKNjBEVXJzaWFCU2NFNExoaTFFcDI5bkpMQlRnZlRmWHcvQS9DNzVNNGw4ek1pM2NoTjN4QVBYME4zWkxBME94Mg==";
static constexpr const char *KG_TOPLIST_URL = "OHd0ZFEzSHlaVTE5K2tibnN0Rk5naDZmbXNTM1l5bjVTVndQbVJ0OTBPTmY0VytpK0FCaVpoQmpIOThMdnhzazJsaUczSXRQamxFTExaTUZNUWlMWlVsYUVERHNzS3NydldUR0xRbXJjVVk9";

class QNetworkRequest;
struct MusicResultDataItem;

/*! @brief The namespace of the kugou request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqKGInterface
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
     * Read tags(lrc and album pic) from query results.
     */
    void parseFromSongAlbumLrc(TTK::MusicSongInformation *info);

    /*!
     * Read album id and name.
     */
    void parseFromSongAlbumInfo(TTK::MusicSongInformation *info, const QString &album);
    /*!
     * Read tags(album info) from query results.
     */
    void parseFromSongAlbumInfo(MusicResultDataItem *item, const QString &hash, const QString &album);

    /*!
     * Read tags(size and bitrate and url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate);
    /*!
     * Read tags(size and bitrate and url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key);

}

#endif // MUSICKGQUERYINTERFACE_H
