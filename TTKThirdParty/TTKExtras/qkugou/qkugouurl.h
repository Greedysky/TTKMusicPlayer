#ifndef QKUGOUURL_H
#define QKUGOUURL_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
     * Object constructor.
     */
    QKugouUrl();

    /*!
     Make Song Yueku Url.
     */
    static QString makeSongYuekuUrl();
    /*!
     Make Song Recommend Url.
     */
    static QString makeSongRecommendUrl();
    /*!
     Make Song Rank Url.
     */
    static QString makeSongRankUrl();
    /*!
     Make Song Singer Url.
     */
    static QString makeSongSingerUrl();
    /*!
     Make Song Category Url.
     */
    static QString makeSongCategoryUrl();


    /*!
     Make Radio Public Url.
     */
    static QString makeRadioPublicUrl();
    /*!
     Make Radio High Fm Url.
     */
    static QString makeRadioHighFmUrl();


    /*!
     Make List Url.
     */
    static QString makeListUrl();


    /*!
     Make MV Radio Url.
     */
    static QString makeMVRadioUrl();
    /*!
     Make MV Recommend Url.
     */
    static QString makeMVRecommendUrl();

    /*!
     Make Web Player Url.
     */
    static QString makeWebPlayerUrl();


    /*!
     Make KuiShe Url.
     */
    static QString makeKuiSheUrl();

};

#endif // QKUGOUURL_H
