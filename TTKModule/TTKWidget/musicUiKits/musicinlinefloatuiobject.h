#ifndef MUSICINLINEFLOATUIOBJECT_H
#define MUSICINLINEFLOATUIOBJECT_H

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

    const QString MKGInlineFloatWallpaperOn = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_wallpaper_on_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_wallpaper_on_hover);}";

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
