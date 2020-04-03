/*
 * @file    mwemul.c
 * @brief   MicroWire/LMC emulator (STE sound)
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

/**
 * @todo
 *
 * - Check overflow in mix routine.
 * - Verify STE / YM volume ratio
 * - Special case for not mixing in Db_alone.
 * - And in the YM emul, skip emulation !!!
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "default.h"
#include "mwemul.h"

#include "emu68/assert68.h"
#include <sc68/file68_msg.h>

#ifndef DEBUG_MW_O
# define DEBUG_MW_O 0
#endif

/* static */
int mw_cat = msg68_DEFAULT;

#define MWHD "ste-mw : "

/* #define MW_CALCUL_TABLE 1 */

enum {
  MW_N_DECIBEL = 121,
  MW_MIX_FIX   = 10,
  MW_STE_MULT  = ((1<<MW_MIX_FIX)/4),
  MW_YM_MULT   = ((1<<MW_MIX_FIX)-MW_STE_MULT)
};

/* $$$/XXX THIS IS WRONG AND NEEDS TO BE FIXED */

static const int Db_alone[MW_N_DECIBEL] = {
  0x40000,0x32d64,0x28619,0x20137,0x197a9,0x143d1,0x10137,0xcc50,
  0xa24b,0x80e9,0x6666,0x5156,0x409c,0x3352,0x28c4,0x2061,
  0x19b8,0x146e,0x103a,0xce4,0xa3d,0x822,0x676,0x521,
  0x413,0x33c,0x292,0x20b,0x19f,0x14a,0x106,0xd0,
  0xa5,0x83,0x68,0x52,0x41,0x34,0x29,0x21,
  0x1a,0x14,0x10,0xd,0xa,0x8,0x6,0x5, 0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0
};

static const int Db_mix[MW_N_DECIBEL] = {
  0x10000,0xcb59,0xa186,0x804d,0x65ea,0x50f4,0x404d,0x3314,
  0x2892,0x203a,0x1999,0x1455,0x1027,0xcd4,0xa31,0x818,
  0x66e,0x51b,0x40e,0x339,0x28f,0x208,0x19d,0x148,
  0x104,0xcf,0xa4,0x82,0x67,0x52,0x41,0x34,
  0x29,0x20,0x1a,0x14,0x10,0xd,0xa,0x8,
  0x6,0x5,0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,
};

static const int Db_mix12[MW_N_DECIBEL] = {
  0x1027,0xcd4,0xa31,0x818,0x66e,0x51b,0x40e,0x339,
  0x28f,0x208,0x19d,0x148,0x104,0xcf,0xa4,0x82,
  0x67,0x52,0x41,0x34,0x29,0x20,0x1a,0x14,
  0x10,0xd,0xa,0x8,0x6,0x5,0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0
};

static void init_volume(void) { }


static mw_parms_t default_parms;

/* ,-----------------------------------------------------------------.
 * |                   Set/Get emulator engine                       |
 * `-----------------------------------------------------------------'
 */

static
const char * mw_engine_name(const int engine)
{
  switch (engine) {
  case MW_ENGINE_SIMPLE: return "SIMPLE";
  case MW_ENGINE_LINEAR: return "LINEAR";
  }
  return 0;
}

int mw_engine(mw_t * const mw, int engine)
{
  switch (engine) {

  case MW_ENGINE_QUERY:
    engine = mw ? mw->engine : default_parms.engine;
    break;

  default:
    msg68_warning(MWHD "invalid engine -- %d\n", engine);

  case MW_ENGINE_DEFAULT:
    engine = default_parms.engine;

  case MW_ENGINE_SIMPLE:
  case MW_ENGINE_LINEAR:
    *(mw ? &mw->engine : &default_parms.engine) = engine;
    msg68(mw_cat, MWHD "%s engine -- *%s*\n",
          mw ? "select" : "default",
          mw_engine_name(engine));
    break;
  }
  return engine;
}


/* ,-----------------------------------------------------------------.
 * |                   Set/Get replay frequency                      |
 * `-----------------------------------------------------------------'
 */
int mw_sampling_rate(mw_t * const mw, int hz)
{
  switch (hz) {

  case MW_HZ_QUERY:
    hz = mw ? mw->hz : default_parms.hz;
    break;

  case MW_HZ_DEFAULT:
    hz = default_parms.hz;

  default:
    if (hz < SPR_MIN)
      hz = SPR_MIN;
    else if (hz > SPR_MAX)
      hz = SPR_MAX;
    *(mw ? &mw->hz : &default_parms.hz) = hz;
    TRACE68(mw_cat, MWHD "%s sampling rate -- *%dhz*\n",
            mw ? "select" : "default", hz);
    break;
  }
  return hz;
}

/* ,-----------------------------------------------------------------.
 * | Set master volume   0=-80 Db, 40=0 Db                           |
 * | Set left volume     0=-40 Db, 20=0 Db                           |
 * | Set right volume    0=-40 Db, 20=0 Db                           |
 * | Set trebble         0=-12 Db, 6=0 Db, 12=12 Db                          |
 * | Set bass  0=-12 Db, 40=12 Db                          |
 * | Set mixer type : 0=-12 Db 1=YM+STE 2=STE-only 3=reserved        |
 * `-----------------------------------------------------------------'
 */

static
const char * const mixermode[4] = {
  "-12-Db", "YM+STE", "STE-ONLY", "RESERVED"
};

int mw_lmc_mixer(mw_t * const mw, int n)
{
  static const int * const table[3] = { Db_mix12, Db_mix, Db_alone };

  if (n == MW_LMC_QUERY) {
    n = mw->lmc.mixer;
  } else {
    n &= 3;
    mw->lmc.mixer = n;
    if (n != 3) {
      mw->db_conv = table[n];
    } else {
      msg68_warning(MWHD "invalid LMC mixer mode -- %d\n", n);
    }
  }
  TRACE68(mw_cat, MWHD "LMC mixer mode -- *%s*\n",
          mixermode[mw->lmc.mixer]);
  return n;
}

/* range [0..40] -> [80..0] (-dB) */
int mw_lmc_master(mw_t * const mw, int n)
{
  if (n == MW_LMC_QUERY) {
    n = ( 80 - mw->lmc.master ) >> 1;
  } else {
    if (n <  0) n = 0;
    if (n > 40) n = 40;
    mw->lmc.master = 80 - (n << 1);
    TRACE68(mw_cat, MWHD "LMC -- master -- *-%02ddB*\n", mw->lmc.master);
  }
  return n;
}

/* range [0..20] -> [40..0] (-dB) */
static int lmc_lr(mw_t * const mw, const int lr, int n)
{
  u8 * pval = lr ? &mw->lmc.left : &mw->lmc.right;

  if (n == MW_LMC_QUERY) {
    n = ( 40 - *pval ) >> 1;
  } else {
    if (n <  0) n = 0;
    if (n > 20) n = 20;
    *pval = 40 - (n << 1);
    mw->lmc.lr = ( mw->lmc.left + mw->lmc.right ) >> 1;
    TRACE68(mw_cat, MWHD "LMC -- %s channel -- *-%02ddB*\n",
            lr ? "left" : "right", *pval);
  }
  return n;
}

int mw_lmc_left(mw_t * const mw, int n)
{
  return lmc_lr(mw,0,n);
}

int mw_lmc_right(mw_t * const mw, int n)
{
  return lmc_lr(mw,1,n);
}

/* range [0..12] -> [12..0] (-dB) */
static int lmc_hl(mw_t * const mw, const int hl, int n)
{
  u8 * pval = hl ? &mw->lmc.high : &mw->lmc.low;

  if (n == MW_LMC_QUERY) {
    n = 12 - *pval;
  } else {
    if (n <  0) n = 0;
    if (n > 12) n = 12;
    *pval = 12 - n;
    TRACE68(mw_cat, MWHD "LMC -- %s pass filter -- *-%02ddB*\n",
            hl ? "high" : "low", *pval);
  }
  return n;
}

int mw_lmc_high(mw_t * const mw, int n)
{
  return lmc_hl(mw,0,n);
}

int mw_lmc_low(mw_t * const mw, int n)
{
  return lmc_hl(mw,1,n);
}

static int command_dispatcher(mw_t * const mw, int n)
{
  const int c = n & 0700;
  n &= 077;

  TRACE68(mw_cat, MWHD "dispatch -- %o:%02x\n", c>>6, (unsigned)n);
  switch(c) {
  case 0000:
    mw_lmc_mixer(mw, n&3);
    break;
  case 0100:
    mw_lmc_low(mw, n&15);
    break;
  case 0200:
    mw_lmc_high(mw, n&15);
    break;
  case 0300:
    mw_lmc_master(mw, n&63);
    break;
  case 0400:
    mw_lmc_right(mw, n&31);
    break;
  case 0500:
    mw_lmc_left(mw, n&31);
    break;
  default:
    TRACE68(mw_cat, MWHD "unknown command -- %o:%02x\n", c>>6, (unsigned)n);
    return -1;
  }
  return 0;
}

int mw_command(mw_t * const mw)
{
  uint_t ctrl, data, comm, bits, mask;

  if (!mw)
    return -1;

  ctrl = ( mw->map[MW_CTRL] << 8 ) + mw->map[MW_CTRL+1];
  data = ( mw->map[MW_DATA] << 8 ) + mw->map[MW_DATA+1];

  /* Clear data, keep control.
   * Normally data is shifted left until it clears whereas control
   * rotated left until it's back to its orginal value.
   */
  mw->map[MW_DATA] = mw->map[MW_DATA+1] = 0;

  TRACE68(mw_cat, MWHD "shifting -- %04x/%04x\n", data, ctrl);

  for (bits=comm=0, mask=0x8000; bits != 11 && mask; mask >>= 1)
    if (ctrl & mask) {
      comm = (comm << 1) | !!(data & mask);
      ++bits;
    }
  if (bits != 11) {
    msg68_warning(MWHD "missing bits -- %04x/%04x\n", data, ctrl);
    assert(bits == 11);
    return -1;
  }
  if (mask && (ctrl & (mask-1))) {
    msg68_warning(MWHD "too many bits -- %04x/%04x\n", data, ctrl);
    assert(0);
  }

  if ( (comm & 03000) != 02000 ) {
    msg68_warning(MWHD "wrong address (%d) -- %04x/%04x\n",
                  comm>>9, data,ctrl);
    assert(0);
    return -1;
  }

  return
    command_dispatcher(mw, comm & 0777);
}


/* ,-----------------------------------------------------------------.
 * |                         Microwire reset                        |
 * `-----------------------------------------------------------------'
 */

static void lmc_reset(mw_t * const mw)
{
  mw_lmc_mixer(mw,MW_MIXER_BOTH);
  mw_lmc_master(mw,40);                 /* 0db */
  mw_lmc_left(mw,20);                   /* 0db */
  mw_lmc_right(mw,20);                  /* 0db */
  mw_lmc_high(mw,6);                    /* 0db */
  mw_lmc_low(mw,6);                     /* 0db */
}

int mw_reset(mw_t * const mw)
{
  int i;

  for ( i=0; i<sizeof(mw->map); i++ ) {
    mw->map[i] = 0;
  }
  mw->ct = mw->end = 0;
  lmc_reset(mw);

  TRACE68(mw_cat, MWHD "%s\n", "chip reset");
  return 0;
}

void mw_cleanup(mw_t * const mw) {}

int mw_setup(mw_t * const mw,
             mw_setup_t * const setup)
{
  if (!mw || !setup || !setup->mem) {
    msg68_error(MWHD "invalid parameter\n");
    return -1;
  }

  /* setup emulation mode */
  setup->parms.engine = mw_engine(mw, setup->parms.engine);

  /* setup sampling rate */
  setup->parms.hz = mw_sampling_rate(mw, setup->parms.hz);

  /* setup memory access */
  mw->mem     = setup->mem;
  mw->log2mem = setup->log2mem;
  mw->ct_fix  = ( sizeof(mwct_t) << 3 ) - mw->log2mem;

  TRACE68(mw_cat, MWHD "%d-bit memory, %d-bit precision\n",
          setup->log2mem, mw->ct_fix);
  mw_reset(mw);

  return 0;
}

/* ,-----------------------------------------------------------------.
 * |                    Microwire initialization                    |
 * `-----------------------------------------------------------------'
 */

int mw_init(int * argc, char ** argv)
{
  if (mw_cat == msg68_DEFAULT)
    mw_cat = msg68_cat("ste","STE sound (DMA/Microwire/LMC1992)", DEBUG_MW_O);

  /* Setup defaults */
  default_parms.engine = MW_ENGINE_LINEAR;
  default_parms.hz     = SPR_DEF;

  /* Init volume table */
  init_volume();

  return 0;
}

void mw_shutdown(void)
{
  msg68_cat_free(mw_cat);
  mw_cat = msg68_DEFAULT;
}

/* Rescale n sample of b with f ( << MW_MIX_FIX ) */
static void rescale(s32 * b, int f, int n)
{
  if (!f) {
    do { *b++ = 0; } while (--n);
  } else if (f != (1<<MW_MIX_FIX)) {
    do {
      int v;
      v = ((*b)*f) >> MW_MIX_FIX;
      *b++ = ( v << 16 ) | ( v & 0xFFFF );
    } while (--n);
  }
}

/* --- Rescale n sample of b with r ( << mw_MIX_FIX ) --- */
static void no_mix_ste(mw_t * const mw, s32 * b, int n)
{
  rescale(b, (mw->db_conv == Db_alone) ? 0 : MW_YM_MULT, n);
}

static void skip_ste(mw_t * const mw, int n)
{
  mwct_t base, end2, ct, end, stp;

  const int        loop = mw->map[MW_ACTI] & 2;
  const int        mono = (mw->map[MW_MODE]>>7) & 1;
  const uint_t      frq = 50066u >> ((mw->map[MW_MODE]&3)^3);
  const int      ct_fix = mw->ct_fix;

 /* Get internal register for sample base and sample end
  * $$$ ??? what if base > end2 ???
  */
  base = mw_counter_read(mw, MW_BASH);
  end2 = mw_counter_read(mw, MW_ENDH);

  /* Get counters */
  ct  = mw->ct;
  end = mw->end;
  stp = ( (mwct_t) ( frq * n ) << ( ct_fix + 1 - mono ) ) / mw->hz;

  if ( stp >= end - ct ) {
    if ( ! loop ) goto out;
    stp -= end - ct;
    ct   = base;
    end  = end2;
    if ( ct == end ) {
      stp = 0;
    } else {
      stp %= end - ct;
    }
    ct += stp;
  }

out:
  if ( !loop && ct >= end ) {
    mw->map[MW_ACTI] = 0;
    ct  = base;
    end = end2;
  }
  mw->ct  = ct;
  mw->end = end;
}

/* $$$ TODO: STe volume balance correction. */
#define _VOL(LR) \
  (mw->db_conv[mw->lmc.master+mw->lmc.LR] >> 1)

static void mix_ste(mw_t * const mw, s32 *b, int n)
{
  mwct_t base, end2, ct, end, stp;
  const int          vl = _VOL(left);
  const int          vr = _VOL(right);
  const int        loop = mw->map[MW_ACTI] & 2;
  const int        mono = (mw->map[MW_MODE]>>7) & 1;
  const uint_t      frq = 50066u >> ((mw->map[MW_MODE]&3)^3);
  const int68_t ym_mult = (mw->db_conv == Db_alone) ? 0 : MW_YM_MULT;
  const int      ct_fix = mw->ct_fix;
  const s8 *        spl = (const s8 *)mw->mem;

  /* Get internal register for sample base and sample end
   * $$$ ??? what if base > end2 ???
   */
  base = mw_counter_read(mw, MW_BASH);
  end2 = mw_counter_read(mw, MW_ENDH);

  /* Get current sample counter and end */
  ct  = mw->ct;
  end = mw->end;

  if (ct >= end) {
    if (!loop) {
      goto out;
    } else {
      mwct_t overflow =  ct - end;
      mwct_t length   = end - base;
      ct  = base;
      if (length) {
        ct += overflow > length ? overflow % length : overflow;
      }
      end = end2;
    }
  }
  /* Calculate sample step.
   * Stereo trick : Advance 2 times faster, take care of word alignment later.
   */
  stp = ( (mwct_t) frq << ( ct_fix + 1 - mono ) ) / mw->hz;

  if (mono) {
    /* mix mono */
    do {
      int68_t v, ym;

      ym = (*b) * ym_mult;
      v = spl[ (int)( ct >> ct_fix ) ];
      *b++ =
        (
          (u16)((v*vl + ym) >> MW_MIX_FIX)
          +
          (((v*vr + ym)>>MW_MIX_FIX)<<16)
          );

      ct += stp;
      if (ct >= end) {
        if (!loop) {
          --n;
          break;
        } else {
          mwct_t overflow = ct-end;
          mwct_t length   = end-base;
          ct  = base;
          if (length) {
            ct += overflow>length ? overflow%length : overflow;
          }
          end = end2;
        }
      }
    } while (--n);

  } else {
    /* mix stereo */
    do {
      int68_t l,r,ym;
      int addr;

      ym = (*b) * ym_mult;
      addr = ( ct >> ct_fix ) & ~1;
      l = spl[addr+0];
      r = spl[addr+1];
      *b++ =
        (
          (u16)((l*vl + ym)>>MW_MIX_FIX)
          +
          (((r*vr + ym)>>MW_MIX_FIX)<<16)
          );

      ct += stp;
      if (ct >= end) {
        if (!loop) {
          --n;
          break;
        } else {
          mwct_t overflow = ct-end;
          mwct_t length   = end-base;
          ct  = base;
          if (length) {
            ct += overflow>length ? overflow%length : overflow;
          }
          end = end2;
        }
      }
    } while (--n);
  }

  out:
  if (!loop && ct >= end) {
    mw->map[MW_ACTI] = 0;
    ct  = base;
    end = end2;
  }
  mw->ct  = ct;
  mw->end = end;

  /* Finish the buffer */
  if (n>0) {
    no_mix_ste(mw,b,n);
  }
}

/* ,-----------------------------------------------------------------.
 * |                      STE sound process                          |
 * `-----------------------------------------------------------------'
 */

void mw_mix(mw_t * const mw, s32 * b, int n)
{
  if ( n <= 0 ) {
    return;
  }
  if ( !b ) {
    if ( mw->map[MW_ACTI] & 1 ) {
      /* no buffer and active : advance counters only */
      skip_ste(mw,n);
    }
  } else if ( ! (mw->map[MW_ACTI] & 1 ) ) {
    /* Microwire desactivated */
    no_mix_ste(mw,b,n);
  } else {
    /* Microwire activated */
    mix_ste(mw,b,n);
  }
}
