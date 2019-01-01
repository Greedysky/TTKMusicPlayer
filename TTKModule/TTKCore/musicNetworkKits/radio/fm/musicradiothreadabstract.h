#ifndef MUSICRADIOTHREADABSTRACT_H
#define MUSICRADIOTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
#include "musicnetworkabstract.h"

const QString RADIO_CHANNEL_URL  = "Mlh6UmtFUnVBNUVhcE5xSGkzaXArYXBHOEw5TlNrUDcwQTV4UEs1QXRib085YThhYkVaam9nR2U2WmRvamU2NA==";
const QString RADIO_PLAYLIST_URL = "NlVIR2JEMVQvcGFqYWxQcTdpREJPQWhCUWRSejFXNlQ5RnkvTU1rQmtnTFF0b3M3QkhyYzRldzNSY3JTOXkxZENDUGdVZkhwbjVBaC83UDY=";
const QString RADIO_SONG_URL     = "T2JMRFREMFM2aG9hTzFPaENXV1RhTW5YS2UwT3hiYmdnRlUwNDRLdjNxVlV3SmJnN3JqWUU4aHhYeURlWWcrTlhYTHJmZU1yMVcxNE5IZVdsQWJCMTNVcjNhWEszU1hN";
const QString RADIO_LRC_URL      = "NmdYZm9tYkZjdHJOc1ZNNjIxSFdsc0NZc0xWa1RsM0dhMCtSaTdveGZPaz0=";

/*! @brief The class of music radio thread base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioThreadAbstract : public MusicNetworkAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRadioThreadAbstract)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRadioThreadAbstract(QObject *parent = nullptr, QNetworkCookieJar *cookie = nullptr);

    virtual ~MusicRadioThreadAbstract();

    /*!
     * Start to download data.
     * Subclass should implement this function.
     */
    virtual void startToDownload(const QString &data) = 0;

protected:
    QNetworkCookieJar *m_cookJar;

};

#endif // MUSICRADIOTHREADABSTRACT_H
