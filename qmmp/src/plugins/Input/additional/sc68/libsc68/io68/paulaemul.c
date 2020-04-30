/*
 * @file    paulaemul.c
 * @brief   Paula emulator (Amiga soundchip)
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

#include "default.h"
#include "paulaemul.h"
#include "emu68/assert68.h"

#include <sc68/file68_msg.h>
#include <sc68/file68_opt.h>
#include <sc68/file68_str.h>

#ifndef DEBUG_PL_O
# define DEBUG_PL_O 0
#endif
int pl_cat = msg68_DEFAULT;

#define PLHD "paula  : "

/* Filled by paula_init() to avoid field order dependencies */
static paula_parms_t default_parms;


static int msw_first = 0;           /* big/little endian compliance */
static int pl_chans  = 15;          /* active channels */

static int onchange_filter(const option68_t * opt, value68_t * val)
{
  paula_engine(0,!val->num?PAULA_ENGINE_SIMPLE:PAULA_ENGINE_LINEAR);
  return 0;
}

static int onchange_clock(const option68_t * opt, value68_t * val)
{
  paula_clock(0,!val->num?PAULA_CLOCK_PAL:PAULA_CLOCK_NTSC);
  return 0;
}

static int onchange_chans(const option68_t * opt, value68_t * val)
{
  val->num = pl_chans = 15 & val->num;
  return 0;
}

static const char * f_clock[] = { "pal","ntsc" };

/* Command line options */
/* static const char prefix[] = "sc68-"; */
#define prefix NULL
static const char engcat[] = "paula";
static option68_t opts[] = {
  OPT68_BOOL(prefix,"amiga-filter",engcat,
             "active paula resample filter",1,onchange_filter),
  OPT68_IRNG(prefix,"amiga-blend",engcat,
             "left/right voices blending factor {128:mono}",
             0,0xFF,1,0),
  OPT68_ENUM(prefix,"amiga-clock",engcat,"paula clock",
             f_clock,sizeof(f_clock)/sizeof(*f_clock),1,onchange_clock),
  OPT68_IRNG(prefix,"amiga-chans",engcat,
             "set active paula channels {bit#0-3 = chan#A-C}",
             0,15,0,onchange_chans),
};
#undef prefix

/* ,-----------------------------------------------------------------.
 * |                         Paula init                              |
 * `-----------------------------------------------------------------'
 */


int paula_init(int * argc, char ** argv)
{
  static const u32 msw = 0x1234;

  if (pl_cat == msg68_DEFAULT)
    pl_cat = msg68_cat("paula","amiga sound emulator", DEBUG_PL_O);

  /* Setup little/big endian swap */
  msw_first = !(*(const u8 *)&msw);

  /* Set default default */
  default_parms.engine = PAULA_ENGINE_SIMPLE;
  default_parms.clock  = PAULA_CLOCK_PAL;
  default_parms.hz     = SPR_DEF;

  /* Register amiga options */
  option68_append(opts,sizeof(opts)/sizeof(*opts));

  /* Default option values */
  option68_iset(opts+0, default_parms.engine!=PAULA_ENGINE_SIMPLE,
                opt68_NOTSET,opt68_CFG);
  option68_iset(opts+1, 0x50, opt68_NOTSET, opt68_CFG);
  option68_iset(opts+2, default_parms.clock!=PAULA_CLOCK_PAL,
                opt68_NOTSET,opt68_CFG);

  /* Parse options */
  *argc = option68_parse(*argc,argv);

  return 0;
}

void paula_shutdown()
{
  msg68_cat_free(pl_cat);
  pl_cat = msg68_DEFAULT;
}

/* ,-----------------------------------------------------------------.
 * |                      Paula Sampling Rate                        |
 * `-----------------------------------------------------------------'
 */

static int set_clock(paula_t * const paula, int clock_type, uint68_t f)
{
  u64 tmp;
  const int ct_fix = paula->ct_fix;
  const int fix    = 40;

  paula->hz    = f;
  paula->clock = clock_type;
  tmp = (clock_type == PAULA_CLOCK_PAL)
    ? PAULA_PAL_FRQ
    : PAULA_NTSC_FRQ
    ;
  tmp <<= fix;
  tmp /= f;

  if ( ct_fix < fix )
    tmp >>= fix - ct_fix;
  else
    tmp <<= ct_fix - fix;
  TRACE68(pl_cat,
          PLHD "clock -- *%s*\n",
          clock_type == PAULA_CLOCK_PAL ? "PAL" : "NTSC");
  paula->clkperspl = (plct_t) tmp;
  return f;
}

int paula_sampling_rate(paula_t * const paula, int hz)
{
  switch (hz) {
  case PAULA_HZ_QUERY:
    hz = paula ? paula->hz : default_parms.hz;
    break;

  case PAULA_HZ_DEFAULT:
      hz = default_parms.hz;

  default:
    if (hz < SPR_MIN)
      hz = SPR_MIN;
    else if (hz > SPR_MAX)
      hz = SPR_MAX;
    if (!paula)
      default_parms.hz = hz;
    else
      set_clock(paula, paula->clock, hz);
    TRACE68(pl_cat,
            PLHD "%s sampling rate -- *%dhz*\n",
            paula ? "select" : "default", hz);
    break;
  }
  return hz;
}

/* ,-----------------------------------------------------------------.
 * |                   Set/Get emulator engine                       |
 * `-----------------------------------------------------------------'
 */

static
const char * pl_engine_name(const int engine)
{
  switch (engine) {
  case PAULA_ENGINE_SIMPLE: return "SIMPLE";
  case PAULA_ENGINE_LINEAR: return "LINEAR";
  }
  return "INVALID";
}

static
const char * pl_clock_name(const int clock)
{
  switch (clock) {
  case PAULA_CLOCK_PAL:  return "PAL";
  case PAULA_CLOCK_NTSC: return "NTSC";
  }
  return "INVALID";
}

int paula_engine(paula_t * const paula, int engine)
{
  switch (engine) {
  case PAULA_ENGINE_QUERY:
    engine = paula ? paula->engine : default_parms.engine;
    break;

  default:
    msg68_warning(PLHD "invalid engine -- %d\n", engine);
  case PAULA_ENGINE_DEFAULT:
    engine = default_parms.engine;
  case PAULA_ENGINE_SIMPLE:
  case PAULA_ENGINE_LINEAR:
    *(paula ? &paula->engine : &default_parms.engine) = engine;
    TRACE68(pl_cat,
            PLHD "%s engine -- *%s*\n",
            paula ? "select" : "default",
            pl_engine_name(engine));
    break;
  }
  return engine;
}

int paula_clock(paula_t * const paula, int clock)
{
  switch (clock) {
  case PAULA_CLOCK_QUERY:
    clock = paula ? paula->clock : default_parms.clock;
    break;

  default:
    clock = default_parms.clock;
  case PAULA_CLOCK_PAL:
  case PAULA_CLOCK_NTSC:
    if (paula) {
      set_clock(paula, clock, paula->hz);
    } else {
      default_parms.clock = clock;
    }
    break;
  }
  return clock;
}

/* ,-----------------------------------------------------------------.
 * |                         paula reset                             |
 * `-----------------------------------------------------------------'
 */

int paula_reset(paula_t * const paula)
{
  int i;

  /* reset shadow registers */
  for (i=0; i<sizeof(paula->map); i++) {
    paula->map[i] = 0;
  }

  /* Some musics does not initialize volume (see support-request #4) */
  for (i=0; i<4; i++) {
    paula->map[PAULA_VOICE(i)+9] = 64;   /* default volume to 64 */
    paula->map[PAULA_VOICE(i)+6] = 0x10; /* default period to 0x1000 */
  }

  /* reset voices */
  for (i=0; i<4; i++) {
    paula->voice[i].adr   = 2;
    paula->voice[i].start = 0;
    paula->voice[i].end   = 0;   /* end < adr should prevent mixing */
  }

  /* Reset DMACON and INTENA/REQ to something that
   * seems acceptable to me.
   */
  paula->dmacon = 1 << 9;  /* DMA general activate, DMA audio desactivate. */
  paula->intreq = 0;       /* No interrupt request.                        */
  paula->intena = 1 << 14; /* Master interrupt enable, audio int disable.  */
  paula->adkcon = 0;       /* No modulation.                               */

  return 0;
}

void paula_cleanup(paula_t * const paula) {}

int paula_setup(paula_t * const paula,
                paula_setup_t * const setup)
{
  if (!paula || !setup || !setup->mem) {
    return -1;
  }

  /* Default sampling rate */
  if (!setup->parms.hz) {
    setup->parms.hz = default_parms.hz;
  }

  /* Default clock mode */
  if (setup->parms.clock == PAULA_CLOCK_DEFAULT) {
    setup->parms.clock = default_parms.clock;
  }

  paula->chansptr = &pl_chans;
  paula->mem      = setup->mem;
  paula->log2mem  = setup->log2mem;
  paula->ct_fix   = ( sizeof(plct_t) << 3 ) - paula->log2mem;

  setup->parms.engine = paula_engine(paula, setup->parms.engine);
  paula_reset(paula);
  set_clock(paula, setup->parms.clock, setup->parms.hz);

  /* Debug trace */
  TRACE68(pl_cat, PLHD "engine -- *%s*\n", pl_engine_name(paula->engine));
  TRACE68(pl_cat, PLHD "clock -- *%s*\n", pl_clock_name(paula->clock));
  TRACE68(pl_cat, PLHD "sampling rate -- *%dhz*\n", (int)paula->hz);

  return 0;
}

#if 0
static void poll_irq(paula_t * const paula, unsigned int N)
{
  u8 *p = paula->map + PAULA_VOICE(N);
  paulav_t * w = paula->voice+N;

  /* Reload internal when interrupt is DENIED */
  if (
    (paula->intreq
     |
     ~((paula->intena << (8*sizeof(int)-1-14) >> (8*sizeof(int)-1))
       & paula->intena)) & (1 << (N + 7))) {
    uint68_t a,l;

    /* Get sample pointer. */
    a = (uint68_t) ( ((p[1] << 16) | (p[2] << 8) | p[3]) & 0x7FFFE )
      << PAULA_ct_fix;
    w->adr = w->start = a;
    /* Get length */
    l = (p[4] << 8) | p[5];
    l |= (!l) << 16;
    l <<= (1 + PAULA_ct_fix);
    w->end = a + l;
  }
  paula->intreq |= 1 << (N + 7);
}
#endif

/* Mix with laudio channel data (1 char instead of 2) */

static void mix_one(paula_t * const paula,
                    int N, const int shift,
                    s32 * b, int n)
{
  const u8 * const mem = paula->mem;
  paulav_t * const w   = paula->voice+N;
  u8       * const p   = paula->map+PAULA_VOICE(N);
  s16      *       b2  = (s16 *)b + shift;
  const int     ct_fix = paula->ct_fix;
  plct_t adr, stp, readr, reend, end, vol, per;
  u8 last, hasloop;

  /* Mask to get the fractionnal part of the sample counter. Therefore
   * forcing it to 0 will disable linear interpolation by forcing
   * exact sample point.
   */
  const plct_t imask = paula->engine == PAULA_ENGINE_LINEAR
    ? ( ( (plct_t) 1 << ct_fix ) - 1 )
    : 0
    ;
  const signed_plct_t one = (signed_plct_t) 1 << ct_fix;

  hasloop = 0;

  /* $$$ FIXME
   * Dunno exactly what if volume is not in proper range [0..64]
   */
  vol = p[9] & 127;
  if (vol >= 64)
    vol = 64;
  vol <<= 1;


  per = ( p[6] << 8 ) + p[7];
  if (!per) per = 1;                    /* or is it +1 for all ?? */
  stp = paula->clkperspl / per;

  /* Audio irq disable for this voice :
   * Internal will be reload at end of block
   */
  readr   = ( p[1] << 16 ) | ( p[2] << 8 ) | p[3];
  readr <<= ct_fix;
  reend   = ((p[4] << 8) | p[5]);
  reend  |= (!reend) << 16;           /* 0 is 0x10000 */
  reend <<= (1 + ct_fix);             /* +1 as unit is 16-bit word */
  reend  += readr;
  assert( reend > readr );
  if (reend <= readr) {
    /* $$$ ??? dunno why I did this !!! May be could happen on a
     * modulo. */
    return;
  }

  adr = w->adr;
  end = w->end;
  if (end <= adr)
    return;

  /* mix stereo */
  do {
    int idx;
    signed_plct_t low, v0, v1;

    low = adr & imask;
    idx = adr >> ct_fix;                /* current index         */
    last = mem[idx++];                  /* save last sample read */

    if ( ( (plct_t) idx << ct_fix ) >= end )
      idx = readr >> ct_fix;            /* loop index     */
    v1 = (s8) mem[idx];                 /* next sample    */
    v0 = (s8) last;                     /* current sample */

    /* linear interpolation (or not if imask is zero) */
    v0 = ( v1 * low + v0 * ( one - low ) ) >> ct_fix;

    /* apply volume */
    v0  *= vol;

    assert(v0 >= -16384);
    assert(v0 <   16384);

    /* Store and advance output buffer */
    *b2 += v0;
    b2  += 2;

    /* Advance */
    adr += stp;
    if (adr >= end) {
      plct_t relen = reend - readr;
      hasloop = 1;
      adr = readr + adr - end;
      end = reend;
      while (adr >= end) {
        adr -= relen;
      }
    }
  } while (--n);

  last &= 0xFF;
  p[0xA] = last + (last << 8);
  w->adr = adr;
  if (hasloop) {
    w->start = readr;
    w->end   = end;
  }
}

/* ,-----------------------------------------------------------------.
 * |                        Paula process                            |
 * `-----------------------------------------------------------------'
 */

static void clear_buffer(s32 * b, int n)
{
  const s32 v = 0;
  if (n & 1) { *b++ = v; }
  if (n & 2) { *b++ = v; *b++ = v; }
  if (n >>= 2, n) do {
      *b++ = v; *b++ = v; *b++ = v; *b++ = v;
    } while (--n);
}


#if DEBUG_PL_O == 1

typedef struct {
  int on;
  unsigned int adr;   /**< current sample counter (<<paula_t::ct_fix). */
  unsigned int start; /**< loop address.                               */
  unsigned int end;   /**< end address (<<paula_t::ct_fix).            */
  int len;
  int vol;
  int per;
} paulav_dbg_t;

static
void paula_dbg(paulav_dbg_t * d, paula_t * const paula, int i)
{
  paulav_t * const w   = paula->voice+i;
  u8       * const p   = paula->map+PAULA_VOICE(i);
  const int     ct_fix = paula->ct_fix;

  d->on   = ((paula->dmacon >> 9) & (paula->dmacon >> i) & 1);
  d->adr  = w->adr >> ct_fix;
  d->end  = w->end >> ct_fix;
  d->start  = w->start >> ct_fix;
  d->len  = d->end-w->start;
  d->vol = p[9] & 127;
  /* if (d->vol >= 64) d->vol = 64; */
  d->per = ( p[6] << 8 ) + p[7];
  /* if (!d->per) d->per = 1; */
}

#endif

void paula_mix(paula_t * const paula, s32 * splbuf, int n)
{

  if ( n > 0 ) {
    const int pl_mask = paula->chansptr ? *paula->chansptr : 15;
    int i, b=0;
#if DEBUG_PL_O == 1
    paulav_dbg_t d[4];
#endif
    clear_buffer(splbuf, n);
    for (i=0; i<4; i++) {
      /* $$$ VERIFY: channel mapping ABCD => LRRL ? */
      const int right = (i^(i>>1)^msw_first)&1;
#if DEBUG_PL_O == 1
      paula_dbg(d+i, paula, i);
#endif
      if ((paula->dmacon >> 9) & ( (pl_mask & paula->dmacon) >> i) & 1) {
        mix_one(paula, i, right, splbuf, n);
        b += 1 << i;
      }
    }

#if DEBUG_PL_O == 1
    if (1) {
#   define ONE "%c:%06x-%06x->%06x,%04x,%02d"
      TRACE68(pl_cat,
              PLHD "%d " ONE " " ONE " " ONE " " ONE "\n",
              n,
              d[0].on?'A':'.', d[0].adr, d[0].end, d[0].start, d[0].per,d[0].vol,
              d[1].on?'B':'.', d[1].adr, d[1].end, d[1].start, d[1].per,d[1].vol,
              d[2].on?'C':'.', d[2].adr, d[2].end, d[2].start, d[2].per,d[2].vol,
              d[3].on?'D':'.', d[3].adr, d[3].end, d[3].start, d[3].per,d[3].vol);
    }
#endif
  }
  /* HaxXx: assuming next mix is next frame reset beam V/H position. */
  paula->vhpos = 0;
}
