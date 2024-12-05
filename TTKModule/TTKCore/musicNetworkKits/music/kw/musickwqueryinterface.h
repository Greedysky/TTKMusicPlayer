#ifndef MUSICKWQUERYINTERFACE_H
#define MUSICKWQUERYINTERFACE_H

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
//TG5KazFERHlYc3hzS1JQd29ldXVEOWVRdGh2dmx0RWxma3I5cnRxekptRjdKM0NvQXo1bzV5c1ZpRDB2WGZ1UQ==
static constexpr const char *KW_SONG_SEARCH_URL = "azRpTk56R3ZhTXY5dk13YzFkaHJkMzRFY3NpRkRmUGdyUnQyWVY5NXNvOE82cUE4Mmg0VklKTTQrRFV0dEZHTExUSkJvTkRXMGdQVEpaK0Z4Rkp6ZUkrUnh1T29iSG1mTUxCNkRmMlVFT25zVzhjQUs1K0VCZk9UMWpTWGovMWR0czNlN0Z0Y1kyUCtxYlZSK3R3YnU3VTZ6dlkwanJIdGN6RWdjWEhxSi96ekRNWjBMMEMrL3JCUGxRQWJ6YkltQkRGeGljR3c4TmgxaXBXcGhzcnZlZ1p5cldlNzZTc1ZlZDFvUDcxTmtDdlJqY3JU";
static constexpr const char *KW_SONG_LRC_URL = "a2VZMjN2anovdkJJR2tmckthZmI2c1M4N0ZqOWN6WGcrbHBXbllJdnRWcS9JUnp4ZWIrTXpGODFETkVFN25QZ0lZc21HRC85bjdQWWI3b3I=";
static constexpr const char *KW_SONG_PATH_V1_URL = "bkNackpLUTZxbEI3VmhXTXR3aVZUVUx1YlcxNkdIVG1oa1JwZmFuSWtiS2xiejIzNUgrWE9aSTM2TGhSdWpHanI1VDZabEVhYkk5MkdPRG54NHhyQUlObEFCL3Q2V3NkUzhvWDNtcE8xZEMrSU5wSVoxWVQ3VWRJL3RVPQ==";
static constexpr const char *KW_SONG_PATH_V2_URL = "VlpjeWIxQkZNbDdkNHA3eUFRMHVnNmVVanpITWJ6MEp1T1ZraElNc2JXaGIvVytMcmFNZVFKNEQ2UXRrcHZyV2dXTjJqQT09";
static constexpr const char *KW_SONG_PATH_V2_DATA_URL = "SEJXeU4yNC9zbGZUalFUd1BnTFczQ1NDSkpyZ2xsUlp4aU14MFgxVEpUNlBIZTc0MVhjOE05SUFmK0tPaXBVZjZEVTVKUXZMbGFtRytIVDRaTU9IYTYrUlFYVFY4ZDJOV3loWG96dk9nbE93cDBCVXpodFgvWUdad2NlWW10bXJxYlk3b3VGRmR5Ukx5c2xQcTFrN3dRPT0=";
static constexpr const char *KW_SONG_INFO_URL = "TjVRY3FwbGZRUkpITGYyczVUSGZ0MG14bEs4VEsyY3VJZE80S0lhSkF6b0p5UWsxSEl5aWpUNzdHY252aEhpSDAwZHhTR3UvM2J0ZlJ2dmc5SDRDa1ZBSGhLZy9SY2UrMllkUjJpUGtmWTA9";
///artist url
static constexpr const char *KW_ARTIST_URL = "YnB5eU5EYWZlV2ROblZIWllJbVJzK0ppMVJqMGxiNVcwM0RaQzVqWE9xdURua0tJWng4OEVaSit1ck5VeGh4SkZ1OGN0SHRaQWNNQS9URXRCM0FVdlZOWDkwcDgzOXZ1Q0JIcGlMMEdyZGlWTUlITE1pZmJOY1YyNmFwS2o5NE1NM3ZDY25RRERlLzEzb1U3NFpKV2lYSGhpOFNINTdDd1ZZWHV4bkUzL0JXY2ZHSkpPYitEazY2QzhiRT0=";
static constexpr const char *KW_ARTIST_INFO_URL = "QW1leS9malJmT2gvWE41OURPbUREVGE5M2Q5ZzlpdUFPWWdreFVaSm0va2p4QlcvblZwdVVJaElwMUZhN1VvbHAvMytVYW44NmNFczl1UjM=";
static constexpr const char *KW_ARTIST_ALBUM_URL = "YXNXaDl6TXNjWHhxeG9UeG9aaWR4aUhQRmpBR25VZ1NFQkZPbGNkdndGT09TbjRYV2ZzTHJJbUtmQjVvRVphQmltaFVUaG50M1dWTXZlWVJUOVNZVkJiVzF0amNVT3FJR09BRUUvVTU2Y1E5QTRiQ1M2SU5ES2MvdWpMeDFiT0dxWDErMkZpVyt5ZnNOeTc1K2dyRG5QVS9NVTRyN0hpNUUyVWVjZz09";
static constexpr const char *KW_ARTIST_MOVIE_URL = "ODRPczVoSnVTb2dObm12Ujd5b3VsMGEvcDR0SDRXbXRLQVpqZW5NT2Voazh2YnRGK0N4eUxYL2xkNnFyWDZJNG9IK0tnbHhWUUdpNGlHbldtMjhYRFRtdGVXeFYxQjlUWHd5V1B2Y1lwbERlTnlmaGZTQzRNQXB3VkdsemhDWWcrbUs2dy9SWUJQb0tRY29rMGhpWk9HalZ6QlUzMjZtdw==";
static constexpr const char *KW_ARTIST_LIST_URL = "NEtJeVhYZFJ4NE4reDlqU2paYVhpUlZqVWtWWXBnbUVnTGVFRVdkbzBTb2dldXhtdnpaNlhVTHZzSk1yaU0wTHowZTR2dWxiYzg4c2xybmtsNU1FU2w1aFZKcTdsWERvMzVKUVdNSWNQemdUQ1FLMVF3V3RUdE1iVTZmQ1FRK3g=";
///album url
static constexpr const char *KW_ALBUM_URL = "Nk5uY2lGelFqNHpteXVuekpQdFhpcWkvUnVRUWdZc1FOdUpXQkdYSWs3VmtDZU1PN29ZcUQ4dTNvczBhSmFMdkRLOGFJNnljSWJNc1QreU1TSU1SRldnN0RnRzlzWFBrUlNIbGlJUzFFelRsNzFoRUJEVkNLbHRzUnAwSzdtdEc2UFRmZEhSV0lvNVVSVXdjamhLT1dvTWxyZEE3aXlvQWFtQTg3Wmtrb05pZ0RIbXU5V3dxeWZLL3hQaFM5WmdKekRaRVhDL3VISVk9";
static constexpr const char *KW_ALBUM_COVER_URL = "NkhRaDluWTFxV2wvTEl6ZkszRmRUa1MvS0JlQUl6MDJVUkpqcjFnYzJ4djZ0b0xmcjhzZHJnV0xscW89";
static constexpr const char *KW_ALBUM_COVER_INFO_URL = "S0Z0Z0dTckV1TysxVnRGem43cU96Q0RRUVhiTS93RVhyNUFTNWNqOEduMDdMalZ1cCtGKzRudHRFcDB0enJ0bXI0VG5QVWMwQU1UKzRRdGc5clZPZ1NwKzRINjZoaDluWVAzVXpMWXlaemQvYnNjd1JwcmM5NGQ5VFhVU3k2SjE=";
///movie url
static constexpr const char *KW_MOVIE_URL = "TjhtdUUydHowZVp1d2RsUVl1UGg0QWkzTjhqZ1NGTWVLNmxqZytTQzV5T291VytwcUpSczNPMGdQV1U9";
static constexpr const char *KW_MOVIE_COVER_URL = "c0IyUURSczNib2MzV0w5b3FEZ1dQTEZOeHliNWtWMVh0RWo1MFhoeHF3MjdUbThqb0Rta0JBPT0=";
static constexpr const char *KW_MOVIE_ATTR_URL = "cnkyNURUa0E0RkpHdVZWamlOZDRVaGVNcVViSHUrSnNZZjBLZWIveGF5bkVEMldMeXFITk1NaVYvNGNkZFYwaURlelNUQzRvQTdRaklLeUNTLzZJMngxYWJha1ZxY1hLQmFtb2ZRenBsSlViZ05DZmpza0YwZ0pCRjMvSVh1RkNFN094NkZaNWY5TjRHK2NNU1pNQ1E3MGxENWpQRXVYb1pKNjRkRW9CbzlxRTBYYWZXRlFzMW52Um11U2J0M3EvdHRFR0pONGtMeDY2Zm16ME92akNZL2QxL2RZY2lXZGFuRk4raTBjSWx0emc0b2R2bG9LTFVMMCtKQThsTzZzS0NSdzArOWQxanNnY2pkTERxYkd0YXc9PQ==";
///toplist url
static constexpr const char *KW_TOPLIST_URL = "T2QyOWl3WmttZlk1SnM4NFZ6S1BoZjhGQnV5Y0lHbm9nUDlEZUR1bDJRcmNITVZIVWxZT2YvYWtSQWxXQ1JjWE9DNjQydDdQRTQzZkZvclFOL25vd2JIUjl1T1RvRzFFOEFPSVFPVWQ0ZUR4dUVBbFYzbjhwUXdSQ09teFBucjA=";
static constexpr const char *KW_TOPLIST_INFO_URL = "Wkl6YXI0QWdzcXRyaUVCbzdLM2lhR3FZUDFZUVNLUDdvLzdpL1AwbjBlWGxrNWcxdGxBWVBiWjNlMldnNFB6cnBWT2ozOWJwZEQ4Q3dHM0RnVy9DSTVHL202RT0=";
///comment url
static constexpr const char *KW_COMMENT_SONG_URL = "eHgyZzQyMWprWUpacGhVdmZjT0tUN2Y3bVF1NFRlR3BZQmpFWU1sRFo1MEd6Ky8rNlBjMUtQbkk2Vm5kd2JsM2t4ZUY3ZDNrY2xnV2IzdlMvUEdLU2RYL1ZFNHA4a2tDMDhmYU5KZ3lMSEdEQWo5bFJYdjBnSTdUVlRqdGFTazk=";
static constexpr const char *KW_COMMENT_PLAYLIST_URL = "S2ptTE9jUTZCK1B5cEVwM0RwT0NwTlQ1bXpWR3JQeXhZbTkrdjlGR3Q0OWZGa2lpeGRYMkowZk82Ly9OS0tka2dPd1YreFkyV0Y3b3pYcHNiNVM0SU1uWEFmSVlYQytCR0d2NU52Q0NINENXT3pwVEV1WjlVTE9pYUxKT0NINE4=";
///playlist url
static constexpr const char *KW_PLAYLIST_URL = "QzgyT0gvMmt3cTFrTDh0LzZ2TFRoNWdIUktZNUJVbjRiVHRnQWxIQkJUaWtCWEw4dUltWVhSSUE4b1RUekkvWnFrS3RRK1NwT1dVNXFJamMreU1STG1KcmhhbE5oL1dUTTZ0a3VLOXQwZm96cm1EeQ==";
static constexpr const char *KW_PLAYLIST_INFO_URL = "a1RvK2Rxb3oza1FHZ3FNeU1ZQ1lSYTdGN0xUanZXZ09rU2RTQlF3UGxsT3dFcTBqc3FXdmdUVW1wL2RQVUJKdGEzM3Z2RUI1cUIrYmVnNmNtdEZla3VlN01ySXdNb3dVa21lSDR1b2VwZlJFWXVVbnY4bDFLSHplSHhicDY5WEFBaFZxdlpZUmxVTkhxL0hNcDI1ZW9oamtXWkZadUhNdWFOZG1iVHM0K2FmY0pLamlXZ2tGMFE9PQ==";


class QNetworkRequest;

/*! @brief The namespace of the kuwo request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace ReqKWInterface
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
