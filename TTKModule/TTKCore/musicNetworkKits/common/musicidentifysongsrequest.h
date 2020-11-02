#ifndef MUSICIDENTIFYSONGSREQUEST_H
#define MUSICIDENTIFYSONGSREQUEST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "musicglobal.h"
#include "musicabstractnetwork.h"

/*! @brief The class of the song identify data item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicSongIdentifyData
{
    QString m_singerName;
    QString m_songName;
}MusicSongIdentifyData;
TTK_DECLARE_LISTS(MusicSongIdentifyData)

/*! @brief The class of the song identify query request.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicIdentifySongsRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicIdentifySongsRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicIdentifySongsRequest(QObject *parent = nullptr);

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Get query song id keys.
     */
    bool getKey();
    /*!
     * To query song by input path.
     */
    void startToDownload(const QString &path);
    /*!
     * Get identify songs.
     */
    inline const MusicSongIdentifyDatas& getIdentifySongs() const { return m_songIdentifys; }

Q_SIGNALS:
    /*!
     * Get key data from net finished.
     */
    void getKeyFinished();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &data);

protected:
    MusicSongIdentifyDatas m_songIdentifys;
    QString m_accessKey, m_accessSecret;

};

#endif // MUSICIDENTIFYSONGSREQUEST_H
