#ifndef MUSICQUERYPLAYLISTREQUEST_H
#define MUSICQUERYPLAYLISTREQUEST_H

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

#include "musicabstractqueryrequest.h"

/*! @brief The class of the query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQueryPlaylistRequest : public MusicAbstractQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryPlaylistRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicQueryPlaylistRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by input value.
     */
    virtual void startToSearchByID(const QString &value) override;
    /*!
     * Start to query playlist info.
     * Subclass should implement this function.
     */
    virtual void startToQueryInfo(MusicResultDataItem &item) = 0;

Q_SIGNALS:
    /*!
     * Create the current playlist item.
     */
    void createPlaylistItem(const MusicResultDataItem &item);

};

#endif // MUSICQUERYPLAYLISTREQUEST_H
