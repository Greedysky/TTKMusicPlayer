#ifndef MUSICABSTRACTDOWNLOADIMAGEREQUEST_H
#define MUSICABSTRACTDOWNLOADIMAGEREQUEST_H

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

#include "musicabstractnetwork.h"

/*! @brief The class of the download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractDownloadImageRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractDownloadImageRequest)
public:
    /*!
     * Object constructor provide artist name and save local path.
     */
    MusicAbstractDownloadImageRequest(const QString &name, const QString &path, QObject *parent = nullptr);

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override final;

    /*!
     * Start to download data from net.
     * Subclass should implement this function.
     */
    virtual void startRequest() = 0;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadDataFinished();

protected:
    int m_index, m_counter;
    QString m_name, m_path;

};

#endif // MUSICABSTRACTDOWNLOADIMAGEREQUEST_H
