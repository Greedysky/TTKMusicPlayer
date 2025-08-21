#ifndef MUSICDOWNLOADTHUNDERSKINREQUEST_H
#define MUSICDOWNLOADTHUNDERSKINREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicabstractdownloadskinrequest.h"

/*! @brief The class of the thunder skin XML config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicThunderSkinConfigManager : public TTKAbstractXml, public TTKAbstractReadInterface<MusicSkinRemoteGroupList>
{
public:
    /*!
     * Object constructor.
     */
    MusicThunderSkinConfigManager();

    /*!
     * Read datas from buffer.
     */
    virtual bool readBuffer(MusicSkinRemoteGroupList &items) override final;

};


/*! @brief The class of the download thunder skin background.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadThunderSkinRequest : public MusicAbstractDownloadSkinRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadThunderSkinRequest(QObject *parent = nullptr);

    /*!
     * Start to download skin data.
     */
    virtual void startToRequest() override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished(const QByteArray &bytes) override final;

};

#endif // MUSICDOWNLOADTHUNDERSKINREQUEST_H
