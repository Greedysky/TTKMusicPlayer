#ifndef MUSICOTHERDEFINE_H
#define MUSICOTHERDEFINE_H

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

#define TTK_QT_CONFIG         "qt.conf"

#define TTK_AUTHOR_NAME       "Greedysky"

#define TTK_COLOR_FILE        "color.jpg"
#define TTK_IMAGE_FILE        "image_cache"
#define TTK_RECORD_FILE       "record.raw"
#define TTK_RECORD_DATA_FILE  "record_data.wav"

#define MIN_ITEM_COUNT        4
#define MAX_IMAGE_COUNT       10

#define MUSIC_MOVIE_RADIO     QUERY_KG_INTERFACE

#define MUSIC_PLAY_NOW        "play_now"
#define MUSIC_PLAY_LATER      "play_later"

#define MUSIC_NONE_LIST       -1
#define MUSIC_NORMAL_LIST     0
#define MUSIC_LOVEST_LIST     1
#define MUSIC_NETWORK_LIST    2
#define MUSIC_RECENT_LIST     3

#define MUSIC_LRC_PAGE        1
#define MUSIC_SEARCH_PAGE     0

#endif // MUSICOTHERDEFINE_H
