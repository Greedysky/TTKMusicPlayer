#ifndef MUSICDESKTOPLRCUIOBJECT_H
#define MUSICDESKTOPLRCUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
        static const QString DeskTopPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_play_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_play_hover); }";

        static const QString DeskTopPause = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_pause_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_pause_hover); }";

        static const QString DeskTopNext = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_next_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_next_hover); }";

        static const QString DeskTopPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_previous_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_previous_hover); }";

        static const QString DeskTopSizeDown = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_size-_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_size-_hover); }";

        static const QString DeskTopSizeUp = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_size+_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_size+_hover); }";

        static const QString DeskTopSetting = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_setting_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_setting_hover); }";

        static const QString DeskTopLock = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_lock_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_lock_hover); }";

        static const QString DeskTopStyle= " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_style_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_style_hover); }";

        static const QString DeskTopHMakeLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_hmake_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_hmake_hover); }";

        static const QString DeskTopHSearchLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_hsearch_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_hsearch_hover); }";

        static const QString DeskTopHUpdateLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_hupdate_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_hupdate_hover); }";

        static const QString DeskTopVMakeLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_vmake_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_vmake_hover); }";

        static const QString DeskTopVSearchLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_vsearch_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_vsearch_hover); }";

        static const QString DeskTopVUpdateLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_vupdate_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_vupdate_hover); }";

        static const QString DeskTopHorizontal = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_horizontal_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_horizontal_hover); }";

        static const QString DeskTopVertical = " \
            QToolButton{ border:none; \
            background-image: url(:/desktop/btn_vertical_normal); } \
            QToolButton:hover{ background-image: url(:/desktop/btn_vertical_hover); }";

    }
}

#endif // MUSICDESKTOPLRCUIOBJECT_H
