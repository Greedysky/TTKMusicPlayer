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

class MUSIC_EXTRAS_EXPORT KugouUrl
{
public:
    KugouUrl();

    static QString getClassName();

    //kugou song
    static QString getSongYuekuUrl();
    static QString getSongRecommendUrl();
    static QString getSongRankUrl();
    static QString getSongSingerUrl();
    static QString getSongCategoryUrl();
    static QString getSongShowUrl();
    static QString getSongHeroesUrl();

    //kugou radio
    static QString getRadioPublicUrl();
    static QString getRadioHighFmUrl();
    static QString getRadioFxUrl();
    static QString getRadioHomeUrl();

    //kugou list
    static QString getListUrl();

    //kugou mv
    static QString getMVRadioUrl();
    static QString getMVRecommendUrl();
    static QString getMVFanxingUrl();
    static QString getMVStarMusicUrl();

};

#endif // KUGOUURL_H
