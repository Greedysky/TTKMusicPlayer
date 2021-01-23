#ifndef MUSICMGQUERYINTERFACE_H
#define MUSICMGQUERYINTERFACE_H

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
const QString MG_UA_URL                 = "bVlDb2p4N2ppaEs3b0RUV1dmQ2puZG9oRWNoZ2w5NmhJRG9DTkFLck00RFc4TzhCQnlmcGFwSjZWb3VkeHFXcUdWM3B0RTJnRzlVRlJFL0gxeS9RUU1VUVpMUVpycEVzNy9QUkVhNlc0ZXFpd1BrZzBmRXk5bDV6UXN2dnlNSTFBRDRSTzFvVkJFRVJpMXpMMHhDK0lOY0xzVXUvMFRBUmhGaUg2aWszRmRvPQ==";
const QString MG_REFERER_URL            = "NXkvTExySGxqaHhUQVZoSUJoL0o1Sm1RR2pSb1liSGt3SldHeUg2WEFrTT0=";
/////song url
const QString MG_SONG_SEARCH_URL        = "SmNOajNwODhXSFNWNWtsUS9pazFnT01tcDAwaG5USEw5SzdnUlhXQmpZRCsycER5SmNnMDNvZzF1TlJYWGQ2N1B6WFVuRjdna0V5SVBLdFBaRlluZDRKODdmV2xsWEFDTnloU0oyam9rQURPYkg0MnJMb1Nxd1V4dGNJPQ==";
const QString MG_SONG_LRC_URL           = "YW43M1ZqZ1B1WmQyTmJJQWxpS29WV0dlQUZjdTA0SFB0ZUk4dzBrV0xNMkJBNEhpcmNpYzBzZ3gwQU1hV3FidHhRNkoxaW0zN0o3eTVtVGo4RmptN1hYaEt6Nmc2SVNjTG5mS2xRPT0=";
const QString MG_SONG_PATH_URL          = "K0cyQ29QR0trTU1tK0tOZE1FOUtINkxUalZ4SzZLcTdLdDZhdTdhUy9iRkZEaExaY2IrQVpmc2V2VCtSUnVCQklkYVF1M000dlkyVlpnOVlXTlQ3VEt2Mm9tOXVINXJneWdQekRlWGxUeHpmd2pkZXg3UjB3NUhQNFVvT2NNYUUvdmVsSE1WM3pXSXJKVGU3c29CZlJLdXBEUXI4T1lEYQ==";
const QString MG_SONG_DETAIL_URL        = "T1NHS3hZZkxRZVVBeUUzZHVyWXVWUVRsUUFwais0NjVQQ0NBOU9qU2srKy9uRWZtdk1HRUlDVzg4TFprcWNiK1AyVDVLeEVRVjhQZ2tFT0phTDdwa1E9PQ==";
const QString MG_SONG_INFO_URL          = "eURzdlNMRU5WaWpQb3ZMRkZnWVVxdThzeHRnRDNFdGxvMHl4QUlsUU5HNkwxTFI0M0NDQzlxMUhtYW84RUJCamNoZHRyQm1MNTBWOGZIdTNZTVdxYUE9PQ==";
const QString MG_SONG_LIST_URL          = "UDhwWjBjKzZob090VVQ3dFlGNjFKVWVOL3JPeVdkSFM4T3hHSU5Hc3kxNkttbnd6SE1wQmZkNCtRZEFRSStja1VzdTQ3UGdNU0gxOEI2RWdSTVl1MC9GU00vVFVLOTdoQlJWbVJvYm03UC9QVEdiZQ==";
/////artist url
const QString MG_ARTIST_URL             = "VDc0UENHR2dKU2pVTjNLNExwaVIreUE2K293VFpXYjYxRDRrUytjeTFFa2NRUE9HMHVvS0puWHk5WVBjcHYyQ2t6blBjeHNLeEhaNDdncHNRMTJGT2wrcHIybEJEU2pmOW82b204KzkxWkttdXFQRWdPeWVvZWxQc255R2k3QWtwNGRpZFE9PQ==";
const QString MG_ARTIST_INFO_URL        = "ZEg1bmsxT1NVRy94djhGYzV6U2g2L2x4Qi9MVXRFSTF2R2Q5TWVzaG92ZzVmMXo4U3Vla0J4QVlJQ0ExMlcvV2ZOZHVoRWdMeEtnSXQ5eDBJZHJoYVpxQ2N3TzJCT0pna3ZxbUxRPT0=";
//const QString MG_ARTIST_ALBUM_URL       = "";
//const QString MG_ARTIST_MOVIE_URL       = "";
//const QString MG_ARTIST_SIMILAR_URL     = "";
//const QString MG_ARTIST_LIST_URL        = "";
/////album url
const QString MG_ALBUM_URL              = "dDAxVm9NZkt3OCtraHlZVFpKSTREL2ErajcxQ0hCL1pyd1JVaFNlY1VyWC8veWVCYXJUY2VTT1I4T1M2ZG9ESWNic1UwcDlkYXJzS1FLdnRzM2M3Z0dpVkcwUm14M0xhQjJFT2ExYzExUGprQlZzeGxoWWtRTit5akxNN1dUYXdJU0pZK1E9PQ==";
const QString MG_ALBUM_INFO_URL         = "dnE3SGQvdnI2UnBKKzhFZW9uQ1dMSlB6Y3FMRHV4TDR0RmFmbElCTlF4M0tOMlhqL2puKys2enBzaHBNdGxySndTOGZZa2J0aFhTYzVoUC9ZNVZFMDl0UDlIMWFHSXkyOWY0MTNnPT0=";
/////movie url
const QString MG_MOVIE_URL              = "b1dFQ0xMNVd5Q01BVzJLTDhxaW1nRlV4b251MXp6T3NvczBwR2MwdDRJM2EvdUcyaS94dy9mRG1LeGdIN3FYSTRpVlZhM0pFYmt5dlRURk1tUTRYOHc9PQ==";
/////toplist url
const QString MG_TOPLIST_URL            = "clo1YVpRdjBtUXB4ZHlSTE82Nk1jdDFaZGF6Zys0SVlGbFFoYVBWVkZhaDN3UjNORGRlUStjNVJkWHM9";
/////comment url
const QString MG_COMMENT_SONG_URL       = "VWQrTFNlanpZRzA5dmpPd081QjNuSDh2Tk5uU1R1TTI5Qll5UC8vZ0dQSXNNNWFtY1M3TmQ1TVg5RlYwV0F6SzVPTUU2dldWbGtzQkNtZHJCQThPRjBrRmRadDFhaW8xazJLTDBmcWkzcUVSQlhwYVZMYm1ZZz09";
const QString MG_COMMENT_PLAYLIST_URL   = "VWQrTFNlanpZRzA5dmpPd081QjNuSDh2Tk5uU1R1TTI5Qll5UC8vZ0dQSXNNNWFtY1M3TmQ1TVg5RlYwV0F6SzVPTUU2dldWbGtzQkNtZHJCQThPRjBrRmRadDFhaW8xazJLTDBmcWkzcUVSQlhwYVZMYm1ZZz09";
/////playlist url
const QString MG_PLAYLIST_URL           = "WEtJYk1tSWFWVms3THZFQnEvTzZWMmNpSnFWRDRyYTJEL3ZBcmtKM1MzRDBtZUp3QkVlY1BaSVF6amxGMEN3cVFJZlAxOEZVYm5kNGQwa0NOejV1TjBrMHdmQWpFS21VTlp0ekV0MW9ZNGRXOWZFb1NqN1hGUTIwdzFwbGErMVhGZTBoMFh6a1ptOGxyWC9qc1kzanBnPT0=";
const QString MG_PLAYLIST_INFO_URL      = "aHRJWGorSEs5aG9JTHoydnNncjlwbFEyd2E2eGdwMTVucEIwR3BVelFvT0M2SzFBQ0lNUmorbWFFbTE3TW1jcmFCdkN5bkE4UEp1M0oySlFFL2oraVpuRHdJYVFyckIxb1RiaUJtcEdWZTgvTmJqOG1Sa2hwMlRUbFAzb0JaMUtydkNqR3hQa2VDNlFQcXQ3K04vZURIR0NSUGM9";
const QString MG_PLAYLIST_DETAIL_URL    = "Z2thZ0ZhTjNyeHJHNHlKSEhMd0c5cktKY252UGNMVDl0WTI3dUlmc3h6Q2dVODVRQVJxdG56bFU1MEFNbTRpRzlHdGIxRTZ0NHFWZkV6QVVIeDFoRDY5UEhrY3dyT1hHQVczaUFRZmVXMzg9";
/////recommend url
//const QString MG_RECOMMEND_URL          = "U281QUUzc3V6dnRPeFRvckNoTEFIeDQxdlE2Y2pjMkJZbysxU1lYTVU0S0lKQm5XRkhFaDRyRmVWb00rOUYyZFcvSW1sWjExQnNMUTZjMVVUVnZTeVNrZERUNmlaSTNZSGZnRVRSSTVidlpyLzhDdjZ2ejM2S2U1Ylc4RnQrZzJHM2ozVVE9PQ==";
/////suggest url
const QString MG_SUGGEST_URL            = "ZU5LU2pxcTg4QmlLVVM3STBCSXVzak94NUZSVnZDSjVzeVc1eEhXN21hZCtPZWNuM0dqNVJCaHlBTWorMGJGK2ZIL3VQbVVXODIyNTlybjQ=";


class MusicResultsItem;

/*! @brief The class to migu query interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicMGQueryInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const bool status, int bitrate, const QString &suffix);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &key, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all);

    /*!
     * Get music time length.
     */
    void setTimeLength(MusicObject::MusicSongInformation *info);
};

#endif // MUSICMGQUERYINTERFACE_H
