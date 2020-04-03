/*
 * @file    emu68/getea68.c
 * @brief   get 68000 addressing mode effective address
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

#include "error68.h"
#include "assert68.h"
#include "excep68.h"
#include "mem68.h"

EMU68_EXTERN
void exception68(emu68_t * const emu68, const int vector, const int level);

static addr68_t ea_error(emu68_t * const emu68, const int reg)
{
  assert(EMU68_BREAK);
  /* $$$ must trigger an exception just not sure which exactly */
  exception68(emu68, ADRERR_VECTOR, -1);
  return 0;
}

/* (AN) */
static addr68_t ea_inAN(emu68_t * const emu68, const int reg)
{
  return (s32) REG68.a[reg];
}

/* (AN)+ */
static addr68_t ea_inANpb(emu68_t * const emu68, const int reg)
{
  addr68_t addr = (s32) REG68.a[reg];
  REG68.a[reg]  = (u32) ( REG68.a[reg] + 1 + (reg==7) );
  return addr;
}

static addr68_t ea_inANpw(emu68_t * const emu68, const int reg)
{
  addr68_t addr = (s32) REG68.a[reg];
  REG68.a[reg]  = (u32) ( REG68.a[reg] + 2 );
  return addr;
}

static addr68_t ea_inANpl(emu68_t * const emu68, const int reg)
{
  addr68_t addr = (s32) REG68.a[reg];
  REG68.a[reg]  = (u32) ( REG68.a[reg] + 4 );
  return addr;
}

/* -(AN) */
static addr68_t ea_inmANb(emu68_t * const emu68, const int reg)
{
  return (s32) ( REG68.a[reg] = (u32) ( REG68.a[reg] - 1 - (reg==7) ) );
}

static addr68_t ea_inmANw(emu68_t * const emu68, const int reg)
{
  return (s32) ( REG68.a[reg] = (u32) ( REG68.a[reg] - 2 ) );
}

static addr68_t ea_inmANl(emu68_t * const emu68, const int reg)
{
  return (s32) ( REG68.a[reg] = (u32) ( REG68.a[reg] - 4 ) );
}

/* d(AN) */
static addr68_t ea_indAN(emu68_t * const emu68, const int reg)
{
  return (s32) ( REG68.a[reg] + get_nextw() );
}

/* d(AN,Xi) */
static addr68_t ea_inANXI(emu68_t * const emu68, const int reg)
{
  int68_t w = get_nextw();
  int68_t reg2;
  reg2 = ( w >> 12 ) & 15;
  reg2 = ( w & 04000 )
    ? (int68_t) (s32) REG68.d[reg2]
    : (int68_t) (s16) REG68.d[reg2]
    ;
  return (s32) ( REG68.a[reg] + (s8) w + reg2 );
}

/* ABS.W */
static addr68_t ea_inABSW(emu68_t * const emu68, const int reg)
{
  return get_nextw();
}

/* ABS.L */
static addr68_t ea_inABSL(emu68_t * const emu68, const int reg)
{
  return get_nextl();
}

/* d(PC) */
static addr68_t ea_inrelPC(emu68_t * const emu68, const int reg)
{
  addr68_t pc = (s32) REG68.pc;
  return (s32) ( pc + get_nextw() );
}

/* d(PC,Xi) */
static addr68_t ea_inPCXI(emu68_t * const emu68, const int reg)
{
  addr68_t  pc = (s32) REG68.pc;
  int68_t    w = get_nextw();
  int68_t reg2 = ( w >> 12 ) & 15;
  pc += ( w & 04000 )
    ? (int68_t) (s32) REG68.d[reg2]
    : (int68_t) (s16) REG68.d[reg2]
    ;
  pc += (s8) w;
  return (s32) pc;
}

/* # */
static addr68_t ea_inIMMb(emu68_t * const emu68, const int reg)
{
  addr68_t pc = (s32) REG68.pc;
  REG68.pc = (u32) ( pc + 2 );
  return (s32) ( pc + 1 );
}

static addr68_t ea_inIMMw(emu68_t * const emu68, const int reg)
{
  s32 pc = REG68.pc;
  REG68.pc = (u32) (pc + 2);
  return pc;
}

static addr68_t ea_inIMMl(emu68_t * const emu68, const int reg)
{
  s32 pc = REG68.pc;
  REG68.pc = (u32) ( pc + 4 );
  return pc;
}

/***************************
 *                          *
 * Opmode tables for Get EA *
 *                          *
 ***************************/

/* Mode 7 tables */

static addr68_t (* const ea_b7[8])(emu68_t * const, const int) =
{
  ea_inABSW, ea_inABSL, ea_inrelPC, ea_inPCXI,
  ea_inIMMb, ea_error, ea_error, ea_error
};

static addr68_t (* const ea_w7[8])(emu68_t * const, const int) =
{
  ea_inABSW, ea_inABSL, ea_inrelPC, ea_inPCXI,
  ea_inIMMw, ea_error, ea_error, ea_error
};

static addr68_t (* const ea_l7[8])(emu68_t * const, const int) =
{
  ea_inABSW, ea_inABSL, ea_inrelPC, ea_inPCXI,
  ea_inIMMl, ea_error, ea_error, ea_error
};

/* Mode 7 redirect functions */

static addr68_t ea_mode7b(emu68_t * const emu68, const int reg)
{
  return (ea_b7[reg])(emu68, reg);
}

static addr68_t ea_mode7w(emu68_t * const emu68, const int reg)
{
  return (ea_w7[reg])(emu68, reg);
}

static addr68_t ea_mode7l(emu68_t * const emu68, const int reg)
{
  return (ea_l7[reg])(emu68, reg);
}

/* Get EA tables */

addr68_t (* const get_eab68[8])(emu68_t * const, const int) =
{
  ea_error, ea_error, ea_inAN, ea_inANpb,
  ea_inmANb,ea_indAN,ea_inANXI,
  ea_mode7b,
};

addr68_t (* const get_eaw68[8])(emu68_t * const, const int) =
{
  ea_error, ea_error, ea_inAN, ea_inANpw,
  ea_inmANw,ea_indAN,ea_inANXI,
  ea_mode7w,
};

addr68_t (* const get_eal68[8])(emu68_t * const, const int) =
{
  ea_error, ea_error, ea_inAN, ea_inANpl,
  ea_inmANl,ea_indAN,ea_inANXI,
  ea_mode7l,
};

