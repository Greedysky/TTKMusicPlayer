#ifndef MUSICDOWNLOADWSINTERFACE_H
#define MUSICDOWNLOADWSINTERFACE_H

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
const QString WS_UA_URL_1          = "SXZ3dittUUxJTXgzckhUMHR5ZWpLcHpsbzNRTHAvMkNVMTdLdmYwQUlrbEg2d2lXRWpYeGlzTzVvdW1SRGIwVjhnVFYxZFZHaFpWd1BzV3I4VVlNT0U0UDU3ZlVUeXRHTkMra0hWK0FWV3BiRlZrMFBrNlI0c0hzdXUwNXB1dTFWaktxVWdpVnh5TUVTZUNCV2dpZjVZRnA3NGlzeklOZVFHR21EMFhKV2R5dVBsOS9YRGJrL2hLeTVVZWNEaDVjVkZ0WjdBPT0=";
///////////////////////////////////////////////////////////////////////////
const QString WS_SONG_SEARCH_URL   = "Si9xUjY1aTFoejlNUjNwSUNlUkpjWkJSRGxOZFZFMDdMQVYzWjFTa2lHYTkzanJtWlcwOUNVY1pwOTkxUjhKMDdZU3RXYnVvN3dNd0xPaGExVzVYS2dtdTBrSnl3L3FHMnh3azRxMkNpaWFadlFDckMvdWl6WGRYN1ZQbmpjYW9uRG1vS3lWUGtrNXJwN0pN";
const QString WS_SONG_URL          = "WnFBaUluSUhRL1FlTk1valhqMnZ6azhCUmNDano1MGk0WUNxU3AyU1JWMFZLZGpCVGtTeDlQVkxGVEczNk1TNlcrUWpzRzBNeFpyRnR5aW5RbmJIZ3N2dURkajVtdWhCU0k5Qk5IUm1YSG1hNVlhNVRWa0dEQTluL2hmd2ZOSzR6NFNSL2FRa1VLeFU3SlpvUGZRdDF0ZUhXVm0wSnZ5M1dXSUNvNUVTb3U1ZjNka3lRYWxZSDBTUU1Pdyt1RG03RGpGalcwdDVWcFMyV1pJWkVPb0ZGZXVZN2g5Zm50QWlwak5KVWkrNUFMZjA4aktWenl0RVVvWldsT05Yb1BjUTlPdjl1eDlaRFhicVVCQUlDN2Qxamc9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString WS_ARTIST_URL        = "cGNRYVg5bVNsZ1VHRkNrWTUzTmx6eWNyYzJMVUl3MmZEM2Q1dmhxcUtYb0k5d05XWDJrQ284OTVxV2treStCTzJWT0NYa3Z2OThESVVvRTNFVnArS3pwZHZxbUhkRzUzTHo2TEFEWDlMeEhENWlHR0NOTC80bHFQWldZcE5YbFQyYmVnOUFBdGxZVFdSVlFTVlVaWjgyWlBKdklFZ3JqbEcyZEFZdnRVaGgvT09mb0hHRjFISFo4ZGttN053RUxFMkxSa1Rha2wyMEJqdkVRWnpzRWt6YW5HaU9vZS9PbUpJdHdCRG85dTJDRUFsa09HaXJoZjBIaWl4b1ZJNXB4enJlM1BqVkdkWjA5djJyZ2Njd21FQUhKUFg2TitOMzNsOTQ4eGhsRXlvd3BaYVBrZEVKKzRIRnJyVDc3SE9zM1lJZm1ZTHZwZlBoVTB0ZXQvR080QzhyS2FNM3V4am45Y2dzNGFrY0hxRTYrOEUyUzhvTGYvNzZXUWh1Uis0Z1JTcjdNdlBpWjZLWXAvZlR0NTBjTldicjhSSWNKcUdTdVlsTUZxS3c9PQ==";
const QString WS_ARTIST_INFO_URL   = "T2Vza2dlUCtwRHdVQ3NRNkZaREI1VWhGRnNGYVczQWRUN05LTksyZC9FUWRlNTUrUXVHL0NBVGNRRFVqck1Pb2tycnNJM3lsbGhCdm55bTB5V2JDcUhhYy93UHV2TGlweEVCbzRpMFBGR0NySkFxd3MwaHRpWW9NKzBDKzlpUm9HdC9lemxYZWhhNEx1cDZhU09Lc0o2NURBeVdRcWhNZkZhTzloMFRQOHpKbE9IS1FKeGh6bVQ5cWgwV1JmaE85aTdVV2lVVjROa2t5YXp4Z0tIeGZGemZvb2dVRnlCOVkzN3pVYlE9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString WS_SG_COMMIT_URL     = "di9KbjV0REtIaU11ZC9BRWcyd1FiT0k5TWk0V1BDTUNrNTUxR2E1UTBKSk83aFpwZWJ2VUxhV2JjOE5YWVdTazdkd2ZSeklLQlBnZG5UZDNFbmFBQlRuV2ZiOUtrYjl2VG0rRWpERXhDQW9JZSszaDFENjZEUnlHWFp1d24xWEhGUTlIVDdYcEozSk5oamh5TEduT1E2aXMrdmdoaTg0WVpEazY1TFk5bGdHS3NpRGl6NWdYay9rMjN4Yz0=";
///////////////////////////////////////////////////////////////////////////
const QString WS_SONG_TOPLIST_URL  = "a1NQc1hKeEhCT2htT1N5MU1EbFlhY3p4WlNUVlV4VysxWU5ZeUs0NFFCTkU2ME0wRzVuWlFmREpyK3hWQ2pnY3VoVTJ5emc0dFFKc0pwRzhQbkF1Ui90TlNEazBRbUowVkpKZUkrZENmQXdieHBYWm9sSWlRZHA2VlJ4S2wvZHRpWUhVZHhKa09wZDQ3cXNEaWRkbW53T3FBRzZxUE9ZRG5WNSsvYlNBeVF5R3YrcEdrLyt6bHhpME1jVDl2N0V3YkRPOUR3SjFZKzVIMzBFdnFVQkxhYUxwL3dwcUhIR3dXWUFWN0JVV0Vsd0FkNVl5MVdjM1BHTTk5T1ZteFlwK1p5cUJjR0Y1b0Nsdnc5aWs3dVYwQzdyUjhQdHl0ZDRwSXJrODI4ZXduZ085K3BuNTVEQWY3YVVVYlJnWG9mSTFuenV0WW5wSFFRU1FRYUQvU25WVzBjdWRwWkYvbXhDM2ZSdGlVUXU1ZVc2ZFBVaEN1UElvZk5zV01ZaFhUREJ3d3RhK3ExQ1VSSVBJMGk5blFLdkZabkRiMHAyVDRscFc=";
const QString WS_SONG_TOPLIST_T_URL= "UjlEUE01dlRRbVdBWVlaM0ZJVGRlK21BS09LdEZjazdhY2ExZ3N5M1pqK3BQV3U1Sm9zK2Y1c0gxL0lEbnpGUUN0QlpLQVJieXNrSjVsckhRUTdNSFNkeEFnSGFud2xQdWJxT01BPT0=";
///////////////////////////////////////////////////////////////////////////
const QString WS_PLAYLIST_URL      = "NXBMdjg2NTIrTTQ2L1AwTy8xYnFoY01ySllBblFIeWkvaHV5Mk5uU2Y1TnhFaUJudkRzbUg5YWdHTngxMWhuSWZpbUg2VjJTb0I4L3ZPbHRzc2tjWlBEM2d0Yk5PRHdvSFNKcnRad1F6OTUyZHFUaW5LTlNuY1BFVytKZU1jN1U=";
const QString WS_PLAYLIST_ATTR_URL = "ZWRTbTVMM2F1d2J2bVc1TmJueTRTMGs5MmYzc0FIVXFuNUtTMGIzTENsSUpldFZ4Nk1OcHA0bmVMRnV1YTNsNWwxN09zZWliSkM3SkRlSG9wTS9DVU1iUTJTRkRQK1NOajgxMGl1b2xnS1pvQk5scWVwNCs3RVJmVUV3T0Y2T0V0UndyNlhqbXhWWkNBVjVQRXY0bldXOE8wSTkzYmJlSWIySGUzVE5Za3phbGhDWEZaRXEyVEdJM1A3VTVEMHdzRXJhb1loUU9NSmJZb2luU0Q5anQ0VjNqcDJ2MGQ0TWNPb3pjbXQ4MkFXOVdEdHJHYWRpOThZUlZIM3ZQTHNoVVVIeUJlUkFBTXplYVhTUTd2dEZXK0NwZGdCaWR1ZS9jK0NSY0lxVjVVSlZOMGViSXMzVnN5aWxGYnA1ejRGTmVielo2LzlQRTRhRUI5djZ3czgvSHphQXcwL2swSFJuT1c5bmFZbkgwTCtkZ2sxQnR6SFhjWWFBZjBZU3d2aU1pWXByVWNlUFcvZms9";
const QString WS_PLAYLIST_INFO_URL = "aE9kMU9kdENKKzBnYjVham5vNGRrVXFTMEdpNGxEK0dtUzd3VEUrVGJPNkdZRjBTcEtubGN0N0VPSEtnbURUTUxVRmIvQkhGWjdaRDMva24=";
///////////////////////////////////////////////////////////////////////////
const QString WS_RCM_URL           = "OXB0WWRUSm9PaXUyNm83bVFwYmJDMUpPQXZYdTlZRjVGVzFOMURTRG5uc08veThoOUduVTZnZm1SYitmM08ya0hUUGE5SlR4TjFWdWg3a29Ed1lHRmcrVW9jbXEyVGpMYnNMZ3lReWVxcktHcjdybw==";
///////////////////////////////////////////////////////////////////////////


/*! @brief The class to wusing query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadWSInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &queryType, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &queryType,
                                    const QString &quality, bool all);

};

#endif // MUSICDOWNLOADWSINTERFACE_H
