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

    static QString getYuekuUrl();
    static QString getRecommendUrl();
    static QString getRadioUrl();
    static QString getRankUrl();
    static QString getSingerUrl();
    static QString getCategoryUrl();
    static QString getShowUrl();
    static QString getCCTVUrl();

};

#endif // KUGOUURL_H
