#ifndef QKUGOUURL_H
#define QKUGOUURL_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "ttkprivate.h"

/*! @brief The class of the kugou url.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QKugouUrl
{
public:
    /*!
     Make Song Yueku Url.
     */
    static QString makeSongYuekuUrl() noexcept;
    /*!
     Make Song Recommend Url.
     */
    static QString makeSongRecommendUrl() noexcept;

    /*!
     Make Radio Public Url.
     */
    static QString makeRadioPublicUrl() noexcept;

    /*!
     Make Movie Url.
     */
    static QString makeMovieRecommendUrl() noexcept;

    /*!
     Make KuiShe Url.
     */
    static QString makeKuiSheUrl() noexcept;

};

#endif // QKUGOUURL_H
