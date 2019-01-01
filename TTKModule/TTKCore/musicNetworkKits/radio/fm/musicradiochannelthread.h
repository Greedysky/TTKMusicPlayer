#ifndef MUSICRADIOCHANNELTHREAD_H
#define MUSICRADIOCHANNELTHREAD_H

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

#include "musicradiothreadabstract.h"

/*! @brief The class of the radio channel info item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicRadioChannelInfo
{
    QString m_id;
    QString m_name;
    QString m_coverUrl;
}MusicRadioChannelInfo;
TTK_DECLARE_LISTS(MusicRadioChannelInfo)

/*! @brief The class of music radio thread of song channel.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioChannelThread : public MusicRadioThreadAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRadioChannelThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRadioChannelThread(QObject *parent = nullptr, QNetworkCookieJar *cookie = nullptr);

    virtual ~MusicRadioChannelThread();

    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Get music channel.
     */
    inline const MusicRadioChannelInfos& getMusicChannel() const { return m_channels; }

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    MusicRadioChannelInfos m_channels;

};

#endif // MUSICRADIOCHANNELTHREAD_H
