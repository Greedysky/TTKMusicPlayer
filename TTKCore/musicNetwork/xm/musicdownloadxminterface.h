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
const QString XM_SONG_ALBUM_URL    = "SlEzVGhEWWhhMDJBaVJFUWJoejFJZHFQMUhDTktmWExESzh4dHY5ZlhhSStmU2dFV3FabzdHcG1tZk9Sa013aUtVb2dTQ1FuQWtGL1ZndGo=";
///////////////////////////////////////////////////////////////////////////
const QString XM_SEARCH_URL     = "N0JRblEwUURDaml2cjBlV3VtSTBhTUFvT1dIZVhoMmdsRm9BZGFHck1jOEpaWmVXNlZ1NjdxR2JoUG93OVZISjdZaVdPZURZMGVyeWJNV0lDVm9VS2l4bmg3TC9iOXFDVGJvdjI5TGZrelZoVVFOTA==";
const QString XM_SONG_PIC_URL   = "T1JqV3kvRksxSW5QUTdWeW9NOGY3L0ppazlDckM1RXYvRElGRlE9PQ==";

const QByteArray XM_BASE_URL    = "bUhKRWJpc0lkTi9sK2lpWU82YUhqdTZnYzZ6cmpJdkg=";
///////////////////////////////////////////////////////////////////////////
const QString XM_SONG_ARTIST_URL    = "alRtVWZ5elpjY3cwUzd1Z25NR04rb2krVDIrUkdsUjU1Y3IwM0UxYUFGT21GVVY3K2EwQkQ1cEhucWRFM00wRQ==";
const QString WY_SONG_QUERY_URL     = "RFJKcUxXdWt5dUhEQ056Rm9RaDhqZW5GSVFBZXZWcXU0bU5QdUZoNlBZY3V3RlFxN25QK1BRPT0=";
const QString WY_SONG_PLAYLIST_URL  = "YWc0dUhjNGdsTFY2VSsyMVBWWnYxMllDYlZrNFNmMEFBb1B5NlRDcGlwNU9tSDdYR010ODB6OHhocGhFQkx1KzJ4Z1R0eHRwVklzTCtkR0M=";
///////////////////////////////////////////////////////////////////////////

/*! @brief The class to xiami query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadXMInterface
{
public:
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADXMINTERFACE_H
