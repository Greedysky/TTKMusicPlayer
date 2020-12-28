#ifndef MUSICFMRADIOSONGSREQUEST_H
#define MUSICFMRADIOSONGSREQUEST_H

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

#include "musicabstractfmradiorequest.h"

/*! @brief The class of fm radio request of song info.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicFMRadioSongsRequest : public MusicAbstractFMRadioRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFMRadioSongsRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFMRadioSongsRequest(QObject *parent = nullptr, QNetworkCookieJar *cookie = nullptr);

    virtual ~MusicFMRadioSongsRequest();

    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Get music song information.
     */
    inline const MusicObject::MusicSongInformation& getMusicSongInfo() const { return m_songInfo; }

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    QString m_cachedID;
    MusicObject::MusicSongInformation m_songInfo;

};

#endif // MUSICFMRADIOSONGSREQUEST_H
