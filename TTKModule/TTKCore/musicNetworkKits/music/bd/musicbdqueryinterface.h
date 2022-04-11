#ifndef MUSICBDQUERYINTERFACE_H
#define MUSICBDQUERYINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include "musicglobaldefine.h"

///cookie url
const QString BD_UA_URL          = "QTllV1FYb3p4ZmZVNDArOW1uSGQrUGp2clowek55RmxrV2kzK3djN2dpb3J4YUlCbHpHSU9FNkJMREtSWDdVdWlVeVRHTnpTQUZ5TjE2RDlLb3lIeFNsMWVHMXBkWEsxSVpIRU85Ui8wVzhEbm80dUdQdHUxc0EzL1VhRHh5WmtxWlpJR1BqR3kzYkMycXVhQWljSmE3K1B3ZFd5dEdWMlE1UGhtWjBzOXJDcVRsN0lrL0d4V2ExdFJjQWtxMy9IbUN4ZkZ1NWhsY0ZqY3ZGcg==";
///base url
const QString BD_BASE_URL        = "VzJWczlXM2hMeCtTZzhLRFJvZWxUUTVmZUVBLzlMWmo=";
/////song url
const QString BD_SONG_PATH_URL   = "NU1oTy85RUFFdStaTm1xZTBWbldVNVN3WkxJRVNFVGZkYldCYzNaL1dHeURMRisrdUpmeE05WGp1RVYvNHkwVFdDTEgvOE5qak9ISjFoeURndTJ3KzRhNTVSSTB5ZjdINkxOWmVUdzZiaGE4Vk45K1lFcFB6QT09";
/////translation url
const QString BD_TRANSLATION_URL = "TXRkdVhlYnQzSEtZUmpJMVpDeHpaVG5DVzhId0NyVE42YXBPYkw2d25YeGJENDBONm9kSVZ2My95eHgvbVJSQjlDSE92clVkam85OG9uYjU=";
/////learn url
const QString BD_LEARN_URL       = "bFZyenh6K0hkblRMT2hxcmNNVjRHT3BMaW9Jc1VGSHFicDdEeFB3MzVqV3NTNk9pOGFVUnlXWHBZZXBZMWN5OHNJQjdrVEpnRGZnQUlUZE5OOEQ2WjhQUkxzN3oyRzNVenQrUTR3N080cWFlM3hPeVcxU3ZQZENtYTU5MDVvNy9zVWNadFZUenprMTRqOGpzQk43ZXFNZXVXaVQ3MXROc0gxWFNqTm0vcWxraXRaTFZIaEMxNDl1Q0NBRUdhVXBG";
const QString BD_LEARN_DATA_URL  = "Y3lUWmlxbUdMaUVtT3dKVUliRCtZRDRlWEFJPQ==";
const QString BD_LEARN_INFO_URL  = "S2srWTRQTVMwc0V1SkFxdU9VeFFSaHVtR0xCZ0lKS3NUNGV3dmFnR0VIZHI4aVVpMzBScUJ5WjNwVkNwb3Q2aTM3aWlxc3R1WlhsMzBTRGIxN0RqZ3QyM2NNa1JkZitRbDVJSWdtcTJ2NTE2cUxIT0Y3ZVJsRitJbjZEKzZYNjFlTVBZVVR2THpaaUQxc3FxNTV3WTgyWTJnVTBZWG5ETQ==";
///recommend url
const QString BD_RECOMMEND_URL   = "NDhiOGZ6dUJWNTBvN3R5OHNOQmkyQVVwOXdWbDNBOG14MmVXWVJxWlVXRkxuNUxxdzdYTEpUYVZRNVE9";


class QNetworkRequest;

namespace MusicBDInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request);

}

/*! @brief The class of baidu query interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBDQueryInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QString &key, int bitrate) const;
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all) const;

};

#endif // MUSICBDQUERYINTERFACE_H
