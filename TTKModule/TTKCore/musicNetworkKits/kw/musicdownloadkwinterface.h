#ifndef MUSICDOWNLOADKWINTERFACE_H
#define MUSICDOWNLOADKWINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicobject.h"
#include "musicglobaldefine.h"

#define _SIGN                        "OGlVTjJWOEdlMkkzSkZIeg=="
//
const QString KW_UA_URL_1          = "QTllV1FYb3p4ZmZVNDArOW1uSGQrUGp2clowek55RmxrV2kzK3djN2dpb3J4YUlCbHpHSU9FNkJMREtSWDdVdWlVeVRHTnpTQUZ5TjE2RDlLb3lIeFNsMWVHMXBkWEsxSVpIRU85Ui8wVzhEbm80dUdQdHUxc0EzL1VhRHh5WmtxWlpJR1BqR3kzYkMycXVhQWljSmE3K1B3ZFd5dEdWMlE1UGhtWjBzOXJDcVRsN0lrL0d4V2ExdFJjQWtxMy9IbUN4ZkZ1NWhsY0ZqY3ZGcg==";
//
const QString KW_ALBUM_URL         = "T0RHWW1RYlRFWi9MdDNRSjh6UXhIUHFqYjR3ZDdHK3VLbTRxUnRXV1RpaG1VWWpYb0hmY2dxaWJ6aWlETThmN2ltYVNDSVRkOXNHZVRmNXJYcDRXM3UvckFEVXJLR3JQbmZmMjNDR1ZxT1ZPbjgzNXV4bWdyM0J2ZTNvNFViVGFTcDdOWjg1NytON3AzcU9Kbjc4YWJNRTFYaEdsMDJJa001eWFjS1ZhcC9oeVNEKzhRYnNkTmFiMml6VDc1ZXJ4bzNScVh5Z0dWR1E9";
const QString KW_ALBUM_COVER_URL   = "bnkrUTZjQmxjcFVtVXZzR1M5c3hBN25MbWx3aHZpbE50aHNQVkRZQ0dKdXdLZUh1TTA5aWJnPT0=";
//
const QString KW_SONG_SEARCH_URL   = "ZENVRUl0SzZ2Z0lBZjZpNmtzdmE3QlNZc01ZbUtJM3hiM3J6RUJ6Y3lTQmNnMWpwMkJlU2R6RFFaSWhVRm5TZmx5WUhzMWcrK3Q3Umd6OWhWK3J2bWNPdHNxYzk4MEtZeTE4bDA3TitEcCtUejVUYU5Mc1lrMkFYNVlrdlpCQkowWU9YRjUxeTM5UkxvNjA1M2J5T2pqbzBZYm1rVElmeUpnblk5WloxcGFnTkhneUdwUDFXVlYrVzFZY2lxWDI2aEx4TGduTWR5eUNHNjQyT0Y0UnRqTjdDbjIzd2Z3N3EyOVA2OVBabVJBS042MnBJ";
const QString KW_SONG_LRC_URL      = "dWdMdkJSZDVlTkFmZmVCQ3FJL1MrV2l0TzNmK2Y4cUVyNXVlUTVHbkdtLzdyNW8xVXhqeklkN0E5N1liVDZZMXZ1ZWhkV2pYeFRETHdPZVBRNVQxbnp6MWIyTT0=";
const QString KW_SONG_ATTR_URL     = "TzlINnJWWWVrVHowRkZhV1NrV2R6ZGh5b2YraFJmNkV6VUhxSlBPMExXVWkxSWt4aVdZNFIwaG9EK200cFdpSzZSYUhLQlA5T0RTQ2l1M1hxNmxjQlU3YkJoZ0JtbkFXbFFlL2JhTVp0WGgxTHdiT3F5MVBDTEowUi9HMXU2R2pqVXVidVE9PQ==";
const QString KW_SONG_ATTR_LL_URL  = "UTgrZGI3eFBTNkdobUIxbHBNMEh2dXpJMnlmakNuR2FyNmN3bWlDNjhTOHovV2s1V1I0VzhHVkJyZTNXTTBSbUNONXlkKzk4N2szQVBGQmplcXdoYzNSK3R6Vy9LNWdNQksxZUtWQ1p6c0NqZDk4dUtET1pJMVB2NkorV0RMb2J3a1REYjBhTTUzUTVxa1YwejRYc01GVlRTZTQ1V2JRSTFTT1ppY09zcVlxWUNYRm9mOUg3dEJwZG5FRkZuNS9tRzVPeWpMM0JaRE8vNEhISDNFNzRvb2I0UFlOTm1NVEVVc3o2RndiVldEVXNLcU1T";
const QString KW_SONG_INFO_URL     = "SW50ZXhadlpVQ1RHMjlkZ0ZLbjlYdnR5TEZmVENaM3VxYzV1emNVUmpXdlJjbm5JTGJ6KzJzbGl1VXFqR3RUZk95ZWFQeCtXNEs5akVvNDkrM29SWEp6akNsOD0=";
//
const QString KW_ARTIST_URL        = "YnB5eU5EYWZlV2ROblZIWllJbVJzK0ppMVJqMGxiNVcwM0RaQzVqWE9xdURua0tJWng4OEVaSit1ck5VeGh4SkZ1OGN0SHRaQWNNQS9URXRCM0FVdlZOWDkwcDgzOXZ1Q0JIcGlMMEdyZGlWTUlITE1pZmJOY1YyNmFwS2o5NE1NM3ZDY25RRERlLzEzb1U3NFpKV2lYSGhpOFNINTdDd1ZZWHV4bkUzL0JXY2ZHSkpPYitEazY2QzhiRT0=";
const QString KW_ARTIST_INFO_URL   = "QW1leS9malJmT2gvWE41OURPbUREVGE5M2Q5ZzlpdUFPWWdreFVaSm0va2p4QlcvblZwdVVJaElwMUZhN1VvbHAvMytVYW44NmNFczl1UjM=";
const QString KW_ARTIST_NINFO_URL  = "eWJjZXJ2OTFjOGZNcHdneEF2aVFtaDhOcytwQWNNbmZXa25UVXQ4NjFYMTdlV3ZKYzVidDR5WGZnK2xqbHBYNXlaaXBRMGVEZjhvPQ==";
const QString KW_AR_ALBUM_URL      = "bTVGSVc1djhoNFFna24vUzNQb0FCQjdsbVIvMHgxNUpsbHJwZndGbmFmR1dENUdTRmw1bVFLTjdJUzJwV2FJYVAxajhOREpZekdGR1IrSW13ZWd6S0Z4YUJsYVFId054Y3hhaWl0dmFucUlwYXIwSkk1K3daeElITVNKQ0JDM0Q5bW5TWFdlVU5nMUNqQ29wcEdoWUd0MlJxN2VIb2F3dW5Mb1NiZz09";
const QString KW_AR_MV_URL         = "b0FaUGp1UFFwbngwdWFSQXU1L2hEWEFyUWJLa1JLTjY5OFlHSHZWZ1dCa1N4SDQxcGdJWXQ5NmkxaWo0NEhidWhWSWNFMzFLS1FlNWZYTWhHelUvWTdOMUlDWGV0N1liMDBKNjlCQVFpclJJS0NpRDZ6YmkvTVpSS3VRNVNlMHk3eThoUG04R1cxTmpOT2RRdHZXM0YxbzBTVTh1WVNGNA==";
const QString KW_AR_SIM_URL        = "c25YQzhiU3dwRXc1djFtY2k4UCtNRDBmMVJmTFVGc3hvTE5pcmpVdkZxWGc0QzNLREcrNXhkZUtUZ0d5c0RoLw==";
const QString KW_AR_LIST_URL       = "NEtJeVhYZFJ4NE4reDlqU2paYVhpUlZqVWtWWXBnbUVnTGVFRVdkbzBTb2dldXhtdnpaNlhVTHZzSk1yaU0wTHowZTR2dWxiYzg4c2xybmtsNU1FU2w1aFZKcTdsWERvMzVKUVdNSWNQemdUQ1FLMVF3V3RUdE1iVTZmQ1FRK3g=";
//
const QString KW_MV_URL            = "TjhtdUUydHowZVp1d2RsUVl1UGg0QWkzTjhqZ1NGTWVLNmxqZytTQzV5T291VytwcUpSczNPMGdQV1U9";
const QString KW_MV_HOME_URL       = "a3gvOXZGUzF6Sko4TDlWcVRBMFMrdy8yMURLS0JFR0xFVEdJcVE9PQ==";
const QString KW_MV_COVER_URL      = "c0IyUURSczNib2MzV0w5b3FEZ1dQTEZOeHliNWtWMVh0RWo1MFhoeHF3MjdUbThqb0Rta0JBPT0=";
const QString KW_MV_ATTR_URL       = "cnkyNURUa0E0RkpHdVZWamlOZDRVaGVNcVViSHUrSnNZZjBLZWIveGF5bkVEMldMeXFITk1NaVYvNGNkZFYwaURlelNUQzRvQTdRaklLeUNTLzZJMngxYWJha1ZxY1hLQmFtb2ZRenBsSlViZ05DZmpza0YwZ0pCRjMvSVh1RkNFN094NkZaNWY5Tj"
                                     "RHK2NNU1pNQ1E3MGxENWpQRXVYb1pKNjRkRW9CbzlxRTBYYWZXRlFzMW52Um11U2J0M3EvdHRFR0pONGtMeDY2Zm16ME92akNZL2QxL2RZY2lXZGFuRk4raTBjSWx0emc0b2R2bG9LTFVMMCtKQThsTzZzS0NSdzArOWQxanNnY2pkTERxYkd0YXc9PQ==";
//
const QString KW_SG_COMMIT_URL     = "R29Ob2xrOUcxbXhpMlF6YmRzZ2kxZVFrRmJPcEdzbk0weUNQazNGUEFXSjl4QXdhWU1kUXdNQ3ExOWxGRE5uOERTblpCNk9pZWdnVmRLTHJkK25GckxxbzhpYmh3UHVTbi9DK1RoYm5qaVdZV2o4MXo2cXBwL1ZLTFprY0dyZENDUDhiT0E9PQ==";
//
const QString KW_SONG_TOPLIST_URL  = "VE56cUdwVjgxMkFMMC80VThzN25sQmZFTzFPNWdaOHZPZEM2YlNyeUxwTndnd0ZTVGtqTUM4cTlQaG5nbWNtRDIzc3lPdVJ6eXFXQkZTRjkvWktnMDZocEdXRStIcHpRMW9CZTJscDZYVnRoUkN2RE12SkRDWVNSMHVkYXVVVGs=";
//
const QString KW_PL_COMMIT_URL     = "b09RdlBRdzh4bjB4QXZCbXlXcE5ENEpUMzhEUGY5cVNCUVAwVUJucjg4YS9tWXhsZ2dINmZDOXFhOUlJVmVOaWVBNWlsaE9JRy9UalllVit2dk1IZG5ST1dhUGFNSXg0ZmVxdlpzUEZINUhXdzJML1hDOVhqYjQ2Y2Raek5oRGNDaGx3Wmc9PQ==";
const QString KW_PLAYLIST_URL      = "OTc4ZUwvTFdHbW0rekNzWGx6aHR1QXg5TnhKRUdmQ2drYlBpdWFRVjhQUkJVN0NaMDl6NmJFZTY5Yy91eHpWWVh3VlFXYzA4clZDRjZrQWdWSExtRnhqNFRZY0xzNnc1R0hhRStpeFQ4QzlKNUk1a1JrdlJtcFU0YkpvUlpSNGUzZFNiR253azVxUlArMEVpS081RlZPMlZLZEE9";
const QString KW_PLAYLIST_ATTR_URL = "UzZ0NDRTM2s1Mm5OUTZvZDdTRUlCZ0hsVjhZbGh3aWZYNWpLcnQ5WC95VWVTSmFZZXNkU242N3Fsc0haT1l4cUxXRUdjOWwrd2c4M2FDUk9hWXJGVldWVThzcG16UC9DT1lvTnlYT2VqZVlyaFhUY1FsTlJFVE9OQ2NOdTQ5RCt4cFY0SS8rV2NRUTdMUlE5SU85NElXNVlhamF4ajdPeGNteWZPQ1loaXVVPQ==";
//
const QString KW_RCM_URL           = "SUVwN29KVHFLbVRzTC9PSW5QelFZTkJlMC9vVFRNUUM0aklEZkJwa2t0MG9FWnZEeVQwU0xNR0VENDdWUUxzejNFVzFYcFhrOFVuUzh6Tm5ZaDhXRzRnckVhSkUrYm90eHFxS2hVNWp2U05jc3dTandGV21VOGpLY2JJS2c4ZFA0d1NycW93Mldyam15Zm9vTXVBanpScmJBSkkybWxRNGJtQkdZaTQvcGQ0PQ==";
//
const QString KW_SUGGEST_URL       = "UXc5cFVXZG92VUxjeFpmR0w4OS9rUG12ZjNtdGZ6d2owb09SaXZrZmIxMEwxbEZYWVY4a0hWMVVQL2d0Y3dNbURPUE9PYWVlcjBXdW5aNjAxbnNLajl3N0ZoMG5UemFGTWFhYjFLeXhzcWJhVU04VA==";
//


/*! @brief The class to kuwo query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadKWInterface
{
public:
    /*!
     * Read mv info attribute from query results.
     */
    void readFromMusicLLAttribute(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &format, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantList &format, const QString &quality, bool all);
    /*!
     * Read tags(lrc\smallpic) from query results.
     */
    void readFromMusicSongPic(MusicObject::MusicSongInformation *info);

};

#endif // MUSICDOWNLOADKWINTERFACE_H
