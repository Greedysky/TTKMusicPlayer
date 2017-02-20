#ifndef MUSICDOWNLOADXMINTERFACE_H
#define MUSICDOWNLOADXMINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

///////////////////////////////////////////////////////////////////////////
const QByteArray XM_BASE_URL         = "bUhKRWJpc0lkTi9sK2lpWU82YUhqdTZnYzZ6cmpJdkg=";
///////////////////////////////////////////////////////////////////////////
const QString XM_ALBUM_URL           = "SlEzVGhEWWhhMDJBaVJFUWJoejFJZHFQMUhDTktmWExESzh4dHY5ZlhhSStmU2dFV3FabzdHcG1tZk9Sa013aUtVb2dTQ1FuQWtGL1ZndGo=";
const QString XM_ALBUM_LOST_URL      = "YXBmU1lkZGg5cUJFSHFwNWttcmxQZnhjdjlOUEdoc1VkZHdDOTQ5a1kzSkJ6NDFpL1R6cVFJMTlyOFZSZk9YOHI1Z3NKQkdEYisrL09oWVNWdUtzVW56RHZ3V1VqcjZwWnFWbGVOdDhaVGxuWlNjek13UFJhWjlOSFlVPQ==";
///////////////////////////////////////////////////////////////////////////
const QString XM_SONG_SEARCH_URL     = "ZU1naWg2S1AyZWRCNnJJeUdJVGI1M2tSS3cyVzRWR3UzcGEwNUEycWxuanhqTXNrUVBUeUJIRVowbDRLSGRQbG52eGJjWHZpU2RIcnArS25xZ2lWV0xWUEI5SGRRa2swSXhQbVVma1paRVZMSWVESQ==";
const QString XM_SONG_PIC_URL        = "T1JqV3kvRksxSW5QUTdWeW9NOGY3L0ppazlDckM1RXYvRElGRlE9PQ==";
const QString XM_SONG_LOST_URL       = "TFo4N2ZkMTFhRm1YMm1wWU1VYnZXYXA2NlEvaDJZOUpqMVB5MVVscmNycWJ5b2NTczY3YVF0SFdzanJHNy9QbksyblJBVGdJbkFpWDg5OEkvaFJJYUE9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString XM_ARTIST_URL          = "alRtVWZ5elpjY3cwUzd1Z25NR04rb2krVDIrUkdsUjU1Y3IwM0UxYUFGT21GVVY3K2EwQkQ1cEhucWRFM00wRQ==";
const QString XM_ARTIST_QUERY_URL    = "RFJKcUxXdWt5dUhEQ056Rm9RaDhqZW5GSVFBZXZWcXU0bU5QdUZoNlBZY3V3RlFxN25QK1BRPT0=";
const QString XM_ARTIST_PLAYLIST_URL = "YWc0dUhjNGdsTFY2VSsyMVBWWnYxMllDYlZrNFNmMEFBb1B5NlRDcGlwNU9tSDdYR010ODB6OHhocGhFQkx1KzJ4Z1R0eHRwVklzTCtkR0M=";
///////////////////////////////////////////////////////////////////////////
const QString XM_MV_ATTR_URL         = "NDY3SUJhdXhRUHA4bUVMUmdXSGYyeXpTcmkzcngwYXpRMFZ2UlE9PQ==";
///////////////////////////////////////////////////////////////////////////

class QNetworkAccessManager;

/*! @brief The class to xiami query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadXMInterface
{
public:
    MusicObject::MusicSongInfomation startLostSong(QNetworkAccessManager *manager, const QString &song);
    /*!
     * Start to Search lost song.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADXMINTERFACE_H
