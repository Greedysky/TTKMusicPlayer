#ifndef MUSICCOVERREQUEST_H
#define MUSICCOVERREQUEST_H

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

/*! @brief The class of the cover data download request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCoverRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCoverRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicCoverRequest(QObject *parent = nullptr);

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override final;

    /*!
     * Start to download data.
     * Subclass should implement this function.
     */
    virtual void startToRequest(const QString &url) = 0;

};


/*! @brief The namespace of the cover helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Check current url is valid or not.
     */
    inline static bool isCoverValid(const QString &url)
    {
        return !(url.isEmpty() || url == TTK_NULL_STR || url == TTK_DEFAULT_STR);
    }
}

#endif // MUSICCOVERREQUEST_H
