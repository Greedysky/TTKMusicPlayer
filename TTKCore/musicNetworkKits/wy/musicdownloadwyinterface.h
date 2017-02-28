#ifndef MUSICDOWNLOADWYINTERFACE_H
#define MUSICDOWNLOADWYINTERFACE_H

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
const QByteArray WY_BASE_URL       = "MVNVTXo4bW9WdHhXR1dXeTZmU3k5dmFOcGlua1VOMlE=";
const QByteArray WY_ENCRYPT_STRING = "3go8&$8*3*3h0k(2)2";
///////////////////////////////////////////////////////////////////////////
const QString WY_ALBUM_URL         = "WkErZ3FMK2hwekNCVlQxZmF6dmVxNlYzSzVEcmlxY0dYYXhwcEFVWDFYZ2pOdlFkbThJMk93PT0=";
///////////////////////////////////////////////////////////////////////////
const QString WY_SONG_SEARCH_URL   = "SFIwVWhHbEpXckhQT0VZUG5tUnBaYUNva2lUZXhSQnNuSkNBOGFMYlBhL2Z2YllBNXBWajFnPT0=";
const QString WY_SONG_QUERY_URL    = "UUs1NUMwUk9yeExmUFVCakUrS29NNm5OQ05lYUV3ZzJPS3NLVW9lZXhNaGwrd2Za";
const QString WY_SONG_URL          = "SkRzeTdTcHMvUVpaa01hcWdveklVK2FHbXRlRmRiTXYvYzRHdS9Hc01ZMllBaFlKNlp4RFBodHV0eERPd2haMEM3MjAzQT09";
const QString WY_SONG_MV_URL       = "cmVKem5MSmhzNk1KeUFRcXJFU2xrcHU3cnhwWHo2TVBNOFIwOC8rSldoSThUbmhQRVd0NEhTSnZGMytkVlFENUZTZ1lDekFLUkJvPQ==";
const QString WY_SONG_PATH_URL     = "SE5nYjBERmZZRzFyMHNhcVh6YVFaQmJWRDVIVWlkYTZoSGZIY2NpdkJ0dmJpTkY3WjZBblV3PT0=";
const QString WY_SONG_PIC_URL      = "eEZpOHNEanlmUmxITG9PRHI4aElxcDRqY1F5VjE1SmtMYm9lV3Z0eVpuakpBWFV4aFNZbDF3PT0=";
const QString WY_SONG_LRC_URL      = "TjNjL2ttNVR4cGluaEpwU1B2VzNucFhEWUdkRERkempFcThjWEIvMzMzMzVjaWVoR3ZNYXN5MHZsOE9sMTQ1TG1yeDNVQT09";
const QString WY_SONG_ALBUM_URL    = "WkErZ3FMK2hwekNCVlQxZmF6dmVxNlYzSzVEcmlxY0dYYXhwcEFVWDFYZ2pOdlFkbThJMk93PT0=";
const QString WY_SONG_DETAIL_URL   = "L3pCVjhOLzB5TUxpNS84czdTRTFndnZvMFkyTjhyKzFTK3lTNVlvQXBjT0xEb1FQZVpqdzZRaW5nNmM9";
///////////////////////////////////////////////////////////////////////////
const QString WY_ARTIST_URL        = "VURUWGM0c0NHamdldHFWcGwvZWlxS24rZ3Fpc1o0TEZmUDRDUDl3aWQvdklQN0VCeVpicmxRPT0=";
///////////////////////////////////////////////////////////////////////////
const QString WY_SONG_COMMIT_URL   = "TFFwV2I3RndydWRyYUFScm9RZjFCVlg0OW5OQy9LSlVFQTJWRUxraDhHNFNrVDFsWk5vaEtDeHNyTXNQOTRYNnVtQmVBTVVLUytBalJTZ3Y0ZTI3T2ZkSjJFdXVNbzZCWnNRZnRBbXBjSitGQUVpTkticGVLYWU1RWZ3aTAxbTFDdnZFYm9Kb0NSY2JiYmpU";
///////////////////////////////////////////////////////////////////////////
const QString WY_SONG_TOPLIST_URL  = "dlk1cmdmdXJvLzZ4cE5VRENlS2I4MUV6TDVoU1NGM0dJVVRKaHpzOGlweWZpTndiL1I2c3VmMTh4QW89";
///////////////////////////////////////////////////////////////////////////
const QString WY_PLAYLIST_URL      = "N1hwclZWdjNxY1M4RGxnTEpvbEZEY1ZOVVhkbVZrSVRXZVdJSk5jdnhpd3FkUDl4UlBxZjlmWHhvN0V5R3RjOWdNRjhVUFdSeWRRR1VzQTIwWStmVVZocG1BN0RkcExtTURqMFVZQktUc2pibndXSGVTMnB3clB0MjNjPQ==";
const QString WY_PLAYLIST_ATTR_URL = "NlRTUVV2OWo3U1psdG51VnIxbzl2WHVIZnV0MXkyaU1EbXgwSzBtQzlIbW9xYW43OGdBZEZWZW1vYmdFVXNhY3Y3cUNUdz09";
///////////////////////////////////////////////////////////////////////////

/*! @brief The class to wangyi query download interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadWYInterface
{
public:
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
    QString encryptedId(qlonglong id);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(const QString &string);
    /*!
     * Encrypted music info id.
     */

};

#endif // MUSICDOWNLOADWYINTERFACE_H
