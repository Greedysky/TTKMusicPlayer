#ifndef MUSICOTHERDEFINE_H
#define MUSICOTHERDEFINE_H

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

#define MUSIC_AUTHOR_NAME           "Greedysky"
#define MUSIC_BING_DIR              "Bing"
#define MUSIC_THUNDER_DIR           "Thunder"
#define MUSIC_BIRD_DIR              "Bird"
#define MUSIC_TIME_INIT             "00:00"
#define MUSIC_MOVIE_RADIO           QUERY_KG_INTERFACE

#define MUSIC_COLOR_FILE            "color.jpg"
#define MUSIC_IMAGE_FILE            "image_cache"

#define MUSIC_RECORD_FILE           "record.raw"
#define MUSIC_RECORD_DATA_FILE      "record_data.wav"

#ifdef Q_OS_WIN
#  define MUSIC_OUTSIDE_OPEN        "-Open"
#  define MUSIC_OUTSIDE_LIST        "-List"
#endif

#define TTK_TIME_FORMAT             "hh:mm"
#define TTK_STIME_FORMAT            "hh:mm:ss"
#define TTK_YEAR_FORMAT             "yyyy-MM-dd"
#define TTK_YEAR_TIME_FORMAT        "yyyy-MM-dd hh:mm"
#define TTK_YEAR_STIME_FORMAT       "yyyy-MM-dd hh:mm:ss"

#define TTK_ITEM_SIZE_S             25
#define TTK_ITEM_SIZE_M             30
#define TTK_ITEM_SIZE_L             40
#define TTK_ITEM_SIZE_XL            60

#define DEFAULT_LOW_LEVEL           -999
#define DEFAULT_NORMAL_LEVEL        -1
#define DEFAULT_HIGH_LEVEL          999

//MusicLrcMakerWidget
#define LRC_MAKER_INDEX_0           0
#define LRC_MAKER_INDEX_1           1
#define LRC_MAKER_INDEX_2           2
#define LRC_MAKER_INDEX_3           3

//MusicConnectMobileWidget
#define CONNECT_MOBILE_INDEX_0      0

//MusicSongsSummariziedWidget
#define MUSIC_NORMAL_LIST           0
#define MUSIC_LOVEST_LIST           1
#define MUSIC_NETWORK_LIST          2
#define MUSIC_RECENT_LIST           3

//MusicIdentifySongsWidget
#define IDENTIFY_SONGS_INDEX_0      0

//MusicLocalSongsManagerWidget
#define LOCAL_MANAGER_INDEX_0       0
#define LOCAL_MANAGER_INDEX_1       1

//MusicSoundKMicroWidget
#define SOUND_KMICRO_INDEX_0        0
#define SOUND_KMICRO_INDEX_1        1

//MusicTimerWidget
#define TIMER_MANAGER_INDEX_0       0
#define TIMER_MANAGER_INDEX_1       1
#define TIMER_MANAGER_INDEX_2       2

//MusicUserWindow
#define USER_WINDOW_INDEX_0         0
#define USER_WINDOW_INDEX_1         1

//MusicVideoPlayWidget
#define VIDEO_WINDOW_INDEX_0        0
#define VIDEO_WINDOW_INDEX_1        1

//MusicPlaylistQueryWidget
#define PLAYLIST_WINDOW_INDEX_0     0
#define PLAYLIST_WINDOW_INDEX_1     1

//MusicSettingWidget
#define SETTING_WINDOW_INDEX_0      0
#define SETTING_WINDOW_INDEX_1      1
#define SETTING_WINDOW_INDEX_2      2
#define SETTING_WINDOW_INDEX_3      3
#define SETTING_WINDOW_INDEX_4      4
#define SETTING_WINDOW_INDEX_5      5
#define SETTING_WINDOW_INDEX_6      6
#define SETTING_WINDOW_INDEX_7      7
#define SETTING_WINDOW_INDEX_8      8

//MusicSourceUpdateWidget
#define SOURCE_UPDATE_INDEX_0       0
#define SOURCE_UPDATE_INDEX_1       1

//MusicApplication
#define APP_WINDOW_INDEX_0          0
#define APP_WINDOW_INDEX_1          1
#define APP_WINDOW_INDEX_2          2
#define APP_WINDOW_INDEX_3          3
#define APP_WINDOW_INDEX_4          4
#define APP_WINDOW_INDEX_5          5
#define APP_WINDOW_INDEX_6          6
#define APP_WINDOW_INDEX_7          7
#define APP_WINDOW_INDEX_8          8


#endif // MUSICOTHERDEFINE_H
