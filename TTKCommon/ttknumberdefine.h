#ifndef TTKNUMBERDEFINE_H
#define TTKNUMBERDEFINE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
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

#define MT_MS           1
#define MT_ONCE         50 * MT_MS

#define MT_S            1
#define MT_S2MS         1000
#define MT_MS2US        1000
#define MT_S2US         (MT_S2MS * MT_MS2US)

#define MT_M            60
#define MT_M2S          MT_M
#define MT_M2MS         (MT_M2S * MT_S2MS)

#define MT_H            60
#define MT_H2M          MT_H
#define MT_H2S          (MT_H2M * MT_M2S)
#define MT_H2MS         (MT_H2S * MT_S2MS)

#define MT_D            24
#define MT_D2H          MT_D
#define MT_D2M          (MT_D2H * MT_H2M)
#define MT_D2S          qint64(MT_D2M * MT_M2S)
#define MT_D2MS         qint64(MT_D2S * MT_S2MS)
//

#define MH_BS           1

#define MH_B            1
#define MH_B2BS         8

#define MH_KB           1024
#define MH_KB2B         MH_KB
#define MH_KB2BS        (MH_KB2B * MH_B2BS)

#define MH_MB           1024
#define MH_MB2KB        MH_MB
#define MH_MB2B         (MH_MB2KB * MH_KB2B)
#define MH_MB2BS        (MH_MB2B * MH_B2BS)

#define MH_GB           1024
#define MH_GB2MB        MH_GB
#define MH_GB2KB        qint64(MH_GB2MB * MH_MB2KB)
#define MH_GB2B         qint64(MH_GB2KB * MH_KB2B)
#define MH_GB2BS        qint64(MH_GB2B * MH_B2BS)

#define MH_TB           1024
#define MH_TB2GB        MH_TB
#define MH_TB2MB        qint64(MH_TB2GB * MH_GB2MB)
#define MH_TB2KB        qint64(MH_TB2MB * MH_MB2KB)
#define MH_TB2B         qint64(MH_TB2KB * MH_KB2B)
#define MH_TB2BS        qint64(MH_TB2B * MH_B2BS)
//

#define MA_0            0
#define MA_30           30
#define MA_45           45
#define MA_60           60
#define MA_90           90
#define MA_120          120
#define MA_180          180
#define MA_270          270
#define MA_360          360
//

#define MB_0            0
#define MB_32           32
#define MB_64           64
#define MB_96           96
#define MB_128          128
#define MB_192          192
#define MB_250          250
#define MB_320          320
#define MB_500          500
#define MB_750          750
#define MB_1000         1000
//


#endif // TTKNUMBERDEFINE_H
