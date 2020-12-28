#ifndef MUSICABSTRACTFMRADIOREQUEST_H
#define MUSICABSTRACTFMRADIOREQUEST_H

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
#include "musicstringutils.h"
#include "musicabstractnetwork.h"

const QString FM_CHANNEL_URL  = "emFrU3NhdGU2YUJDeTZNdGQ0Snh2cmJBZ0lKdm5VNm5pb2tLNzNtTHRDcTVFVDV4RWRYN1pOcHlJa0l6VTVmbA==";
const QString FM_SONG_URL     = "RXUydVV6M21qbDNWc3BVT29idUF6VXlBalBrRU55eTY4bkxhOEx0ZTUweFE1TDlwenJYYmRTdTZiZVF4OElUUmFxcVNWbEhWR0FxTUhjOEVrZmE5QTE0R1c2UUFsYVIvbEFpcFBET1FMb3RsZ24zK2VsTHV2dDkvb1lwM0w0RUZoK0RqZWJSTlJLYz0=";
const QString FM_LRC_URL      = "TWNqdlhpeEtEVm1qbzN4TXNNZ2hYTU1TdGFLZ0I0bUNWQkZ1WlRHZ2UzWXAwdCtYNGk3bit3eW1tb1JCeGhBUTNEU2xTNEUzckVvPQ==";

/*! @brief The class of fm radio request base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicAbstractFMRadioRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractFMRadioRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAbstractFMRadioRequest(QObject *parent = nullptr, QNetworkCookieJar *cookie = nullptr);

    virtual ~MusicAbstractFMRadioRequest();

    /*!
     * Start to download data.
     * Subclass should implement this function.
     */
    virtual void startToDownload(const QString &data) = 0;

protected:
    QNetworkCookieJar *m_cookJar;

};

#endif // MUSICABSTRACTFMRADIOREQUEST_H
