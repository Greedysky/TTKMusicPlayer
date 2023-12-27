#ifndef MUSICCOVERSOURCEREQUEST_H
#define MUSICCOVERSOURCEREQUEST_H

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

#include "musicdatasourcerequest.h"

/*! @brief The class of the cover source data download request.
 * @author Greedysky <greedysky@163.com>
 */
using MusicCoverSourceRequest = MusicDataSourceRequest;

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

#endif // MUSICCOVERSOURCEREQUEST_H
