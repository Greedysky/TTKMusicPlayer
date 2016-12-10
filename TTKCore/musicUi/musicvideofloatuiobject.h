#ifndef MUSICVIDEOFLOATUIOBJECT_H
#define MUSICVIDEOFLOATUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the video float button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGVideoFloatDownload = " \
            QPushButton{ border:none;\
            background-image: url(:/video/btn_download_normal);} \
            QPushButton:hover{ background-image: url(:/video/btn_download_hover);}";

    const QString MKGVideoFloatFresh = " \
            QPushButton{ border:none;\
            background-image: url(:/video/btn_fresh_normal);} \
            QPushButton:hover{ background-image: url(:/video/btn_fresh_hover);}";

    const QString MKGVideoFloatFullscreen = " \
            QPushButton{ border:none;\
            background-image: url(:/video/btn_fullscreen_normal);} \
            QPushButton:hover{ background-image: url(:/video/btn_fullscreen_hover);}";

    const QString MKGVideoFloatSearch = " \
            QPushButton{ border:none;\
            background-image: url(:/video/btn_search_normal);} \
            QPushButton:hover{ background-image: url(:/video/btn_search_hover);}";

    const QString MKGVideoFloatShare = " \
            QPushButton{ border:none;\
            background-image: url(:/video/btn_share_normal);} \
            QPushButton:hover{ background-image: url(:/video/btn_share_hover);}";

}

#endif // MUSICVIDEOFLOATUIOBJECT_H
