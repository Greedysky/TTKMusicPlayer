#ifndef TTKNUMBERDEFINE_H
#define TTKNUMBERDEFINE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkglobal.h"

// ttk date time format define
#define TTK_TIME_INIT           "00:00"
#define TTK_TIME_FORMAT         "hh:mm"
#define TTK_TIMEZ_FORMAT        "hh:mm:ss"
#define TTK_YEAR_FORMAT         "yyyy-MM-dd"
#define TTK_YEARD_FORMAT        "yyyy.MM.dd"
#define TTK_YEAR_TIME_FORMAT    "yyyy-MM-dd hh:mm"
#define TTK_YEAR_TIMEZ_FORMAT   "yyyy-MM-dd hh:mm:ss"


// ttk item size define
#define TTK_ITEM_SIZE_S         25
#define TTK_ITEM_SIZE_M         30
#define TTK_ITEM_SIZE_L         40
#define TTK_ITEM_SIZE_XL        60


// ttk level define
#define TTK_NONE_LEVEL          -3
#define TTK_LOW_LEVEL           -2
#define TTK_NORMAL_LEVEL        -1
#define TTK_HIGH_LEVEL          999


// ttk buffer define
#define TTK_LOW_BUFFER          256
#define TTK_NORMAL_BUFFER       512
#define TTK_HIGH_BUFFER         1024


// ttk date time number define
#define TTK_DN_NS           1

#define TTK_DN_US           1
#define TTK_DN_US2NS        1000

#define TTK_DN_MS           1
#define TTK_DN_MS2US        1000
#define TTK_DN_MS2NS        (TTK_DN_MS2US * TTK_DN_US2NS)
#define TTK_DN_ONCE         (50 * TTK_DN_MS)

#define TTK_DN_S            1
#define TTK_DN_S2MS         1000
#define TTK_DN_S2US         (TTK_DN_S2MS * TTK_DN_MS2US)
#define TTK_DN_S2NS         (TTK_DN_S2US * TTK_DN_US2NS)

#define TTK_DN_M            1
#define TTK_DN_M2S          60
#define TTK_DN_M2MS         (TTK_DN_M2S * TTK_DN_S2MS)
#define TTK_DN_M2US         TTKInt64(TTK_DN_M2MS * TTK_DN_MS2US)
#define TTK_DN_M2NS         TTKInt64(TTK_DN_M2US * TTK_DN_US2NS)

#define TTK_DN_H            1
#define TTK_DN_H2M          60
#define TTK_DN_H2S          (TTK_DN_H2M * TTK_DN_M2S)
#define TTK_DN_H2MS         (TTK_DN_H2S * TTK_DN_S2MS)
#define TTK_DN_H2US         TTKInt64(TTK_DN_H2MS * TTK_DN_MS2US)
#define TTK_DN_H2NS         TTKInt64(TTK_DN_H2US * TTK_DN_US2NS)

#define TTK_DN_D            1
#define TTK_DN_D2H          24
#define TTK_DN_D2M          (TTK_DN_D2H * TTK_DN_H2M)
#define TTK_DN_D2S          (TTK_DN_D2M * TTK_DN_M2S)
#define TTK_DN_D2MS         TTKInt64(TTK_DN_D2S * TTK_DN_S2MS)
#define TTK_DN_D2US         TTKInt64(TTK_DN_D2MS * TTK_DN_MS2US)
#define TTK_DN_D2NS         TTKInt64(TTK_DN_D2US * TTK_DN_US2NS)


// ttk size number define
#define TTK_SN_BT           1

#define TTK_SN_B            1
#define TTK_SN_B2BT         8

#define TTK_SN_KB           1
#define TTK_SN_KB2B         1024
#define TTK_SN_KB2BS        (TTK_SN_KB2B * TTK_SN_B2BT)

#define TTK_SN_MB           1
#define TTK_SN_MB2KB        1024
#define TTK_SN_MB2B         (TTK_SN_MB2KB * TTK_SN_KB2B)
#define TTK_SN_MB2BT        (TTK_SN_MB2B * TTK_SN_B2BT)

#define TTK_SN_GB           1
#define TTK_SN_GB2MB        1024
#define TTK_SN_GB2KB        (TTK_SN_GB2MB * TTK_SN_MB2KB)
#define TTK_SN_GB2B         TTKInt64(TTK_SN_GB2KB * TTK_SN_KB2B)
#define TTK_SN_GB2BT        TTKInt64(TTK_SN_GB2B * TTK_SN_B2BT)

#define TTK_SN_TB           1
#define TTK_SN_TB2GB        1024
#define TTK_SN_TB2MB        (TTK_SN_TB2GB * TTK_SN_GB2MB)
#define TTK_SN_TB2KB        TTKInt64(TTK_SN_TB2MB * TTK_SN_MB2KB)
#define TTK_SN_TB2B         TTKInt64(TTK_SN_TB2KB * TTK_SN_KB2B)
#define TTK_SN_TB2BT        TTKInt64(TTK_SN_TB2B * TTK_SN_B2BT)


// ttk angle number define
#define TTK_AN_0            0
#define TTK_AN_30           30
#define TTK_AN_45           45
#define TTK_AN_60           60
#define TTK_AN_90           90
#define TTK_AN_120          120
#define TTK_AN_180          180
#define TTK_AN_270          270
#define TTK_AN_360          360


// ttk bitrate number define
#define TTK_BN_0            0
#define TTK_BN_32           32
#define TTK_BN_64           64
#define TTK_BN_96           96
#define TTK_BN_128          128
#define TTK_BN_192          192
#define TTK_BN_250          250
#define TTK_BN_320          320
#define TTK_BN_500          500
#define TTK_BN_750          750
#define TTK_BN_1000         1000


// ttk range number define
#define TTK_RN_MIN          0
#define TTK_RN_MAX          100


#endif // TTKNUMBERDEFINE_H
