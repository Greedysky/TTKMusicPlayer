#ifndef MUSICDOWNLOADWYINTERFACE_H
#define MUSICDOWNLOADWYINTERFACE_H

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
const QString WY_COOKIE_URL          = "SHV0MTlHYTRxNytPZklyeTMwcEJzUDE3YWxRVmRlMTNlVVZYSlorVzIrVEpLM1pmZ3MyNmY0M0t1MHV6QlZhSVdvS2V4MWNIbnhTNnplUmM5SU5pLzZEcHI5blNWc2tteXBaS1lhME1YTHgxMHdSVkpDVVhWUGp3YzU1ZTZXMHU2aG1iTDRFK3RFQnRneHpqSm51NDFUNS9oVlZGc2VLSjd1RzNaTkFINzc3bjhQd21ic0R4emdHWHhUYkhOQWdGWmFTWW9nPT0=";
const QString WY_UA_URL_1            = "N1hBb3hrWFVaNnNTejltZUpIeUhieG9xN0FuV0J3QmkzOElteHZYNkQzL1Z2VEIzVDdsMVlURmhUVFlZT3NSU3BSQzVmUDFWNzBIbEwvaHJ4UFg0ODVzUVBXQzZNcjVheWZEcmF2ZitwSm1WUFV1Y1ozZXdCUXBsdXFoZ1ZyTTNURGVHUTZYQmRMQXJDZ2ZrQ3lNTU0rb3UxMkJmQUpxYXdaYVlCdC9kdm84PQ==";
///////////////////////////////////////////////////////////////////////////
const QString WY_BASE_URL            = "MVNVTXo4bW9WdHhXR1dXeTZmU3k5dmFOcGlua1VOMlE=";
const QString WY_ENCRYPT_STRING      = "3go8&$8*3*3h0k(2)2";
const QString WY_SECKRY              = "411571dca16717d9af5ef1ac97a8d21cb740329890560688b1b624de43f49fdd7702493835141b06ae45f1326e264c98c24ce87199c1a776315e5f25c11056b02dd92791fcc012bff8dd4fc86e37888d5ccc060f7837b836607dbb28bddc703308a0ba67c24c6420dd08eec2b8111067486c907b6e53c027ae1e56c188bc568e";
///////////////////////////////////////////////////////////////////////////
const QString WY_SONG_SEARCH_URL     = "b2dKQmJVWmxNRXlqYU1saWFpbXV5Y1NkenZvN29UVFh4WjhScXpST0Nic3VDNDJkZ0QrWUxnPT0=";
const QString WY_SONG_SQUERY_URL     = "UUs1NUMwUk9yeExmUFVCakUrS29NNm5OQ05lYUV3ZzJPS3NLVW9lZXhNaGwrd2Za";
const QString WY_SONG_MV_NDT_URL     = "dmt4VnNDdWErYjRYUXBnOWdubVBIQT09";
const QString WY_SONG_MV_N_URL       = "dFowcHhjYXk3UWRkZlo2eW40NkY2TEU4VzRoMC84TFh4Wm93N1hCbWk5V21WQnZnYzIrVUhvYmVtK0xtUW90cGpidHBqdz09";
const QString WY_SONG_PATH_URL       = "SXZWVUpDL0FCM1pUODJsaDNGZjN4OFlVZk1JQzdXOXRpWVJTSTVEUEZJZkI3YUJOMUdLQ3lnPT0=";
const QString WY_SONG_LRC_URL        = "eEZRWnA5eWZZZWFvakpnQ2F2Sml1aTZQa1crekFlZUJoTXUwRUgwekRtOXFsU2l2S1BKZFdwTUVaNGRzWUJ2ZTNpdlRDNnVuTUt2M0xtUTRsaWh2NTZBQ0JacnNTbXp6";
const QString WY_SONG_INFO_URL       = "N2NkQWg4V3o0NXYvMlFHamVOWVZ5Y2NnTDk2cTNFdFVROVNwTndJV2REYXM0K01Yd2NIT1pwaGcyTGJ0QVVsNHVxT0lTZHFKNTI2QnlWMnVBWFNDdWYrb09Waz0=";
const QString WY_SONG_INFO_NDT_URL   = "ei9SdFJqZWlHWlh3eXBsWHJTS09FbUpRU1J4Zi9wZDlOQUMyVTVnL3c0YW9GSjVH";
const QString WY_SONG_INFO_N_URL     = "M21IV2dlSHNzaE1zZEpyb2hrQ0dqVEY3Ly9DZ3ZteDM1RStuSlJIRVVCRTljbWJKKzRBVnNZY3JEcEZKY045dEJ5KzM0WHArSDVCYXhrMHBoZmlTZXBWdHIvND0=";
const QString WY_SG_DETAIL_NDT_URL   = "Zmh6cVNyd2REdlVlZ3JQVUZpNjd5YlBaYVR0SEQrWFAydVpzOWc9PQ==";
const QString WY_SG_DETAIL_N_URL     = "S3paVUxRY3hZVDVweWhJZFU0K0FpakhJc3R5YW1hbmpmRUVsL3JTRUdwcnlDdDlMWFJSR0ludFFPZ1MycWExVVlLNjg5Njh3VmV2cGxpQzE=";
///////////////////////////////////////////////////////////////////////////
const QString WY_AR_N_URL            = "YmhRbVZDYy8rbytiRWc5cXc2MUVUdEFKRmw2eFhTVmhqWTZ6MmRVNEMzbVg0Q3BFQVF4d0VYUktBMk1aVUNVb3ZaNk5VL3BYRjBZPQ==";
const QString WY_AR_INFO_NDT_URL     = "dmt4VnNDdWErYjRYUXBnOWdubVBIQT09";
const QString WY_AR_INFO_N_URL       = "RnJtNVNTT01ncFI2cDJMRTMrSFFwWFBWVEEzQnZNYmg4TEJrRjZCeHorTE13aVNCNUxScGo2QzNzVUNKNzhKbkI2VjNKUT09";
const QString WY_AR_ALBUM_DATA_N_URL = "RWhFbktEb0lLdGVwOFE3VHF0SHdOekVQanlOUjJkdE02b0FIdnhXWmkxNHBqaVBYcy9BRDJDRVdqdDM0cmN1cw==";
const QString WY_AR_ALBUM_N_URL      = "dEJpSTdiaUh3R1hnRGVDUlpCNzJKRnBqNWZHY2xLVmhUcWQ0WW1PUVY0NjlONmRvMmtGaWZiMDBscjAvd0dxZg==";
const QString WY_AR_MV_DATA_N_URL    = "OE95dm9DdmtycVlzcnZaaWRhRFpHYjNBenl0bmFKM0ZLa1lVaFFVenBFQnZ5N2RoNE5VR3N5TzlDTitNTjFiUk9vdDZ4QndxSHY2QjVzcFlPM0laTlplUEt0bz0=";
const QString WY_AR_MV_N_URL         = "eFdablNDbDlZVzVvYlo4UU9waEFVRlluRUFzdXA4TXFzVE9RMllXWVdQUXE2WjlsRi9DMzc0VlNXQlU9";
const QString WY_AR_SIM_DATA_N_URL   = "OHhrOUw5Y0JLYmxHaG82bUp6YXVLNHV5UnlVPQ==";
const QString WY_AR_SIM_N_URL        = "TmVISndYN2pvQzUrS0Q4OEkySnBvT092Q2R4c1Iwa2ovUkdxZ1RuUGRERXhDVEFUMDBPbmUwSklvTVZPQXRUdnZ4QlZDUT09";
///////////////////////////////////////////////////////////////////////////
const QString WY_ALBUM_N_URL         = "QzJrMDNKTGlpVHpvY2taYm5BSWZGVTZWM09xYW5STDdXRFE5eTEzczhLZHB1RWljWlIxOVhRUXVDcmpuVjJsQnhqcmZRZE9UU0JZPQ==";
///////////////////////////////////////////////////////////////////////////
const QString WY_SG_TOPLIST_NDT_URL  = "UzFpUWk1ZjVnTlpZS2Fabjl0dE50Sk1mUmJZZjlRVTNwVHJlem9ndG1mUzNpSXdvaDluK3NXUlEzdjkyRTl6bXoxYjg1c3g0UXZPWnlPUkxUM0YzMWtmZzVVdzduV2M1";
const QString WY_SG_TOPLIST_N_URL    = "VUdTa0xMeG1Nblc4cThoS2E1QzhJK0lrTHJBRmk4TUJZMC9NQW1GanZydDJmR0VJVVRZNmJQSEhOZU96aE55bTJxdW55RldJOHl6aElNdTlHbkZkSmc9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString WY_COMMIT_NDT_URL      = "cmVkanJSR0J1UGdWVlAwaSs2bVFSeTFMdVhvQzVHNVgyWUVGSWFESXgyUXlSNkU5KzhqQXlBdlI1b3h0UmI5N3lUZlVBM2NIZTE2Tk4zV2Y=";
const QString WY_SG_COMMIT_N_URL     = "VjJRaU8vR2xYZmIxM0RIMUNxU2NJRzlLSDVqZTY5ZVVXNlhUWndSWkc1TmlEajFKaGRwaS9TRThKRCtyL0Z4SWVSWmovSzhqMzFKV1R3QmxwUVBHSzA5ZGx0d2VmN0JMaVRFaVRnPT0=";
///////////////////////////////////////////////////////////////////////////
const QString WY_PL_COMMIT_N_URL     = "aURTWlNhUlJISEc3VmExQ2FsUXZqS1lXOVdGSzBOc3dmaHNMK3M0U2tScnNIdHc3Y2NJSGJBVVhhUTc1eHIrNEJrdmgzUlpCOU96UmNsZEEwbnIvQkZQU0tFZU9oY0JKazNlRWZRPT0=";
const QString WY_PL_URL              = "aDd0V0k1Y1lWcXBqZ3VMQWhaWmp6eFlUYXJmZmRyanFwcVFINmo2Qk1KNDVETW9TWE4zellMREllOG1NZ1YvWkZRM1drcE9PbmVpblRBOVcwZUhaRDA3WVoxYW9XK3lPckptL0VjaTd3UDd4ajlGUEhlZE5FK2x5Q3g0PQ==";
const QString WY_PL_ATTR_NDT_URL     = "VkpFemFKQWtOY2ZXeUNpZ210VjVGYndOZ2VibUZlZHV6amZJaE5VL1czaWFzRkNqV2lYKy9CR29McXl6LzUrUldWcmtKS0Fjb0s5dFBFeXpCaHRCc1dhYlp1bGd3U1ZY";
const QString WY_PL_ATTR_N_URL       = "VUdTa0xMeG1Nblc4cThoS2E1QzhJK0lrTHJBRmk4TUJZMC9NQW1GanZydDJmR0VJVVRZNmJQSEhOZU96aE55bTJxdW55RldJOHl6aElNdTlHbkZkSmc9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString WY_RCM_N_URL           = "VklDaWtHcm5waU1raUxyRUNKMjR3Mk4zRmpMSzRhRUFlL200S2thQmdZZU5BMVpDY3hFUmNuME9mZWdGc1hBZzRVSzBDUmZhK1RJbmxpdnM=";
///////////////////////////////////////////////////////////////////////////
const QString WY_SUGGEST_NDT_URL     = "ajU1Wm14WjV3KzhBeTdpSlNmVE5lUT09";
const QString WY_SUGGEST_N_URL       = "alpUZU5qWnJJc2NHTDdiN0xDTm5NZnpxN2s0c21BTHU2eUFOcTNiOVFSeHE1dms1UXFhQWZHVGpjY0l4OVhGL0RoeWYvUT09";
///////////////////////////////////////////////////////////////////////////


class QNetworkRequest;

/*! @brief The class to wangyi query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadWYInterface
{
public:
    /*!
     * Make token query url string.
     */
    void makeTokenQueryQequest(QNetworkRequest *request);
    /*!
     * Make token query url string.
     */
    QByteArray makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                    const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                       const QString &quality, bool all);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(qint64 id);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(const QString &string);

};

#endif // MUSICDOWNLOADWYINTERFACE_H
