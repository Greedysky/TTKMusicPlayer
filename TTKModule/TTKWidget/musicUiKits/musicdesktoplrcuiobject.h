#ifndef MUSICDESKTOPLRCUIOBJECT_H
#define MUSICDESKTOPLRCUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MQSSDeskTopPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_play_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_play_hover); }";

    const QString MQSSDeskTopPause = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_pause_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_pause_hover); }";

    const QString MQSSDeskTopNext = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_next_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_next_hover); }";

    const QString MQSSDeskTopPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_previous_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_previous_hover); }";

    const QString MQSSDeskTopSizeDown = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_size-_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_size-_hover); }";

    const QString MQSSDeskTopSizeUp = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_size+_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_size+_hover); }";

    const QString MQSSDeskTopSetting = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_setting_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_setting_hover); }";

    const QString MQSSDeskTopLock = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_lock_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_lock_hover); }";

    const QString MQSSDeskTopStyle= " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_style_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_style_hover); }";

    const QString MQSSDeskTopHErrorLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_herror_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_herror_hover); }";

    const QString MQSSDeskTopHMakeLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_hmake_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_hmake_hover); }";

    const QString MQSSDeskTopHSearchLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_hsearch_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_hsearch_hover); }";

    const QString MQSSDeskTopHUpdateLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_hupdate_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_hupdate_hover); }";

    const QString MQSSDeskTopVErrorLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_verror_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_verror_hover); }";

    const QString MQSSDeskTopVMakeLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_vmake_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_vmake_hover); }";

    const QString MQSSDeskTopVSearchLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_vsearch_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_vsearch_hover); }";

    const QString MQSSDeskTopVUpdateLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_vupdate_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_vupdate_hover); }";

    const QString MQSSDeskTopHorizontal = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_horizontal_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_horizontal_hover); }";

    const QString MQSSDeskTopVertical = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_vertical_normal); } \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_vertical_hover); }";

}

#endif // MUSICDESKTOPLRCUIOBJECT_H
