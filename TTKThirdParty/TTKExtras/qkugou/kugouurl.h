#ifndef KUGOUURL_H
#define KUGOUURL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
