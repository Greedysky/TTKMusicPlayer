#ifndef MUSICFUNCTIONUIOBJECT_H
#define MUSICFUNCTIONUIOBJECT_H

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
namespace MusicUIObject
{
    const QString BtnUnDownload = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_download_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_download_hover); }";

    const QString BtnDownload = BtnUnDownload + " \
            QToolButton{ margin-left:-20px; }";

    const QString BtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_sound_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_sound_hover); }";

    const QString BtnMore = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_more_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_more_hover); }";

    const QString BtnUnLove = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_unloved_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_unloved_hover); }";

    const QString BtnLove = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_loved_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_loved_hover); }";

    const QString BtnDKLrc = " \
            QCheckBox{ background-color:transparent; } \
            QCheckBox:hover{ image: url(:/functions/btn_lrc_hover); } \
            QCheckBox::indicator::unchecked{ image: url(:/functions/btn_lrc_normal); } \
            QCheckBox::indicator:checked { image: url(:/functions/btn_lrc_hover); }";

    const QString BtnMinimum = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_min_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_min_hover); }";

    const QString BtnTClose = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_close_hover); } \
            QToolButton:hover{ background-image: url(:/functions/btn_close_hover); }";

    const QString BtnPClose = " \
            QPushButton{ border:none; \
            background-image: url(:/functions/btn_close_hover); } \
            QPushButton:hover{ background-image: url(:/functions/btn_close_hover); }";

    const QString BtnSkin = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_skin_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_skin_hover); }";

    const QString BtnIdentify = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_identify_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_identify_hover); }";

    const QString BtnTools = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_tools_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_tools_hover); }";

    const QString BtnSetting = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_setting_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_setting_hover); }";

    const QString BtnRemote = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_remote_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_remote_hover); }";

    const QString BtnConciseOut = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_show_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_show_hover); }";

    const QString BtnConciseIn = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_unshow_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_unshow_hover); }";

    const QString BtnListLoop = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_list_loop_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_list_loop_hover); }";

    const QString BtnOnce = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_once_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_once_hover); }";

    const QString BtnOneLoop = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_one_loop_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_one_loop_hover); }";

    const QString BtnOrder = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_order_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_order_hover); }";

    const QString BtnRandom = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_random_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_random_hover); }";

    const QString BtnPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_play_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_play_hover); }";

    const QString BtnPause = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_pause_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_pause_hover); }";

    const QString BtnNext = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_next_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_next_hover); }";

    const QString BtnPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_previous_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_previous_hover); }";

    const QString BtnMagic = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_hover); }";

    const QString BtnBackBack = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_back_back_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_back_back_hover); }";

    const QString BtnBackFresh = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_fresh_back_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_fresh_back_hover); }";

    const QString BtnPlayedList = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_queue_normal); } \
            QToolButton:hover{ background-image: url(:/functions/btn_queue_hover); }";

    const QString BtnResetWindow = " \
            QPushButton{ border:none; \
            background-image: url(:/functions/btn_reset_window_normal); } \
            QPushButton:hover{ background-image: url(:/functions/btn_reset_window_hover); }";

}

#endif // MUSICFUNCTIONUIOBJECT_H
