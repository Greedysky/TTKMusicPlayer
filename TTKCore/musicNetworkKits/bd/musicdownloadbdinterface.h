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
const QString BD_ALBUM_URL         = "OUQ3VXh4N0dMckNHZGp5UGROTFo2OG1MaXlTSm1QcHUzWWdlTCtuMmY0TE90a3dEREZNWnNheFZtWHJpZzc0Tk1kWkdMS3Q5ZXdOSmRSc1hxaVU3eWNMdFo4U0p6WkdyYjFkUXBXOXZWSkZ0bWFtM254WEE0UTZidmRSZzhkRWE3ek95UEJ6cExxR0RnelNMRFk1U1BkL1NyQlNuWnQ3NldudUR4am9FVzd5OXJhbytadG1lMitpdU1WRnp4a3oy";
/////////////////////////////////////////////////////////////////////////////
const QString BD_SONG_SEARCH_URL   = "R01SUmphWmlqU1JBVWhydHJjM2gvZ1Y4WSs5MXVVMU45dlFtS1FDTlBQQ0Mzckc2UTNkRW8ySWtQSjZ0OVhyWE9jR2pzSWJ1a0ZzMVVHdUQ0eVV6RW1wZXBXWTIvc0dXcVp5dkFZUklQbW45TWk2b2RtMUV3eU1UTFRhMlcwOXM1RDZSZzQwKzczTDhZZE5ENXJySXBLUUtnZC9kczZTSFB0cXV5RHg2MjZLZjN2akJ6M3FYdXlhYVNTQzNvdmhMRUxzVnFqWUFQY044azFwQUVyRHRBN0FHT2xsOS9hU0gyYzdtankxSFIvdlVCMTlFRG4vL3NQN0JoSU1MTy9PWEpVY2VYbUVUWUwwSnhtWjg=";
const QString BD_SONG_ATTR_URL     = "Nmo3T3RwdUpyN045Zk1mSjV0RlZ4ekdWMEZUUjBmWHdMM1dJRkFRTi8zNXY1VHZKVnZ2T0p2cUUwekQxMmFxWG9hbzJoemY3eFBlNDFZOWNSbVA3OEU5WnhKUkZOVUIvV3lBNTZRbDF1dWNiTjZkV3NOTUM4emx2NlpXT1VjRlkrSHpvM2w4Q3pTdHlqN256a1JVeU1DREZmVjQxSTVTSmMvb0wwV29FSlNrV0dzak40ZCtBY0E9PQ==";
const QString BD_SONG_ATTR_PA_URL  = "RStnQ2VJUDdYaHVXRmRFS29tVVFPbVNRNmxZPQ==";
const QString BD_SONG_INFO_URL     = "bjFyb25RejVHSXg4cWN1NzNPRVJ0WlJTLzFEeTV5R0pJYWJhUHF1TWtDKzhBdzQ3VmNvcE9mSW5DOFdrUTlVZkcyTFI2ajVhMUhadk1tUUNLanB4eC9kMjYyU1podUZnMzJYNHV3SnRuOUtNa1B4bm5IL3QzMDYzbG13YWFzOTd3cW5kb1EwdHF4R2FnYVRYZWxWSUJab3hMdVBiejNTMjEwRkFmNFpwSm14VXAzOXkxL25nVU0vRVcwOVNHbU9IVnVoeWE5UjhIOHdjK1kxakdPTUQzMmhoQUlvPQ==";
const QString BD_SONG_FMINFO_URL   = "WFBDcWZMT1djUmlIMVF1bmVRNlhNUlJvaldaOWJSWWJDZzhiNnZ1Y3dua1hjVjlnblVTMU50TktyeGtLc2pTNjFWYzlGY0Q2TW5wM0dweGFqU1ZRNUc3VUdLT3ZRRVgrUnJWa1ZMWC81eTJDMVU5ZittSzhhdz09";
const QString BD_SONG_YYDOWN_URL   = "YTc1c0N1SVZCTXJlbEVIdUQ5U1lHR3Bta0ZSSm9xWk8wMTZqbTZ0dTB4QTUyRjFZ";
const QString BD_SONG_SSDOWN_URL   = "UkU5VWt3SjY4SURoUFpoSm5EeHdFOGI5RG9RVHBPcm1yc1ZSVDJmdmFKbCtEQlo0RERNRjVBV2NBMWxUcE1BL2RGN1ZWeWxUSFRZeEVaaFc=";
/////////////////////////////////////////////////////////////////////////////
const QString BD_ARTIST_URL        = "RHpvNUpacEpGNjFVV0dhN3JZWmEyWVpjS0hBMVZTeUVFT3FLd0ZyYi9PME5lZjdzL3l5V2Y1TjEvVldEL0dVNTRPdlhNejJIb1E2VVduWkhaRG51YnMwcXpYeW5pdXRzS1QzTm5QOUxyeDNWb2pjOWZ4WElHMU13VlpYWlZmS3lsNlBZQjJnVElkS0RIdVJLTU4ybklzVzVDZ1JnZ3dsbjFmVHBrVzlQODJaSDJtUGtHMHBJVTRvNDcwRzNzLzNIaGIrQ0xYNVlCTlZUYTNUakJZT2hKM2taMVk4MXpXRSs0R3NXSnc9PQ==";
/////////////////////////////////////////////////////////////////////////////
const QString BD_MV_INFO_URL       = "VU9kR3dUKzFhVUNPcnRHdEpMdXhWeHI3RS9KYUJLV3NGTk5qQ0MxRUdGaVBkMGQ5a055d0VLL2kvZXB6WXV4SUJXNXJjdDZMMGRUeWRkT3lOUTY5c3ZsMFpzZjBjL1R2a1NMOSswemxNdzFFQjBRN3VMcXUvZnhqN2o1WDhoTXR3M1RRc0o3QVFWTGxrVU8rQXBMRFhxdmdzaTZvSk1hV2lzRUxRM0QzRmRod2FSYWVJMERITi95bVVseTFuQ3JQZnBtVWtiS1hNUXp1TkZBZDJvYWNqMWkybTJ3PQ==";
const QString BD_MV_INFO_ATTR_URL  = "c0h2R2dkNk9JQ2wxZFU3cit1cHllaVNyQ1NpWWZ0U1RESVY0aGprQ2RqWnprbWVkbk1VQlJkZTVIU3VTRERpcEFIOEk4cHBZVE9FZGpjV3pDOGR6d3c9PQ==";
/////////////////////////////////////////////////////////////////////////////

class QNetworkAccessManager;

/*! @brief The class to baidu query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadBDInterface
{
public:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                    const QString &bit);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                    const QString &format, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicLLAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicPayAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADBDINTERFACE_H
