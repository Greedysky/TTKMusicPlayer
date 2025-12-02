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

///song url
static constexpr const char *WY_SONG_SEARCH_URL = "STg2eHNFN0FLWnRzeDNlQXRlaHIvVlJ6b2k5a2s3Y2JmMG9lUEQzTytuNG5JenowY3ZleC9SbXFyak1ta29pYzA3dytZZz09";
static constexpr const char *WY_SONG_SEARCH_DATA_URL = "enoyd2tlWk4xYk1JUUIraXg5bzBUV2FvMTBvVWNVMWVxUmh2YUJqcVhoMW44cldwRldISVNqK09WVml0dDg4d2YwdllwZz09";
static constexpr const char *WY_SONG_LRC_OLD_URL = "eEZRWnA5eWZZZWFvakpnQ2F2Sml1aTZQa1crekFlZUJoTXUwRUgwekRtOXFsU2l2S1BKZFdwTUVaNGRzWUJ2ZTNpdlRDNnVuTUt2M0xtUTRsaWh2NTZBQ0JacnNTbXp6";
static constexpr const char *WY_SONG_PATH_V1_URL = "N2NkQWg4V3o0NXYvMlFHamVOWVZ5Y2NnTDk2cTNFdFVROVNwTndJV2REYXM0K01Yd2NIT1pwaGcyTGJ0QVVsNHVxT0lTZHFKNTI2QnlWMnVBWFNDdWYrb09Waz0=";
static constexpr const char *WY_SONG_PATH_V2_URL = "M21IV2dlSHNzaE1zZEpyb2hrQ0dqVEY3Ly9DZ3ZteDM1RStuSlJIRVVCRTljbWJKKzRBVnNZY3JEcEZKY045dEJ5KzM0WHArSDVCYXhrMHBoZmlTZXBWdHIvND0=";
static constexpr const char *WY_SONG_PATH_V2_DATA_URL = "ei9SdFJqZWlHWlh3eXBsWHJTS09FbUpRU1J4Zi9wZDlOQUMyVTVnL3c0YW9GSjVH";
static constexpr const char *WY_SONG_PATH_V3_URL = "cmNsdVp5dFI1TG5VMVg1N2w1bnZwOEx0TGlUazZuZ1JFMkg0dVcvS0ZKbExPL2d0UW9kT0RaZHNhMzFXcjZiVytiZmtLY2hvYjk0bVEyZDhWaldiTmx6V0pmaz0=";
static constexpr const char *WY_SONG_PATH_V3_QUERY_URL = "OCtmZUdQbVRENjVqWGo5eERUbnlnQ3BsLzVoa3p5WnFxYnJKcWZ3K1NCQWtiS01p";
static constexpr const char *WY_SONG_PATH_V3_DATA_URL = "TDlnVTZReGNvWk9YTkFFdVF4bDJJcUk4K1BHRFU1N2I3eHlNNmwwYnIzcEVRdDZVelpRVEpRODNjTDVWdXprVEJtcENlNmFmUHZXYUNsdXkxSHV6elE9PQ==";
static constexpr const char *WY_SONG_INFO_URL = "S3paVUxRY3hZVDVweWhJZFU0K0FpakhJc3R5YW1hbmpmRUVsL3JTRUdwcnlDdDlMWFJSR0ludFFPZ1MycWExVVlLNjg5Njh3VmV2cGxpQzE=";
static constexpr const char *WY_SONG_INFO_DATA_URL = "Zmh6cVNyd2REdlVlZ3JQVUZpNjd5YlBaYVR0SEQrWFAydVpzOWc9PQ==";
///artist url
static constexpr const char *WY_ARTIST_URL = "YmhRbVZDYy8rbytiRWc5cXc2MUVUdEFKRmw2eFhTVmhqWTZ6MmRVNEMzbVg0Q3BFQVF4d0VYUktBMk1aVUNVb3ZaNk5VL3BYRjBZPQ==";
static constexpr const char *WY_ARTIST_INFO_URL = "RnJtNVNTT01ncFI2cDJMRTMrSFFwWFBWVEEzQnZNYmg4TEJrRjZCeHorTE13aVNCNUxScGo2QzNzVUNKNzhKbkI2VjNKUT09";
static constexpr const char *WY_ARTIST_INFO_DATA_URL = "dmt4VnNDdWErYjRYUXBnOWdubVBIQT09";
static constexpr const char *WY_ARTIST_ALBUM_URL = "dEJpSTdiaUh3R1hnRGVDUlpCNzJKRnBqNWZHY2xLVmhUcWQ0WW1PUVY0NjlONmRvMmtGaWZiMDBscjAvd0dxZg==";
static constexpr const char *WY_ARTIST_ALBUM_DATA_URL = "Smx6dWVoWEJJOGEvdlZvRTNtcUFEVEJzWjI5NzB4bUM1anV4Z2lXa1N4RHRtTW5ZRllEeGpHSzN0VmxTTzJ4ZU93dW9Edz09";
static constexpr const char *WY_ARTIST_MOVIE_URL = "eFdablNDbDlZVzVvYlo4UU9waEFVRlluRUFzdXA4TXFzVE9RMllXWVdQUXE2WjlsRi9DMzc0VlNXQlU9";
static constexpr const char *WY_ARTIST_MOVIE_DATA_URL = "OE95dm9DdmtycVlzcnZaaWRhRFpHYjNBenl0bmFKM0ZLa1lVaFFVenBFQnZ5N2RoNE5VR3N5TzlDTitNTjFiUk9vdDZ4QndxSHY2QjVzcFlPM0laTlplUEt0bz0=";
static constexpr const char *WY_ARTIST_LIST_URL = "dkxWWG8wQTVVa0xkY1dydjlwTWtNNGtSb3RBWWZqbURVZGtCWHMraFlSNVVRWG9rSnEwSzI4M3o4SDg9";
static constexpr const char *WY_ARTIST_LIST_DATA_URL = "eWJCN095NWdwcXlZRkpIN1luQlVQV21vZXVxSytrUnIyUDFabFBDWUhvd2N0bkVhbkZyYVU3OXNjR3hJOVF5cUUzWDVQcFQ2RDh1cE1mRG52ZExFVTF0UEJDb09KdUkyWXV4T3p3PT0=";
///album url
static constexpr const char *WY_ALBUM_URL = "QzJrMDNKTGlpVHpvY2taYm5BSWZGVTZWM09xYW5STDdXRFE5eTEzczhLZHB1RWljWlIxOVhRUXVDcmpuVjJsQnhqcmZRZE9UU0JZPQ==";
///movie url
static constexpr const char *WY_MOVIE_URL = "dFowcHhjYXk3UWRkZlo2eW40NkY2TEU4VzRoMC84TFh4Wm93N1hCbWk5V21WQnZnYzIrVUhvYmVtK0xtUW90cGpidHBqdz09";
static constexpr const char *WY_MOVIE_DATA_URL = "dmt4VnNDdWErYjRYUXBnOWdubVBIQT09";
static constexpr const char *WY_VIDEO_INFO_URL = "VGVNaXVsNW9GSTA0d1VYMnFlRnhuVFoyd1BTV0U0Y1RMWGxON25tTlZ4S2lxSUthSFkvSXFOcFB1MjE1eG4xVFBhUXpITWVEZkVrbU5uNk0=";
static constexpr const char *WY_VIDEO_INFO_DATA_URL = "MTFxaHNCYnNQYnh5YWFMQUpadk9sWmRCY3FJPQ==";
static constexpr const char *WY_VIDEO_PATH_URL = "VjZLL2tEVlFnYjhQclRPMDVnblZiNmE4Q0NDMzBqa3ZaeStjallYbkdBbllYU2YxSWE0OFFIRXppVDRLeEtsM2g4OTROdz09";
static constexpr const char *WY_VIDEO_PATH_DATA_URL = "OE8vNmRpSVV6VXg4OGRKL0s5OStEdFQ4K1R2SGJlblZnSGljeVllZ1RQUW5ES1JONWIvdVhNSWdtZGc9";
///toplist url
static constexpr const char *WY_TOPLIST_URL = "VUdTa0xMeG1Nblc4cThoS2E1QzhJK0lrTHJBRmk4TUJZMC9NQW1GanZydDJmR0VJVVRZNmJQSEhOZU96aE55bTJxdW55RldJOHl6aElNdTlHbkZkSmc9PQ==";
static constexpr const char *WY_TOPLIST_DATA_URL = "dTJrZloxZDVFSU9GSEowWjFoaGI1Z2FlMFd1alE4aFRiS05zUFZHMDNJd0lJejFMSVIwWXVndWhRU2ZZRXA5WnY3b0dVcklGZHd1K3R2aEJqRVo2c0JvTkdKWkNEL3Qz";
///comment url
static constexpr const char *WY_COMMENT_SONG_URL = "VjJRaU8vR2xYZmIxM0RIMUNxU2NJRzlLSDVqZTY5ZVVXNlhUWndSWkc1TmlEajFKaGRwaS9TRThKRCtyL0Z4SWVSWmovSzhqMzFKV1R3QmxwUVBHSzA5ZGx0d2VmN0JMaVRFaVRnPT0=";
static constexpr const char *WY_COMMENT_PLAYLIST_URL = "aURTWlNhUlJISEc3VmExQ2FsUXZqS1lXOVdGSzBOc3dmaHNMK3M0U2tScnNIdHc3Y2NJSGJBVVhhUTc1eHIrNEJrdmgzUlpCOU96UmNsZEEwbnIvQkZQU0tFZU9oY0JKazNlRWZRPT0=";
static constexpr const char *WY_COMMENT_DATA_URL = "c0RITC8rR0FPY1VXdVpzT2Rsb0hJYVBITUM5cDR4d0YxTERqRzJMUUpPWlhUemt3Z2l0M0dMNTQ2MkJ0aVdCTUZaYW5pdkRDWTdsbTE5aEM=";
///playlist url
static constexpr const char *WY_PLAYLIST_URL = "aFZGMG5CbXZXNmNGaS9jZEV6S0c2RE53Tm5Ud3I3WGJSS2ErQVQ5RjdYRlhUMENuS25yRnYwVHUzMnIrQUJQaFRVTlMrSTJUM1pNPQ==";
static constexpr const char *WY_PLAYLIST_DATA_URL = "NXA2YlMvSVEyRWhvckRqclAzdDdreHY5L0JMb0tYS2tJRGhnRDlidVRUcHBFR0I5b3dXb1pPU0dDWGdpMjc3VDBpaUpBT05oNnlOc2czZmgxVGVpR2dTR01pbWhsVUdFRWhWYnpscUkzeTA9";
static constexpr const char *WY_PLAYLIST_INFO_V1_URL = "VUdTa0xMeG1Nblc4cThoS2E1QzhJK0lrTHJBRmk4TUJZMC9NQW1GanZydDJmR0VJVVRZNmJQSEhOZU96aE55bTJxdW55RldJOHl6aElNdTlHbkZkSmc9PQ==";
static constexpr const char *WY_PLAYLIST_INFO_V1_DATA_URL = "VkpFemFKQWtOY2ZXeUNpZ210VjVGYndOZ2VibUZlZHV6amZJaE5VL1czaWFzRkNqV2lYKy9CR29McXl6LzUrUldWcmtKS0Fjb0s5dFBFeXpCaHRCc1dhYlp1bGd3U1ZY";
static constexpr const char *WY_PLAYLIST_INFO_V2_URL = "VENmNTdHK2VacjVvTFp3WmFKT2wrNmNnWHM1N3FDUHNGUHZ4MWdvU2x2bzdMM0xCSXFaaFRWWnNYTkpLeGdnVw==";
static constexpr const char *WY_PLAYLIST_INFO_V2_DATA_URL = "cTlvTkRSVU9ncjYvTDBJaDZldWtNTXh0d1dLcmd6N0k=";


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
