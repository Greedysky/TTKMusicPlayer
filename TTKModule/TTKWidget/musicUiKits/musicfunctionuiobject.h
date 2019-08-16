#ifndef MUSICFUNCTIONUIOBJECT_H
#define MUSICFUNCTIONUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

/*! @brief The namespace of the function button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGBtnUnDownload = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_download_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_download_hover);}";

    const QString MKGBtnDownload = MKGBtnUnDownload + " \
            QToolButton{ margin-left:-20px; }";

    const QString MKGBtnSimilar = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_similar_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_similar_hover);}";

    const QString MKGBtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_sound_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_sound_hover);}";

    const QString MKGBtnMore = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_more_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_more_hover);}";

    const QString MKGBtnUnLove = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_unloved_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_unloved_hover);}";

    const QString MKGBtnLove = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_loved_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_loved_hover);}";

    const QString MKGBtnDKLrc = " \
            QCheckBox{ background-color:transparent; } \
            QCheckBox:hover{ image: url(:/functions/btn_lrc_hover);} \
            QCheckBox::indicator::unchecked{ image: url(:/functions/btn_lrc_normal); } \
            QCheckBox::indicator:checked { image: url(:/functions/btn_lrc_hover); }";

    const QString MKGBtnMinimum = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_min_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_min_hover);}";

    const QString MKGBtnTClose = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_close_hover);} \
            QToolButton:hover{ background-image: url(:/functions/btn_close_hover);}";

    const QString MKGBtnPClose = " \
            QPushButton{ border:none; \
            background-image: url(:/functions/btn_close_hover);} \
            QPushButton:hover{ background-image: url(:/functions/btn_close_hover);}";

    const QString MKGBtnSkin = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_skin_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_skin_hover);}";

    const QString MKGBtnIdentify = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_identify_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_identify_hover);}";

    const QString MKGBtnTools = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_tools_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_tools_hover);}";

    const QString MKGBtnSetting = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_setting_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_setting_hover);}";

    const QString MKGBtnRemote = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_remote_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_remote_hover);}";

    const QString MKGBtnConciseOut = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_show_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_show_hover);}";

    const QString MKGBtnConciseIn = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_unshow_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_unshow_hover);}";

    const QString MKGBtnListLoop = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_listLoop_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_listLoop_hover);}";

    const QString MKGBtnOnce = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_once_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_once_hover);}";

    const QString MKGBtnOneLoop = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_oneLoop_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_oneLoop_hover);}";

    const QString MKGBtnOrder = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_order_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_order_hover);}";

    const QString MKGBtnRandom = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_random_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_random_hover);}";

    const QString MKGBtnPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_play_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_play_hover);}";

    const QString MKGBtnPause = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_pause_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_pause_hover);}";

    const QString MKGBtnNext = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_next_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_next_hover);}";

    const QString MKGBtnPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_previous_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_previous_hover);}";

    const QString MKGBtnMagic = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_hover);}";

    const QString MKGBtnBackBack = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_back_back_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_back_back_hover);}";

    const QString MKGBtnBackFresh = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_fresh_back_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_fresh_back_hover);}";

    const QString MKGBtnPlayedList = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_queue_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_queue_hover);}";

    const QString MKGBtnResetWindow = " \
            QPushButton{ border:none; \
            background-image: url(:/functions/btn_reset_window_normal);} \
            QPushButton:hover{ background-image: url(:/functions/btn_reset_window_hover);}";

}

#endif // MUSICFUNCTIONUIOBJECT_H
