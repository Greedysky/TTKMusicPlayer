#ifndef MUSICDOWNLOADWYINTERFACE_H
#define MUSICDOWNLOADWYINTERFACE_H

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
const QByteArray WY_BASE_URL       = "MVNVTXo4bW9WdHhXR1dXeTZmU3k5dmFOcGlua1VOMlE=";
const QByteArray WY_ENCRYPT_STRING = "3go8&$8*3*3h0k(2)2";
///////////////////////////////////////////////////////////////////////////
const QString WY_ALBUM_URL         = "WkErZ3FMK2hwekNCVlQxZmF6dmVxNlYzSzVEcmlxY0dYYXhwcEFVWDFYZ2pOdlFkbThJMk93PT0=";
///////////////////////////////////////////////////////////////////////////
const QString WY_SONG_SEARCH_URL   = "b2dKQmJVWmxNRXlqYU1saWFpbXV5Y1NkenZvN29UVFh4WjhScXpST0Nic3VDNDJkZ0QrWUxnPT0=";
const QString WY_SONG_QUERY_URL    = "UUs1NUMwUk9yeExmUFVCakUrS29NNm5OQ05lYUV3ZzJPS3NLVW9lZXhNaGwrd2Za";
const QString WY_SONG_URL          = "SkRzeTdTcHMvUVpaa01hcWdveklVK2FHbXRlRmRiTXYvYzRHdS9Hc01ZMllBaFlKNlp4RFBodHV0eERPd2haMEM3MjAzQT09";
const QString WY_SONG_MV_URL       = "cmVKem5MSmhzNk1KeUFRcXJFU2xrcHU3cnhwWHo2TVBNOFIwOC8rSldoSThUbmhQRVd0NEhTSnZGMytkVlFENUZTZ1lDekFLUkJvPQ==";
const QString WY_SONG_PATH_URL     = "SXZWVUpDL0FCM1pUODJsaDNGZjN4OFlVZk1JQzdXOXRpWVJTSTVEUEZJZkI3YUJOMUdLQ3lnPT0=";
const QString WY_SONG_PIC_URL      = "eEZpOHNEanlmUmxITG9PRHI4aElxcDRqY1F5VjE1SmtMYm9lV3Z0eVpuakpBWFV4aFNZbDF3PT0=";
const QString WY_SONG_LRC_URL      = "TjNjL2ttNVR4cGluaEpwU1B2VzNucFhEWUdkRERkempFcThjWEIvMzMzMzVjaWVoR3ZNYXN5MHZsOE9sMTQ1TG1yeDNVQT09";
const QString WY_SONG_ALBUM_URL    = "WkErZ3FMK2hwekNCVlQxZmF6dmVxNlYzSzVEcmlxY0dYYXhwcEFVWDFYZ2pOdlFkbThJMk93PT0=";
const QString WY_SONG_DETAIL_URL   = "L3pCVjhOLzB5TUxpNS84czdTRTFndnZvMFkyTjhyKzFTK3lTNVlvQXBjT0xEb1FQZVpqdzZRaW5nNmM9";
const QString WY_SONG_INFO_URL     = "N2NkQWg4V3o0NXYvMlFHamVOWVZ5Y2NnTDk2cTNFdFVROVNwTndJV2REYXM0K01Yd2NIT1pwaGcyTGJ0QVVsNHVxT0lTZHFKNTI2QnlWMnVBWFNDdWYrb09Waz0=";
///////////////////////////////////////////////////////////////////////////
const QString WY_ARTIST_URL        = "VURUWGM0c0NHamdldHFWcGwvZWlxS24rZ3Fpc1o0TEZmUDRDUDl3aWQvdklQN0VCeVpicmxRPT0=";
///////////////////////////////////////////////////////////////////////////
const QString WY_SG_COMMIT_URL     = "UjF0OUhmdnIxdXFWNEEwam5ZT0t6Q2EraGdGQzZqT01aYzN1bG1HQUdoOWxDbFhWTVpVWUk2NUdpSEFHcUo4UUlLL3BvWXJZSXJsTlU3M3JiN05sMUFldit5bzRXMFJNYjNjQ3pTOFp3TnBJSERSQjA3VDlycFRXaVpHV3BZOXFSU3NyVUFrZzRWeXRQY0lJ";
///////////////////////////////////////////////////////////////////////////
const QString WY_SONG_TOPLIST_URL  = "NlRTUVV2OWo3U1psdG51VnIxbzl2WHVIZnV0MXkyaU1EbXgwSzBtQzlIbW9xYW43OGdBZEZWZW1vYmdFVXNhY3Y3cUNUdz09";
///////////////////////////////////////////////////////////////////////////
const QString WY_PL_COMMIT_URL     = "ZXRML1piRWJpcS9NN2xRQXg2K2J3dGNUVnAxL3NSbkEwMG9Ta2VmZ2ZWYVI3MmNEL1gyYXJ6eXpld2Z0Rmp6S3ZwaHY0NVF6TjBLTVpNdjk0aFRuRmRSWVpNNm16Qy93ZHJkbjRFeGI0SmU0WUhod2tKU3lsVk0wNmZHeEV1c3ZZUDA1Y0pxZWd3UHZGUVA3";
const QString WY_PLAYLIST_URL      = "aDd0V0k1Y1lWcXBqZ3VMQWhaWmp6eFlUYXJmZmRyanFwcVFINmo2Qk1KNDVETW9TWE4zellMREllOG1NZ1YvWkZRM1drcE9PbmVpblRBOVcwZUhaRDA3WVoxYW9XK3lPckptL0VjaTd3UDd4ajlGUEhlZE5FK2x5Q3g0PQ==";
const QString WY_PLAYLIST_ATTR_URL = "NlRTUVV2OWo3U1psdG51VnIxbzl2WHVIZnV0MXkyaU1EbXgwSzBtQzlIbW9xYW43OGdBZEZWZW1vYmdFVXNhY3Y3cUNUdz09";
///////////////////////////////////////////////////////////////////////////


/*! @brief The class to wangyi query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadWYInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &id, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                    const QString &id, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key,
                                    const QString &quality, bool all);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(qlonglong id);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(const QString &string);

};

#endif // MUSICDOWNLOADWYINTERFACE_H
