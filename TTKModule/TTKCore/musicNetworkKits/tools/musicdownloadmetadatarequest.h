#ifndef MUSICDOWNLOADMETADATAREQUEST_H
#define MUSICDOWNLOADMETADATAREQUEST_H

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

#include "musicsongmeta.h"
#include "musicdownloaddatarequest.h"

/*! @brief The class of the download the type of data by custom tags.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadMetaDataRequest : public MusicDownloadDataRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadMetaDataRequest)
public:
    /*!
     * Object constructor provide download url save local path and download type.
     */
    MusicDownloadMetaDataRequest(const QString &url, const QString &path, TTK::Download type, QObject *parent = nullptr);

    /*!
     * Set custom tags.
     */
    void setSongMeta(MusicSongMeta &meta);
    /*!
     * Start to download data from net.
     */
    virtual void startRequest() override final;

Q_SIGNALS:
    /*!
     * Modify tag finished.
     */
    void finished();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Send download byte data from net.
     */
    void downLoadFinished(const QByteArray &bytes);

private:
    MusicSongMeta m_songMeta;

};

#endif // MUSICDOWNLOADMETADATAREQUEST_H
