#ifndef MUSICABSTRACTDOWNLOADREQUEST_H
#define MUSICABSTRACTDOWNLOADREQUEST_H

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

#include <QFile>
#include <QTimer>
#include "musicobject.h"
#include "musicabstractnetwork.h"

/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicAbstractDownLoadRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractDownLoadRequest)
public:
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    MusicAbstractDownLoadRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent = nullptr);

    virtual ~MusicAbstractDownLoadRequest();

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;

    /*!
     * Start to download data.
     * Subclass should implement this function.
     */
    virtual void startToDownload() = 0;

public Q_SLOTS:
    /*!
     * Get download received and total data.
     */
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    /*!
     * Download reply error.
     */
    virtual void replyError(QNetworkReply::NetworkError error) override;
#ifndef QT_NO_SSL
    /*!
     * Download ssl reply error.
     */
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) override;
#endif
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
    MusicObject::DownloadType m_downloadType;
    qint64 m_hasReceived, m_currentReceived, m_totalSize;
    QTimer m_speedTimer;
};

#endif // MUSICABSTRACTDOWNLOADREQUEST_H
