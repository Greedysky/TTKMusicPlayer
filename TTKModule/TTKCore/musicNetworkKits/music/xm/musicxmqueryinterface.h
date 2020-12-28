#ifndef MUSICXMQUERYINTERFACE_H
#define MUSICXMQUERYINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

///cookie url
const QString XM_UA_URL                 = "TzVnZHRZRDZUWmxoeXA3aG5xai9Ib2JUZUdCTlhiZUVqWVR1QndjMU9WcGs0c3lUSGpoUHZZVzdnYWFpUUJ1NG1obktHQndrblVDUTdxNS9jcnZCY3kvNE9TRjlHai9mZHlLbU02VHdiOFpzZWZJa3hqcHpCaGpvUmJjTGJmelBjb3hmL2pzMmlVeEV3RFFoc2x3Szh4TFVXUnRkSlBQYlI3S3A1L3JJRkZFR1dYbTNDL1Z0aVRIVFZoU3pWVWtxMW5MSVhGbTFVemM9";
const QString XM_COOKIE_URL             = "clJiVUJlWUpiMWdpYjJRM004QW54WUw2OUQzeE10b2lXUHVQN3ZZdmY0OWZpZGhmbzMyeEtiR0NQa3NiWVdBOXNkR0djTXduZHFFeWZlYUd6WWFaYUVIUHYxRWx5eXZTNTNYZ2VpTmFnTHNtVXdLK3dkS09SeFRuaUNnPQ";
///base url
const QString XM_BASE_URL               = "Y3VIQlFKcUY3aXhLVTB4N25CWGJFK09YbldPZlptekRSNnN5T25pU3E1b3A3aVZlQk5DbGVjbm5NN010UEpsek0xRWtJc1RQSjBLOVRzSFprbzRyc2xUam56aWpKUmRwMHVuendYZkhETEZNYmk5UStMODk1SkxLdjlFRFR0K2JoazY4QXgrRCtwZE9jb1RORStEK3lEU0Y5Z2E3bkhVNDYvOTRLZz09";
const QString XM_ACS_BASE_URL           = "SHBBcmMwS21NSkk4SnM3L3pIdXZsSGpuSk8ySlUyZGFXWDFwNUR1Tkp2bzgwckRUemVZdWh3czF0bDA3R2NLM2c2ZzJ5S0hvbVVUemR6RjBkOUtUZXAzc3l0Sk5lMkgwV210U0dSejEzZWJER0R6UHlRd1BQZWhVc2I5STN6K0VQeVVZTXg5MTFEdk5NcUZrOHBoMDk2eUxFNWZXc3BaUHZ5R0NUUT09";
const QString XM_BASE_DATA_URL          = "c2VVR0g1YU44QkhlRnlHT3pkM1hvL0ZpMW0zMW40TGtCRWR1Ti9ta1lBL3AvU2hHcHFVMWlaK3FIaWhDY1JPVXhkL2pGc0ZFNEJQR2t6cFB5SXQ1VGw0V2JYR1hIcnNCdDhKQmxEeVU1V2lhclhHb0hDN3JPNTliNmRpU1crcVVIcHdyKzEyOHdqRWpMcThKOG8yeFNuWkVDMitaMEdSdVc5eC9sTG1leDdPZG42b0xYT1hsVFc0Wld1NzA0TERqRWk0L0R6cjJSd29iT3VwbC80bEhTajNUcTJrZGJ3bDIrV0NScGM2UzZ4U25LZUszZjk2ZjdzV3p0b3hUUkhVZzZCLzczNUdZNG05NGJ3NGVlRldQL3BnVnBGaExZZTc2VXRRSUNLMk0weHIrcTl2VWtjMWNmMWdWSXAyTVFMcEw5Q1JQV1BDM25aYVBCUnZtMTZRZkZsVWhQa0RrZE5iUWhGODk1YkFpdUpxcEdCM2NmTEpBQkl5U3hkRWZZZWJyVXJNV0ZCQ01MR2V6ZmJlMWhrVTV5T21mVmg3ak41ZG1DRkYzaXRhRmlWOHd1Q0hHSXdpQVpVWFUwN21tTnVKY1VQajhMZ3lZTHF4WUllR2NJMXAxaGZEY3dWTT0=";
///song url
const QString XM_SONG_SEARCH_URL        = "MDFXcWwxdmVuaVVqc0hQOG9BMDZpQ01ReGx4b0N0d3BsVkZEcEg4cnhaU0N0a1ZDVkh3L0hVUnFhazBhbUZqMGYwOFVoUT09";
const QString XM_SONG_SEARCH_DATA_URL   = "TWl1SGl6RzUzUndpNFlQTFdEWVdxQ0I4S21SZUtKOVd2RFRvQ3BXTGlubnFwcWRhazVabG1zdnI4MDNranY2OEd4ck94UGZpVVg4K3VOYi9UdHZCTGVZek1uTTdSY0pw";
const QString XM_SONG_LRC_DATA_URL      = "Vis5ZDJwQmFVSEN2RkR1VGRBaHNINTdnREFyd3dqZUM=";
const QString XM_SONG_LRC_URL           = "WUIweEdQZHRXSjhxVWo5VVd3ZjRDNkhib2hXdkw4bjBkRjJWUE1aZWZaLzF5emxpZ2xOeUx6V2tFbUEwdmpJTTl3SFEzZz09";
const QString XM_SONG_INFO_URL          = "RUdxWnkvbTNLN05Wb0FBOWtqcnMrWGh1c1FrcWdqQk9kVk9JemRHWWVCSC9TMzM2RUdTV1h6alVST3JhMGZIZ0psYjBhUT09";
const QString XM_SONG_INFO_DATA_URL     = "Vis5ZDJwQmFVSEN2RkR1VGRBaHNINTdnREFyd3dqZUM=";
///artist url
const QString XM_ARTIST_URL             = "aDJ2V2JmUWhPK2Y2VnhsSDBXT2ZGQkxuUDBHUFo4MWhWWDFIT1JRSWdjM3M1UVY2YWIvc0xBbDN1V2hlTXFGMDFFRTRTUT09";
const QString XM_ARTIST_DATA_URL        = "aEorVXFna1ZnaWdUUkF1ZVRaYi9vQ1JhcjZSU2RGaHg3UXRZdExIQk85d3BtY28wK2hKL2l0NkViZWhwaUhoUjB1Z0o3clZSRXl4K0dFaFVUci9GaDlQV3hhRitBeUpSSFUyZ2hRPT0=";
const QString XM_ARTIST_INFO_URL        = "N3FVa3VLTVV3YVUraXJmTnZvRVN4Y0FDSTJwaHhDVmF1ZVJneTJEaUdXdDR0RWFjYlQrWjhPUWVmYlQxR0RmeXhaV3BvQWFpby9BPQ==";
const QString XM_ARTIST_INFO_DATA_URL   = "TzJUK1MwS2pYcmM2dklmRCtqN1NzdmNwRU5SejAzSHdJS2RETlE9PQ==";
const QString XM_ARTIST_ALBUM_DATA_URL  = "VWNnTTQzQWdvQytVM2xJNU5OV00zOWVPT1l6ZmxNZUphdVlzM3hvYVNTa1VxZzc2WUF0UG85d2VDa3hYellxRU04dmd2OTNNRlZaSmR2aXVPdVZGNXYrZ09rWHBFdThS";
const QString XM_ARTIST_ALBUM_URL       = "S0JOVUZHOUVnaEpwY2NvUDFKZEdmaVEvenIrUGdDV29zS2M2N2xmZVBlNFFkZ3RwdUVoZDdZcHRSVnJ4Um10RExMRjZldz09";
const QString XM_ARTIST_MOVIE_URL       = "SXN1NTdnQURKRjN5cGw4Ynh6M3dBTlVOTWs0NS9zOWYycmF5QVN5TG5VZ2VyQ2F1SHBEMHVjS1BsNlBYN3lZZA==";
const QString XM_ARTIST_MOVIE_DATA_URL  = "Wi96bzFOSWV0OHgxMlNRSjU1ZktJQjhFSHA2eTlCaEhEMzRYZ09oVmZoRnJtS1R3bVhJUzVqNkR4dDlvb1lkaG8xWjRyWlRTcDVTT1dRMTg0dnlHNms5cUtrZzhzQmVHNlBNbU93PT0=";
const QString XM_ARTIST_SIMILAR_URL     = "QWpHN1JOb3pnMmVxam5QVzdIMTBaNCtlc2VYQjY3ekxIZHpNZmVXQTdxRFpUYkdiRXBscFBKZUJXaGxud2xPV0t6emdrRmZkT29MK1crQ2U=";
const QString XM_ARTIST_SIMILAR_DATA_URL= "a3MxRG5GaXNUUldITU5Rd1p6dHFJcDh3S3RDTEJwRmQzSG8vaVE4NlRBb05IU1kvYWtTYkgzNG8wc1dIK2xWcXB0bUZ3aGV6aDlvelJLLzNpTldkL1E9PQ==";
const QString XM_ARTIST_LIST_URL        = "UTVmTDdLdmJBMGlROWhGaFpCOE8yMS9HbmtwVHNoM0grNHRJVXdYdmNIWkFpUjJyb3l6V0NCQmI1dkNpVUxqYno0QTNTNlE3MmJGRTVaV21GVXpVamJ6VGpLU3poKzk5MEhudjErOHZqNzVMR0RkSQ==";
///album url
const QString XM_ALBUM_URL              = "aUxaMFJ6b3ZldFBvTXNpcFhOTmNvMmk4TXBlODNueHo0RWphSDh6Q2tBN3B6WXM3b0ZqZzN3Q1MvUzBUUXFud0NlU1Yxdz09";
const QString XM_ALBUM_DATA_URL         = "WnRoTWJTU3IwZ1ZIa05YcEJKTWF2Um53cVFHSDJRdUdnaTdicXc9PQ==";
///movie url
const QString XM_MOVIE_URL              = "M0NYaUw1MHAyc3cyQmRNblgrejVEZWhZZE40UVBvdFRUNEVIVnQvMmZBeXNPYW5sYXVPalVJT2E5a3BTajlCRA==";
const QString XM_MOVIE_DATA_URL         = "U0N6clNXVnN3bFQyQk9rTVMwWHF1ZUM1anJXVFE3b2w=";
///toplist url
const QString XM_TOPLIST_URL            = "bDAwMGJ6ZWxydWFvZjB6dmt1QUhCRlBNeW94S3QzdGtSRzNuMWM5aDlFYVJKMmRGL215N2cwNitwNDdwSFdLL3Zsa2hxS0hPcWpIRFVXZGI=";
const QString XM_TOPLIST_DATA_URL       = "OGkxTFFnbXdQU2FUK041ZW4rdzhVZFoxckVLN3VCcXJCMDFQUUczUktybz0=";
///comment url
const QString XM_COMMENT_URL            = "dEh6MVNYd2dSdDFnR2NyREl0RittVWxwQVh5UkpTeUpwRGRsallKKzBoaEV4cm9JTnRnYStZZTN0ZFQvTjVYRUlyK1JqOE1SMVhjPQ==";
const QString XM_COMMENT_SONG_URL       = "bWJNaVZaUC9UQkRudjZYTCt3QjIxR1hrck9UN1FDWWFjbm5LN1RzanV6Q3NqREtmZ0NQL0RFZWYzR0hNZDZ4aW1KMGU1bzVBaWRJT0kzb3BTaWhKTjhlZllWV3E2OEZkT0hYS2pqSHkwSXU5QmRsUjVPdmdzUCtWNi9nY3BhK3c=";
const QString XM_COMMENT_PLAYLIST_URL   = "U2p1a0dBbThPVDc1V0g4T09wZVF4SWpSaFRmQVRyVWlMaTR0SkdWNnVvTlBSSCt6cEJyRndha0tQdTRONlZ0UDI2aTJZQmtySDgzZ0RCVi9IM1JNMUwxendMMXd1S3BId0hzT0pFY3lFSkxYYllFOHZpTGdqbzRPa3VTTG5oQ3Y1TTgyMGc9PQ==";
///playlist url
const QString XM_PLAYLIST_URL           = "dU0zeENFRlhjYkRnd0QvWmg4RTJjSkZqNEo5SFYwK0pwSXZKWG1PcWdWbGVLUUlVUW0yTkM2ZkdSRXEzemxNelZiMkdGZmF4eFFJPQ==";
const QString XM_PLAYLIST_DATA_URL      = "eGFxWVE3OFQzNUErdkZ2VmJmZWNGRm5MVGQ5OG54ZC9oWW96akFIK0tXWU1ML1lKUktEanMrNE11N3BQYkVNYTIxbTI3Zz09";
const QString XM_PLAYLIST_INFO_URL      = "ZVpnUGt4NWZnUDRGQ0doWU1POEl5RDdWc0dpVlNtMWFzNlJFQldKVHFydUd2VGpqRDRkbERYcVFlY2N0MUk2MERvTUh2Y3FPNStIUXhYUHI=";
const QString XM_PLAYLIST_INFO_DATA_URL = "ZVlSN3dmUmw4UzNrbVduZGdITVNxRm5oZFBjaGxpWWFVQzVyT1dQRExya29OcGt5bW15MnlQRko3M09CS09qSThvUC80OXN0Q1JtUTZXZDcxZGNlQVhDNzBocysyVWFxQjVEV20zbElvTERHcGpRTnM4SmZOejYwM3BiZlRZV3M=";
///recommend url
const QString XM_RECOMMEND_URL          = "cFptMnVnTUp5akV4QklFTUVhejlsSHhsVDlKWW1TYitWUVFmbThJZmJPdmk1ZU10bkQ5T0Zoc0wzVVUrUVhxVjQ2K3pZVVVHby84PQ==";
const QString XM_RECOMMEND_DATA_URL     = "b2dac3VCQ3U0aFdiUUZubEcyS0tzdE1OV045ZHZ3MDVvNTJiYlhQbEhwdUxiTFhwN2h3NnczUTFaNWhUTU1yUjlUZmFTRlVQditCTk5iZ3hzQ0tXOStiQytQNGxTOEJjcklUYUhRPT0=";
///suggest url
const QString XM_SUGGEST_URL            = "ZXNnYTE4b0J4a0Q5QkZpSlMzUXR5TFFSUTBXVFZMQ1BrZFdnOENSbG9YcFFJdU1qc2RGSjQzQ01BeGJFZ05EOVJUN01WdlpuTkJBPQ==";
const QString XM_SUGGEST_DATA_URL       = "STUyQkVHUE1rRmhlL1BuLzlVZHk5TkJlZy9hdFQxaU8=";


class QNetworkRequest;
class QNetworkAccessManager;

/*! @brief The class to xiami query interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicXMQueryInterface
{
public:
    /*!
     * Make token query cookies string.
     */
    bool makeTokenQueryCookies(QString &tk, QString &tke);
    /*!
     * Make token query url string.
     */
    void makeTokenQueryUrl(QNetworkRequest *request, bool mode, const QString &query, const QString &type);

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

#endif // MUSICXMQUERYINTERFACE_H
