#ifndef MUSICINTERIORLRCUIOBJECT_H
#define MUSICINTERIORLRCUIOBJECT_H

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
    const QString MQSSInteriorTranslation = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_translation_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_translation_hover); }";

    const QString MQSSInteriorMicrophone = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_microphone_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_microphone_hover); }";

    const QString MQSSInteriorMessage = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_message_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_message_hover); }";

    const QString MQSSInteriorMovie = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_mv_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_mv_hover); }";
            
    const QString MQSSInteriorStar = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_star_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/btn_star_hover); } \
            QPushButton:pressed{ background-image: url(:/lrc/btn_star_clicked); }";
            
    const QString MQSSInteriorMakeUp = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/lb_make_up_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_make_up_hover); }";
            
    const QString MQSSInteriorMakeDown = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/lb_make_down_normal); } \
            QPushButton:hover{ background-image: url(:/lrc/lb_make_down_hover); }";

}

#endif // MUSICINTERIORLRCUIOBJECT_H
