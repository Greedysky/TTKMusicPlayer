#ifndef MUSICABSTRACTDOWNLOADREQUEST_H
#define MUSICABSTRACTDOWNLOADREQUEST_H

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

#include <QFile>
#include <QTimer>
#include "musicabstractnetwork.h"

/*! @brief The class of the abstract download data.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractDownLoadRequest : public MusicAbstractNetwork
{
    Q_OBJECT
public:
    /*!
     * Object constructor provide download url save local path and download type.
     */
    MusicAbstractDownLoadRequest(const QString &url, const QString &path, TTK::Download type, QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractDownLoadRequest();

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override final;

    /*!
     * Start to request data from net.
     * Subclass should implement this function.
     */
    virtual void startToRequest() = 0;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Get download received and total data.
     */
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    /*!
     * Updata download speed due the user mod the net speed limited.
     */
    virtual void updateDownloadSpeed();

protected:
    /*!
     * Map the enum type to string.
     */
    QString mapCurrentQueryData() const;

    QFile *m_file;
    QString m_url, m_savePath;
    TTK::Download m_downloadType;
    qint64 m_hasReceived, m_currentReceived, m_totalSize;
    QTimer m_speedTimer;

};

#endif // MUSICABSTRACTDOWNLOADREQUEST_H
