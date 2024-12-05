#ifndef MUSICDISCOVERLISTREQUEST_H
#define MUSICDISCOVERLISTREQUEST_H

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

#include "musicpagequeryrequest.h"

/*! @brief The class of the download discover list request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDiscoverListRequest : public MusicPageQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDiscoverListRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDiscoverListRequest(QObject *parent = nullptr);

    /*!
     * Start to search data.
     * Subclass should implement this function.
     */
    virtual void startToSearch() = 0;

    /*!
     * Get discover list info.
     */
    inline const QString& item() const { return m_item; }

protected:
    QString m_item;

};

#endif // MUSICDISCOVERLISTREQUEST_H
