#ifndef MUSICTXDOWNLOADIMAGEREQUEST_H
#define MUSICTXDOWNLOADIMAGEREQUEST_H

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

#include "ttkabstractxml.h"
#include "musicabstractdownloadimagerequest.h"

/*! @brief The class of the tencent query info xml data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTXImageConfigManager : public TTKXmlDocument, public TTKAbstractReadInterface<QStringList>
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTXImageConfigManager)
public:
    /*!
     * Object constructor.
     */
    explicit MusicTXImageConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from buffer.
     */
    virtual bool readBuffer(QStringList &items) override final;

};


/*! @brief The class of the tencent download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTXDownloadBackgroundRequest : public MusicAbstractDownloadImageRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTXDownloadBackgroundRequest)
public:
    /*!
     * Object constructor provide artist name and save local path.
     */
    MusicTXDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent = nullptr);

    /*!
     * Start to download data from net.
     */
    virtual void startRequest() override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Download data from net finished.
     */
    void downLoadUrlFinished();

private:
    /*!
     * Download data from net.
     */
    void downLoadUrl(const QString &id);

};

#endif // MUSICTXDOWNLOADIMAGEREQUEST_H
