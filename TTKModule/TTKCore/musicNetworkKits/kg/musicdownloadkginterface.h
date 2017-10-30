#ifndef MUSICDOWNLOADKGINTERFACE_H
#define MUSICDOWNLOADKGINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

///////////////////////////////////////////////////////////////////////////
const QString KG_ALBUM_URL         = "T1I2UzExcHpQU0lzNVJTVkR4UkVabkxndmF6eGp3ajVvSHJaZWFQdFlybENrVDJiNWZ0OUw0VjN2TjArWFg4MS9LT0pRMmRrYzlIeWYvZmpEVkR6aFdwSUZhN2pLNGx5NHdaWFFGSlRUWDZ2aWFWazBhWmZWSXBEQXBHcjhIb1c=";
const QString KG_ALBUM_INFO_URL    = "ajBGaXREQ3Y1S3pwZGkxd1pJUEREbUNxL3d0SWpPZzFNQjhZQjdyZk0rQmRzQVZXUFBrcVp0ekh6NEtLbjJWTklDNWIxbGpycFFqbHVVRE95aDRWMUJ4NUNVRTUySWdU";
///////////////////////////////////////////////////////////////////////////
const QString KG_SONG_SEARCH_URL   = "K3l5ZjdYOTcySmR6cFdhbHhNTU1KSWswY0JKa1JFWmJoZjJHTExaZFdQejltM3JxbFRWNWRHVmlzL01ORzIwSTVGTHd3dzFaUWdWTG1QRk9ockZJNkczb3poTjBKYVFIdHBNeUpmb013bmFXWXpIZWErZXhJcWRUSEpyTlQ3eGVsWXlpMFJvMzZCZytYMnJMQSs4a2FPYks1T0hwQzFvQWFpZjFWVU5UTXA1YlN2MWxHWW1SQ1VFeTJiYjVTV2d1";
const QString KG_SONG_LRC_URL      = "Yld5K1Rac2JFc3d0YThTbnluN2RkQ0U0bjBWQkxSQWJGNDZoT2xvUERFdEUzV0RkalhjbVpYT1FGcmY0UlZJTDRoTHFMMklXNWRsdyswcG5XYzArUlJ6bHhoWXdyQ3JtSXRiWEdXbE53dXNjcU1QMzRBNFBhMDdWOE95eVVURG5KRHRPNFE9PQ==";
const QString KG_SONG_ATTR_URL     = "SFBKRGpUWnBOS3BVMEZVTWxzWFFDNUd0NDdaZkg4WWEyWUhEOVpidGcyY0c1QmJaWTBIZUIwY0UvcVZLT1ZKK052NGg4YmlTSFZVWmpZdUFGVDRBVWNKQ2JkWi9rTUhSWlNTcElnPT0=";
const QString KG_SONG_INFO_URL     = "M1pINTArNUl1RFdvNmtNU3U3bUo4MkxDUnp0UkxrZ3dqS01NQXRKZm9hTGVWYzVidTNmKzNoU1dxdG5odXRIekZISXlVVWlLNVBUdG55bElFWFl6Qy9Eclgyaz0=";
///////////////////////////////////////////////////////////////////////////
const QString KG_ARTIST_URL        = "b0pLNUhiUmVtZktML3REeTRLQUFRY0tBZXFtU2IyaU9tWGtNbmg0WUlzYTZWUUdQTU00VkVJdWRWYkJxL013QjBMY1YvUzhGU3RWWEFmR2ZQVW1pVkl4enhDZ1p5ZG14bllHZnpkWm5qemR5NlprUGxDaEs1bnhrQzJQYkhCOUY2NWV3UmxuVEs2dVU0U3UwbjJCZ053PT0=";
const QString KG_ARTIST_INFO_URL   = "c04rT3hSOFNZYU5oNWoxL0xuMUNna08yRjNlUTBPeEd4OGU2a3ZTRlYwRkRxQVJIcGFZbnI3M2J5TGZvRGFWSFFGTXlZUkw3dFVmSHRqSjlrdUlubkhoNHFsZ1YzWmZid28xYUNQK2UwSmM9";
///////////////////////////////////////////////////////////////////////////
const QString KG_MV_ATTR_URL       = "ajJaR0tsRUUzcUE0ZnZGYmhhR205TkwvUGFMcSt3RU5RaGhqQVFEelRjemRVR3RKNjBEVXJzaWFCU2NFNExoaTFFcDI5bkpMQlRnZlRmWHcvQS9DNzVNNGw4ek1pM2NoTjN4QVBYME4zWkxBME94Mg==";
///////////////////////////////////////////////////////////////////////////
const QString KG_SG_COMMIT_URL     = "bEoxaXdMVjlvczg1T2VCZEtCOENUQkFPNXlzZ3BsS3FVVUc3M0ZibnM2Q0VvbURVTHV1NmFHdmR2VnpBRFhXVnl0OVFBZys2MmErVitzZ3VpT2ZjZTUydHpMN3ExQ0tUVnFmRXNiR0lER2tkZmVTUWVuL2tpODBKRS9SWk9kNnNlbitTYzdIV2EwZUtHOFhn";
///////////////////////////////////////////////////////////////////////////
const QString KG_SONG_TOPLIST_URL  = "MlJFQW9HR2FuQUFtalo0d1ZhT21JMTFxdGg1YlNPSEpUSXFtV2ZOdXBWUVd3RFUwRWR3VXV4Qk10K1ZmeUdGSWVKTEtxZXBCQm5Qcm9xSGo=";
///////////////////////////////////////////////////////////////////////////
const QString KG_PL_COMMIT_URL     = "WDh3QVJkNnZqR3lXZ29ZQmw3dG5Ib0loVDB1UlZVNDRVTzI0bFZVekU0bkdWalo3V2JwRzlVcG15ZnlNbTZSZ21rY0tUZGhOV1V4SVpGOFk3ekpvZVhldFhaMHIzL2w0czloSkFLaXFCUUlxN1FSVzd1eTREWXJUbmxEeU5MYjZXWkxBRDhVbjAxMHVuT25mcWUxYTBpZURrMkZWWXJYRGdEOGhPb2ZWZUpuWklIb01YZTl0NWU3cUJZcWt3WmdUc3Yra1lMcVBPZ1FEa2drbTE1aU9VZz09";
const QString KG_PLAYLIST_URL      = "cmdDakZXanFhVHdmTysyQ2FzbDVxMERadXBJaHJodmVuQytKYTM2TXpnN0hudjd1ekppTDZKaThBQ3FSbm1jVjlNTkxQTGxNc1JWMWNONUt5ZC9wMG5jU0t3MTNRS0VnQ1RCY2ljMzdQcnU3WmhaZlMzYm1yN3RNY2NsSEZRVjJzSjJmdVVqL28zM2VYSUpHTFJJK3VnPT0=";
const QString KG_PLAYLIST_ATTR_URL = "WFZ6WjlEOWtHQ3UrTnUxbDVmZHNSd2IrblY1VlNDS2pZTXN0TEloRUc1aW42eEthN1RVdzFvRllNcW5hdWdqUmJSYmh0T2hzVGRDLzh5dmpZUm1pc2taTHJIdnlUeWRiMkdEYXZpb1J1d3FiQ01heGJQM1diRXArYTllUGZKakdKVUltV0RxSGhHaz0=";
///////////////////////////////////////////////////////////////////////////

class MusicPlaylistItem;

/*! @brief The class to kugou query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadKGInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &hash);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                    const QString &quality, bool all);
    /*!
     * Read tags(lrc\smallpic) from query results.
     */
    void readFromMusicSongLrcAndPic(MusicObject::MusicSongInformation *info, const QString &hash);
    /*!
     * Read tags(album info) from query results.
     */
    void readFromMusicSongAlbumInfo(MusicPlaylistItem *info, const QString &album);

};

#endif // MUSICDOWNLOADKGINTERFACE_H
