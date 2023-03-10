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
        const QString DeskTopPlay = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_play_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_play_hover); }";

        const QString DeskTopPause = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_pause_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_pause_hover); }";

        const QString DeskTopNext = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_next_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_next_hover); }";

        const QString DeskTopPrevious = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_previous_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_previous_hover); }";

        const QString DeskTopSizeDown = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_size-_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_size-_hover); }";

        const QString DeskTopSizeUp = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_size+_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_size+_hover); }";

        const QString DeskTopSetting = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_setting_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_setting_hover); }";

        const QString DeskTopLock = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_lock_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_lock_hover); }";

        const QString DeskTopStyle= " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_style_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_style_hover); }";

        const QString DeskTopHMakeLrc = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_hmake_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_hmake_hover); }";

        const QString DeskTopHSearchLrc = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_hsearch_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_hsearch_hover); }";

        const QString DeskTopHUpdateLrc = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_hupdate_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_hupdate_hover); }";

        const QString DeskTopVMakeLrc = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_vmake_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_vmake_hover); }";

        const QString DeskTopVSearchLrc = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_vsearch_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_vsearch_hover); }";

        const QString DeskTopVUpdateLrc = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_vupdate_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_vupdate_hover); }";

        const QString DeskTopHorizontal = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_horizontal_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_horizontal_hover); }";

        const QString DeskTopVertical = " \
                QToolButton{ border:none; \
                background-image: url(:/desktop/btn_vertical_normal); } \
                QToolButton:hover{ background-image: url(:/desktop/btn_vertical_hover); }";

    }
}

#endif // MUSICDESKTOPLRCUIOBJECT_H
