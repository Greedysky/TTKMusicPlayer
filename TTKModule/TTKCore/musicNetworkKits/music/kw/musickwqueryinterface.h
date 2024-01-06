#ifndef MUSICKWQUERYINTERFACE_H
#define MUSICKWQUERYINTERFACE_H

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
//TG5KazFERHlYc3hzS1JQd29ldXVEOWVRdGh2dmx0RWxma3I5cnRxekptRjdKM0NvQXo1bzV5c1ZpRDB2WGZ1UQ==
//MnBiMVlsRXJML2c5V2FFaHNOM05hVWtVYUpBc0VrbjdNUDZhOGx5OTR2cz0=
static const QString KW_SONG_SEARCH_URL      = "azRpTk56R3ZhTXY5dk13YzFkaHJkMzRFY3NpRkRmUGdyUnQyWVY5NXNvOE82cUE4Mmg0VklKTTQrRFV0dEZHTExUSkJvTkRXMGdQVEpaK0Z4Rkp6ZUkrUnh1T29iSG1mTUxCNkRmMlVFT25zVzhjQUs1K0VCZk9UMWpTWGovMWR0czNlN0Z0Y1kyUCtxYlZSK3R3YnU3VTZ6dlkwanJIdGN6RWdjWEhxSi96ekRNWjBMMEMrL3JCUGxRQWJ6YkltQkRGeGljR3c4TmgxaXBXcGhzcnZlZ1p5cldlNzZTc1ZlZDFvUDcxTmtDdlJqY3JU";
static const QString KW_SONG_LRC_URL         = "a2VZMjN2anovdkJJR2tmckthZmI2c1M4N0ZqOWN6WGcrbHBXbllJdnRWcS9JUnp4ZWIrTXpGODFETkVFN25QZ0lZc21HRC85bjdQWWI3b3I=";
static const QString KW_SONG_DETAIL_URL      = "akMvL0pwQk8ybDBRczJqSnVtbG16MG15b2NpR280SGNOQW9nLzV3aGlqWWl6L1liU2NhRUNYZURhYWVCT0s4Qzgyd3V3R3FJZElqZ3ZZT2d4OElYLzZKeGk1RlBjSEQ3R2toaCtKSTI1czdxWS9ZeXVHR3ByT0ZKSzdnQlRQUGVadk1iNDRteFhodkZTaFFxUldiUXRycmNSSTQ9";
static const QString KW_SONG_DETAIL_CGG_URL  = "Sy9PQjM5WTkvUlkwTnR0WkVlWWIwMnB6SW5aeEJua1h1UTdvQWlzd2daT25iVUlIdzFuaDQzb1FTV1VrODAxYg==";
static const QString KW_SONG_INFO_URL        = "TjVRY3FwbGZRUkpITGYyczVUSGZ0MG14bEs4VEsyY3VJZE80S0lhSkF6b0p5UWsxSEl5aWpUNzdHY252aEhpSDAwZHhTR3UvM2J0ZlJ2dmc5SDRDa1ZBSGhLZy9SY2UrMllkUjJpUGtmWTA9";
///artist url
static const QString KW_ARTIST_URL           = "YnB5eU5EYWZlV2ROblZIWllJbVJzK0ppMVJqMGxiNVcwM0RaQzVqWE9xdURua0tJWng4OEVaSit1ck5VeGh4SkZ1OGN0SHRaQWNNQS9URXRCM0FVdlZOWDkwcDgzOXZ1Q0JIcGlMMEdyZGlWTUlITE1pZmJOY1YyNmFwS2o5NE1NM3ZDY25RRERlLzEzb1U3NFpKV2lYSGhpOFNINTdDd1ZZWHV4bkUzL0JXY2ZHSkpPYitEazY2QzhiRT0=";
static const QString KW_ARTIST_INFO_URL      = "QW1leS9malJmT2gvWE41OURPbUREVGE5M2Q5ZzlpdUFPWWdreFVaSm0va2p4QlcvblZwdVVJaElwMUZhN1VvbHAvMytVYW44NmNFczl1UjM=";
static const QString KW_ARTIST_ALBUM_URL     = "YXNXaDl6TXNjWHhxeG9UeG9aaWR4aUhQRmpBR25VZ1NFQkZPbGNkdndGT09TbjRYV2ZzTHJJbUtmQjVvRVphQmltaFVUaG50M1dWTXZlWVJUOVNZVkJiVzF0amNVT3FJR09BRUUvVTU2Y1E5QTRiQ1M2SU5ES2MvdWpMeDFiT0dxWDErMkZpVyt5ZnNOeTc1K2dyRG5QVS9NVTRyN0hpNUUyVWVjZz09";
static const QString KW_ARTIST_MOVIE_URL     = "ODRPczVoSnVTb2dObm12Ujd5b3VsMGEvcDR0SDRXbXRLQVpqZW5NT2Voazh2YnRGK0N4eUxYL2xkNnFyWDZJNG9IK0tnbHhWUUdpNGlHbldtMjhYRFRtdGVXeFYxQjlUWHd5V1B2Y1lwbERlTnlmaGZTQzRNQXB3VkdsemhDWWcrbUs2dy9SWUJQb0tRY29rMGhpWk9HalZ6QlUzMjZtdw==";
static const QString KW_ARTIST_LIST_URL      = "NEtJeVhYZFJ4NE4reDlqU2paYVhpUlZqVWtWWXBnbUVnTGVFRVdkbzBTb2dldXhtdnpaNlhVTHZzSk1yaU0wTHowZTR2dWxiYzg4c2xybmtsNU1FU2w1aFZKcTdsWERvMzVKUVdNSWNQemdUQ1FLMVF3V3RUdE1iVTZmQ1FRK3g=";
///album url
static const QString KW_ALBUM_URL            = "Nk5uY2lGelFqNHpteXVuekpQdFhpcWkvUnVRUWdZc1FOdUpXQkdYSWs3VmtDZU1PN29ZcUQ4dTNvczBhSmFMdkRLOGFJNnljSWJNc1QreU1TSU1SRldnN0RnRzlzWFBrUlNIbGlJUzFFelRsNzFoRUJEVkNLbHRzUnAwSzdtdEc2UFRmZEhSV0lvNVVSVXdjamhLT1dvTWxyZEE3aXlvQWFtQTg3Wmtrb05pZ0RIbXU5V3dxeWZLL3hQaFM5WmdKekRaRVhDL3VISVk9";
static const QString KW_ALBUM_COVER_URL      = "NkhRaDluWTFxV2wvTEl6ZkszRmRUa1MvS0JlQUl6MDJVUkpqcjFnYzJ4djZ0b0xmcjhzZHJnV0xscW89";
static const QString KW_ALBUM_COVER_INFO_URL = "S0Z0Z0dTckV1TysxVnRGem43cU96Q0RRUVhiTS93RVhyNUFTNWNqOEduMDdMalZ1cCtGKzRudHRFcDB0enJ0bXI0VG5QVWMwQU1UKzRRdGc5clZPZ1NwKzRINjZoaDluWVAzVXpMWXlaemQvYnNjd1JwcmM5NGQ5VFhVU3k2SjE=";
///movie url
static const QString KW_MOVIE_URL            = "TjhtdUUydHowZVp1d2RsUVl1UGg0QWkzTjhqZ1NGTWVLNmxqZytTQzV5T291VytwcUpSczNPMGdQV1U9";
static const QString KW_MOVIE_COVER_URL      = "c0IyUURSczNib2MzV0w5b3FEZ1dQTEZOeHliNWtWMVh0RWo1MFhoeHF3MjdUbThqb0Rta0JBPT0=";
static const QString KW_MOVIE_ATTR_URL       = "cnkyNURUa0E0RkpHdVZWamlOZDRVaGVNcVViSHUrSnNZZjBLZWIveGF5bkVEMldMeXFITk1NaVYvNGNkZFYwaURlelNUQzRvQTdRaklLeUNTLzZJMngxYWJha1ZxY1hLQmFtb2ZRenBsSlViZ05DZmpza0YwZ0pCRjMvSVh1RkNFN094NkZaNWY5TjRHK2NNU1pNQ1E3MGxENWpQRXVYb1pKNjRkRW9CbzlxRTBYYWZXRlFzMW52Um11U2J0M3EvdHRFR0pONGtMeDY2Zm16ME92akNZL2QxL2RZY2lXZGFuRk4raTBjSWx0emc0b2R2bG9LTFVMMCtKQThsTzZzS0NSdzArOWQxanNnY2pkTERxYkd0YXc9PQ==";
///toplist url
static const QString KW_TOPLIST_URL          = "T2QyOWl3WmttZlk1SnM4NFZ6S1BoZjhGQnV5Y0lHbm9nUDlEZUR1bDJRcmNITVZIVWxZT2YvYWtSQWxXQ1JjWE9DNjQydDdQRTQzZkZvclFOL25vd2JIUjl1T1RvRzFFOEFPSVFPVWQ0ZUR4dUVBbFYzbjhwUXdSQ09teFBucjA=";
static const QString KW_TOPLIST_INFO_URL     = "Wkl6YXI0QWdzcXRyaUVCbzdLM2lhR3FZUDFZUVNLUDdvLzdpL1AwbjBlWGxrNWcxdGxBWVBiWjNlMldnNFB6cnBWT2ozOWJwZEQ4Q3dHM0RnVy9DSTVHL202RT0=";
///comment url
static const QString KW_COMMENT_SONG_URL     = "R29Ob2xrOUcxbXhpMlF6YmRzZ2kxZVFrRmJPcEdzbk0weUNQazNGUEFXSjl4QXdhWU1kUXdNQ3ExOWxGRE5uOERTblpCNk9pZWdnVmRLTHJkK25GckxxbzhpYmh3UHVTbi9DK1RoYm5qaVdZV2o4MXo2cXBwL1ZLTFprY0dyZENDUDhiT0E9PQ==";
static const QString KW_COMMENT_PLAYLIST_URL = "b09RdlBRdzh4bjB4QXZCbXlXcE5ENEpUMzhEUGY5cVNCUVAwVUJucjg4YS9tWXhsZ2dINmZDOXFhOUlJVmVOaWVBNWlsaE9JRy9UalllVit2dk1IZG5ST1dhUGFNSXg0ZmVxdlpzUEZINUhXdzJML1hDOVhqYjQ2Y2Raek5oRGNDaGx3Wmc9PQ==";
///playlist url
static const QString KW_PLAYLIST_URL         = "cjgyRkJaRnB2bElLbG1MS2M4TjcrWjRabU4xZkVpY1p6OTZBSTlrM3IxWWNjUTBPS3d0OFo4d0s4M29pd0h4WFZtKzhxTjVoNXcwUjRiQlE2aFZ3VXhGMlhqYURxdlZSSGR5cHJidUdndVV3Y3hEQlRBbm4yQytKMExoNTAvUUs2TE92SFQzSUVLMVJ3VFlNUUZXQkZXa2t0UmdQWTl1VTF3bGhCUT09";
static const QString KW_PLAYLIST_INFO_URL    = "a1RvK2Rxb3oza1FHZ3FNeU1ZQ1lSYTdGN0xUanZXZ09rU2RTQlF3UGxsT3dFcTBqc3FXdmdUVW1wL2RQVUJKdGEzM3Z2RUI1cUIrYmVnNmNtdEZla3VlN01ySXdNb3dVa21lSDR1b2VwZlJFWXVVbnY4bDFLSHplSHhicDY5WEFBaFZxdlpZUmxVTkhxL0hNcDI1ZW9oamtXWkZadUhNdWFOZG1iVHM0K2FmY0pLamlXZ2tGMFE9PQ==";


class QNetworkRequest;

/*! @brief The namespace of the kuwo request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicKWInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request);

    /*!
     * Make cover pixmap url.
     */
    void makeCoverPixmapUrl(QString &url, const QString &id);

    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format);

}

#endif // MUSICKWQUERYINTERFACE_H
