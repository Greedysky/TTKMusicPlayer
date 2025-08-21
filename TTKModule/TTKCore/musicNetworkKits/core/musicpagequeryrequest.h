#ifndef MUSICPAGEQUERYREQUEST_H
#define MUSICPAGEQUERYREQUEST_H

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

#include "musicabstractnetwork.h"

#define SONG_PAGE_SIZE 30
#define COMMENT_PAGE_SIZE 20
#define TOPLIST_PAGE_SIZE 30
#define ARTIST_PAGE_SIZE 50
#define ARTIST_ATTR_PAGE_SIZE 12
#define ARTIST_LIST_PAGE_SIZE 100
#define MAX_PAGE_SIZE 1000

/*! @brief The class of the abstract download page query request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPageQueryRequest : public MusicAbstractNetwork
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPageQueryRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by offset page.
     */
    virtual void startToPage(int offset);

    /*!
     * Return the page total size.
     */
    int pageTotalSize() const noexcept;

    /*!
     * Return the each page max size.
     */
    inline int pageSize() const noexcept { return m_pageSize; }
    /*!
     * Return the total number.
     */
    inline int totalSize() const noexcept { return m_totalSize; }
    /*!
     * Return the page index number.
     */
    inline int pageIndex() const noexcept { return m_pageIndex; }
    /*!
     * Return the page index valid or not.
     */
    inline bool pageValid() const noexcept { return pageIndex() + 1 < pageTotalSize(); }

protected:
    int m_pageSize;
    int m_totalSize;
    int m_pageIndex;

};

#endif // MUSICPAGEQUERYREQUEST_H
