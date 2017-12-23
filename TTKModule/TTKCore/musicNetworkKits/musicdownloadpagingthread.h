#ifndef MUSICDOWNLOADPAGINGTHREAD_H
#define MUSICDOWNLOADPAGINGTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicnetworkabstract.h"

/*! @brief The class to abstract download paing thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadPagingThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadPagingThread(QObject *parent = 0);

    virtual ~MusicDownLoadPagingThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset);

    /*!
     * Return the current raw data.
     */
    inline QVariantMap getRawData() const { return m_rawData; }
    /*!
     * Return the each page max size.
     */
    inline int getPageSize() const { return m_pageSize; }
    /*!
     * Return the page total number.
     */
    inline int getPageTotal() const { return m_pageTotal; }

protected:
    QVariantMap m_rawData;
    int m_pageSize, m_pageTotal;

};

#endif // MUSICDOWNLOADPAGINGTHREAD_H
