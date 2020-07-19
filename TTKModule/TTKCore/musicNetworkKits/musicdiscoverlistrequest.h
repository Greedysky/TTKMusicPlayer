#ifndef MUSICDISCOVERLISTREQUEST_H
#define MUSICDISCOVERLISTREQUEST_H

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

#include "musicpagingrequest.h"

/*! @brief The class to download discover toplist request.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDiscoverListRequest : public MusicPagingRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDiscoverListRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDiscoverListRequest(QObject *parent = nullptr);

    /*!
     * Start to Search data from toplist.
     * Subclass should implement this function.
     */
    virtual void startToSearch() = 0;

    /*!
     * Get top list info.
     */
    inline const QString &getToplistInfo() const { return m_toplistInfo; }

protected:
    QString m_toplistInfo;

};

#endif // MUSICDISCOVERLISTREQUEST_H
