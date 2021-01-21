#ifndef MUSICPAGINGREQUEST_H
#define MUSICPAGINGREQUEST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include "musicabstractnetwork.h"

/*! @brief The class to abstract download paing request.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicPagingRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPagingRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPagingRequest(QObject *parent = nullptr);

    virtual ~MusicPagingRequest();

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset);

    /*!
     * Return the each page max size.
     */
    inline int getPageSize() const { return m_pageSize; }
    /*!
     * Return the total number.
     */
    inline int getTotalSize() const { return m_totalSize; }
    /*!
     * Return the page index number.
     */
    inline int getPageIndex() const { return m_pageIndex; }

protected:
    int m_pageSize, m_totalSize, m_pageIndex;

};

#endif // MUSICPAGINGREQUEST_H
