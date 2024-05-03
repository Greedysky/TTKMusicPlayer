#ifndef MUSICVIDEOUIOBJECT_H
#define MUSICVIDEOUIOBJECT_H

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
        static constexpr const char *VideoBtnPlay = " \
            QPushButton{ border:none; \
            background-image:url(:/video/btn_play_normal); } \
            QPushButton:hover{ background-image:url(:/video/btn_play_hover); }";

        static constexpr const char *VideoBtnPause = " \
            QPushButton{ border:none; \
            background-image:url(:/video/btn_pause_normal); } \
            QPushButton:hover{ background-image:url(:/video/btn_pause_hover); }";

        static constexpr const char *VideoBtnBarrageOff = " \
            QPushButton{ border:none; \
            background-image:url(:/video/btn_barrage_off_normal); } \
            QPushButton:hover{ background-image:url(:/video/btn_barrage_off_hover); }";

        static constexpr const char *VideoBtnBarrageOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/btn_barrage_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/btn_barrage_on_hover); }";

        static constexpr const char *VideoBtnBarrageSend = " \
            QPushButton{ border:none; \
            background-image:url(:/video/btn_barrage_send_normal); } \
            QPushButton:hover{ background-image:url(:/video/btn_barrage_send_hover); } \
            QPushButton:disabled{ background-image:url(:/video/btn_barrage_send_disable); }";

        static constexpr const char *VideoBtnBarrageStyle = " \
            QToolButton{ border:none; \
            background-image:url(:/video/btn_barrage_style_normal); } \
            QToolButton:hover{ background-image:url(:/video/btn_barrage_style_hover); } \
            QToolButton:disabled{ background-image:url(:/video/btn_barrage_style_disable); }";

        static constexpr const char *VideoBtnSTMode = " \
            QToolButton{ border:none; \
            background-image:url(:/video/btn_st_normal); } \
            QToolButton:hover{ background-image:url(:/video/btn_st_hover); }";

        static constexpr const char *VideoBtnSDMode = " \
            QToolButton{ border:none; \
            background-image:url(:/video/btn_sd_normal); } \
            QToolButton:hover{ background-image:url(:/video/btn_sd_hover); }";

        static constexpr const char *VideoBtnHDMode = " \
            QToolButton{ border:none; \
            background-image:url(:/video/btn_hd_normal); } \
            QToolButton:hover{ background-image:url(:/video/btn_hd_hover); }";

        static constexpr const char *VideoBtnSQMode = " \
            QToolButton{ border:none; \
            background-image:url(:/video/btn_sq_normal); } \
            QToolButton:hover{ background-image:url(:/video/btn_sq_hover); }";

        static constexpr const char *VideoBtnBlue = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_blue_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_blue_hover); }";

        static constexpr const char *VideoBtnBlueOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_blue_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_blue_on_hover); }";

        static constexpr const char *VideoBtnGreen = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_green_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_green_hover); }";

        static constexpr const char *VideoBtnGreenOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_green_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_green_on_hover); }";

        static constexpr const char *VideoBtnOrange = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_orange_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_orange_hover); }";

        static constexpr const char *VideoBtnOrangeOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_orange_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_orange_on_hover); }";

        static constexpr const char *VideoBtnPurple = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_purple_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_purple_hover); }";

        static constexpr const char *VideoBtnPurpleOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_purple_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_purple_on_hover); }";

        static constexpr const char *VideoBtnRed = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_red_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_red_hover); }";

        static constexpr const char *VideoBtnRedOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_red_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_red_on_hover); }";

        static constexpr const char *VideoBtnWhite = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_white_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_white_hover); }";

        static constexpr const char *VideoBtnWhiteOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_white_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_white_on_hover); }";

        static constexpr const char *VideoBtnYellow = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_yellow_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_yellow_hover); }";

        static constexpr const char *VideoBtnYellowOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_yellow_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_yellow_on_hover); }";

        static constexpr const char *VideoBtnSmall = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_small_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_small_hover); }";

        static constexpr const char *VideoBtnSmallOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_small_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_small_on_hover); }";

        static constexpr const char *VideoBtnMiddle = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_middle_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_middle_hover); }";

        static constexpr const char *VideoBtnMiddleOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_middle_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_middle_on_hover); }";

        static constexpr const char *VideoBtnBig = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_big_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_big_hover); }";

        static constexpr const char *VideoBtnBigOn = " \
            QPushButton{ border:none; \
            background-image:url(:/video/lb_big_on_normal); } \
            QPushButton:hover{ background-image:url(:/video/lb_big_on_hover); }";

    }
}

#endif // MUSICVIDEOUIOBJECT_H
