#ifndef MUSICVIDEOUIOBJECT_H
#define MUSICVIDEOUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
namespace MusicUIObject
{
    const QString MQSSVideoBtnPlay = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_play_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_play_hover); }";

    const QString MQSSVideoBtnPause = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_pause_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_pause_hover); }";

    const QString MQSSVideoBtnBarrageOff = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_barrage_off_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_barrage_off_hover); }";

    const QString MQSSVideoBtnBarrageOn = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_barrage_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_barrage_on_hover); }";

    const QString MQSSVideoBtnBarrageSend = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_barrage_send_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_barrage_send_hover); } \
            QPushButton:disabled{ background-image: url(:/video/btn_barrage_send_disable); }";

    const QString MQSSVideoBtnBarrageStyle = " \
            QToolButton{ border:none; \
            background-image: url(:/video/btn_barrage_style_normal); } \
            QToolButton:hover{ background-image: url(:/video/btn_barrage_style_hover); } \
            QToolButton:disabled{ background-image: url(:/video/btn_barrage_style_disable); }";

    const QString MQSSVideoBtnSTMode  = " \
            QToolButton{ border:none; \
            background-image: url(:/video/btn_st_normal); } \
            QToolButton:hover{ background-image: url(:/video/btn_st_hover); }";

    const QString MQSSVideoBtnSDMode  = " \
            QToolButton{ border:none; \
            background-image: url(:/video/btn_sd_normal); } \
            QToolButton:hover{ background-image: url(:/video/btn_sd_hover); }";

    const QString MQSSVideoBtnHDMode  = " \
            QToolButton{ border:none; \
            background-image: url(:/video/btn_hd_normal); } \
            QToolButton:hover{ background-image: url(:/video/btn_hd_hover); }";

    const QString MQSSVideoBtnSQMode = " \
            QToolButton{ border:none; \
            background-image: url(:/video/btn_sq_normal); } \
            QToolButton:hover{ background-image: url(:/video/btn_sq_hover); }";

    const QString MQSSVideoBtnBlue  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_blue_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_blue_hover); }";

    const QString MQSSVideoBtnBlueOn  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_blue_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_blue_on_hover); }";

    const QString MQSSVideoBtnGreen = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_green_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_green_hover); }";

    const QString MQSSVideoBtnGreenOn  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_green_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_green_on_hover); }";

    const QString MQSSVideoBtnOrange  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_orange_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_orange_hover); }";

    const QString MQSSVideoBtnOrangeOn = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_orange_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_orange_on_hover); }";

    const QString MQSSVideoBtnPurple  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_purple_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_purple_hover); }";

    const QString MQSSVideoBtnPurpleOn  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_purple_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_purple_on_hover); }";

    const QString MQSSVideoBtnRed = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_red_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_red_hover); }";

    const QString MQSSVideoBtnRedOn = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_red_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_red_on_hover); }";

    const QString MQSSVideoBtnWhite  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_white_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_white_hover); }";

    const QString MQSSVideoBtnWhiteOn  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_white_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_white_on_hover); }";

    const QString MQSSVideoBtnYellow = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_yellow_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_yellow_hover); }";

    const QString MQSSVideoBtnYellowOn = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_yellow_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_yellow_on_hover); }";

    const QString MQSSVideoBtnSmall = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_small_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_small_hover); }";

    const QString MQSSVideoBtnSmallOn = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_small_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_small_on_hover); }";

    const QString MQSSVideoBtnMiddle  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_middle_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_middle_hover); }";

    const QString MQSSVideoBtnMiddleOn  = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_middle_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_middle_on_hover); }";

    const QString MQSSVideoBtnBig = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_big_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_big_hover); }";

    const QString MQSSVideoBtnBigOn = " \
            QPushButton{ border:none; \
            background-image: url(:/video/lb_big_on_normal); } \
            QPushButton:hover{ background-image: url(:/video/lb_big_on_hover); }";

    const QString MQSSVideoBtnOrigin = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_origin_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_origin_hover); }";

    const QString MQSSVideoBtnOriginOff = " \
            QPushButton{ border:none; \
            background-image: url(:/video/btn_accompaniment_normal); } \
            QPushButton:hover{ background-image: url(:/video/btn_accompaniment_hover); }";

}

#endif // MUSICVIDEOUIOBJECT_H
