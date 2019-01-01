#ifndef MUSICTINYUIOBJECT_H
#define MUSICTINYUIOBJECT_H

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

/*! @brief The namespace of the tiny button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGTinyBtnPlay = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_normal);} \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_play_hover);}";

    const QString MKGTinyBtnPause = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_pause_normal);} \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_pause_hover);}";

    const QString MKGTinyBtnNext = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_next_normal);} \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_next_hover);}";

    const QString MKGTinyBtnPrevious = " \
            QToolButton,QPushButton{ border:none; \
            background-image: url(:/tiny/btn_previous_normal);} \
            QToolButton:hover,QPushButton:hover{ background-image: url(:/tiny/btn_previous_hover);}";

    const QString MKGTinyBtnList = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_list_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_list_hover);}";
            
    const QString MKGTinyBtnSetting = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_setting_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_setting_hover);}";
            
    const QString MKGTinyBtnWallpaperOn = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_wallpaper_on_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_wallpaper_on_hover);}";
            
    const QString MKGTinyBtnWallpaperOff = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_wallpaper_off_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_wallpaper_off_hover);}";
            
    const QString MKGTinyBtnLrcOn = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_lrc_on_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_lrc_on_hover);}";
            
    const QString MKGTinyBtnLrcOff = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_lrc_off_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_lrc_off_hover);}";
            
    const QString MKGTinyBtnCollapse = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_collapse_w_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_collapse_w_hover);}";
            
    const QString MKGTinyBtnExpand = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_expand_w_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_expand_w_hover);}";
            
    const QString MKGTinyBtnUnDownload = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_download_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_download_hover);}";

    const QString MKGTinyBtnDownload = MKGTinyBtnUnDownload + " \
            QPushButton{ margin-left:-16px; }";

    const QString MKGTinyBtnUnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_unloved_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_unloved_hover);}";

    const QString MKGTinyBtnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_loved_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_loved_hover);}";

    const QString MKGTinyBtnDelete = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_delete_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_delete_hover);}";

    const QString MKGTinyBtnMV = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_mv_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_mv_hover);}";

    const QString MKGTinyBtnPlayLater = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_later_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_play_later_hover);}";

    const QString MKGTinyBtnMore = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_more_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_more_hover);}";

    const QString MKGTinyBtnLocalSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_hover);}";

    const QString MKGTinyBtnMainSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_main_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_main_hover);}";

    const QString MKGTinyBtnLocation = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_location_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_location_hover);}";

    const QString MKGTinyBtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound_hover);}";

    const QString MKGTinyBtnSoundWhite = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound);}";

    const QString MKGTinyBtnClose = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_close_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_close_hover);}";

    const QString MKGTinyBtnLrcExpand = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_collapse_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_collapse_hover);}";

    const QString MKGTinyBtnLrcCollapse = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_expand_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_expand_hover);}";

    const QString MKGTinyBtnWintopOn = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_top_on_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_top_on_hover);}";

    const QString MKGTinyBtnWintopOff = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_top_off_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_top_off_hover);}";
            
    const QString MKGTinyBtnShare = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_share_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_share_hover);}";
            
    const QString MKGTinyBtnEnhanceLossless = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_enhance_lossless_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_enhance_lossless_hover);}";
            
    const QString MKGTinyBtnListMenu = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_list_menu_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_list_menu_hover);}";
            
    const QString MKGTinyBtnPlaylist = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_playlist_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_playlist_hover);}";
            
}

#endif // MUSICTINYUIOBJECT_H
