#ifndef MUSICDOWNLOADKWINTERFACE_H
#define MUSICDOWNLOADKWINTERFACE_H

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

/////////////////////////////////////////////////////////////////////////////
const QString KW_ALBUM_URL         = "T0RHWW1RYlRFWi9MdDNRSjh6UXhIUHFqYjR3ZDdHK3VLbTRxUnRXV1RpaG1VWWpYb0hmY2dxaWJ6aWlETThmN2ltYVNDSVRkOXNHZVRmNXJYcDRXM3UvckFEVXJLR3JQbmZmMjNDR1ZxT1ZPbjgzNXV4bWdyM0J2ZTNvNFViVGFTcDdOWjg1NytON3AzcU9Kbjc4YWJNRTFYaEdsMDJJa001eWFjS1ZhcC9oeVNEKzhRYnNkTmFiMml6VDc1ZXJ4bzNScVh5Z0dWR1E9";
const QString KW_ALBUM_COVER_URL   = "bnkrUTZjQmxjcFVtVXZzR1M5c3hBN25MbWx3aHZpbE50aHNQVkRZQ0dKdXdLZUh1TTA5aWJnPT0=";
/////////////////////////////////////////////////////////////////////////////
const QString KW_SONG_SEARCH_URL   = "ZENVRUl0SzZ2Z0lBZjZpNmtzdmE3QlNZc01ZbUtJM3hiM3J6RUJ6Y3lTQmNnMWpwMkJlU2R6RFFaSWhVRm5TZmx5WUhzMWcrK3Q3Umd6OWhWK3J2bWNPdHNxYzk4MEtZeTE4bDA3TitEcCtUejVUYU5Mc1lrMkFYNVlrdlpCQkowWU9YRjUxeTM5UkxvNjA1M2J5T2pqbzBZYm1rVElmeUpnblk5WloxcGFnTkhneUdwUDFXVlYrVzFZY2lxWDI2aEx4TGduTWR5eUNHNjQyT0Y0UnRqTjdDbjIzd2Z3N3EyOVA2OVBabVJBS042MnBJ";
const QString KW_SONG_INFO_URL     = "dWdMdkJSZDVlTkFmZmVCQ3FJL1MrV2l0TzNmK2Y4cUVyNXVlUTVHbkdtLzdyNW8xVXhqeklkN0E5N1liVDZZMXZ1ZWhkV2pYeFRETHdPZVBRNVQxbnp6MWIyTT0=";
const QString KW_SONG_ATTR_URL     = "TzlINnJWWWVrVHowRkZhV1NrV2R6ZGh5b2YraFJmNkV6VUhxSlBPMExXVWkxSWt4aVdZNFIwaG9EK200cFdpSzZSYUhLQlA5T0RTQ2l1M1hxNmxjQlU3YkJoZ0JtbkFXbFFlL2JhTVp0WGgxTHdiT3F5MVBDTEowUi9HMXU2R2pqVXVidVE9PQ==";
const QString KW_SONG_ATTRS_URL    = "MjlwQmQxUlFrazRrVXlrR29BSEtCaDNaNnpRSGJPdlA4RDE4K3Q4clJqZFpkT2d3d2VLQ3ViSmdnUHRzWThLeVFDekZBME5NK1V1amRsYW5ueWNyQmdUUE5LcFpYWnlYcjlJMUw1dkJvM3BweDRabC9yR0tDbFQ4a2c0PQ==";
/////////////////////////////////////////////////////////////////////////////
const QString KW_ARTIST_URL        = "YnB5eU5EYWZlV2ROblZIWllJbVJzK0ppMVJqMGxiNVcwM0RaQzVqWE9xdURua0tJWng4OEVaSit1ck5VeGh4SkZ1OGN0SHRaQWNNQS9URXRCM0FVdlZOWDkwcDgzOXZ1Q0JIcGlMMEdyZGlWTUlITE1pZmJOY1YyNmFwS2o5NE1NM3ZDY25RRERlLzEzb1U3NFpKV2lYSGhpOFNINTdDd1ZZWHV4bkUzL0JXY2ZHSkpPYitEazY2QzhiRT0=";
const QString KW_ARTIST_INFO_URL   = "QW1leS9malJmT2gvWE41OURPbUREVGE5M2Q5ZzlpdUFPWWdreFVaSm0va2p4QlcvblZwdVVJaElwMUZhN1VvbHAvMytVYW44NmNFczl1UjM=";
/////////////////////////////////////////////////////////////////////////////
const QString KW_MV_ATTR_URL       = "OUptM3B0M3I1SFNvWmZkaE1KNHVPRUhrUkVDa1BPOUpWdmxIcVBVeVhvcHNZR1FEbG1RdjhscjBnVjA9";
///////////////////////////////////////////////////////////////////////////
const QString KW_SG_COMMIT_URL     = "R29Ob2xrOUcxbXhpMlF6YmRzZ2kxZVFrRmJPcEdzbk0weUNQazNGUEFXSjl4QXdhWU1kUXdNQ3ExOWxGRE5uOERTblpCNk9pZWdnVmRLTHJkK25GckxxbzhpYmh3UHVTbi9DK1RoYm5qaVdZV2o4MXo2cXBwL1ZLTFprY0dyZENDUDhiT0E9PQ==";
///////////////////////////////////////////////////////////////////////////
const QString KW_SONG_TOPLIST_URL  = "UjQydzFlazUwM1BRNm1DbXhlcHZjR3RpRzY0QWV2Sm93dmQ2d1JuQVFLbmtHSmFpakk1TGcvUG1iYVYzc2k1VTJBNlgvYWxUK1BZVnVnWUpZSEY4L0xSVkQveDdoMnI5";
///////////////////////////////////////////////////////////////////////////
const QString KW_PL_COMMIT_URL     = "b09RdlBRdzh4bjB4QXZCbXlXcE5ENEpUMzhEUGY5cVNCUVAwVUJucjg4YS9tWXhsZ2dINmZDOXFhOUlJVmVOaWVBNWlsaE9JRy9UalllVit2dk1IZG5ST1dhUGFNSXg0ZmVxdlpzUEZINUhXdzJML1hDOVhqYjQ2Y2Raek5oRGNDaGx3Wmc9PQ==";
const QString KW_PLAYLIST_URL      = "OTc4ZUwvTFdHbW0rekNzWGx6aHR1QXg5TnhKRUdmQ2drYlBpdWFRVjhQUkJVN0NaMDl6NmJFZTY5Yy91eHpWWVh3VlFXYzA4clZDRjZrQWdWSExtRnhqNFRZY0xzNnc1R0hhRStpeFQ4QzlKNUk1a1JrdlJtcFU0YkpvUlpSNGUzZFNiR253azVxUlArMEVpS081RlZPMlZLZEE9";
const QString KW_PLAYLIST_ATTR_URL = "UzZ0NDRTM2s1Mm5OUTZvZDdTRUlCZ0hsVjhZbGh3aWZYNWpLcnQ5WC95VWVTSmFZZXNkU242N3Fsc0haT1l4cUxXRUdjOWwrd2c4M2FDUk9hWXJGVldWVThzcG16UC9DT1lvTnlYT2VqZVlyaFhUY1FsTlJFVE9OQ2NOdTQ5RCt4cFY0SS8rV2NRUTdMUlE5SU85NElXNVlhamF4ajdPeGNteWZPQ1loaXVVPQ==";
///////////////////////////////////////////////////////////////////////////


/*! @brief The class to kuwo query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadKWInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &suffix,
                                    const QString &format, const QString &id, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QString &suffix,
                                        const QString &format, const QString &id, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                    const QString &format, const QString &quality, bool all);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                    const QVariantList &format, const QString &quality, bool all);
    /*!
     * Read tags(lrc\smallpic) from query results.
     */
    void readFromMusicSongPic(MusicObject::MusicSongInformation *info, const QString &id);

};

#endif // MUSICDOWNLOADKWINTERFACE_H
