#ifndef MUSICKWQUERYINTERFACE_H
#define MUSICKWQUERYINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

static constexpr const char *_SIGN           = "OGlVTjJWOEdlMkkzSkZIeg==";

///song url
static const QString KW_SONG_SEARCH_URL      = "azRpTk56R3ZhTXY5dk13YzFkaHJkMzRFY3NpRkRmUGdyUnQyWVY5NXNvOE82cUE4Mmg0VklKTTQrRFV0dEZHTExUSkJvTkRXMGdQVEpaK0Z4Rkp6ZUkrUnh1T29iSG1mTUxCNkRmMlVFT25zVzhjQUs1K0VCZk9UMWpTWGovMWR0czNlN0Z0Y1kyUCtxYlZSK3R3YnU3VTZ6dlkwanJIdGN6RWdjWEhxSi96ekRNWjBMMEMrL3JCUGxRQWJ6YkltQkRGeGljR3c4TmgxaXBXcGhzcnZlZ1p5cldlNzZTc1ZlZDFvUDcxTmtDdlJqY3JU";
static const QString KW_SONG_LRC_URL         = "a2VZMjN2anovdkJJR2tmckthZmI2c1M4N0ZqOWN6WGcrbHBXbllJdnRWcS9JUnp4ZWIrTXpGODFETkVFN25QZ0lZc21HRC85bjdQWWI3b3I=";
static const QString KW_SONG_DETAIL_URL      = "cG9Mbk9QVklpdGJ1U080TkRKM0JnMWkwbnhrN2UzaWIyNzU4VTM5Y0wzbFRhT1UyczVqVFM1VkdUUDEya0NJaCtpS1R0eUphb1JSeURjZHBoQ3hXS1BGN2NMY2dna0VGVGg3ZkhwY3JYOW89";
static const QString KW_SONG_INFO_URL        = "TjVRY3FwbGZRUkpITGYyczVUSGZ0MG14bEs4VEsyY3VJZE80S0lhSkF6b0p5UWsxSEl5aWpUNzdHY252aEhpSDAwZHhTR3UvM2J0ZlJ2dmc5SDRDa1ZBSGhLZy9SY2UrMllkUjJpUGtmWTA9";
///artist url
static const QString KW_ARTIST_URL           = "YnB5eU5EYWZlV2ROblZIWllJbVJzK0ppMVJqMGxiNVcwM0RaQzVqWE9xdURua0tJWng4OEVaSit1ck5VeGh4SkZ1OGN0SHRaQWNNQS9URXRCM0FVdlZOWDkwcDgzOXZ1Q0JIcGlMMEdyZGlWTUlITE1pZmJOY1YyNmFwS2o5NE1NM3ZDY25RRERlLzEzb1U3NFpKV2lYSGhpOFNINTdDd1ZZWHV4bkUzL0JXY2ZHSkpPYitEazY2QzhiRT0=";
static const QString KW_ARTIST_INFO_URL      = "QW1leS9malJmT2gvWE41OURPbUREVGE5M2Q5ZzlpdUFPWWdreFVaSm0va2p4QlcvblZwdVVJaElwMUZhN1VvbHAvMytVYW44NmNFczl1UjM=";
static const QString KW_ARTIST_ALBUM_URL     = "bTVGSVc1djhoNFFna24vUzNQb0FCQjdsbVIvMHgxNUpsbHJwZndGbmFmR1dENUdTRmw1bVFLTjdJUzJwV2FJYVAxajhOREpZekdGR1IrSW13ZWd6S0Z4YUJsYVFId054Y3hhaWl0dmFucUlwYXIwSkk1K3daeElITVNKQ0JDM0Q5bW5TWFdlVU5nMUNqQ29wcEdoWUd0MlJxN2VIb2F3dW5Mb1NiZz09";
static const QString KW_ARTIST_MOVIE_URL     = "b0FaUGp1UFFwbngwdWFSQXU1L2hEWEFyUWJLa1JLTjY5OFlHSHZWZ1dCa1N4SDQxcGdJWXQ5NmkxaWo0NEhidWhWSWNFMzFLS1FlNWZYTWhHelUvWTdOMUlDWGV0N1liMDBKNjlCQVFpclJJS0NpRDZ6YmkvTVpSS3VRNVNlMHk3eThoUG04R1cxTmpOT2RRdHZXM0YxbzBTVTh1WVNGNA==";
static const QString KW_ARTIST_LIST_URL      = "NEtJeVhYZFJ4NE4reDlqU2paYVhpUlZqVWtWWXBnbUVnTGVFRVdkbzBTb2dldXhtdnpaNlhVTHZzSk1yaU0wTHowZTR2dWxiYzg4c2xybmtsNU1FU2w1aFZKcTdsWERvMzVKUVdNSWNQemdUQ1FLMVF3V3RUdE1iVTZmQ1FRK3g=";
///album url
static const QString KW_ALBUM_URL            = "Nk5uY2lGelFqNHpteXVuekpQdFhpcWkvUnVRUWdZc1FOdUpXQkdYSWs3VmtDZU1PN29ZcUQ4dTNvczBhSmFMdkRLOGFJNnljSWJNc1QreU1TSU1SRldnN0RnRzlzWFBrUlNIbGlJUzFFelRsNzFoRUJEVkNLbHRzUnAwSzdtdEc2UFRmZEhSV0lvNVVSVXdjamhLT1dvTWxyZEE3aXlvQWFtQTg3Wmtrb05pZ0RIbXU5V3dxeWZLL3hQaFM5WmdKekRaRVhDL3VISVk9";
static const QString KW_ALBUM_COVER_URL      = "NkhRaDluWTFxV2wvTEl6ZkszRmRUa1MvS0JlQUl6MDJVUkpqcjFnYzJ4djZ0b0xmcjhzZHJnV0xscW89";
///movie url
static const QString KW_MOVIE_URL            = "TjhtdUUydHowZVp1d2RsUVl1UGg0QWkzTjhqZ1NGTWVLNmxqZytTQzV5T291VytwcUpSczNPMGdQV1U9";
static const QString KW_MOVIE_COVER_URL      = "c0IyUURSczNib2MzV0w5b3FEZ1dQTEZOeHliNWtWMVh0RWo1MFhoeHF3MjdUbThqb0Rta0JBPT0=";
static const QString KW_MOVIE_ATTR_URL       = "cnkyNURUa0E0RkpHdVZWamlOZDRVaGVNcVViSHUrSnNZZjBLZWIveGF5bkVEMldMeXFITk1NaVYvNGNkZFYwaURlelNUQzRvQTdRaklLeUNTLzZJMngxYWJha1ZxY1hLQmFtb2ZRenBsSlViZ05DZmpza0YwZ0pCRjMvSVh1RkNFN094NkZaNWY5TjRHK2NNU1pNQ1E3MGxENWpQRXVYb1pKNjRkRW9CbzlxRTBYYWZXRlFzMW52Um11U2J0M3EvdHRFR0pONGtMeDY2Zm16ME92akNZL2QxL2RZY2lXZGFuRk4raTBjSWx0emc0b2R2bG9LTFVMMCtKQThsTzZzS0NSdzArOWQxanNnY2pkTERxYkd0YXc9PQ==";
///toplist url
static const QString KW_TOPLIST_URL          = "VE56cUdwVjgxMkFMMC80VThzN25sQmZFTzFPNWdaOHZPZEM2YlNyeUxwTndnd0ZTVGtqTUM4cTlQaG5nbWNtRDIzc3lPdVJ6eXFXQkZTRjkvWktnMDZocEdXRStIcHpRMW9CZTJscDZYVnRoUkN2RE12SkRDWVNSMHVkYXVVVGs=";
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
    void makeCoverPixmapUrl(QString &url);

    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QString &bitrate, QString &url);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format, TTK::QueryQuality quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantList &format, TTK::QueryQuality quality, bool all);

}

#endif // MUSICKWQUERYINTERFACE_H
