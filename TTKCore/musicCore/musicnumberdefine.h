#ifndef MUSICNUMBERDEFINE_H
#define MUSICNUMBERDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

#define MT_MS           1

#define MT_S            1
#define MT_S2MS         1000

#define MT_M            60
#define MT_M2S          MT_M
#define MT_M2MS         (MT_M2S*MT_S2MS)

#define MT_H            60
#define MT_H2M          MT_H
#define MT_H2S          (MT_H2M*MT_M2S)
#define MT_H2MS         (MT_H2S*MT_S2MS)

#define MT_D            24
#define MT_D2H          MT_D
#define MT_D2M          (MT_D2H*MT_H2M)
#define MT_D2S          qint64(MT_D2M*MT_M2S)
#define MT_D2MS         qint64(MT_D2S*MT_S2MS)
////////////////////////////////////////////////////

#define MH_BS           1

#define MH_B            1
#define MH_B2BS         8

#define MH_KB           1024
#define MH_KB2B         MH_KB
#define MH_KB2BS        (MH_KB2B*MH_B2BS)

#define MH_MB           1024
#define MH_MB2KB        MH_MB
#define MH_MB2B         (MH_MB2KB*MH_KB2B)
#define MH_MB2BS        (MH_MB2B*MH_B2BS)

#define MH_GB           1024
#define MH_GB2MB        MH_GB
#define MH_GB2KB        (MH_GB2MB*MH_MB2KB)
#define MH_GB2B         (MH_GB2KB*MH_KB2B)
#define MH_GB2BS        (MH_GB2B*MH_B2BS)

#define MH_TB           1024
#define MH_TB2GB        MH_TB
#define MH_TB2MB        (MH_TB2GB*MH_GB2MB)
#define MH_TB2KB        qint64(MH_TB2MB*MH_MB2KB)
#define MH_TB2B         qint64(MH_TB2KB*MH_KB2B)
#define MH_TB2BS        qint64(MH_TB2B*MH_B2BS)
////////////////////////////////////////////////////

#define MA_30           30
#define MA_45           45
#define MA_60           60
#define MA_90           90
#define MA_120          120
#define MA_180          180
#define MA_270          270
#define MA_360          360
////////////////////////////////////////////////////

#define MB_32           32
#define MB_64           64
#define MB_128          128
#define MB_192          192
#define MB_320          320
#define MB_500          500
#define MB_750          750
#define MB_1000         1000
////////////////////////////////////////////////////


#endif // MUSICNUMBERDEFINE_H
