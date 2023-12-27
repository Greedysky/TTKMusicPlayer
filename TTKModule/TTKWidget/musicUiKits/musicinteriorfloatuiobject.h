#ifndef MUSICINTERIORFLOATUIOBJECT_H
#define MUSICINTERIORFLOATUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace UI
    {
        static const QString InteriorFloatSetting = " \
            QPushButton{ border:1px solid #CCCCCC; border-radius:2px; } \
            QPushButton:hover{ border:1px solid #FFFFFF; border-radius:2px; }";

        static const QString InteriorFloatMore = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_more_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_more_hover); }";

        static const QString InteriorFloatUpdate = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_update_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_update_hover); }";

        static const QString InteriorFloatWallpaper = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_wallpaper_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_wallpaper_hover); }";

        static const QString InteriorFloatWallpaperOn = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_wallpaper_on_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_wallpaper_on_hover); }";

        static const QString InteriorFloatSearch = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_search_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_search_hover); }";

        static const QString InteriorFloatPhoto = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/lb_photo_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_photo_hover); }";

        static const QString InteriorFloatPhotoItem = " \
            QCheckBox::indicator::unchecked{ image:url(:/lrc/lb_photo_unchecked); } \
            QCheckBox::indicator:checked{ image:url(:/lrc/lb_photo_checked); }";

        static const QString InteriorFloatPhotoPrevious = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/btn_photo_previous_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_photo_previous_hover); }";

        static const QString InteriorFloatPhotoNext = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/btn_photo_next_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_photo_next_hover); }";

        static const QString InteriorLrcBigger = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/btn_bigger_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_bigger_hover); }";

        static const QString InteriorLrcSmaller = " \
            QPushButton{ border:none;\
            background-image: url(:/lrc/btn_smaller_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_smaller_hover); }";

    }
}

#endif // MUSICINTERIORFLOATUIOBJECT_H
