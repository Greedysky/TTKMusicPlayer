#ifndef MUSICKWDOWNLOADIMAGEREQUEST_H
#define MUSICKWDOWNLOADIMAGEREQUEST_H

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

#include "musicdownloadimagerequest.h"
#include "musicabstractdownloadrequest.h"

/*! @brief The class of kuwo download art cover image.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWDownLoadCoverRequest : public MusicAbstractDownLoadRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKWDownLoadCoverRequest)
public:
    /*!
     * Object contsructor provide download url save local path and download type.
     */
    MusicKWDownLoadCoverRequest(const QString &url, const QString &path, QObject *parent = nullptr);

    /*!
     * Start to download data.
     */
    virtual void startToDownload() override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged();

};


/*! @brief The class of kuwo download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWDownloadBackgroundRequest : public MusicDownloadImageRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKWDownloadBackgroundRequest)
public:
    /*!
     * Object contsructor provide artist name and save local path.
     */
    MusicKWDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent = nullptr);

    /*!
     * Start to download artist picture from net.
     */
    virtual void startToDownload() override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

};

#endif // MUSICKWDOWNLOADIMAGEREQUEST_H
