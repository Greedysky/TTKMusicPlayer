#ifndef MUSICTINYUIOBJECT_H
#define MUSICTINYUIOBJECT_H

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
    const QString TinyBtnPlay = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_normal); } \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_play_hover); }";

    const QString TinyBtnPause = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_pause_normal); } \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_pause_hover); }";

    const QString TinyBtnNext = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_next_normal); } \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_next_hover); }";

    const QString TinyBtnPrevious = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_previous_normal); } \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_previous_hover); }";

    const QString TinyBtnList = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_list_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_list_hover); }";
            
    const QString TinyBtnSetting = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_setting_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_setting_hover); }";

    const QString TinyBtnVisual = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_visual_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_visual_hover); }";

    const QString TinyBtnWallpaperOff = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_wallpaper_off_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_wallpaper_off_hover); }";

    const QString TinyBtnLrcOff = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_lrc_off_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_lrc_off_hover); }";
            
    const QString TinyBtnCollapse = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_collapse_w_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_collapse_w_hover); }";
            
    const QString TinyBtnExpand = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_expand_w_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_expand_w_hover); }";
            
    const QString TinyBtnUnDownload = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_download_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_download_hover); }";

    const QString TinyBtnDownload = TinyBtnUnDownload + " \
            QPushButton{ margin-left:-16px; }";

    const QString TinyBtnUnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_unloved_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_unloved_hover); }";

    const QString TinyBtnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_loved_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_loved_hover); }";

    const QString TinyBtnDelete = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_delete_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_delete_hover); }";

    const QString TinyBtnMV = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_mv_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_mv_hover); }";

    const QString TinyBtnPlayLater = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_later_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_play_later_hover); }";

    const QString TinyBtnMore = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_more_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_more_hover); }";

    const QString TinyBtnLocalSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_normal); } \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_hover); }";

    const QString TinyBtnMainSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_main_normal); } \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_main_hover); }";

    const QString TinyBtnLocation = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_location_normal); } \
            QToolButton:hover{ background-image: url(:/tiny/btn_location_hover); }";

    const QString TinyBtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound_normal); } \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound_hover); }";

    const QString TinyBtnSoundWhite = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound); } \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound); }";

    const QString TinyBtnClose = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_close_normal); } \
            QToolButton:hover{ background-image: url(:/tiny/btn_close_hover); }";

    const QString TinyBtnLrcExpand = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_collapse_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_collapse_hover); }";

    const QString TinyBtnLrcCollapse = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_expand_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_expand_hover); }";

    const QString TinyBtnShare = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_share_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_share_hover); }";
            
    const QString TinyBtnListMenu = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_list_menu_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_list_menu_hover); }";
            
    const QString TinyBtnPlaylist = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_playlist_normal); } \
            QPushButton:hover{ background-image: url(:/tiny/btn_playlist_hover); }";
            
}

#endif // MUSICTINYUIOBJECT_H
