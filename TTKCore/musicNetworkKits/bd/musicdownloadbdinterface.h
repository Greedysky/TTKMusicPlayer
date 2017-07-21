#ifndef MUSICDOWNLOADBDINTERFACE_H
#define MUSICDOWNLOADBDINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

/////////////////////////////////////////////////////////////////////////////
const QString BD_ALBUM_URL          = "aldnSzNmNHJBaFlGSTlXN3FUSDhzWjlhZ3U1SmN2eEJYWUE0WlM5RWhrQ09lK3MvTzRTcHlWNklhZk4xY0JMTFhVbVhWMzE2anYxT01IVjd1V2g2Rmx0S3dUaUpRUEtTZGsyUkIwbk1VeU8rSldaU2NsMGVPWkdmQWFQQUJZRnUvNnNGVXV6elZCa0tBUkorUTlkaVdTN3Baa0UvZDJqUG5hNis4OW1CZTFzcldWUmNqNnl6SjZVem1JdkVwYk9z";
/////////////////////////////////////////////////////////////////////////////
const QString BD_SONG_SEARCH_URL    = "OGk2ZWNtaFVwM3pOZ05Rb2RHbTRvUmxxUFlDUzBjdG1ob3E1dnR2VlFrM1dJYjFyU1ZZaEwwNm9jcTBmZlJ0SGtQMzNhN0h1a1BYR0pHeW5mK2pQRkdtdlZjN1BQQXN6ck9McTRNckpNS2E5MkViVkhBZEdqclpJNU5MRlBNU1Zpd0JJb3p5VUJLanhhbVVUczAvVDVFZHFPVWMyVzRic3hEM3NXYVYrRXhNYmJFK0JvcFBZTlRPcEQ5bys0NUJFMjRHZDl3b3VsRG1udHRUUjNUR0FLMjU5amY1UWJ3TnorQ1pjSzZwK0NNcm83OHFZRS96cTk1YTNOK3gra3I3MA==";
const QString BD_SONG_ATTR_URL      = "MVRVZlZHQTkrc0R4QWxkMmFVdTZ1RGo2UllpMkNXRDZ6VkVieWRubzE1VzcrclEvZ3ZvblJVdGlWemZ6b3BGNjdSYTlKSnJCWHBDelNXaGk4d0YvWFhGbmdUWWlmWXVVN3N5QUxkelhGdHdXNHMzREtuOGM5aEdmSlB2WGFDbm5FTHV1ZU5GaGdjNU9wN3lrK2gxUHRMRE1xOXhFRUMzYzdpVkZGRWhBUnZNUlg4djg0K1NVcEE9PQ==";
const QString BD_SONG_ATTR_PA_URL   = "RStnQ2VJUDdYaHVXRmRFS29tVVFPbVNRNmxZPQ==";
const QString BD_SONG_INFO_URL      = "bjFyb25RejVHSXg4cWN1NzNPRVJ0WlJTLzFEeTV5R0pJYWJhUHF1TWtDKzhBdzQ3VmNvcE9mSW5DOFdrUTlVZkcyTFI2ajVhMUhadk1tUUNLanB4eC9kMjYyU1podUZnMzJYNHV3SnRuOUtNa1B4bm5IL3QzMDYzbG13YWFzOTd3cW5kb1EwdHF4R2FnYVRYZWxWSUJab3hMdVBiejNTMjEwRkFmNFpwSm14VXAzOXkxL25nVU0vRVcwOVNHbU9IVnVoeWE5UjhIOHdjK1kxakdPTUQzMmhoQUlvPQ==";
const QString BD_SONG_FMINFO_URL    = "WFBDcWZMT1djUmlIMVF1bmVRNlhNUlJvaldaOWJSWWJDZzhiNnZ1Y3dua1hjVjlnblVTMU50TktyeGtLc2pTNjFWYzlGY0Q2TW5wM0dweGFqU1ZRNUc3VUdLT3ZRRVgrUnJWa1ZMWC81eTJDMVU5ZittSzhhdz09";
const QString BD_SONG_YYDOWN_URL    = "YTc1c0N1SVZCTXJlbEVIdUQ5U1lHR3Bta0ZSSm9xWk8wMTZqbTZ0dTB4QTUyRjFZ";
const QString BD_SONG_SSDOWN_URL    = "UkU5VWt3SjY4SURoUFpoSm5EeHdFOGI5RG9RVHBPcm1yc1ZSVDJmdmFKbCtEQlo0RERNRjVBV2NBMWxUcE1BL2RGN1ZWeWxUSFRZeEVaaFc=";
/////////////////////////////////////////////////////////////////////////////
const QString BD_ARTIST_URL         = "Sk5aUlJ3TlpVeUV0c3NKUDVtTFdXSC91MWVuM09tWmVHRStaOXVrUjdqY3gxUmFlNjFjQ3lNSVVkamFOdUpKT0FYd1poN0owVUE2MkZCWWJmbEZvQ1poalAyRzMrTlNzaTg4ZWtjNDVuekhHYmlGcWFEd0JpTE9hM1dZOE9VK09MdlVGU21zWk9SZ2tzUUJTNzZoWnJ0TXBTSTZXMkpYTFZxc21ZT1RQUVQrNHhxalp5UzNlaVVJek9LelZsSG9XVXR3SWdTRU5sUVpOUE9yQWpQVExMWWdYVktXdklwTjJSbkhXUXc9PQ==";
/////////////////////////////////////////////////////////////////////////////
const QString BD_MV_INFO_URL        = "alhXNU05emZCL0p6aWNBdGxpbFY4MXN4VkZ5RHllMjZrNk1xcExBVFhnUTZxZjVuRDlvUnA5Y2pGcGNvNndUMVZ4NjhsUk5NRXBBWVp2MWxrbklXQlNobWRWSXh0LzZSdFF1dDYxRkRlTnpmc3ZRUlh5WlJUR1E2dGFBNDhSQlJVckhkWGw5ajh5NlRhRXJSckxIK2lHWTYvaHBLZy84c00yK0w5UlBoWGUwa05icG1yOFlXVFoyZDlKUDBoYy9OMzZKK0FKdUNZQktLRnFYM0J6azNaU2VkV2RvPQ==";
const QString BD_MV_INFO_ATTR_URL   = "c0h2R2dkNk9JQ2wxZFU3cit1cHllaVNyQ1NpWWZ0U1RESVY0aGprQ2RqWnprbWVkbk1VQlJkZTVIU3VTRERpcEFIOEk4cHBZVE9FZGpjV3pDOGR6d3c9PQ==";
/////////////////////////////////////////////////////////////////////////////
const QString BD_SG_LEAEN_PA_URL    = "Y3lUWmlxbUdMaUVtT3dKVUliRCtZRDRlWEFJPQ==";
const QString BD_SG_LEAEN_URL       = "bFZyenh6K0hkblRMT2hxcmNNVjRHT3BMaW9Jc1VGSHFicDdEeFB3MzVqV3NTNk9pOGFVUnlXWHBZZXBZMWN5OHNJQjdrVEpnRGZnQUlUZE5OOEQ2WjhQUkxzN3oyRzNVenQrUTR3N080cWFlM3hPeVcxU3ZQZENtYTU5MDVvNy9zVWNadFZUenprMTRqOGpzQk43ZXFNZXVXaVQ3MXROc0gxWFNqTm0vcWxraXRaTFZIaEMxNDl1Q0NBRUdhVXBG";
const QString BD_SG_LEAEN_INFO_URL  = "S2srWTRQTVMwc0V1SkFxdU9VeFFSaHVtR0xCZ0lKS3NUNGV3dmFnR0VIZHI4aVVpMzBScUJ5WjNwVkNwb3Q2aTM3aWlxc3R1WlhsMzBTRGIxN0RqZ3QyM2NNa1JkZitRbDVJSWdtcTJ2NTE2cUxIT0Y3ZVJsRitJbjZEKzZYNjFlTVBZVVR2THpaaUQxc3FxNTV3WTgyWTJnVTBZWG5ETQ==";
/////////////////////////////////////////////////////////////////////////////
const QString BD_SG_COMMIT_DATA_URL = "bUZVbEtEZ3JjcDMwakdDSUdlZ0EwS0xuVnFlNHV6aEVJT3ZNTCtWbU9KNGw0djExY3J5ai8wRk15eXRZdWhSOVA4d0t6ZmNUK2cyczFPMVRMdGpwVUNialpJNHA0QlM0";
const QString BD_SG_COMMIT_URL      = "MGJxK3E1QkhZdmluMExvcTJHbU1pbW9adVg5bnhQbFFHT3lCSERvS0MyaHIrcTl4QkovUVJDL1VaTVNsbGo4MURSWVdPL2R5VzNNanZNYzl6VDZSY0dSMVdvaXRJWEZDbmhtdkErQkVraU9yTG5IWXpOYUdzTHFyMVVnQ3prclBoQ3IzSEFMaW53Z0ZhVlBUMms0RXU5R0RXdjZ4V29STlpwWXluZkdraHNNPQ==";
/////////////////////////////////////////////////////////////////////////////
const QString BD_SONG_TOPLIST_URL   = "a2hLc2RibXBTbm5ZNzV5OHBPb1puMWMwWGY0ZVNJU0R2ZDdRYU9MS2ZmSllEc2ptS1F5RVBRPT0=";
/////////////////////////////////////////////////////////////////////////////
const QString BD_PLAYLIST_URL       = "OTg2WnpjQ3N2RnZZc2ZHUWJEdVA1aWRFaGhqVHN4OEdhQ3lJSUlHc1FNSjcrYkU0OEpjWHRZVi91VXBrb2hzQ21sRkJaZlZHVVFpT3ZtNGdQbkZzUXc9PQ==";
const QString BD_PLAYLIST_ATTR_URL  = "NXJsZ3ArR0JHeXFJYmdWUi9sOVNTaHlxbEpuekJYSVFrVkloY0U3d2VtU1VKOFZqcjlQTVNkMkJLbzJDSUxBb0ZWM0laZ0lVZHh1Y2RHUWRxVU5rS2hNMEpFM08yK0hxZXlYZGNlK0hpcys2RkVKRm94Uk5nUXlEVGo4QjlwQnFKQ0JmV0hGenBTTERoRG4xUGc0Z3I5OWtWMEhLMVh0RzBoeTdLQT09";
///////////////////////////////////////////////////////////////////////////


/*! @brief The class to baidu query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadBDInterface
{
public:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, const QString &bit);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, const QString &format,
                                    const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicLLAttribute(MusicObject::MusicSongInfomation *info);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicPayAttribute(MusicObject::MusicSongInfomation *info);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADBDINTERFACE_H
