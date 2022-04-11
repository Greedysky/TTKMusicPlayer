#ifndef MUSICFMRADIOCHANNELREQUEST_H
#define MUSICFMRADIOCHANNELREQUEST_H

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

#include "musicabstractfmradiorequest.h"

/*! @brief The class of the fm radio channel info item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFMRadioChannelData
{
    QString m_id;
    QString m_name;
    QString m_coverUrl;
};
TTK_DECLARE_LIST(MusicFMRadioChannelData);

/*! @brief The class of fm radio request of song channel.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFMRadioChannelRequest : public MusicAbstractFMRadioRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFMRadioChannelRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFMRadioChannelRequest(QObject *parent = nullptr);

    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override final;

    /*!
     * Get music channel.
     */
    inline const MusicFMRadioChannelDataList& musicChannel() const { return m_channels; }

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

protected:
    MusicFMRadioChannelDataList m_channels;

};

#endif // MUSICFMRADIOCHANNELREQUEST_H
