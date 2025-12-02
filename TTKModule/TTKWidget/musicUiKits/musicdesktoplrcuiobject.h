#ifndef MUSICDESKTOPLRCUIOBJECT_H
#define MUSICDESKTOPLRCUIOBJECT_H

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
        static constexpr const char *DeskTopPlay = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_play_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_play_hover); }";

        static constexpr const char *DeskTopPause = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_pause_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_pause_hover); }";

        static constexpr const char *DeskTopNext = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_next_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_next_hover); }";

        static constexpr const char *DeskTopPrevious = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_previous_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_previous_hover); }";

        static constexpr const char *DeskTopSizeDown = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_size-_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_size-_hover); }";

        static constexpr const char *DeskTopSizeUp = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_size+_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_size+_hover); }";

        static constexpr const char *DeskTopSetting = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_setting_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_setting_hover); }";

        static constexpr const char *DeskTopLock = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_lock_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_lock_hover); }";

        static constexpr const char *DeskTopStyle= " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_style_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_style_hover); }";

        static constexpr const char *DeskTopHMakeLrc = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_hmake_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_hmake_hover); }";

        static constexpr const char *DeskTopHSearchLrc = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_hsearch_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_hsearch_hover); }";

        static constexpr const char *DeskTopHUpdateLrc = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_hupdate_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_hupdate_hover); }";

        static constexpr const char *DeskTopVMakeLrc = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_vmake_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_vmake_hover); }";

        static constexpr const char *DeskTopVSearchLrc = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_vsearch_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_vsearch_hover); }";

        static constexpr const char *DeskTopVUpdateLrc = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_vupdate_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_vupdate_hover); }";

        static constexpr const char *DeskTopHorizontal = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_horizontal_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_horizontal_hover); }";

        static constexpr const char *DeskTopVertical = " \
            QToolButton{ border:none; \
            background-image:url(:/desktop/btn_vertical_normal); } \
            QToolButton:hover{ background-image:url(:/desktop/btn_vertical_hover); }";

    }
}

#endif // MUSICDESKTOPLRCUIOBJECT_H
