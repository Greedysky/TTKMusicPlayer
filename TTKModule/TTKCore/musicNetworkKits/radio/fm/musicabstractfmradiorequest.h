#ifndef MUSICABSTRACTFMRADIOREQUEST_H
#define MUSICABSTRACTFMRADIOREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicabstractnetwork.h"

static const QString FM_UA_URL       = "ZUlYWDFBRkw4YXBraW94cTFYbFZVZjZ5VDFZRk9kSHkwamh1VXFZWVlGeG5OenduZDMyc2c0c0FKMm4rb0U2d3NQOVNKZ2lKd09qWVJwRkZXTXk5V3ArYXZWcjVRd3RxckU3ODhnPT0=";
static const QString FM_CHANNEL_URL  = "MUs2SnhKN1FOVUFNMzJrVGF3MzM3NVg1NjkxNVUzZUNxTzNZNHVzWi9yMHhvd2JidTRRb1BRb0NTWHU0akIzUzRFVHFudXMyRDFLZVVtTWdydThKVDROYjJ1UkVEdGJjcFhSZXJ6Y2REWHQyU0ZUWWVPZW8xQT09";
static const QString FM_PLAYLIST_URL = "MnZrS1UwZENac2FaRVhuQ01hZFpQczdaNDMrMXc5UDJGeEVnRlpOSm1xeGFtNjBlcFRndjRmRWlKbExKUmR0NUdhTkR1WmtaL3d3TkdicVdDWHFSMlRqSDNqdmZQUXJlQ2lHTUZzRXN3cHhMek9qaWcyQUhlNWl1ZXR6MFR4VWc0R1J5YTk0N1hNUT0=";
static const QString FM_LRC_URL      = "em1WeFVSZFRVSmtZYThPYUhsZzJqVExLVnhmQXVzZmFoQkJxRldxa0dvdFRHaVQ2V2FlK3NKbzdWek16RjVQMkt3eldrZz09";

class QNetworkRequest;

/*! @brief The namespace of the fm request interface.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicFMInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request);

}

/*! @brief The class of the fm radio request base.
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

    /*!
     * Start to download data from net.
     * Subclass should implement this function.
     */
    virtual void startRequest(const QString &data) = 0;

};

#endif // MUSICABSTRACTFMRADIOREQUEST_H
