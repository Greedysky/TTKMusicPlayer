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

const QString FM_API_KEY      = "02646d3fb69a52ff072d47bf23cef8fd";
const QString FM_CHANNEL_URL  = "OFZkR1h6cGZ2OFZpdERYUFhhcnF3VWRBWTZmblhPK0RxOUFzQ2hFMGI5YVhiS2xaZ3U3V3UyN0FyU2VLaHJqMXZDTUZ2N202ZWxjPQ==";
const QString FM_SONG_URL     = "NVBxM3kzUVllaUtJU3Q3YUFrVm5JNU1CVXpvMTRGekpuZkxPanNMZVUrNHBYZEc4dmtNWFJhYmFtTEdsd2pnU3BnMG91c2VJdDdTZkpWaDZsYjVzam5vOXkwVzF0Z3RCcDBpVm5CQTljT3FIajNlQU95eW95a1hqS0MxVlY5Ui9kTDNvNDk5amZHZzIxbXVWMS9zRFZIL3lIeWs9";
const QString FM_LRC_URL      = "M2FqVHV6Z29kcUpNNEcvcHo3NmM5Rkc3S0FBVnFzYnRsdVJKMnFQSmd0NGJNYW1QYmpLWXg2UkhicVlRUnRkNVBmMUdwSXY0OVQ4T2xMd0gvbUFCOHc9PQ==";

/*! @brief The class of fm radio request base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractFMRadioRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractFMRadioRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAbstractFMRadioRequest(QObject *parent = nullptr);

    virtual ~MusicAbstractFMRadioRequest();

    /*!
     * Start to download data.
     * Subclass should implement this function.
     */
    virtual void startToDownload(const QString &data) = 0;

};

#endif // MUSICABSTRACTFMRADIOREQUEST_H
