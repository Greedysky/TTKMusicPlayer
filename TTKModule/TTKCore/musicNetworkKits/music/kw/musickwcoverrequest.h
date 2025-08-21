#ifndef MUSICKWCOVERREQUEST_H
#define MUSICKWCOVERREQUEST_H

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

#include "musiccoverrequest.h"
#include "musicabstractdownloadrequest.h"

/*! @brief The class of the kuwo cover source data download request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWCoverSourceRequest : public MusicCoverRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicKWCoverSourceRequest(QObject *parent = nullptr);

    /*!
     * Start to download data.
     */
    virtual void startToRequest(const QString &url) override final;

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadRawDataFinished(const QByteArray &bytes);

private:
    bool m_decodeUrl;

};


/*! @brief The class of the kuwo cover data download request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWDownLoadCoverRequest : public MusicAbstractDownLoadRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor provide download url save local path and download type.
     */
    MusicKWDownLoadCoverRequest(const QString &url, const QString &path, QObject *parent = nullptr);

    /*!
     * Start to download text data.
     */
    virtual void startToRequest() override final;

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

};

#endif // MUSICKWCOVERREQUEST_H
