#ifndef KUGOUURL_H
#define KUGOUURL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the kugou url.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT KugouUrl
{
public:
    KugouUrl();
    /*!
     * Object contsructor.
     */

    //kugou song
    static QString getSongYuekuUrl();
    /*!
     * Get Song Yueku Url.
     */
    static QString getSongRecommendUrl();
    /*!
     * Get Song Recommend Url.
     */
    static QString getSongRankUrl();
    /*!
     * Get Song Rank Url.
     */
    static QString getSongSingerUrl();
    /*!
     * Get Song Singer Url.
     */
    static QString getSongCategoryUrl();
    /*!
     * Get Song Category Url.
     */
    static QString getSongShowUrl();
    /*!
     * Get Song Show Url.
     */
    static QString getSongHeroesUrl();
    /*!
     * Get Song Heroes Url.
     */

    //kugou radio
    static QString getRadioPublicUrl();
    /*!
     * Get Radio Public Url.
     */
    static QString getRadioHighFmUrl();
    /*!
     * Get Radio High Fm Url.
     */
    static QString getRadioFxUrl();
    /*!
     * Get Radio Fx Url.
     */
    static QString getRadioHomeUrl();
    /*!
     * Get Radio Home Url.
     */

    //kugou list
    static QString getListUrl();
    /*!
     * Get List Url.
     */

    //kugou mv
    static QString getMVRadioUrl();
    /*!
     * Get MV Radio Url.
     */
    static QString getMVRecommendUrl();
    /*!
     * Get MV Recommend Url.
     */
    static QString getMVFanxingUrl();
    /*!
     * Get MV Fanxing Url.
     */
    static QString getMVStarMusicUrl();
    /*!
     * Get MV Star Music Url.
     */

    //kugou web
    static QString getWebPlayerUrl();
    /*!
     * Get Web Player Url.
     */

    //kuishe web
    static QString getKuiSheUrl();
    /*!
     * Get KuiShe Url.
     */

};

#endif // KUGOUURL_H
