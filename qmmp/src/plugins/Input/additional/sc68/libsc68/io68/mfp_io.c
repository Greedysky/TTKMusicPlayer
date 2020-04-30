/*
 * @file    mfp_io.c
 * @brief   MFP-68901 I/O plugin
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

#include "mfp_io.h"
#include "mfpemul.h"
#include <assert.h>

#ifdef DEBUG
# include <sc68/file68_msg.h>

static const char * const regnames[] = {
  "GPIP", /* 01 - General Purpose I/O Interrupt port     */
  "AER",  /* 03 - Active Edge Register                   */
  "DDR",  /* 05 - Data Direction Register                */
  "IERA", /* 07 - IERA (Interrupt Enable Register A)     */
  "IERB", /* 09 - IERB (Interrupt Enable Register B)     */
  "IPRA", /* 0B - IPRA (Interrupt Pending Register A)    */
  "IPRB", /* 0D - IPRB (Interrupt Pending Register B)    */
  "ISRA", /* 0F - ISRA (Interrupt In Service Register A) */
  "ISRB", /* 11 - ISRB (Interrupt In Service Register B) */
  "IMRA", /* 13 - IMRA (Interrupt Mask Register A)       */
  "IMRB", /* 15 - IMRB (Interrupt Mask Register B)       */
  "VR",   /* 17 - VR (Vector Register)                   */
  "TACR", /* 19 - TACR  (Timer A Control Register)       */
  "TBCR", /* 1B - TBCR  (Timer B Control Register)       */
  "TCDCR",/* 1D - TCDCR (Timer C/D Control Register)     */
  "TADR", /* 1F - TADR  (Timer A Data Register)          */
  "TBDR", /* 21 - TBDR  (Timer B Data Register)          */
  "TCDR", /* 23 - TCDR  (Timer C Data Register)          */
  "TDDR", /* 25 - TDDR  (Timer D Data Register)          */
  "SCR",  /* 27 - SCR (Synchronous Character Register)   */
  "UCR",  /* 29 - UCR,USART (Control Register)           */
  "RSR",  /* 2B - RSR (Receiver Status Register)         */
  "TSR",  /* 2D - TSR (Tranmitter Status Register)       */
  "UDR",  /* 2F - UDR,USART (DataRegister)               */
};

static const char * chnnames[16] = {
  "GPIP#0", "GPIP#1", "GPIP#2", "GPIP#3",
  "TIMER-D", "TIMER-C", "GPIP#6", "GPIP#5",
  "TIMER-B", "XMIT-ERR", "XMIT-EMPTY","RCV-ERR",
  "RCV-FULL", "TIMER-A", "GPIP#6", "GPIP#7"
};

#define MYHD "mfp    : "

static const char * regname(int reg)
{
  if (! (reg & 1) )
    return "!ODD";                      /* not even (odd) */
  reg >>= 1;
  if (reg < 0 || reg >= sizeof(regnames)/sizeof(*regnames))
    return "!OOR";                      /* out-of-rand */
  return regnames[reg];
}

extern int mfp_cat;

# define REPORTR(N) REPORTA(N,mfp->map[N])
# define REPORTA(N,V)                                   \
  TRACE68(mfp_cat, MYHD "R [%02x]  > $%02x (%s)\n",     \
          N, (unsigned) (V), regname(N))
# define REPORTW(N,V)                                   \
  TRACE68(mfp_cat, MYHD "W [%02x] <  $%02x (%s)\n",     \
          N, (unsigned)(V), regname(N))
# define MAYREPORT(N,V) if (mfp->map[N] != (V)) REPORTW(N,V)

#else
# define REPORTR(N)    for(;0;)
# define REPORTA(N,V)  for(;0;)
# define REPORTW(N,V)  for(;0;)
# define MAYREPORT(N,V) for(;0;)
#endif

typedef struct {
  io68_t io;
  mfp_t  mfp;
} mfp_io68_t;

/* $$$ Currently hardcoded for 8Mhz cpu */
#define cpu2bogo(mfpio,cycle) ((bogoc68_t)((cycle) * MFP_BOGO_MUL))
#define bogo2cpu(mfpio,bogoc) ((cycle68_t)((bogoc) / MFP_BOGO_MUL))

/* 00/01 GPIP   General Purpose I/O */
static int68_t mfpr_01(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(GPIP);
  return mfp->map[GPIP];
}
static void mfpw_01(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(GPIP,v);
  mfp->map[GPIP] = v;
}

/* 01/03 AER    Active Edge Register */
static int68_t mfpr_03(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(AER);
  return mfp->map[AER];
}
static void mfpw_03(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  /* $$$ Writing AER might trigger an interrupt */
  REPORTW(AER,v);
  mfp->map[AER]=v;
}

/* 02/05  DDR   Data Direction Register */
static int68_t mfpr_05(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(DDR);
  return mfp->map[DDR];
}
static void mfpw_05(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(DDR,v);
  mfp->map[DDR] = v;
}

/* 03/07 IERA   Interrupt Enable Register A */
static int68_t mfpr_07(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(IERA);
  return mfp->map[IERA];
}
static void mfpw_07(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  /* Disabling a line clears pending interrupt on that line */
  REPORTW(IERA,v);
  mfp->map[IPRA] &= v;
  mfp->map[IERA]  = v;
  REPORTW(IPRA,mfp->map[IPRA]);
}

/* 04/09  IERB  Interrupt Enable Register B */
static int68_t mfpr_09(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(IERB);
  return mfp->map[IERB];
}
static void mfpw_09(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  /* Disabling a line clears pending interrupt on that line */
  REPORTW(IERB,v);
  mfp->map[IPRB] &= v;
  mfp->map[IERB]  = v;
  REPORTW(IPRB,mfp->map[IPRB]);
}

/* 05/0B IPRA   Interrupt Pending Register A */
static int68_t mfpr_0B(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(IPRA);
  return mfp->map[IPRA];
}
static void mfpw_0B(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  /* Writing IP register only clears pending interrupts. */
  mfp->map[IPRA] &= v;
  REPORTW(IPRA,mfp->map[IPRA]);
}

/* 06/0D  IPRB  Interrupt Pending Register B */
static int68_t mfpr_0D(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(IPRB);
  return mfp->map[IPRB];
}
static void mfpw_0D(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  /* Writing IP register only clears pending interrupts. */
  mfp->map[IPRB] &= v;
  REPORTW(IPRA,mfp->map[IPRB]);
}

/* 07/0F ISRA   Interrupt In-Service Register A */
static int68_t mfpr_0F(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(ISRA);
  return mfp->map[ISRA];
}
static void mfpw_0F(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(ISRA,v);
  mfp->map[ISRA] = v;
}

/* 08/11 ISRB   Interrupt In-Service Register B */
static int68_t mfpr_11(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(ISRB);
  return mfp->map[ISRB];
}
static void mfpw_11(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(ISRB,v);
  mfp->map[ISRB] = v;
}

/* 09/13 IMRA   Interrupt Mask Register A */
static int68_t mfpr_13(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(IMRA);
  return mfp->map[IMRA];
}
static void mfpw_13(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(IMRA,v);
  mfp->map[IMRA] = v;
}

/* 0A/15 IMRB   InterrupT Mask Register B */
static int68_t mfpr_15(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(IMRB);
  return mfp->map[IMRB];
}
static void mfpw_15(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(IMRB,v);
  mfp->map[IMRB] = v;
}

/* 0B/17 VR     Vector Register */
static int68_t mfpr_17(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(VR);
  return mfp->map[VR];
}
static void mfpw_17(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  REPORTW(VR,v);
  mfp->map[VR] = v;
}

/* 0C/19 TACR   Timer A Control Register */
static int68_t mfpr_19(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(TACR);
  return mfp->map[TACR];
}
static void mfpw_19(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TACR,v);
  mfp_put_tcr(mfp, TIMER_A, v, bogoc);
}

/* 0D/1B TBCR   Timer B Control Register */
static int68_t mfpr_1B(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(TBCR);
  return mfp->map[TBCR];
}
static void mfpw_1B(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TBCR,v);
  mfp_put_tcr(mfp, TIMER_B, v, bogoc);
}

/* 0E/1D TCDCR  Timers C and D Control Register */
static int68_t mfpr_1D(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(TCDCR);
  return mfp->map[TCDCR];
}
static void mfpw_1D(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TCDCR,v);
  mfp_put_tcr(mfp, TIMER_C, v, bogoc);
}

/* 0F/1F TADR   Timer A Data Register */
static int68_t mfpr_1F(mfp_t * const mfp, const bogoc68_t bogoc) {
  const int68_t ret = mfp_get_tdr(mfp, TIMER_A, bogoc);
  REPORTA(TADR,ret);
  return ret;
}
static void mfpw_1F(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TADR,v);
  mfp_put_tdr(mfp, TIMER_A, v, bogoc);
}

/* 10/21 TBDR   Timer B Data Register */
static int68_t mfpr_21(mfp_t * const mfp, const bogoc68_t bogoc) {
  const int68_t ret = mfp_get_tdr(mfp, TIMER_B, bogoc);
  REPORTA(TBDR,ret);
  return ret;
}
static void mfpw_21(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TBDR,v);
  mfp_put_tdr(mfp, TIMER_B, v, bogoc);
}

/* 11/23 TCDR   Timer C Data Register */
static int68_t mfpr_23(mfp_t * const mfp, const bogoc68_t bogoc) {
  const int68_t ret = mfp_get_tdr(mfp, TIMER_C, bogoc);
  REPORTA(TCDR,ret);
  return ret;
}
static void mfpw_23(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TCDR,v);
  mfp_put_tdr(mfp, TIMER_C, v, bogoc);
}

/* 12/25 TDDR   Timer D Data Register */
static int68_t mfpr_25(mfp_t * const mfp, const bogoc68_t bogoc) {
  const int68_t ret = mfp_get_tdr(mfp, TIMER_D, bogoc);
  REPORTA(TDDR,ret);
  return ret;
}
static void mfpw_25(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{
  REPORTW(TDDR,v);
  mfp_put_tdr(mfp, TIMER_D, v, bogoc);
}

/* 13/27 SCR    Sync Character Register */
static int68_t mfpr_27(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(SCR);
  return mfp->map[SCR];
}
static void mfpw_27(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc) {
  mfp->map[SCR] = v;
}

/* 14/29 UCR    USART Control Register */
static int68_t mfpr_29(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(UCR);
  return mfp->map[UCR];
}
static void mfpw_29(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[UCR]=v; }

/* 15/2B RSR    Receiver Status Register */
static int68_t mfpr_2B(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(RSR);
  return mfp->map[RSR];
}
static void mfpw_2B(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[RSR]=v; }


/* 16/2D TSR    Transmitter Status Register */
static int68_t mfpr_2D(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(TSR);
  return mfp->map[TSR];
}
static void mfpw_2D(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[TSR]=v; }

/* 17/2F UDR    USART Data Register */
static int68_t mfpr_2F(mfp_t * const mfp, const bogoc68_t bogoc) {
  REPORTR(UDR);
  return mfp->map[UDR];
}
static void mfpw_2F(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[UDR]=v; }


/* Following access are not real MFP port. Not sure what happen. */
static int68_t mfpr_31(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x31];
}
static int68_t mfpr_33(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x33];
}
static int68_t mfpr_35(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x35];
}
static int68_t mfpr_37(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x37];
}
static int68_t mfpr_39(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x39];
}
static int68_t mfpr_3B(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x3B];
}
static int68_t mfpr_3D(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x3D];
}
static int68_t mfpr_3F(mfp_t * const mfp, const bogoc68_t bogoc) {
  return mfp->map[0x3F];
}
static void mfpw_31(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x31]=v; }
static void mfpw_33(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x33]=v; }
static void mfpw_35(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x35]=v; }
static void mfpw_37(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x37]=v; }
static void mfpw_39(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x39]=v; }
static void mfpw_3B(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x3B]=v; }
static void mfpw_3D(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x3D]=v; }
static void mfpw_3F(mfp_t * const mfp, const int68_t v, const bogoc68_t bogoc)
{ mfp->map[0x3F]=v; }

/* MFP read jump table */
static int68_t (* const mfpr_func[32])
(mfp_t * const, const bogoc68_t) =
{
  mfpr_01,mfpr_03,mfpr_05,mfpr_07,
  mfpr_09,mfpr_0B,mfpr_0D,mfpr_0F,
  mfpr_11,mfpr_13,mfpr_15,mfpr_17,
  mfpr_19,mfpr_1B,mfpr_1D,mfpr_1F,
  mfpr_21,mfpr_23,mfpr_25,mfpr_27,
  mfpr_29,mfpr_2B,mfpr_2D,mfpr_2F,
  mfpr_31,mfpr_33,mfpr_35,mfpr_37,
  mfpr_39,mfpr_3B,mfpr_3D,mfpr_3F,
};

/* MFP write jump table */
static void (* const mfpw_func[32])
(mfp_t * const, const int68_t, const bogoc68_t) =
{
  mfpw_01,mfpw_03,mfpw_05,mfpw_07,
  mfpw_09,mfpw_0B,mfpw_0D,mfpw_0F,
  mfpw_11,mfpw_13,mfpw_15,mfpw_17,
  mfpw_19,mfpw_1B,mfpw_1D,mfpw_1F,
  mfpw_21,mfpw_23,mfpw_25,mfpw_27,
  mfpw_29,mfpw_2B,mfpw_2D,mfpw_2F,
  mfpw_31,mfpw_33,mfpw_35,mfpw_37,
  mfpw_39,mfpw_3B,mfpw_3D,mfpw_3F,
};

static int68_t _mfp_readB(mfp_io68_t * const mfpio, const int addr,
                          const bogoc68_t bogoc)
{
  return !(addr&1)
    ? 0
    : mfpr_func[(addr>>1)&0x1f](&mfpio->mfp, bogoc)
    ;
}

static void mfpio_readB(io68_t * const io)
{
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  io->emu68->bus_data =
    _mfp_readB((mfp_io68_t *)io,
               io->emu68->bus_addr, bogoc);
}

static void mfpio_readW(io68_t * const io)
{
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  /* Expected EVEN addr for 16 bit access */
  io->emu68->bus_data =
    _mfp_readB((mfp_io68_t *)io,
               io->emu68->bus_addr+1, bogoc);
}

static void mfpio_readL(io68_t * const io)
{
  /* Expected EVEN addr for 16 bit access */
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  const int addr = io->emu68->bus_addr;

  io->emu68->bus_data =
    (_mfp_readB((mfp_io68_t *)io, addr+1, bogoc)<<16)
    |_mfp_readB((mfp_io68_t *)io, addr+3, bogoc);
}

static inline void _mfp_writeB(mfp_io68_t * const mfpio, const int addr,
                               const int68_t v, const bogoc68_t bogoc)
{
  if (!(addr&1)) return;
  mfpw_func[(addr>>1)&0x1f](&mfpio->mfp, (u8)v, bogoc);
}

static void mfpio_writeB(io68_t * const io)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  _mfp_writeB(mfpio, io->emu68->bus_addr, io->emu68->bus_data, bogoc);
}

/* Expected EVEN addr for 16 bit access */
static void mfpio_writeW(io68_t * const io)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  _mfp_writeB(mfpio, io->emu68->bus_addr+1, io->emu68->bus_data, bogoc);
}

/* Expected EVEN addr for 32 bit access */
static void mfpio_writeL(io68_t * const io)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  _mfp_writeB(mfpio,
              io->emu68->bus_addr+1, io->emu68->bus_data>>16, bogoc);
  _mfp_writeB((mfp_io68_t *)io,
              io->emu68->bus_addr+3, io->emu68->bus_data    ,bogoc);
}

static int mfpio_reset(io68_t * const io)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  const bogoc68_t bogoc = cpu2bogo(io,io->emu68->cycle);
  mfp_reset(&mfpio->mfp,bogoc);
  return 0;
}

static interrupt68_t * mfpio_interrupt(io68_t * const io,
                                       const cycle68_t cycle)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  const bogoc68_t bogoc = cpu2bogo(io,cycle);
  interrupt68_t * const inter =
    mfp_interrupt(&mfpio->mfp, bogoc);
  if (inter) {
    assert(inter->cycle < bogoc);
    inter->cycle = bogo2cpu(io,inter->cycle);
    assert(inter->cycle < cycle);
  }
  return inter;
}

static void mfpio_adjust_cycle(io68_t * const io,
                               const cycle68_t cycle)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  const bogoc68_t bogoc = cpu2bogo(io, cycle);
  mfp_adjust_bogoc(&mfpio->mfp, bogoc);
}

void mfpio_destroy(io68_t * const io)
{
  mfp_io68_t * const mfpio = (mfp_io68_t * const)io;
  if (io) {
    mfp_cleanup(&mfpio->mfp);
    emu68_free(io);
  }
}

static io68_t mfp_io =
{
  0,
  "MK-68901",
  0xFFFFFA00, 0xFFFFFA2F,
  mfpio_readB,mfpio_readW,mfpio_readL,
  mfpio_writeB,mfpio_writeW,mfpio_writeL,
  mfpio_interrupt,/* mfpio_nextinterrupt */0,
  mfpio_adjust_cycle,
  mfpio_reset,
  mfpio_destroy
};

int mfpio_init(int * argc, char ** argv)
{
  argc = argc;
  argv = argv;
  return mfp_init();
}

void mfpio_shutdown(void)
{
  mfp_shutdown();
}

io68_t * mfpio_create(emu68_t * const emu68)
{
  mfp_io68_t * mfpio = 0;

  if (emu68) {
    mfpio = emu68_alloc(sizeof(*mfpio));
    if (mfpio) {
      mfpio->io = mfp_io;
      mfp_setup(&mfpio->mfp);
    }
  }
  return &mfpio->io;
}
