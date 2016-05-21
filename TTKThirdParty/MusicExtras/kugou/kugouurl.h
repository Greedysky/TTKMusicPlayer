#ifndef KUGOUURL_H
#define KUGOUURL_H

#include <QObject>
#include "musicextrasglobaldefine.h"

class MUSIC_EXTRAS_EXPORT KugouUrl
{
public:
    KugouUrl();

    static QString getYuekuUrl();
    static QString getRecommendUrl();
    static QString getRankUrl();
    static QString getSingerUrl();
    static QString getCategoryUrl();
    static QString getShowUrl();

};

#endif // KUGOUURL_H
