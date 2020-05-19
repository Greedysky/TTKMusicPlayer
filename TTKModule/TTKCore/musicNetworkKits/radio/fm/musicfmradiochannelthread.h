#ifndef MUSICFMRADIOCHANNELTHREAD_H
#define MUSICFMRADIOCHANNELTHREAD_H

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

#include "musicfmradiothreadabstract.h"

/*! @brief The class of the fm radio channel info item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicFMRadioChannelInfo
{
    QString m_id;
    QString m_name;
    QString m_coverUrl;
}MusicFMRadioChannelInfo;
TTK_DECLARE_LISTS(MusicFMRadioChannelInfo)

/*! @brief The class of fm radio thread of song channel.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicFMRadioChannelThread : public MusicFMRadioThreadAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFMRadioChannelThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFMRadioChannelThread(QObject *parent = nullptr, QNetworkCookieJar *cookie = nullptr);

    virtual ~MusicFMRadioChannelThread();

    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Get music channel.
     */
    inline const MusicFMRadioChannelInfos& getMusicChannel() const { return m_channels; }

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    MusicFMRadioChannelInfos m_channels;

};

#endif // MUSICFMRADIOCHANNELTHREAD_H
