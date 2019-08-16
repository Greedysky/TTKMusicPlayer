#ifndef KUGOUURL_H
#define KUGOUURL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include <QObject>
#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the kugou url.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT KugouUrl
{
public:
    /*!
     * Object contsructor.
     */
    KugouUrl();

    /*!
     * Get Song Yueku Url.
     */
    static QString getSongYuekuUrl();
    /*!
     * Get Song Recommend Url.
     */
    static QString getSongRecommendUrl();
    /*!
     * Get Song Rank Url.
     */
    static QString getSongRankUrl();
    /*!
     * Get Song Singer Url.
     */
    static QString getSongSingerUrl();
    /*!
     * Get Song Category Url.
     */
    static QString getSongCategoryUrl();
    /*!
     * Get Song Show Url.
     */
    static QString getSongShowUrl();
    /*!
     * Get Song Heroes Url.
     */
    static QString getSongHeroesUrl();



    /*!
     * Get Radio Public Url.
     */
    static QString getRadioPublicUrl();
    /*!
     * Get Radio High Fm Url.
     */
    static QString getRadioHighFmUrl();
    /*!
     * Get Radio Fx Url.
     */
    static QString getRadioFxUrl();
    /*!
     * Get Radio Home Url.
     */
    static QString getRadioHomeUrl();



    /*!
     * Get List Url.
     */
    static QString getListUrl();



    /*!
     * Get MV Radio Url.
     */
    static QString getMVRadioUrl();
    /*!
     * Get MV Recommend Url.
     */
    static QString getMVRecommendUrl();
    /*!
     * Get MV Fanxing Url.
     */
    static QString getMVFanxingUrl();



    /*!
     * Get Web Player Url.
     */
    static QString getWebPlayerUrl();



    /*!
     * Get KuiShe Url.
     */
    static QString getKuiSheUrl();

};

#endif // KUGOUURL_H
