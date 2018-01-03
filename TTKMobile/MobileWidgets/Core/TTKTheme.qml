/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

import QtQuick 2.5

QtObject {
    property int music_normal_list:   0
    property int music_download_list: 1
    property int music_recent_list:   2
    property int music_lovest_list:   3
    property int music_downmv_list:   4
    property int music_musicrg_list:  5
    /////////////////////////////////////////////////////
    property int search_type_null:                  0
    property int search_type_song:                  1
    property int search_type_movie:                 2
    property int search_type_lrc:                   3
    property int search_type_download_song:         4
    property int search_type_download_song_index:   5
    property int search_type_download_mv_index:     6
    /////////////////////////////////////////////////////
    property color topbar_background: "#31C27B"
    property int   topbar_height: 50

    /////////////////////////////////////////////////////
    property color color_white: "#FFFFFF"
    property color color_black: "#000000"
    property color color_gray:  "gray"
    property color color_red:   "red"
    property color color_green: "#00FF00"

    property color color_alpha_lv0:  '#00FFFFFF'
    property color color_alpha_lv1:  '#11FFFFFF'
    property color color_alpha_lv2:  '#33FFFFFF'
    property color color_alpha_lv3:  '#55FFFFFF'
    property color color_alpha_lv4:  '#77FFFFFF'
    property color color_alpha_lv5:  '#99FFFFFF'
    property color color_alpha_lv6:  '#BBFFFFFF'
    property color color_alpha_lv7:  '#DDFFFFFF'
    property color color_alpha_lv8:  '#00000000'
    property color color_alpha_lv9:  '#11000000'
    property color color_alpha_lv10: '#33000000'
    property color color_alpha_lv11: '#55000000'
    property color color_alpha_lv12: '#77000000'
    property color color_alpha_lv13: '#99000000'
    property color color_alpha_lv14: '#BB000000'
    property color color_alpha_lv15: '#DD000000'
    /////////////////////////////////////////////////////

}
