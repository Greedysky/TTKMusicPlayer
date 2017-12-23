#ifndef MUSICDOWNLOADMGINTERFACE_H
#define MUSICDOWNLOADMGINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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
const QString MG_UA_URL_1           = "RmRqVWRxSmI0Z0ZEWHNtNkhvQlpleXprbXF6VThWak1WZnBQU2xOb3ZhQjJDTE9UUk5xNlZFMVcvc0tnUjNDc3hWL0hmWmw0UkFweURvMVJlMkgzMlN0bE1xOD0=";
/////////////////////////////////////////////////////////////////////////////
const QString MG_ALBUM_URL          = "dDVkT1UxOWUzR3dDb0N5UXkwcEFUZmt3SWJvWGU4ajYxRTBGc3pMYnRMUzJMUDV5VmREUjdoL2IwZFpJYlNvanJiVGJPbERzcTkwdWR3UmN5Sm0ydlQ0NWQ5cHBpcGFpalBaendxYmMwUDBhNllyWmdpY1M0am1oSFZsVVNZM1lDbEVMcE5PUnNZcGtQeTBhTEhLV0l5R2NzU0U9";
/////////////////////////////////////////////////////////////////////////////
const QString MG_SONG_SEARCH_URL    = "N0s5WnhWSklNelFwcXJNc21RbXZTWXdMM3g2SlVwQWxCcDJSSzI1RzRsYldRV3BQeU9aMkZwaE9mZC9NQVoxNTJoMEpCcFUvbGNWUzVIWTArZGNWcVBLSXBKSjM2SVk3SkthTFllNlMyL01GYW1UV1NCMWVmNUtSMzNZPQ==";
const QString MG_SONG_INFO_URL      = "VWRwS0xoYWVINW03MDFOQkZUN0szTWduMm1aVktNbm9BRXI1SkxaZk8ySURmQnVqNC9vaERwMFRVeWpqS3ZpbmNnMkh1VDgvK29aS3gzTzNiNWpoK1cyZXhxNU5GbHVtRy9tSWEyZkpFNFF5bFJaY1NZZ3YxS2VrMk5RPQ==";
const QString MG_SONG_LRC_URL       = "UVR1SngwZVgyTUpXVlZmVU02UXZXLzhMSnB2VUdnblVOMi9CekNSaEZpSEJFNTFhVVBQNUhmMTJLTnZIUGJwV01JSDI2N2FxNEgxbmpRYXU1Y0M2TUNKN3RTa25ncUFBSSt6bTFrWlJWb2Jqb1ZrVVAzaGVmdz09";
const QString MG_SONG_PATH_URL      = "WXNLRXVSdjlpUG55YWMyMHk5N3hSSDdXdGs1U1FEZlRtdEY5Mm5CZFQ3TE5DcjY3RFBQclhjSjQ4NitiVW9saGQ5ZEs2cjcySmkzN1dscVB2VWppUzdjOU1NdnJkNUVFaHhCdHVyOXMwNE0yMmNpUG5Pano3dHdkVHVNVThEZHQ0c3YwZWJUZVVnR1FsWmtNMm1lcGszUTRvcjA9";
const QString MG_SONG_LL_INFO_URL   = "SFVWbUdhb0N3RDkyUFhpbjNTalVPdzFrdlJ2WlhwUU5HMzJOd1NiTUQvWmYyZ1hsZGN4UDd4N3E5V2kyT3EvNmRHaFVsdDM0NXZlTGo0Zjl2aUpwMHJmWVg4NVJYM25KUHZjUW8xM2pGTU5Wc2FQdy9yb3hZUk13YWxPWDQ3QU5nWGVFR3ptOWc4aSs2Y3ROdmVhQnh5TGcwZmZ1a0pqU1dzbjV5UT09";
const QString MG_SONG_LL_PATH_URL   = "QUw0ODhGWDhEalk2c2lrdnpKdFBDT3dDRXF1ZnUwTmE5bVFzSGhla0VKWEJsTGFMSjJCQkFVWkNzL1pqQXVTZHZ0ckEzVUJ5S3hyYVZrcWd4VHl4bGNudnJPenFtSGlpOU00MENPREN0aTA9";
const QString MG_SONG_LL_DATA_URL   = "UmNUSmEwQ1F4bld6S1FlcUFVSktybnNxcVBvdHAvRTBBTjNGd2JPdTlQVTR4L3pCRUNnT2hiU01kSSs1dVJwL28ydVlockd6cG1IN2lDbkw=";
/////////////////////////////////////////////////////////////////////////////
const QString MG_ARTIST_URL         = "bmJVKzhURTlQMmVHQ1lyWG9FdERQRUFWa1RWTkxTWTNSaDc1a0xhL1RxRUp4N25xY1FBeUJETzZwa0NsVG9qSzZoYlptRy9FNkFpaS9qcE0zaG1YYVcwR2xKaWhhVEVoMG96NHg5YXhZUjV5bmozeVZFSE5QU0g4d3E2VEd1NHJVaURmSFE9PQ==";
const QString MG_AR_ALBUM_URL       = "b21uei9xZlFlWkNtMDBPSWRJUm40dm1VeTQ0M202THBjRHAxbU4rYmZnZnFvZ0dVVmpjaGtsM1c4L2hiTzQvR3ZZcVlIdUZSeFBuV2FyNG1idmFLS0J5cTVMZlIwdE9OT1EyZjNvdjRUb0NsUUxwZVJuVWtNaEVuNGxiWVBwaUUxNzQ0SVNickZrMENwaEZyMEs1bDFZb04yemlGTWxHcg==";
const QString MG_AR_MV_URL          = "ZHhISTdyQ0dhdENuUTJnc3BIKzBETjRwYzZmRFZicjJaVE9ydko0d1hVQXp4TEgrQWIvWlFWUFh0K3BjeDdBeHBoK0pXU3lzdHN2N2N4ZVR5V0YzUVBHdmVGRzNzdVh2cGx4Q0JEMkJoZjI0aHlDVVg2Z0dhVDMwUTl6VUFSQU83OHZXZjltamxHeTAvK0ZzUkN3anRTZUg2a0U9";
const QString MG_AR_SIM_URL         = "K0hUaGtJU2ZtR2lRbW1kL1BUMkVBSTZwdHVOVURjVnlIWEZtcW1ENmVsVEJZM2o5UFlBZWZBZmMwMHM9";
/////////////////////////////////////////////////////////////////////////////
const QString MG_MV_INFO_URL        = "Y0IxOE8zK0xtNXlRUVVkU3U2bHZ2YnVXWnFKQW1KTUdreldneklJdTVsRVFzcWJEOXlSeURnMFFPcWRpZmI2a25HVUcwY25qQ3FkMnFTSmtiK0h1UjVRVFNkZzdTcWNwUXFWR2NLRyswWnU1UHRzWFNCcEo3azM5Wmxibzd4WjVPTlo4a3c9PQ==";
/////////////////////////////////////////////////////////////////////////////
const QString MG_SG_COMMIT_URL      = "UWZ3UlR1MnFHemlaa09uUm4xbmwyWlg2TWZibnZYOVppb2NReHEzUU0zcmQ5Zy9rc1AwUjNsZVQxMnhHK2R0VkxtbWlMY3BkSTh5ZVByc2oyY1ZIa0FaUFNpVXhEcVEzRG9naXhPOU90alJwYTRVMUJyMTBEZz09";
/////////////////////////////////////////////////////////////////////////////
const QString MG_SONG_TOPLIST_URL   = "dSt0MHMxZ3c3Q3FNYlZseU4rVHhPbms4b1VBL3g2VTJxTnUwWURlQXp5SEhMNUs4aGFsUVU1dTFvNURjNC8rUVNuYytTbXlHR0pmaVBUek1TbEFOOU8wSVBnYjFmaGxEZmVQd0xKTjQwY1FLYnJLYTJsWldTQT09";
/////////////////////////////////////////////////////////////////////////////
const QString MG_PL_COMMIT_URL      = "K2F4Y0FDYWhwSGxaL3ZvZ05ReFVsTWEvNHB3TFNVcFFMYlBFT2ZHTm5haFZvK1ZUUGI0UUVIT3BnNzZ1VUNGVjhMNG1HQUpoN0duTmdpZkwzMFFXd3BjMTNTYjNkNUlGcTdEaW0vM1JFVE09";
const QString MG_PLAYLIST_URL       = "dER4T1JRWUJwWTd5WXpCMnc4dE9UMUVNZi95T1Rncm0xbERRaDlCNlZQa0dNSmpWVmZ2ZzdPZnNFNmRtUFFhZ2YrZWpHblkxSVJUdU9EYzRnRmwrLzVYQ2VFM2w4ekhwU2h1OEdDSFhPdWRvMnFxUTNsdmlKdFM1anpWcWVCYW9PbG1OQktuQnFmL3ZneVdQUzlhd1BnQnVXZVMreGMvUw==";
const QString MG_PLAYLIST_ATTR_URL  = "R1E5aFV6d2RETDJLU01BNDRHUm9hVTN2bHNERHZPaXRSZkp5TnUxYUVZT3p3bVdVWUhYa1VWTHVKMG8vRUJjOFQxbmwrSjBHYzZqMFJ1SFUyekpyaGh5RXg3OG1vYWJkZmsrZEFvcm9SYW4xSlJ2YURwQmhvaVRIUEZjVnNwUk9oTmtoT210TFMrd1VPbUdwVnFuckJnPT0=";
///////////////////////////////////////////////////////////////////////////
const QString MG_RCM_URL            = "SnA0YUxQb2VlelZEZGpnclR6ZVJKMGdOL29PdHNrZGJNV1JyNzJVdGVpeWR4dEsrVlFNSTdWcGZuY0hDNG9ZL25sUHdSWWhTNVRhd1ZnNlpwYUxHTm11MTVyNkQzeHVjUVo3NngwVW1uRTYyZFlRZWZQRWJUY0xsUVJFMWdBUUthemxlWTAvL3UydmI3Nno3emJiOG9zbURXdmFvQTA0RQ==";
///////////////////////////////////////////////////////////////////////////
const QString MG_SUGGEST_URL        = "YjMzN29GZ1ppc1gyVlZvZ3oxUTBkWTVUckQ0R0hWbkR6U0NrS1pKdkxHUVVVd1czNFo1ZUFQL0FlR01xckd6TUF4ckw3Zz09";
///////////////////////////////////////////////////////////////////////////


class QNetworkRequest;

/*! @brief The class to migu query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadMGInterface
{
public:
    /*!
     * Make token query url string.
     */
    void makeTokenQueryUrl(QNetworkRequest *request, const QString &id);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, int bit,
                                    const QString &format, int type);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &format,
                                    const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicLLAttribute(MusicObject::MusicSongInformation *info);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicLLAttribute(MusicObject::MusicSongInformation *info, const QString &param);

};

#endif // MUSICDOWNLOADMGINTERFACE_H
