#ifndef MUSICQUERYTOPLISTREQUEST_H
#define MUSICQUERYTOPLISTREQUEST_H

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

/*! @brief The class of the query toplist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQueryToplistRequest : public MusicAbstractQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryToplistRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicQueryToplistRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by input data.
     */
    virtual void startToSearch(const QString &value) override;

Q_SIGNALS:
    /*!
     * Create the current toplist info item.
     */
    void createToplistItem(const MusicResultDataItem &item);

protected:
    /*!
     * Query toplist info.
     * Subclass should implement this function.
     */
    virtual void queryToplistInfo(const QVariantMap &input) = 0;

};

#endif // MUSICQUERYTOPLISTREQUEST_H
