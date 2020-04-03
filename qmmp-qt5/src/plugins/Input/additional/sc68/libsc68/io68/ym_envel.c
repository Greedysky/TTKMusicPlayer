/*
 * @file    ym_envel.c
 * @brief   YM-2149 emulator - YM-2149 envelop shapes table
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "emu68/type68.h"

#define _V(X) ( (X<<10) | (X<<5) | X )

#define UP \
  _V(000),_V(001),_V(002),_V(003),_V(004),_V(005),_V(006),_V(007), \
  _V(010),_V(011),_V(012),_V(013),_V(014),_V(015),_V(016),_V(017), \
  _V(020),_V(021),_V(022),_V(023),_V(024),_V(025),_V(026),_V(027), \
  _V(030),_V(031),_V(032),_V(033),_V(034),_V(035),_V(036),_V(037)

#define DW \
  _V(037),_V(036),_V(035),_V(034),_V(033),_V(032),_V(031),_V(030), \
  _V(027),_V(026),_V(025),_V(024),_V(023),_V(022),_V(021),_V(020), \
  _V(017),_V(016),_V(015),_V(014),_V(013),_V(012),_V(011),_V(010), \
  _V(007),_V(006),_V(005),_V(004),_V(003),_V(002),_V(001),_V(000)

#define LO \
  _V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000), \
  _V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000), \
  _V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000), \
  _V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000),_V(000)

#define HI \
  _V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037), \
  _V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037), \
  _V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037), \
  _V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037),_V(037)

static const u16 dwdw[96] = { DW,DW,DW };
static const u16 dwhi[96] = { DW,HI,HI };
static const u16 dwlo[96] = { DW,LO,LO };
static const u16 dwup[96] = { DW,UP,DW };
static const u16 updw[96] = { UP,DW,UP };
static const u16 uphi[96] = { UP,HI,HI };
static const u16 uplo[96] = { UP,LO,LO };
static const u16 upup[96] = { UP,UP,UP };

const u16 * ym_envelops[16] =
{
  /* 0 \_ */ dwlo,
  /* 1 \_ */ dwlo,
  /* 2 \_ */ dwlo,
  /* 3 \_ */ dwlo,
  /* 4 /_ */ uplo,
  /* 5 /_ */ uplo,
  /* 6 /_ */ uplo,
  /* 7 /_ */ uplo,
  /* 8 \\ */ dwdw,
  /* 9 \_ */ dwlo,
  /* A \/ */ dwup,
  /* B \- */ dwhi,
  /* C // */ upup,
  /* D /- */ uphi,
  /* E /\ */ updw,
  /* F /_ */ uplo
};
