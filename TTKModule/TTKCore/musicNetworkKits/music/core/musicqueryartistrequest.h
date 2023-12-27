#ifndef MUSICQUERYARTISTREQUEST_H
#define MUSICQUERYARTISTREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

/*! @brief The class of the query artist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQueryArtistRequest : public MusicAbstractQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryArtistRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicQueryArtistRequest(QObject *parent = nullptr);

Q_SIGNALS:
    /*!
     * Create the current artist info item.
     */
    void createArtistItem(const MusicResultDataItem &item);

};

#endif // MUSICQUERYARTISTREQUEST_H
