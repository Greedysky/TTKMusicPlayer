#ifndef MUSICINTERIORFLOATUIOBJECT_H
#define MUSICINTERIORFLOATUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
        static constexpr const char *InteriorFloatSetting = " \
            QPushButton{ border:1px solid #CCCCCC; border-radius:2px; } \
            QPushButton:hover{ border:1px solid #FFFFFF; border-radius:2px; }";

        static constexpr const char *InteriorFloatMore = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/lb_more_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/lb_more_hover); }";

        static constexpr const char *InteriorFloatUpdate = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/lb_update_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/lb_update_hover); }";

        static constexpr const char *InteriorFloatWallpaper = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/lb_wallpaper_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/lb_wallpaper_hover); }";

        static constexpr const char *InteriorFloatWallpaperOn = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/lb_wallpaper_on_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/lb_wallpaper_on_hover); }";

        static constexpr const char *InteriorFloatSearch = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/lb_search_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/lb_search_hover); }";

        static constexpr const char *InteriorFloatPhoto = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/lb_photo_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/lb_photo_hover); }";

        static constexpr const char *InteriorFloatPhotoItem = " \
            QCheckBox::indicator::unchecked{ image:url(:/lrc/lb_photo_unchecked); } \
            QCheckBox::indicator:checked{ image:url(:/lrc/lb_photo_checked); }";

        static constexpr const char *InteriorFloatPhotoPrevious = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/btn_photo_previous_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/btn_photo_previous_hover); }";

        static constexpr const char *InteriorFloatPhotoNext = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/btn_photo_next_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/btn_photo_next_hover); }";

        static constexpr const char *InteriorLrcBigger = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/btn_bigger_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/btn_bigger_hover); }";

        static constexpr const char *InteriorLrcSmaller = " \
            QPushButton{ border:none;\
            background-image:url(:/lrc/btn_smaller_normal); } \
            QPushButton:hover{ background-image:url(:/lrc/btn_smaller_hover); }";

    }
}

#endif // MUSICINTERIORFLOATUIOBJECT_H
