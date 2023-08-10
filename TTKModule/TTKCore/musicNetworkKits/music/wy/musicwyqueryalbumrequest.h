#ifndef MUSICWYQUERYALBUMREQUEST_H
#define MUSICWYQUERYALBUMREQUEST_H

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

#include "musicwyqueryinterface.h"
#include "musicqueryalbumrequest.h"

/*! @brief The class of the wangyi query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWYQueryAlbumRequest : public MusicQueryAlbumRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWYQueryAlbumRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWYQueryAlbumRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by input data.
     */
    virtual void startToSearch(const QString &value) override final;
    /*!
     * Start to search data by given id.
     */
    virtual void startToSingleSearch(const QString &id) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Download single data from net finished.
     */
    void downLoadSingleFinished();

};

#endif // MUSICWYQUERYALBUMREQUEST_H
