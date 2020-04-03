/*
 * @file    emu68/inst68.c
 * @brief   68000 instructions emulation
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

#include "inst68.h"
#include "assert68.h"
#include "srdef68.h"
#include "struct68.h"
#include "excep68.h"
#include "cc68.h"
#include "mem68.h"
#include "emu68.h"

/* ,-----------.
   | Exception |
   `-----------' */

#include "inl68_exception.h"

void exception68(emu68_t * const emu68, const int vector, const int level)
{
  if ( vector < 0x100 ) {
    /* Standard 68k exceptions */

    int savesr = REG68.sr;              /* save sr as it was */
    int savest = emu68->status;         /* save emu68 runstate */

    emu68->status = EMU68_XCT;         /* enter exception stat      */
    REG68.sr &= ~SR_T;                 /* no TRACE                  */
    REG68.sr |=  SR_S;                 /* Supervisor                */

    if ( savest == EMU68_XCT &&
         ( vector == BUSERR_VECTOR || vector == ADRERR_VECTOR ) ) {
      /* That's a double fault (exception produced an exception) */
      emu68->status = EMU68_HLT;        /* Halt processor */
      /* Let user know. */
      if (emu68->handler)
        emu68->handler(emu68, HWHALT_VECTOR, emu68->cookie);
      /* If user did not override notify the error. */
      if (emu68->status == EMU68_HLT)
        emu68_error_add(emu68, "double-fault @$%06x vector:%$x",
                        emu68->inst_pc, vector);
      return;
    } else if ( vector == RESET_VECTOR ) {
      /* Reset reload SP and PC */
      REG68.sr  |= SR_I;
      REG68.a[7] = read_L(RESET_SP_VECTOR << 2);
      REG68.pc   = read_L(RESET_PC_VECTOR << 2);
    } else {
      /* Standard exception, set IPL and trigger exception. */
      if ( (unsigned int)level < 8u ) {
        SET_IPL(REG68.sr,level);
      }
      pushl(REG68.pc);
      pushw(savesr);
      REG68.pc  = read_L(vector << 2);

      /* Back to saved runstate (not totally sure about that). */
      emu68->status = savest;
    }
  }

  /* Finally call the user handler for all kind of exceptions. */
  if (emu68->handler)
    emu68->handler(emu68, vector, emu68->cookie);

}

void buserror68(emu68_t * const emu68, const int addr, const int mode)
{
  inl_buserror68(emu68, addr, mode);
}

void linea68(emu68_t * const emu68)
{
  inl_linea68(emu68);
}

void linef68(emu68_t * const emu68)
{
  inl_linef68(emu68);
}


/* ,------------.
   | Arithmetic |
   `------------' */

#include "inl68_arithmetic.h"

int68_t add68(emu68_t * const emu68, const int68_t s, int68_t d, int68_t c)
{
  return inl_add68(emu68, s, d, c);
}

int68_t sub68(emu68_t * const emu68, const int68_t s, int68_t d, int68_t c)
{
  return inl_sub68(emu68, s, d, c);
}

void cmp68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  inl_cmp68(emu68, s, d);
}

int68_t muls68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  return inl_muls68(emu68, s, d);
}

int68_t mulu68(emu68_t * const emu68, const uint68_t s, uint68_t d)
{
  return inl_mulu68(emu68, s, d);
}

int68_t divs68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  return inl_divs68(emu68, s, d);
}

int68_t divu68(emu68_t * const emu68, const uint68_t s, uint68_t d)
{
  return inl_divu68(emu68, s, d);
}

int68_t clr68(emu68_t * const emu68)
{
  return inl_clr68(emu68);
}

int68_t neg68(emu68_t * const emu68, int68_t d, int68_t c)
{
  return inl_neg68(emu68, d, c);
}


/* ,-------.
   | Logic |
   `-------' */

#include "inl68_logic.h"

int68_t and68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  return inl_and68(emu68, s, d);
}

int68_t orr68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  return inl_orr68(emu68, s, d);
}

int68_t eor68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  return inl_eor68(emu68, s, d);
}

int68_t not68(emu68_t * const emu68, int68_t d)
{
  return inl_not68(emu68, d);
}


/* ,----------------.
   | System Control |
   `----------------' */

#include "inl68_systctrl.h"

void illegal68(emu68_t * const emu68)
{
  inl_illegal68(emu68);
}

void trapv68(emu68_t * const emu68)
{
  inl_trapv68(emu68);
}

void trap68(emu68_t * const emu68, const int trap_n)
{
  inl_trap68(emu68, trap_n);
}

void chk68(emu68_t * const emu68, const int68_t a, const int68_t b)
{
  inl_chk68(emu68, a, b);
}

void andtosr68(emu68_t * const emu68, int68_t v)
{
  inl_andtosr68(emu68, v);
}

void orrtosr68(emu68_t * const emu68, int68_t v)
{
  inl_orrtosr68(emu68, v);
}

void eortosr68(emu68_t * const emu68, int68_t v)
{
  inl_eortosr68(emu68, v);
}

void reset68(emu68_t * const emu68)
{
  inl_reset68(emu68);
}

void stop68(emu68_t * const emu68)
{
  inl_stop68(emu68);
}


/* ,------------------.
   | Bit manipulation |
   `------------------' */

#include "inl68_bitmanip.h"

void btst68(emu68_t * const emu68, const int68_t v, const int bit)
{
  inl_btst68(emu68, v, bit);
}

int68_t bset68(emu68_t * const emu68, const int68_t v, const int bit)
{
  return inl_bset68(emu68, v, bit);
}

int68_t bclr68(emu68_t * const emu68, const int68_t v, const int bit)
{
  return inl_bclr68(emu68, v, bit);
}

int68_t bchg68(emu68_t * const emu68, const int68_t v, const int bit)
{
  return inl_bchg68(emu68, v, bit);
}


/* ,----------------------.
   | Binary Coded Decimal |
   `----------------------' */

#include "inl68_bcd.h"

int68_t abcd68(emu68_t * const emu68, const int68_t a, int68_t b)
{
  return inl_abcd68(emu68, a, b);
}

int68_t sbcd68(emu68_t * const emu68, const int68_t a, int68_t b)
{
  return inl_sbcd68(emu68, a, b);
}

int68_t nbcd68(emu68_t * const emu68, int68_t a)
{
  return inl_nbcd68(emu68, a);
}


/* ,-----------------.
   | Program Control |
   `-----------------' */

#include "inl68_progctrl.h"

void rts68(emu68_t * const emu68)
{
  inl_rts68(emu68);
}

void rte68(emu68_t * const emu68)
{
  inl_rte68(emu68);
}

void rtr68(emu68_t * const emu68)
{
  inl_rtr68(emu68);
}

void bsr68(emu68_t * const emu68, const addr68_t addr)
{
  inl_bsr68(emu68, addr);
}

#define BCC_TEMPLATE(CC)                                                \
  static void bcc_##CC(emu68_t * const emu68, const addr68_t addr) {     \
  if (inl_is_cc##CC(REG68.sr))                                           \
    REG68.pc = addr;                                                    \
  }

static void bcc_0(emu68_t * const emu68, const addr68_t addr)
{
  REG68.pc = addr;
}

static void bcc_1(emu68_t * const emu68, const addr68_t addr)
{
  inl_bsr68(emu68, addr);
}

BCC_TEMPLATE(2)
BCC_TEMPLATE(3)
BCC_TEMPLATE(4)
BCC_TEMPLATE(5)
BCC_TEMPLATE(6)
BCC_TEMPLATE(7)
BCC_TEMPLATE(8)
BCC_TEMPLATE(9)
BCC_TEMPLATE(A)
BCC_TEMPLATE(B)
BCC_TEMPLATE(C)
BCC_TEMPLATE(D)
BCC_TEMPLATE(E)
BCC_TEMPLATE(F)

void (* const bcc68[16])(emu68_t * const, const addr68_t) = {
  bcc_0, bcc_1, bcc_2, bcc_3, bcc_4, bcc_5, bcc_6, bcc_7,
  bcc_8, bcc_9, bcc_A, bcc_B, bcc_C, bcc_D, bcc_E, bcc_F
};

void jmp68(emu68_t * const emu68, const addr68_t addr)
{
  inl_jmp68(emu68, addr);
}

void jsr68(emu68_t * const emu68, const addr68_t addr)
{
  inl_jsr68(emu68, addr);
}

void nop68(emu68_t * const emu68)
{
  inl_nop68(emu68);
}

void tst68(emu68_t * const emu68, const int68_t a)
{
  inl_tst68(emu68, a);
}

int68_t tas68(emu68_t * const emu68, const int68_t d)
{
  return inl_tas68(emu68, d);
}

#define DBCC_TEMPLATE(CC)                                               \
  static void dbcc_##CC(emu68_t * const emu68, const int dn) {          \
    const uint68_t pc = REG68.pc;                                       \
    if (!inl_is_cc##CC(REG68.sr)) {                                     \
      int68_t a = (u16)(REG68.d[dn]-1);                                 \
      REG68.d[dn] = (REG68.d[dn] & 0xFFFF0000) | a;                     \
      if (a != 0xFFFF) {                                                \
        REG68.pc = pc + get_nextw();                                    \
      } else {                                                          \
        REG68.pc = pc + 2;                                              \
      }                                                                 \
    } else {                                                            \
      REG68.pc = pc + 2;                                                \
    }                                                                   \
  }

DBCC_TEMPLATE(0)
DBCC_TEMPLATE(1)
DBCC_TEMPLATE(2)
DBCC_TEMPLATE(3)
DBCC_TEMPLATE(4)
DBCC_TEMPLATE(5)
DBCC_TEMPLATE(6)
DBCC_TEMPLATE(7)
DBCC_TEMPLATE(8)
DBCC_TEMPLATE(9)
DBCC_TEMPLATE(A)
DBCC_TEMPLATE(B)
DBCC_TEMPLATE(C)
DBCC_TEMPLATE(D)
DBCC_TEMPLATE(E)
DBCC_TEMPLATE(F)

void (* const dbcc68[])(emu68_t * const, const int) = {
  dbcc_0, dbcc_1, dbcc_2, dbcc_3, dbcc_4, dbcc_5, dbcc_6, dbcc_7,
  dbcc_8, dbcc_9, dbcc_A, dbcc_B, dbcc_C, dbcc_D, dbcc_E, dbcc_F
};

#define SCC_TEMPLATE(CC)                                                \
  static int scc_##CC(emu68_t * const emu68)                            \
  {                                                                     \
    return (u8) -inl_is_cc##CC(REG68.sr);                               \
  }

SCC_TEMPLATE(0)
SCC_TEMPLATE(1)
SCC_TEMPLATE(2)
SCC_TEMPLATE(3)
SCC_TEMPLATE(4)
SCC_TEMPLATE(5)
SCC_TEMPLATE(6)
SCC_TEMPLATE(7)
SCC_TEMPLATE(8)
SCC_TEMPLATE(9)
SCC_TEMPLATE(A)
SCC_TEMPLATE(B)
SCC_TEMPLATE(C)
SCC_TEMPLATE(D)
SCC_TEMPLATE(E)
SCC_TEMPLATE(F)

int (* const scc68[])(emu68_t * const) = {
  scc_0, scc_1, scc_2, scc_3, scc_4, scc_5, scc_6, scc_7,
  scc_8, scc_9, scc_A, scc_B, scc_C, scc_D, scc_E, scc_F
};

/* ,----------.
   | Shifting |
   `----------' */

#include "inl68_shifting.h"

void swap68(emu68_t * const emu68, const int dn)
{
  inl_swap68(emu68, dn);
}

int68_t lsl68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  return inl_lsl68(emu68, d, s, l);
}

int68_t lsr68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  return inl_lsr68(emu68, d, s, l);
}

int68_t asl68(emu68_t * const emu68, int68_t d, int s, const int l)
{
  return inl_asl68(emu68, d, s, l);
}

int68_t asr68(emu68_t * const emu68, int68_t d, int s, const int l)
{
  return inl_asr68(emu68, d, s, l);
}

int68_t rol68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  return inl_rol68(emu68, d, s, l);
}

int68_t ror68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  return inl_ror68(emu68, d, s, l);
}

int68_t roxl68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  return inl_roxl68(emu68, d, s, l);
}

int68_t roxr68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  return inl_roxr68(emu68, d, s, l);
}

/* ,-----------.
   | Data Move |
   `-----------' */

#include "inl68_datamove.h"

void exg68(emu68_t * const emu68, const int reg0, const int reg9)
{
  inl_exg68(emu68, reg0, reg9);
}

addr68_t lea68(emu68_t * const emu68, const  int mode, const int reg)
{
  return inl_lea68(emu68, mode, reg);
}

addr68_t pea68(emu68_t * const emu68, const  int mode, const int reg)
{
  return inl_pea68(emu68, mode, reg);
}

void link68(emu68_t * const emu68, const int reg)
{
  inl_link68(emu68, reg);
}

void unlk68(emu68_t * const emu68, const int reg)
{
  inl_unlk68(emu68, reg);
}
