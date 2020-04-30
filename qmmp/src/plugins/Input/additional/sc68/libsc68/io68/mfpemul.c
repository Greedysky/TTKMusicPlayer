/*
 * @file    mfpemul.c
 * @brief   MFP 68901 emulator (Atari ST timers)
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

#include "mfpemul.h"
#include <sc68/file68_msg.h>
#include <assert.h>

#define CPU    8010612u                 /* or 8000000u or 80006400u ? */
#define BGM    MFP_BOGO_MUL
#define BGD    MFP_BOGO_DIV

#define cpp(V)      ((V)*prediv_width[(int)ptimer->tcr])
#define bogotohz(V) ( ( (CPU)*BGM ) / (uint68_t)(V) )
#define timerfrq(V) bogotohz(cpp(V))

#define MYHD "mfp    : "

#define MFP_VECTOR_BASE (mfp->map[0x17] & 0xF0)
#define SEI             (mfp->map[0x17] & 0x08)
#define AEI             (!SEI)

#ifndef DEBUG_MFP_O
# define DEBUG_MFP_O 0
#endif
int mfp_cat = msg68_DEFAULT;

/* Define for more accurate emulation (about pending bit...)
   $$$ May be broken $$$
*/
#undef _MFPIO68_SUPER_EMUL_

/* About cycles.
 *
 *   The MFP timers use a different clock. Its frequency is not a
 *   multiple of the 68K one (~8Mhz).
 *   In order to convert 68K cycles to MFP ones the emulator use an
 *   internal cycle unit : a "BOGO" cycle.
 *   - 1 "BOGO" cycle => BGM (256) "8mhz 68K" cycles
 *   - 1 "BOGO" cycle => BGD (834) "mfp" cycle
 */

static const mfp_timer_def_t timer_def[4] =
{
/*                 Vector   level bit   chan  letter */
  /* Timer A */  { 0x34>>2, 6,    1<<5, 0,    'A'    },
  /* Timer B */  { 0x20>>2, 6,    1<<0, 0,    'B'    },
  /* Timer C */  { 0x14>>2, 6,    1<<5, 2,    'C'    },
  /* Timer D */  { 0x10>>2, 6,    1<<4, 2,    'D'    },
};

/* MFP prediviser value in ``bogo-cycle''. */
static const bogoc68_t prediv_width[8] = {
   0*BGD,   4*BGD,  10*BGD,  16*BGD,
  50*BGD,  64*BGD, 100*BGD, 200*BGD
};

/* Which timer will first interrupt ?
 * It's the running timer (tcr!=0) with the lowest Cycle To Interrupt.
 */
static mfp_timer_t * find_next_int(const mfp_t * const mfp)
{
  const mfp_timer_t *ptimer, *itimer;

  /* find first running timer */
  for(itimer=0, ptimer=mfp->timers;
      ptimer<mfp->timers+4 && !ptimer->tcr;
      ++ptimer)
    ;
  if (ptimer < mfp->timers+4) {
    /* Got one. */
    bogoc68_t cti = ptimer->cti;
    itimer = ptimer;
    /* Now testing remaining timers for a lower CTI */
    while (++ptimer < mfp->timers+4) {
      if (ptimer->tcr && ptimer->cti<cti) {
        cti = ptimer->cti;
        itimer = ptimer;
      }
    }
  }
  return (mfp_timer_t *)itimer;
}

/* Number of bogo-cycle before this timer interruption. */
/* static inline bogoc68_t timer_ncti(const mfp_timer_t * const ptimer) */
/* { */
/*   return ptimer->tdr_cur * ptimer->psc_width - ptimer->psc_cnt; */
/* } */

/* Get number of tdr_cur decrements after nbogoc cycle. */
/* static inline uint68_t timer_ntdr(const mfp_timer_t * const ptimer, */
/*                                const bogoc68_t nbogoc) */
/* { */
/* ncti = tdr_cur * psc_width - psc_cnt  */
/* ncti + psc_cnt = tdr_cur * psc_width   */
/* tdr_cur = (ncti + psc_cnt) / psc_width */
/*   return (nbogoc+ptimer->psc_cnt) / ptimer->psc_width; */
/* } */


/* ,-----------------------------------------------------------------.
 * |                    MFP timer register access                    |
 * `-----------------------------------------------------------------'
 */

/*
  <psw>
  I  4  |  3  |  2  |  1  I  4  |  3  |  2  |  1  I  4  |  3  |  2  |  1  I
  444444333333222222111111444444333333222222111111444444333333222222111111
  BBBBBBAAAAAA999999888888777777666666555555444444333333222222111111000000
*/

static inline
int timer_get_tdr(const mfp_timer_t * const ptimer, const bogoc68_t bogoc)
{
  assert(ptimer->cti >= bogoc);
  assert(ptimer->tcr > 0 && ptimer->tcr < 8);
  if (1) {
    const bogoc68_t cti = ptimer->cti - bogoc;       /* cycles to interrupt */
    const uint68_t  psw = prediv_width[ptimer->tcr]; /* cycles count-down   */
    const uint68_t  cnt = cti/psw;                   /* count-down          */
    const uint68_t  tdr = cnt%ptimer->tdr_res+1;
    return tdr;
  }
}

/* Control register changes, adjust ``cti'' (cycle to next interrupt)
 *
 *    This case is a bit tricky : Changing timer prescale on the fly
 *    may have unpredictable result mostly because we dunno how
 *    prescaler works exactly. Here I assume the prescaler is
 *    resetted.
 *
 *    !!! chipmon of synergy does !!!
 *
 */
static inline
void reconf_timer(mfp_timer_t * const ptimer, int tcr, const bogoc68_t bogoc)
{
#if !defined(NDEBUG) || !defined(CPP_SUPPORTS_VA_MACROS)
  uint_t          frq = timerfrq(ptimer->tdr_res); /* old frequency       */
#endif
  const bogoc68_t cti = ptimer->cti - bogoc;       /* cycles to interrupt */
  const uint_t    psw = prediv_width[ptimer->tcr]; /* cycles count-down   */
  const uint_t    cnt = cti/psw;                   /* count-down          */
  const uint_t    psr = cti % psw;
  /* const uint68_t  psc = psw-psr; */

  /* cnt%ptimer->tdr_res+1; no MODULO since TDR may have change and anyway
     cti was calculated with 1 timer cycle !!! */
  const uint_t    tdr = cnt+1;
  const cycle68_t new_psw = prediv_width[(int)tcr];

  if (bogoc > ptimer->cti) {
    TRACE68(mfp_cat,
          MYHD "timer-%c -- reconf out of range -- @%u > cti:%u\n",
          ptimer->def.letter, (unsigned) bogoc, (unsigned) ptimer->cti);
    ptimer->cti = bogoc + psw * ptimer->tdr_res;
  } else {
    ptimer->cti = bogoc + psr + (tdr-1) * new_psw;
    ptimer->cti = bogoc + /* psr + */ (tdr/* -1 */) * new_psw;
  }

  ptimer->tcr = tcr;

  TRACE68(mfp_cat,
          MYHD "timer-%c -- reconf @%u cti:%u cpp:%u -- %u:%uhz\n",
          ptimer->def.letter, (unsigned) bogoc,
          (unsigned) ptimer->cti, (unsigned) cpp(ptimer->tdr_res),
          (unsigned) frq, (unsigned) timerfrq(ptimer->tdr_res));
}

/* Stop a running timer: tcr !0->0

   $$$ Must check whether:
   - 1) tdr is reset when timer stop.
   - 2) tdr keeps its current value and can be resume later.

   $$$ Testing with Scavenger Leif Sid Version tend to prove the 2)
   case is good. However the version of emulator used does not
   work properly with Scavenger DBA tune so this deduction may be
   a side effect of the bug.

   $$$ After verification on a real ST the previous assumption is
   true. Definitively mfp timers can resume. We just need to know
   more about the predivisor. Currently assuming the predivisor
   count is reset when the timer resumes which seems much more
   likely.

   $$$ Motorola Datasheet: The content of the timer's main counter are
   not affected although any residual count in prescaler is lost.

*/
static inline
void stop_timer(mfp_timer_t * const ptimer, const bogoc68_t bogoc)
{
  if (ptimer->tcr)
    ptimer->tdr_cur = timer_get_tdr(ptimer, bogoc);
  ptimer->tcr = 0;
  ptimer->psc = 0;
}

/* Resume a stopped timer: tcr 0->!0
 */
static inline
void resume_timer(mfp_timer_t * const ptimer, int tcr, bogoc68_t bogoc)
{
  ptimer->tcr = tcr;
  ptimer->cti = bogoc + ptimer->tdr_cur * prediv_width[tcr] - ptimer->psc;

  TRACE68(mfp_cat,
          MYHD "timer-%c  -- resume @%u cti:%u cpp:%u "
          "tdr:%u/%u psw:%u(%u) -- %uhz\n",
          ptimer->def.letter, (unsigned) bogoc, (unsigned) ptimer->cti,
          (unsigned) cpp(ptimer->tdr_res),
          (unsigned) ptimer->tdr_cur, (unsigned) ptimer->tdr_res,
          (unsigned) prediv_width[ptimer->tcr], (unsigned) ptimer->tcr,
          (unsigned) timerfrq(ptimer->tdr_res));
}

/* Read timer data register:

   $$$ TO VERIFY: May be reset value is reload when timer is stopped!
   Anyway the stop timer take care of that so returning tdr_cur
   here sohould be safe.

   $$$ VERIFIED: Both experience and Motorola Datasheet verify that
   the timer counter value is not affected unless TDR is written.

*/

int68_t mfp_get_tdr(mfp_t * const mfp, const int timer, const bogoc68_t bogoc)
{
  mfp_timer_t * const ptimer = &mfp->timers[timer&3];

  if (ptimer->tcr) {
    ptimer->tdr_cur = timer_get_tdr(ptimer, bogoc);
  }
  return(u8)ptimer->tdr_cur;
}

/* Write Timer Data Register:

   Changing the value of the TDR change the value of the internal
   reset latch. However when the timer is stopped the reset value is
   reload into TDR in the next MFP cycle (so it seems to be the case
   according to my test on a real ST). The tricky point is that as MFP
   and CPU clock are separate (for Atari-ST) reading the value just
   after a write can occur before the next MFP cycle in which case the
   current TDR (not the one written) is returned! Fortunatly we don't
   really need to emulate this since it is more a glitch than anything
   and no one should have used it!

   Motorola Datasheet: the TDRs contain the value of their respective
   main counter. This value was captured on the last low-to-high
   transistion of the data strobe pin. The main counter is initialized
   by writting the to the TDR. If the timer is stopped the data is
   loaded simultaneously into both TDR and maincounter. If the TDR is
   written to while the timer is enabled, the value is not loaded into
   the timer until the timer counts through 01. If a write is
   performed while the timer is counting through 01 then an
   INDETERMINATE value is loaded into the main counter.

*/
void mfp_put_tdr(mfp_t * const mfp, int timer, int68_t v, const bogoc68_t bogoc)
{
  mfp_timer_t * const  ptimer = &mfp->timers[timer&3];
#ifndef NDEBUG
  const uint_t old_tdr = ptimer->tdr_res;
#endif

  /* Interrupt when count down to 0 so 0 is 256 */
  v  = (u8)v; v += (!v)<<8;
  ptimer->tdr_res = v;

  if (!ptimer->tcr) {
    ptimer->tdr_cur = v;
    TRACE68(mfp_cat,
          MYHD "timer-%c -- reload TDR @%u -- %u\n",
            ptimer->def.letter, (unsigned) bogoc, (unsigned) ptimer->tdr_res);
  }
#ifndef NDEBUG
  else if (ptimer->tcr && v != old_tdr) {
    uint_t old_frq = timerfrq(old_tdr);
    TRACE68(mfp_cat,
            MYHD "timer-%c -- change @%u cti:%u psw:%u(%u) cpp:%u"
            " -- %u(%u) -> %u(%u)hz\n",
            ptimer->def.letter, (unsigned) bogoc, (unsigned) ptimer->cti,
            (unsigned) prediv_width[ptimer->tcr], (unsigned) ptimer->tcr,
            (unsigned) cpp(ptimer->tdr_res),
            (unsigned) old_frq, (unsigned) old_tdr,
            (unsigned) timerfrq(ptimer->tdr_res), (unsigned) ptimer->tdr_res);
  }
#endif
}

/* Write Timer Control Register

   $$$ From Motorola Datasheet: when resetting ta0/tb0 (set bit 4) of
   TDCRA/B the other bits in TCR must be written with their previous
   value to avoid altering the operating mode; this verify the fact
   than writting 2 times the same value in the predivisor does not
   affect its count.

*/
static void mfp_put_tcr_bogo(mfp_timer_t * const ptimer,
                             int v, const bogoc68_t bogoc)
{
  assert(v >= 0 && v < 8);
  if (v != ptimer->tcr) {
    if (!v) {
      stop_timer(ptimer,bogoc);
    } else if (!ptimer->tcr) {
      resume_timer(ptimer, v, bogoc);
    } else {
      reconf_timer(ptimer, v, bogoc);
    }
  }
}

void mfp_put_tcr(mfp_t * const mfp,
                 int timer, int68_t v, const bogoc68_t bogoc)
{
  assert( (timer & 3) == timer);
  if (timer < TIMER_C) {
    /* Timer A or B */
    mfp->map[0x19+(timer<<1)] = v &= 15;

    if (v >= 8) {
      /* PWM or ECM */
      TRACE68(mfp_cat,
              MYHD "timer-%c -- %s-mode not supported --  %02x\n",
              timer_def[timer].letter,
              v == 8 ? "event-count" : "pulse-width", v);
      assert(!"mfp mode not supported");
      v = 0;                            /* stop timer disable  */
    }
    mfp_put_tcr_bogo(mfp->timers+timer, v, bogoc);
  } else {
    /* Timer C and D do not have ECM nor PWM */
    mfp->map[0x1D] = v &= 0x77;
    mfp_put_tcr_bogo(mfp->timers+TIMER_C, v>>4, bogoc);
    mfp_put_tcr_bogo(mfp->timers+TIMER_D, v&07, bogoc);
  }
}

/* ,-----------------------------------------------------------------.
 * |                      MFP next interuption                       |
 * `-----------------------------------------------------------------'
 */

bogoc68_t mfp_nextinterrupt(const mfp_t * const mfp)
{
  const mfp_timer_t * itimer = find_next_int(mfp);
  const bogoc68_t bogoc =
    (!itimer) ? IO68_NO_INT : itimer->cti;
  return bogoc;
}

static inline
void timer_interrupt(mfp_t * const mfp, mfp_timer_t * const ptimer)
{
  /* Fill interrupt info struct. */
  ptimer->interrupt.vector = MFP_VECTOR_BASE + ptimer->def.vector;
  ptimer->interrupt.level  = ptimer->def.level;
  ptimer->interrupt.cycle  = ptimer->cti;
  ptimer->cti += prediv_width[ptimer->tcr] * ptimer->tdr_res;
  ptimer->tdr_cur = ptimer->tdr_res;
}

interrupt68_t * mfp_interrupt(mfp_t * const mfp, const bogoc68_t bogoc)
{
  mfp_timer_t * itimer;

  /* Better recheck which timer interrupts because previous interrupt
     code may have modified timer.
  */
  while ((itimer = find_next_int(mfp)) && itimer->cti < bogoc) {
    /* Have a candidate */
    mfp_timer_t * const ptimer = itimer;

    /* Process interruption. */
    timer_interrupt(mfp,ptimer);

#ifndef _MFPIO68_SUPER_EMUL_
    /* ======================== */
    /* Simple Emulation Version */
    /* ======================== */
    if (mfp->map[0x07+ptimer->def.channel] &
        mfp->map[0x13+ptimer->def.channel] &
        ptimer->def.bit )
    {
      ++ptimer->int_fall;
      return &ptimer->interrupt;
    }
#else
    /* ========================== */
    /* Accurate Emulation Version */
    /* ========================== */

    /* Test Interrupt Enable */
    if ( mfp->map[0x07+ptimer->channel] & ptimer->bit ) {
      /* Set Interrupt Pending Bit */
      mfp->map[0x0B+ptimer->channel] |= ptimer->bit;

      /* Test Interrupt In Service */
      if (!(mfp->map[0x0F+ptimer->channel]&ptimer->bit)) {
        /* SEI : Set Interrupt In Service */
        if (SEI) mfp->map[0x0B+ptimer->channel] |= ptimer->bit;
        /*  Test Interrupt Mask */
        if (mfp->map[0x13+ptimer->channel] & ptimer->bit) {
          ++ptimer->int_fall;
          return &ptimer->interrupt;
        }
      }
    }
#endif
    /* For some reason interruption was cancelled. */
    ++ptimer->int_mask;
  }
  return 0;
}

/* ,-----------------------------------------------------------------.
 * |                    MFP adjust cycle base                        |
 * `-----------------------------------------------------------------'
 */

void mfp_adjust_bogoc(mfp_t * const mfp, const bogoc68_t bogoc)
{
  mfp_timer_t *ptimer;

  if (!bogoc) return;
  for (ptimer = mfp->timers; ptimer != mfp->timers+4; ++ptimer) {
    if (ptimer->tcr) {
      if (ptimer->cti < bogoc) {
        TRACE68(mfp_cat,
              MYHD "timer-%c -- adjust -- cti:%u cycle:%u\n",
              ptimer->def.letter, (unsigned) ptimer->cti, (unsigned) bogoc);
      }
      assert(ptimer->cti >= bogoc);
      while (ptimer->cti < bogoc) {
        /* $$$ !!! SHOULD NOT HAPPEN !!! */
        ++ptimer->int_lost;
        ptimer->cti += cpp(ptimer->tdr_res);
      }
      if (ptimer->int_lost) {
        msg68_critical(MYHD "timer-%c -- adjust has lost interrupt -- %d\n",
                       ptimer->def.letter, ptimer->int_lost);
        ptimer->int_lost = 0;
      }
      ptimer->cti -= bogoc;
    }
  }
}

/* ,-----------------------------------------------------------------.
 * |                          MFP reset                              |
 * `-----------------------------------------------------------------'
 */

static void reset_timer(mfp_timer_t * const ptimer, bogoc68_t bogoc)
{
  ptimer->cti       = bogoc;
  ptimer->tcr       = 0;
  ptimer->psc       = 0;

  /* $$$ Motorola Datasheet: timer counters should not be cleared
     but we don't really care
  */
  ptimer->tdr_res   = 256;
  ptimer->tdr_cur   = ptimer->tdr_res;
  ptimer->int_lost  = 0;
  ptimer->int_mask  = 0;
  ptimer->int_fall  = 0;
}

static int mfp_reset_bogo(mfp_t * const mfp, const bogoc68_t bogoc)
{
  int i;

  for (i=0; i<(int)sizeof(mfp->map); i++) {
    mfp->map[i]=0;
  }
  mfp->map[0x17] = 0x40;
  mfp->map[0x01] = 0x80;
  for (i=0; i<4;i++) {
    reset_timer(mfp->timers+i, bogoc);
  }
  return 0;
}

int mfp_reset(mfp_t * const mfp, const bogoc68_t bogoc)
{
  return mfp_reset_bogo(mfp, bogoc);
}

/* ,-----------------------------------------------------------------.
 * |                          MFP setup                              |
 * `-----------------------------------------------------------------'
 */

static void setup_timer(mfp_timer_t * const ptimer, int idx, bogoc68_t bogoc)
{
  ptimer->def     = timer_def[idx&3];
  ptimer->tdr_res = 256;
  ptimer->tdr_cur = ptimer->tdr_res;
  reset_timer(ptimer, bogoc);
}

int mfp_setup(mfp_t * const mfp)
{
  int ret = -1;

  if (mfp) {
    setup_timer(&mfp->timers[0], 0, 0);
    setup_timer(&mfp->timers[1], 1, 0);
    setup_timer(&mfp->timers[2], 2, 0);
    setup_timer(&mfp->timers[3], 3, 0);
    ret = mfp_reset_bogo(mfp, 0);
  }
  return ret;
}

void mfp_cleanup(mfp_t * const mfp)
{
}


/* ,-----------------------------------------------------------------.
 * |                          MFP init                               |
 * `-----------------------------------------------------------------'
 */

int mfp_init(void)
{
  if (mfp_cat == msg68_DEFAULT)
    mfp_cat = msg68_cat("mfp","MFP-68901 emulator", DEBUG_MFP_O);
  return 0;
}

void mfp_shutdown(void)
{
  msg68_cat_free(mfp_cat);
  mfp_cat = msg68_DEFAULT;
}
