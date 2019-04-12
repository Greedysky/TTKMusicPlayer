#ifndef MUSICDOWNLOADKGINTERFACE_H
#define MUSICDOWNLOADKGINTERFACE_H

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

//
const QString KG_UA_URL_1          = "cGhYNDZVdmNaVG5KZk50NVFvcUJyYWVQdmdNTkFTMmM=";
//
const QString KG_ALBUM_URL         = "T1I2UzExcHpQU0lzNVJTVkR4UkVabkxndmF6eGp3ajVvSHJaZWFQdFlybENrVDJiNWZ0OUw0VjN2TjArWFg4MS9LT0pRMmRrYzlIeWYvZmpEVkR6aFdwSUZhN2pLNGx5NHdaWFFGSlRUWDZ2aWFWazBhWmZWSXBEQXBHcjhIb1c=";
const QString KG_ALBUM_INFO_URL    = "ajBGaXREQ3Y1S3pwZGkxd1pJUEREbUNxL3d0SWpPZzFNQjhZQjdyZk0rQmRzQVZXUFBrcVp0ekh6NEtLbjJWTklDNWIxbGpycFFqbHVVRE95aDRWMUJ4NUNVRTUySWdU";
//
const QString KG_SONG_SEARCH_URL   = "K3l5ZjdYOTcySmR6cFdhbHhNTU1KSWswY0JKa1JFWmJoZjJHTExaZFdQejltM3JxbFRWNWRHVmlzL01ORzIwSTVGTHd3dzFaUWdWTG1QRk9ockZJNkczb3poTjBKYVFIdHBNeUpmb013bmFXWXpIZWErZXhJcWRUSEpyTlQ3eGVsWXlpMFJvMzZCZytYMnJMQSs4a2FPYks1T0hwQzFvQWFpZjFWVU5UTXA1YlN2MWxHWW1SQ1VFeTJiYjVTV2d1";
const QString KG_SONG_LRC_URL      = "Yld5K1Rac2JFc3d0YThTbnluN2RkQ0U0bjBWQkxSQWJGNDZoT2xvUERFdEUzV0RkalhjbVpYT1FGcmY0UlZJTDRoTHFMMklXNWRsdyswcG5XYzArUlJ6bHhoWXdyQ3JtSXRiWEdXbE53dXNjcU1QMzRBNFBhMDdWOE95eVVURG5KRHRPNFE9PQ==";
const QString KG_SONG_ATTR_URL     = "SFBKRGpUWnBOS3BVMEZVTWxzWFFDNUd0NDdaZkg4WWEyWUhEOVpidGcyY0c1QmJaWTBIZUIwY0UvcVZLT1ZKK052NGg4YmlTSFZVWmpZdUFGVDRBVWNKQ2JkWi9rTUhSWlNTcElnPT0=";
const QString KG_SONG_INFO_URL     = "dnVZTEYrYXgxcWkvT2dmWHdLNE1oMUZURStIKytVSnBIMzVXUlk2cVkyaURGcEl1WVRZa2lIRE13dTVSQ0JUcWwwM0hkUT09";
//
const QString KG_ARTIST_URL        = "b0pLNUhiUmVtZktML3REeTRLQUFRY0tBZXFtU2IyaU9tWGtNbmg0WUlzYTZWUUdQTU00VkVJdWRWYkJxL013QjBMY1YvUzhGU3RWWEFmR2ZQVW1pVkl4enhDZ1p5ZG14bllHZnpkWm5qemR5NlprUGxDaEs1bnhrQzJQYkhCOUY2NWV3UmxuVEs2dVU0U3UwbjJCZ053PT0=";
const QString KG_ARTIST_INFO_URL   = "c04rT3hSOFNZYU5oNWoxL0xuMUNna08yRjNlUTBPeEd4OGU2a3ZTRlYwRkRxQVJIcGFZbnI3M2J5TGZvRGFWSFFGTXlZUkw3dFVmSHRqSjlrdUlubkhoNHFsZ1YzWmZid28xYUNQK2UwSmM9";
const QString KG_AR_ALBUM_URL      = "RENyS3hWZFdJTUQ1RGJHczRscTljSkFHbFd0MzZhcFVvZ1d5bDVqVzUxRzMwN1pPZnJPUE5IdmdaQXVLL0pWaE4zc25ieCtJMVpyd0M5YnVrMnIvcExqQ2RJT1FJbko4bEpkWmt1VnlValE9";
const QString KG_AR_MV_URL         = "VkNHdSt6bWtxZ1Q1bEFjYjVoN0tvZzdFZFN1Rm1Vc1U1dE9vTlpoYjloNTBKMDFOQVluUGRRYkxFQ0ZDVFJ2QzM3ZDdPdy9RRDY4RW4vOW8zVHBaNXQ2ZHljbVJ1L1ljMTJERFpnPT0=";
const QString KG_AR_SIM_URL        = "ZlFXdHRhenFreHA1QjFnMTFvWk8vZFdMTWpOOVA0RU5yWjBDTmxUMkJKMFhUOHJ2YVZhV1M1Q1BYMkFxTjliYQ==";
const QString KG_AR_LIST_URL       = "ZCt5ZDJpbFdCZHNvUWVHZXJjMDl2L0FZd01mMVZSckkxaVJLcDJXYnZyT0FWNWQyWDByNkxZNXdDWm1yZ3BpNlg4MGtsTVpzTDZNWXdjRFU=";
//
const QString KG_MV_HOME_URL       = "TDRVU3d2Z0ozOXBMYzlhQlo4MkJObDBGOE9DVG9jRGRpT21XVHp3S0svZ1NzaFFrb3k1NFlmTU9raVU3eTV1ZQ==";
const QString KG_MV_ATTR_URL       = "ajJaR0tsRUUzcUE0ZnZGYmhhR205TkwvUGFMcSt3RU5RaGhqQVFEelRjemRVR3RKNjBEVXJzaWFCU2NFNExoaTFFcDI5bkpMQlRnZlRmWHcvQS9DNzVNNGw4ek1pM2NoTjN4QVBYME4zWkxBME94Mg==";
//
const QString KG_SG_COMMIT_URL     = "bEoxaXdMVjlvczg1T2VCZEtCOENUQkFPNXlzZ3BsS3FVVUc3M0ZibnM2Q0VvbURVTHV1NmFHdmR2VnpBRFhXVnl0OVFBZys2MmErVitzZ3VpT2ZjZTUydHpMN3ExQ0tUVnFmRXNiR0lER2tkZmVTUWVuL2tpODBKRS9SWk9kNnNlbitTYzdIV2EwZUtHOFhn";
//
const QString KG_SONG_TOPLIST_URL  = "MlJFQW9HR2FuQUFtalo0d1ZhT21JMTFxdGg1YlNPSEpUSXFtV2ZOdXBWUVd3RFUwRWR3VXV4Qk10K1ZmeUdGSWVKTEtxZXBCQm5Qcm9xSGo=";
//
const QString KG_PL_COMMIT_URL     = "TEZ4NXdPYnM4MkNJSTZMNUVjU2h5cGFvY0FzWmI2Lzc1RGM5UUJTRTdVcVZEQndyMGRQZ0x0cXJlb1E0dXVqRUZSWXRoSUR4V1lRUVRLQjVuNkdyUDdFZ2x1ZitLZml2eThxU2R2TEJHYUM5a1hDak5GSS9weStvb2JYRXhKL3hkMGVMdmVVbm5DOFEwL3BTMjd6N1VvY1pXYzRlcWdpNFJKVmZqa1hEOTRWUitrVEE3UTBWcEMwMllsMXJZSjV4S1FQSE9wWTlsdmtXQ1pYaA==";
const QString KG_PLAYLIST_URL      = "VXJlbFlzWElZQ25iMzhicGJIVWJqRGpMdnJJU0tlZXdUTlVpWUJkN0xxR1Qya3dud3dLY2JSaWphWTF6QUw5RDJFK3hlZWhxdGhrRWFhQkpJUFhxOGUrY1J2d1NQVGZNM29pOXVXU3h0V1hrR0RIMnhHK2JYZz09";
const QString KG_PLAYLIST_ATTR_URL = "UnFQY3RkQVYvN0lza0dkRXowbFo2QThBcldxM2FtL2p6WmR5b1FJbjJtS3pMeDYxQk1vc3kwYjhBV1owQ3R4K2pxY2dJNXM3cWswSU8rUDlQbjFQWC9ZcjhldGMwVEtPcTFnbnpvSTdaM0JQSHM1dHFyUDI2cDRPamhUZW00T0ZYaHpBUWc9PQ==";
const QString KG_PLAYLIST_INFO_URL = "cEt0bHpiRkFKMTFZbzUyQ1prK1RCMjBqWnRRMGRKY2FqYnVSRllPZHMwcmoyVkZ3WG8vTWhMVUR0ZTdIZUszYmdybXBUdEVxOHdiSEM3Nmw=";
//
const QString KG_RCM_URL           = "U281QUUzc3V6dnRPeFRvckNoTEFIeDQxdlE2Y2pjMkJZbysxU1lYTVU0S0lKQm5XRkhFaDRyRmVWb00rOUYyZFcvSW1sWjExQnNMUTZjMVVUVnZTeVNrZERUNmlaSTNZSGZnRVRSSTVidlpyLzhDdjZ2ejM2S2U1Ylc4RnQrZzJHM2ozVVE9PQ==";
//
const QString KG_SUGGEST_URL       = "UUFkNUtUeXBhZ2hxbG1nY0ZDRzBxTS9GNzdZMDdDbmpMUDVTaFJKeHMxSFBhdEw4dGZSRWVCL0tJY2NkMUdObjVyM0dObFdaaUtqbHZhcVpFWFJDTU5JREdnVmR1RFZ0";
//


class MusicResultsItem;

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
    void readFromMusicSongLrcAndPic(MusicObject::MusicSongInformation *info);
    /*!
     * Read tags(album info) from query results.
     */
    void readFromMusicSongAlbumInfo(MusicResultsItem *info, const QString &album);

};

#endif // MUSICDOWNLOADKGINTERFACE_H
