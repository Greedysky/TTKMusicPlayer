#ifndef MUSICOTHERDEFINE_H
#define MUSICOTHERDEFINE_H

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

#define TTK_TIME_FORMAT             "hh:mm"
#define TTK_STIME_FORMAT            "hh:mm:ss"
#define TTK_YEAR_FORMAT             "yyyy-MM-dd"
#define TTK_YEAR_TIME_FORMAT        "yyyy-MM-dd hh:mm"
#define TTK_YEAR_STIME_FORMAT       "yyyy-MM-dd hh:mm:ss"

#define TTK_ITEM_SIZE_S             25
#define TTK_ITEM_SIZE_M             30
#define TTK_ITEM_SIZE_L             40
#define TTK_ITEM_SIZE_XL            60

#define TTK_NONE_LEVEL              -3
#define TTK_LOW_LEVEL               -2
#define TTK_NORMAL_LEVEL            -1
#define TTK_HIGH_LEVEL              999

#define TTK_COLOR_FILE              "color.jpg"
#define TTK_IMAGE_FILE              "image_cache"
#define TTK_RECORD_FILE             "record.raw"
#define TTK_RECORD_DATA_FILE        "record_data.wav"

#define MIN_ITEM_COUNT              4
#define MAX_IMAGE_COUNT             10

#define MUSIC_AUTHOR_NAME           "Greedysky"
#define MUSIC_BING_DIR              "Bing"
#define MUSIC_THUNDER_DIR           "Thunder"
#define MUSIC_BIRD_DIR              "Bird"
#define MUSIC_TIME_INIT             "00:00"
#define MUSIC_MOVIE_RADIO           QUERY_KG_INTERFACE

#define MUSIC_OUTSIDE_OPEN          "-Open"
#define MUSIC_OUTSIDE_LIST          "-List"

#define MUSIC_PLAY_NOW              "play_now"
#define MUSIC_PLAY_LATER            "play_later"

#define MUSIC_NORMAL_LIST           0
#define MUSIC_LOVEST_LIST           1
#define MUSIC_NETWORK_LIST          2
#define MUSIC_RECENT_LIST           3

#define MUSIC_LRC_PAGE              1
#define MUSIC_SEARCH_PAGE           0

#endif // MUSICOTHERDEFINE_H
