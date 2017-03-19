#ifndef MUSICINLINEFLOATUIOBJECT_H
#define MUSICINLINEFLOATUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the inline float button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGInlineFloatMore = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_more_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_more_hover);}";

    const QString MKGInlineFloatUpdate = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_update_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_update_hover);}";

    const QString MKGInlineFloatWallpaper = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_wallpaper_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_wallpaper_hover);}";

    const QString MKGInlineFloatSearch = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_search_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_search_hover);}";

    const QString MKGInlineFloatPhoto = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_photo_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_photo_hover);}";

    const QString MKGInlineFloatSetting = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_border_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_border_hover);}";

    const QString MKGInlineLrcBigger = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/btn_bigger_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_bigger_hover);}";

    const QString MKGInlineLrcSmaller = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/btn_smaller_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_smaller_hover);}";

}

#endif // MUSICINLINEFLOATUIOBJECT_H
