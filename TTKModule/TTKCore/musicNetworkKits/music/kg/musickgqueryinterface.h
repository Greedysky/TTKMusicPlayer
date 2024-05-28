#ifndef MUSICKGQUERYINTERFACE_H
#define MUSICKGQUERYINTERFACE_H

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

///cookie url
static constexpr const char *KG_UA_URL               = "cGhYNDZVdmNaVG5KZk50NVFvcUJyYWVQdmdNTkFTMmM=";
///song url
//QSszbjQ4WXNpUjVJVElVNnZFSER5TUZCaTBTQzJqdnE1MUg5eUxRTS8yZU1sZzdxRGgvQnBHdmQzOFNDTnBsZHc2RUtBb0Q0OUF4K3IrTG0=
static constexpr const char *KG_SONG_SEARCH_URL      = "ZGdSV3NWWE1CY3hDT2FMMXNqOW5uTXExZzc2Q1RFN1FMcUp2ZEdnRFpOMVE5QjloaUlXSjIyVmZaZHFsRnd2WDFxbVFjRUNzVVZhMU5HbzFUd3R5NU5aSHVoMzVMS0NOQ3NleStXUGNiU0RZNjZtdU5zb2N1Z2FBSEtVL2JGa2JqcllOekZrMHQ2QmpWREgvN2lKaFk3d2NRUkVpYkJ5UkxETDhuUlVUSVhaODFNN3dyUGhRbVZUN01CNnFxZmlqRE5wS2l3PT0=";
static constexpr const char *KG_SONG_LRC_URL         = "Yld5K1Rac2JFc3d0YThTbnluN2RkQ0U0bjBWQkxSQWJGNDZoT2xvUERFdEUzV0RkalhjbVpYT1FGcmY0UlZJTDRoTHFMMklXNWRsdyswcG5XYzArUlJ6bHhoWXdyQ3JtSXRiWEdXbE53dXNjcU1QMzRBNFBhMDdWOE95eVVURG5KRHRPNFE9PQ==";
static constexpr const char *KG_SONG_PATH_V1_URL     = "c0FiK1UzOWFtRGNobi9oYlo1WmpEbWUwRzR0UStHd1lkZFFRakxkSGlMZmpHMmoxOWc4NGtGa3NJRUtiVXJEYlJqYzNYNmRHNlZ0dUZIR05Rb0RZcUgyb2lFMm5pNU9KaDhMeHNkQnJzcWJqeHhwQWdpUXBPV1ZEQ05RT0kvNE5IWG9qZ2hYYk9BMG8yQ25qbzFYUGFYeUVMWVk9";
static constexpr const char *KG_SONG_PATH_V2_URL     = "Q3dWL3pJbVlWTDY1Nk0vT3BkYWNGNC9ObWEyVmtyQ3lqVFlUZjE1VjU2V0ZhZFN2L1hFUXlob1JxM2Zjak9KcjBPZGE1QkxQRlhUaFpEMWxQNDMzbmdjWG5kMVNxRy9zR0tvVXB5SDlZUHl1OVUzaGlVM2NHUTBTY0l4cG1GNjY0alYzdmI5eU1ldXB2KzFzU2JHMkgrTHdxaUdSL3pwY0o1bk9kSk1GcG4yK2JVSndqUFRYNXRTbkk5VHBQcVM0Y256ZElxV1BCcy80NGpXaXNvWWZvQT09";
static constexpr const char *KG_SONG_PATH_V3_URL     = "RWx2TXJmVWFSM3NGMW5NWFJackIzRlBHZUg3NW4wclB2a0hHSktoYm82SFdFZUhzdVYvVzYxYysvb0VEL0FCcGdkaFJrZz09";
static constexpr const char *KG_SONG_INFO_URL        = "dnVZTEYrYXgxcWkvT2dmWHdLNE1oMUZURStIKytVSnBIMzVXUlk2cVkyaURGcEl1WVRZa2lIRE13dTVSQ0JUcWwwM0hkUT09";
///artist url
static constexpr const char *KG_ARTIST_URL           = "WnFuNGZxOVZEZEpYekZJOEhhZzFuOENvbURCMFlBNGFlbzJhMUxMY3hSQUg3S3VyMEJRQ1BkVEJqUmJVKytmaW9vTWFUeEtvZjkvVW9HeTZDdTBTanVKeDNNU1B4aC8zSGZaZXRsYnBhNGwyYkd0anFBc2JyRS9Ebks4NlZJRjJGSTBobnlMZklvT3pYVTBHNlhRdXNRPT0=";
static constexpr const char *KG_ARTIST_INFO_URL      = "ZVl0R25mTkNsQUVIbnVKaXMzU3pvSXNFOVRqZWh2Nm5RSTFPMHNBTkpLYS9ldjVpRysyT3lYbEQ4bUtKUFNHWGZmVlJsZzBaZmMzTFJDUzdvMEhIQTF6cHVQblVNazNYUUpDN2xQbVpZZVU9";
static constexpr const char *KG_ARTIST_ALBUM_URL     = "OWJUSnVoMWRJY0FONGRTVnRoSlJ1ZkNUQjNPZnBjKzgrQzg1K0RVczJsaWprZUpXMnk1U0Z4Vm5BcXNKVlR5K0U0aWptd0hUME41eXdPa0xPMGg4Z01HTTBGVWFmTFM4WUNQaFduU3FYUXM9";
static constexpr const char *KG_ARTIST_MOVIE_URL     = "M3E4VGoxSkNEWXh4WjJSY1FRRTdGb3RJOUQ2MXI0M3hDS0Mvd1BzeFBzS3VFNGlqU0pQei9KTzhBU1VkZHZ6WUc4dDFyclhDZG55Q2Y2d1Z1YnA0QzIrYnl2d3ZKMnlQZEp5ajlnPT0=";
static constexpr const char *KG_ARTIST_LIST_URL      = "ZCt5ZDJpbFdCZHNvUWVHZXJjMDl2L0FZd01mMVZSckkxaVJLcDJXYnZyT0FWNWQyWDByNkxZNXdDWm1yZ3BpNlg4MGtsTVpzTDZNWXdjRFU=";
///album url
static constexpr const char *KG_ALBUM_URL            = "SnJGeC9vRFdybkgyQnNLMUpvaFN4c0hLMDhoVW9UVmRFNExWWW9ZMk1EaEw2cGFwend6eWJEMFgzdHkxYnRFNXVYUUhaVXp4bFE4UklTWVpNZ05CY29KYUVNYzU3S3M3T1NERFpjR0JMWURYNUpOMWJMTGZmRmh0NkdjVlNhS254YUp6eUE9PQ==";
static constexpr const char *KG_ALBUM_INFO_URL       = "ejhjT0JOd0RyMWtmaTdTa2x4REdsMXlucDkyYk8wT3RTZ21DOVVTeU15MFQ1Z1k0SVJ6bHdiY2hNVURKRE91NGdWSWc2NTJNZEsyNnF0b0YrK0E0TSs3UjRKam40VmhqM0JxTWI1dmtyR1ZQWk1tOGFnR1NGcnAzVzJ4M3dvZWU2YlhMR0IvZWVSND0=";
///movie url
static constexpr const char *KG_MOVIE_URL            = "SXBFRzlFelBDNXoybnRvamNoc2R3QTdlZ0syWXN3MjY3N1puUHZIZjJXRGYxUzBkS2xEKzdCR1NkTTA9";
static constexpr const char *KG_MOVIE_INFO_URL       = "ajJaR0tsRUUzcUE0ZnZGYmhhR205TkwvUGFMcSt3RU5RaGhqQVFEelRjemRVR3RKNjBEVXJzaWFCU2NFNExoaTFFcDI5bkpMQlRnZlRmWHcvQS9DNzVNNGw4ek1pM2NoTjN4QVBYME4zWkxBME94Mg==";
///toplist url
static constexpr const char *KG_TOPLIST_URL          = "OHd0ZFEzSHlaVTE5K2tibnN0Rk5naDZmbXNTM1l5bjVTVndQbVJ0OTBPTmY0VytpK0FCaVpoQmpIOThMdnhzazJsaUczSXRQamxFTExaTUZNUWlMWlVsYUVERHNzS3NydldUR0xRbXJjVVk9";
static constexpr const char *KG_TOPLIST_INFO_URL     = "b1QrYWx1TlJMWm00UHRMSUF3Ymcram5IWVRML1lXazdIR2I2RXVmZ0VzTUV3MjNEZG1nS003T2ZJV0c2Njl0NA==";
///comment url
static constexpr const char *KG_COMMENT_SONG_URL     = "WklSbmRxMm50MkdJOS91Y0Q1b09ET0IwMlZtNTVKRitGS3BCdkxyUkJXeHkwVGNMeVFZa29weTVEa2VFSFEzUGQvQi9DanNLNnVmcDhEdzU0bUYrMUtpOWNNaXhOTUlUWFF3RG9JSlFhbEpIMndaWS90c2xBb1Q4SWY2dG91NjYxNCtoZDJ3czI5ZU54U2ZlcTdjcGpVNTd1dFR6MGh0Tk5mNGVLT2VXRHFHb1RNZmVaR0NhU1R2VTFhZ25yTVFI";
static constexpr const char *KG_COMMENT_PLAYLIST_URL = "TEZ4NXdPYnM4MkNJSTZMNUVjU2h5cGFvY0FzWmI2Lzc1RGM5UUJTRTdVcVZEQndyMGRQZ0x0cXJlb1E0dXVqRUZSWXRoSUR4V1lRUVRLQjVuNkdyUDdFZ2x1ZitLZml2eThxU2R2TEJHYUM5a1hDak5GSS9weStvb2JYRXhKL3hkMGVMdmVVbm5DOFEwL3BTMjd6N1VvY1pXYzRlcWdpNFJKVmZqa1hEOTRWUitrVEE3UTBWcEMwMllsMXJZSjV4S1FQSE9wWTlsdmtXQ1pYaA==";
///playlist url
static constexpr const char *KG_PLAYLIST_URL         = "VXJlbFlzWElZQ25iMzhicGJIVWJqRGpMdnJJU0tlZXdUTlVpWUJkN0xxR1Qya3dud3dLY2JSaWphWTF6QUw5RDJFK3hlZWhxdGhrRWFhQkpJUFhxOGUrY1J2d1NQVGZNM29pOXVXU3h0V1hrR0RIMnhHK2JYZz09";
static constexpr const char *KG_PLAYLIST_INFO_URL    = "NHZmb0FWT2NGVmkrQ01QNFBYTXM3RGFMbkJrb3VtQXExWXBLK2JIRW5VZEQ1YUN1eEM4Y1l3U09Lb20zWDlKTHBSZXhEdHhveHdLbWFMWnhMR3FnL2c0TTk4SUFMbFdDVDFScVF1UXZMS3BrYVpqdnFURmRUUElxcXlFd3FNVTJNRDhPQ0E9PQ==";
static constexpr const char *KG_PLAYLIST_DETAIL_URL  = "cEt0bHpiRkFKMTFZbzUyQ1prK1RCMjBqWnRRMGRKY2FqYnVSRllPZHMwcmoyVkZ3WG8vTWhMVUR0ZTdIZUszYmdybXBUdEVxOHdiSEM3Nmw=";


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
    void makeRequestRawHeader(QNetworkRequest *request);

    /*!
     * Make song artist name.
     */
    QString makeSongArtist(const QString &name);

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
