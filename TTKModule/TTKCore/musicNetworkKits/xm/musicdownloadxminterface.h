#ifndef MUSICDOWNLOADXMINTERFACE_H
#define MUSICDOWNLOADXMINTERFACE_H

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
const QString XM_UA_URL_1              = "TzVnZHRZRDZUWmxoeXA3aG5xai9Ib2JUZUdCTlhiZUVqWVR1QndjMU9WcGs0c3lUSGpoUHZZVzdnYWFpUUJ1NG1obktHQndrblVDUTdxNS9jcnZCY3kvNE9TRjlHai9mZHlLbU02VHdiOFpzZWZJa3hqcHpCaGpvUmJjTGJmelBjb3hmL2pzMmlVeEV3RFFoc2x3Szh4TFVXUnRkSlBQYlI3S3A1L3JJRkZFR1dYbTNDL1Z0aVRIVFZoU3pWVWtxMW5MSVhGbTFVemM9";
//
const QString XM_COOKIE_URL            = "cFJQbVQzb0JoUTdmYVhObVRlZlMwSmo2aEhZRHNRQ1JFaldJQzdESFp5WXZoaGJIKzEzbFJQNnI0UExUS2tuVFh5QmJmNkF4djJVY1dtc2NFdGRoY1NBQ1MyRjIvQm5aeTNDQkNUQXNVNnpHdi9VUjkrK3VzQytycjE4PQ==";
const QString XM_COOKIE_DATA_URL       = "TytOTG5sWnJSOGlrS1p0T1ZzOEpOUThsZHRaU1hvdWZFNmUxZGdWcWZ6QWNGbStNV2E5UU5NQjNGaEZhcE4xVzk1dDdKSkdFSU1VPQ==";
const QString XM_QUERY_DATA_URL        = "c2VVR0g1YU44QkhlRnlHT3pkM1hvL0ZpMW0zMW40TGtCRWR1Ti9ta1lBL3AvU2hHcHFVMWlaK3FIaWhDY1JPVXhkL2pGc0ZFNEJQR2t6cFB5SXQ1VGw0V2JYR1hIcnNCdDhKQmxEeVU1V2lhclhHb0hDN3JPNTliNmRpU1crcVVIcHdyKzEyOHdqRWpMcThKOG8yeFNuWkVDMitaMEdSdVc5eC9sTG1le"
                                         "DdPZG42b0xYT1hsVFc0Wld1NzA0TERqRWk0L0R6cjJSd29iT3VwbC80bEhTajNUcTJrZGJ3bDIrV0NScGM2UzZ4U25LZUszZjk2ZjdzV3p0b3hUUkhVZzZCLzczNUdZNG05NGJ3NGVlRldQL3BnVnBGaExZZTc2VXRRSUNLMk0weHIrcTl2VWtjMWNmMWdWSXAyTVFMcEw5Q1JQV1BDM25aYVBCUnZtMT"
                                         "ZRZkZsVWhQa0RrZE5iUWhGODk1YkFpdUpxcEdCM2NmTEpBQkl5U3hkRWZZZWJyVXJNV0ZCQ01MR2V6ZmJlMWhrVTV5T21mVmg3ak41ZG1DRkYzaXRhRmlWOHd1Q0hHSXdpQVpVWFUwN21tTnVKY1VQajhMZ3lZTHF4WUllR2NJMXAxaGZEY3dWTT0=";
const QString XM_QUERY_URL             = "Y3VIQlFKcUY3aXhLVTB4N25CWGJFK09YbldPZlptekRSNnN5T25pU3E1b3A3aVZlQk5DbGVjbm5NN010UEpsek0xRWtJc1RQSjBLOVRzSFprbzRyc2xUam56aWpKUmRwMHVuendYZkhETEZNYmk5UStMODk1SkxLdjlFRFR0K2JoazY4QXgrRCtwZE9jb1RORStEK3lEU0Y5Z2E3bkhVNDYvOTRLZz09";
//
const QString XM_LRC_DATA_URL          = "Vis5ZDJwQmFVSEN2RkR1VGRBaHNINTdnREFyd3dqZUM=";
const QString XM_LRC_URL               = "WUIweEdQZHRXSjhxVWo5VVd3ZjRDNkhib2hXdkw4bjBkRjJWUE1aZWZaLzF5emxpZ2xOeUx6V2tFbUEwdmpJTTl3SFEzZz09";
//
const QString XM_ALBUM_DATA_URL        = "WnRoTWJTU3IwZ1ZIa05YcEJKTWF2Um53cVFHSDJRdUdnaTdicXc9PQ==";
const QString XM_ALBUM_URL             = "aUxaMFJ6b3ZldFBvTXNpcFhOTmNvMmk4TXBlODNueHo0RWphSDh6Q2tBN3B6WXM3b0ZqZzN3Q1MvUzBUUXFud0NlU1Yxdz09";
//
const QString XM_SONG_DATA_URL         = "TWl1SGl6RzUzUndpNFlQTFdEWVdxQ0I4S21SZUtKOVd2RFRvQ3BXTGlubnFwcWRhazVabG1zdnI4MDNranY2OEd4ck94UGZpVVg4K3VOYi9UdHZCTGVZek1uTTdSY0pw";
const QString XM_SONG_URL              = "MDFXcWwxdmVuaVVqc0hQOG9BMDZpQ01ReGx4b0N0d3BsVkZEcEg4cnhaU0N0a1ZDVkh3L0hVUnFhazBhbUZqMGYwOFVoUT09";
const QString XM_SONG_DATA_INFO_URL    = "Vis5ZDJwQmFVSEN2RkR1VGRBaHNINTdnREFyd3dqZUM=";
const QString XM_SONG_INFO_URL         = "RUdxWnkvbTNLN05Wb0FBOWtqcnMrWGh1c1FrcWdqQk9kVk9JemRHWWVCSC9TMzM2RUdTV1h6alVST3JhMGZIZ0psYjBhUT09";
//
const QString XM_ARTIST_DATA_URL       = "aEorVXFna1ZnaWdUUkF1ZVRaYi9vQ1JhcjZSU2RGaHg3UXRZdExIQk85d3BtY28wK2hKL2l0NkViZWhwaUhoUjB1Z0o3clZSRXl4K0dFaFVUci9GaDlQV3hhRitBeUpSSFUyZ2hRPT0=";
const QString XM_ARTIST_URL            = "aDJ2V2JmUWhPK2Y2VnhsSDBXT2ZGQkxuUDBHUFo4MWhWWDFIT1JRSWdjM3M1UVY2YWIvc0xBbDN1V2hlTXFGMDFFRTRTUT09";
const QString XM_ARTIST_DATA_INFO_URL  = "TzJUK1MwS2pYcmM2dklmRCtqN1NzdmNwRU5SejAzSHdJS2RETlE9PQ==";
const QString XM_ARTIST_INFO_URL       = "N3FVa3VLTVV3YVUraXJmTnZvRVN4Y0FDSTJwaHhDVmF1ZVJneTJEaUdXdDR0RWFjYlQrWjhPUWVmYlQxR0RmeXhaV3BvQWFpby9BPQ==";
const QString XM_AR_ALBUM_DATA_URL     = "VWNnTTQzQWdvQytVM2xJNU5OV00zOWVPT1l6ZmxNZUphdVlzM3hvYVNTa1VxZzc2WUF0UG85d2VDa3hYellxRU04dmd2OTNNRlZaSmR2aXVPdVZGNXYrZ09rWHBFdThS";
const QString XM_AR_ALBUM_URL          = "S0JOVUZHOUVnaEpwY2NvUDFKZEdmaVEvenIrUGdDV29zS2M2N2xmZVBlNFFkZ3RwdUVoZDdZcHRSVnJ4Um10RExMRjZldz09";
const QString XM_AR_MV_URL             = "ZTFwMTFrV0Q2YTNUVVdNWHlDRGJWdktkV1pNQ1dUVUVzVjlLaUN1NVh4QnRHSFh2K2RnTWEyalNaWGFYOE1MNg==";
const QString XM_AR_SIM_URL            = "c0J4d3JUREduOXJwQlovNzlXV0pGQnZvOERDOGg1MHhORUM1SHIrOEcyYUtITWZIbWhQYmZnRUc1VXovaDlDay83clppZz09";
const QString XM_AR_LIST_URL           = "UTVmTDdLdmJBMGlROWhGaFpCOE8yMS9HbmtwVHNoM0grNHRJVXdYdmNIWkFpUjJyb3l6V0NCQmI1dkNpVUxqYno0QTNTNlE3MmJGRTVaV21GVXpVamJ6VGpLU3poKzk5MEhudjErOHZqNzVMR0RkSQ==";
//
const QString XM_MV_ATTR_URL           = "UGN6RDlxNDdUMmltZi8rUCtrakc4YzRxV1NLZnpIeFNFdkIwdVlwMVRuZz0=";
const QString XM_MV_QUERY_URL          = "Y2FxZjF5a2xFZXBvN1F3eThTRnZpZk9ZMHNjWDVveE95MTFEdXUySHpzRXRKakJuNm9JSnJNQy9ROHhYVzdoQU10V1BLZXBkWHMxamNoU0h5WGpVVjdJTUcvbz0=";
//
const QString XM_SG_COMMIT_DATA_URL    = "bWJNaVZaUC9UQkRudjZYTCt3QjIxR1hrck9UN1FDWWFjbm5LN1RzanV6Q3NqREtmZ0NQL0RFZWYzR0hNZDZ4aW1KMGU1bzVBaWRJT0kzb3BTaWhKTjhlZllWV3E2OEZkT0hYS2pqSHkwSXU5QmRsUjVPdmdzUCtWNi9nY3BhK3c=";
const QString XM_COMMIT_URL            = "dEh6MVNYd2dSdDFnR2NyREl0RittVWxwQVh5UkpTeUpwRGRsallKKzBoaEV4cm9JTnRnYStZZTN0ZFQvTjVYRUlyK1JqOE1SMVhjPQ==";
//
const QString XM_SONG_TOPLIST_DATA_URL = "OGkxTFFnbXdQU2FUK041ZW4rdzhVZFoxckVLN3VCcXJCMDFQUUczUktybz0=";
const QString XM_SONG_TOPLIST_URL      = "bDAwMGJ6ZWxydWFvZjB6dmt1QUhCRlBNeW94S3QzdGtSRzNuMWM5aDlFYVJKMmRGL215N2cwNitwNDdwSFdLL3Zsa2hxS0hPcWpIRFVXZGI=";
//
const QString XM_PL_COMMIT_DATA_URL    = "U2p1a0dBbThPVDc1V0g4T09wZVF4SWpSaFRmQVRyVWlMaTR0SkdWNnVvTlBSSCt6cEJyRndha0tQdTRONlZ0UDI2aTJZQmtySDgzZ0RCVi9IM1JNMUwxendMMXd1S3BId0hzT0pFY3lFSkxYYllFOHZpTGdqbzRPa3VTTG5oQ3Y1TTgyMGc9PQ==";
const QString XM_PLAYLIST_DATA_URL     = "eGFxWVE3OFQzNUErdkZ2VmJmZWNGRm5MVGQ5OG54ZC9oWW96akFIK0tXWU1ML1lKUktEanMrNE11N3BQYkVNYTIxbTI3Zz09";
const QString XM_PLAYLIST_URL          = "dU0zeENFRlhjYkRnd0QvWmg4RTJjSkZqNEo5SFYwK0pwSXZKWG1PcWdWbGVLUUlVUW0yTkM2ZkdSRXEzemxNelZiMkdGZmF4eFFJPQ==";
const QString XM_PLAYLIST_A_DATA_URL   = "ZVlSN3dmUmw4UzNrbVduZGdITVNxRm5oZFBjaGxpWWFVQzVyT1dQRExya29OcGt5bW15MnlQRko3M09CS09qSThvUC80OXN0Q1JtUTZXZDcxZGNlQVhDNzBocysyVWFxQjVEV20zbElvTERHcGpRTnM4SmZOejYwM3BiZlRZV3M=";
const QString XM_PLAYLIST_A_URL        = "ZVpnUGt4NWZnUDRGQ0doWU1POEl5RDdWc0dpVlNtMWFzNlJFQldKVHFydUd2VGpqRDRkbERYcVFlY2N0MUk2MERvTUh2Y3FPNStIUXhYUHI=";
//
const QString XM_RCM_DATA_URL          = "b2dac3VCQ3U0aFdiUUZubEcyS0tzdE1OV045ZHZ3MDVvNTJiYlhQbEhwdUxiTFhwN2h3NnczUTFaNWhUTU1yUjlUZmFTRlVQditCTk5iZ3hzQ0tXOStiQytQNGxTOEJjcklUYUhRPT0=";
const QString XM_RCM_URL               = "cFptMnVnTUp5akV4QklFTUVhejlsSHhsVDlKWW1TYitWUVFmbThJZmJPdmk1ZU10bkQ5T0Zoc0wzVVUrUVhxVjQ2K3pZVVVHby84PQ==";
//
const QString XM_SUGGEST_URL           = "N3AxWkhmSndLSzB2d1RUc3BDTmo3dEhLMFRkOVhFVW82MmRtdGNzRWZFaEcvMGxCZUJaS0FJNW05SDA0YWt0Zw==";
//


class QNetworkRequest;
class QNetworkAccessManager;

/*! @brief The class to xiami query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadXMInterface
{
public:
    /*!
     * Make token query cookies string.
     */
    bool makeTokenQueryCookies(QString &tk, QString &tke);
    /*!
     * Make token query url string.
     */
    void makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariant &key, const QString &quality, bool all);
    /*!
     * Read tags(lrc) from query results.
     */
    void readFromMusicSongLrc(MusicObject::MusicSongInformation *info);

};

#endif // MUSICDOWNLOADXMINTERFACE_H
