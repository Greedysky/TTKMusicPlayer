/*
 * @file    ym_puls.c
 * @brief   YM-2149 emulator - YM-2149 pulse engine
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

#if 0
#ifdef HAVE_CONFIG_OPTION68_H
# include "config_option68.h"
#else
# include "default_option68.h"
#endif
#endif


#include "ymemul.h"
#include "emu68/assert68.h"

#include <sc68/file68_msg.h>
#include <sc68/file68_str.h>
#include <sc68/file68_opt.h>

extern int ym_cat;                      /* defined in ymemul.c */
extern int ym_dac_out;                  /* defined in ymemul.c */
extern const u16 * ym_envelops[16];     /* defined in ym_envel.c */

#ifndef INTMSB
# define INTMSB (sizeof(int)*8-1)
#endif

#define YM_OUT_MSK(C,B,A)                       \
  (((((C)&0x1F)<<10))                           \
   |((((B)&0x1F)<< 5))                          \
   |((((A)&0x1F)    )))

const int ym_smsk_table[8] = {
  /* 000 */ YM_OUT_MSK(00,00,00),
  /* 001 */ YM_OUT_MSK(00,00,-1),
  /* 010 */ YM_OUT_MSK(00,-1,00),
  /* 011 */ YM_OUT_MSK(00,-1,-1),
  /* 100 */ YM_OUT_MSK(-1,00,00),
  /* 101 */ YM_OUT_MSK(-1,00,-1),
  /* 110 */ YM_OUT_MSK(-1,-1,00),
  /* 111 */ YM_OUT_MSK(-1,-1,-1)
};

/*********************/
/* Filters functions */
/*********************/

static void filter_none(ym_t * const);
static void filter_1pole(ym_t * const);
static void filter_2pole(ym_t * const);
static void filter_mixed(ym_t * const);
static void filter_boxcar(ym_t * const);
static void filter_dacout(ym_t * const);

static const char f_2poles[] = "2-poles";
static const char f_mixed[]  = "mixed";
static const char f_1pole[]  = "1-pole";
static const char f_boxcar[] = "boxcar";
static const char f_none[]   = "none";
static const char f_dacout[] = "dacout";

static struct {
  const char * name;
  ym_puls_filter_t filter;
} filters[] = {
  { f_2poles, filter_2pole  },          /* first is default */
  { f_mixed,  filter_mixed  },
  { f_1pole,  filter_1pole  },
  { f_boxcar, filter_boxcar },
  { f_none,   filter_none   },
  { f_dacout, filter_dacout },
};

static const char * f_names[] = {
  f_2poles,f_mixed,f_1pole,f_boxcar,f_none,f_dacout
};
static const int n_filters = sizeof(filters)/sizeof(*filters);
static int default_filter = 0;

#define PULS ym->emu.puls

static int reset(ym_t * const ym, const cycle68_t ymcycle)
{

  /* Reset envelop generator */
  PULS.envel_idx          = -1;     /* ct==1 triggers +1 instantly */
  PULS.envel_ct           = 1;

  /* Reset noise generator */
  PULS.noise_bit          = 1;
  PULS.noise_ct           = 1;

  /* Reset tone generator (add bias to avoid cancel-out sync effect). */
#if 1
  PULS.voice_ctA          = 1;
  PULS.voice_ctB          = 1;
  PULS.voice_ctC          = 1;
  PULS.levels             = 0x7fff;
#else
  PULS.voice_ctA          = 53;
  PULS.voice_ctB          = 109;
  PULS.voice_ctC          = 157;
  PULS.levels             = 0;
#endif

  /* Reset filters */
  PULS.hipass_inp1 = 0;
  PULS.hipass_out1 = 0;
  PULS.lopass_out1 = 0;

  /* Reset butterworth */
  PULS.btw.x[0] = PULS.btw.x[1] = 0;
  PULS.btw.y[0] = PULS.btw.y[1] = 0;

  /* Butterworth low-pass cutoff=15.625khz sampling=250khz */
  PULS.btw.a[0] =  0x01eac69f; /* fix 30 */
  PULS.btw.a[1] =  0x03d58d3f;
  PULS.btw.a[2] =  0x01eac69f;
  PULS.btw.b[0] = -0x5d1253b0;
  PULS.btw.b[1] =  0x24bd6e2f;

  return 0;
}

/* ,-----------------------------------------------------------------.
 * |                          All in one                             |
 * `-----------------------------------------------------------------'
 */


static int generator(ym_t  * const ym, int ymcycles)
{
  const u16 * waveform = ym_envelops[15 & ym->reg.name.env_shape];

  /* int ctA,  ctB,  ctC,  ctN,  ctE; */
  int perA, perB, perC, perN, perE;
  int smsk, emsk, vols, nmsk;
  int rem_cycles, v;

  rem_cycles = ymcycles & 7;
  ymcycles >>= 3;
  if(!ymcycles) goto finish;

  smsk = ym_smsk_table[7 &  ym->reg.name.ctl_mixer      ];
  nmsk = ym_smsk_table[7 & (ym->reg.name.ctl_mixer >> 3)];

  /* 3 voices buzz or lvl mask */
  emsk = vols = 0;

  v = ym->reg.name.vol_a & 0x1F;
  if(v&0x10) emsk |= YM_OUT_MSK_A;
  else       vols |= (v<<1)+1;

  v = ym->reg.name.vol_b & 0x1F;
  if(v&0x10) emsk |= YM_OUT_MSK_B;
  else       vols |= (v<<6)+(1<<5);

  v = ym->reg.name.vol_c & 0x1F;
  if(v&0x10) emsk |= YM_OUT_MSK_C;
  else       vols |= (v<<11)+(1<<10);

  perA = ym->reg.name.per_a_lo | ((ym->reg.name.per_a_hi&0xF)<<8);
  perB = ym->reg.name.per_b_lo | ((ym->reg.name.per_b_hi&0xF)<<8);
  perC = ym->reg.name.per_c_lo | ((ym->reg.name.per_c_hi&0xF)<<8);
  perE = ym->reg.name.per_env_lo | (ym->reg.name.per_env_hi<<8);
  perN = (ym->reg.name.per_noise & 0x1F) << 1;

  /* $$$ X/ME << 1 because the noise generator base frequency is
     master/16 but we have to match the envelop generator frequency
     which is master/8. */

  /* $$$ x/ME DELME XXX TEST */
  if (!perA) perA = 1;
  if (PULS.voice_ctA > perA) PULS.voice_ctA %= perA;
  if (!perB) perB = 1;
  if (PULS.voice_ctB > perB) PULS.voice_ctB %= perB;
  if (!perC) perC = 1;
  if (PULS.voice_ctC > perC) PULS.voice_ctC %= perC;
  if (!perE) perE = 1;
  if (PULS.envel_ct > perE) PULS.envel_ct %= perE;
  if (!perN) perN = 1;
  if (PULS.noise_ct > perN) PULS.noise_ct %= perN;

  do {
    int sq;

    if (--PULS.noise_ct <= 0) {
      PULS.noise_ct = perN;
      /* *** Based on MAME. Bit have been reversed for optimzation :) ***
       *
       *   The Random Number Generator of the 8910 is a 17-bit shift
       *   register. The input to the shift register is bit0 XOR bit2.
       *   bit0 is the output.
       */

      /* bit 17 := bit 0 ^ bit 2 */
      PULS.noise_bit |= ((PULS.noise_bit^(PULS.noise_bit>>2)) & 1)<<17;
      PULS.noise_bit >>= 1;
    }

    if (--PULS.envel_ct <= 0) {
      PULS.envel_ct = perE;
      if (++PULS.envel_idx == 96) PULS.envel_idx = 32;
    }

    if (--PULS.voice_ctA <= 0) {
      PULS.levels ^= YM_OUT_MSK_A;
      PULS.voice_ctA = perA;
    }

    if (--PULS.voice_ctB <= 0) {
      PULS.levels ^= YM_OUT_MSK_B;
      PULS.voice_ctB = perB;
    }

    if (--PULS.voice_ctC <= 0) {
      PULS.levels ^= YM_OUT_MSK_C;
      PULS.voice_ctC = perC;
    }

    sq = PULS.levels | smsk;                      /* Apply tone. */
    sq &= (-(PULS.noise_bit&1) | nmsk);           /* Apply noise. */
    sq &= (waveform[PULS.envel_idx]&emsk) | vols; /* Apply volume. */
    sq &= ym->voice_mute;                          /* Apply mute. */
    *ym->outptr++ = sq;

  } while (--ymcycles);

finish:
  return rem_cycles;
}


static void simulation(ym_t * const ym, cycle68_t ymcycle)
{
  ym_event_t * event;
  cycle68_t lastcycle;

  if (!ymcycle)
    return;

  for (event = ym->event_buf, lastcycle = 0; event < ym->event_ptr; ++event) {
    const int ymcycles = event->ymcycle - lastcycle;
    assert(event->ymcycle <= ymcycle);
    if (ymcycles)
      lastcycle = event->ymcycle - generator(ym, ymcycles);
    ym->reg.index[event->reg] = event->val;
    if(event->reg == YM_ENVTYPE) {
      /* $$$ X/ME Should env_ct be initialized to the period value ? */
      PULS.envel_idx = -1;         /* ct==1 triggers +1 instantly */
      PULS.envel_ct  = 1;
    }
  }
  generator(ym, ymcycle-lastcycle);
}

/* ,-----------------------------------------------------------------.
 * |                         Run emulation                           |
 * `-----------------------------------------------------------------'
 */

/******************************************************/
/*                                                    */
/* Recursive single pole low-pass filter              */
/* -------------------------------------              */
/*                                                    */
/*   o[N] = i[N] * A + o[N-1] * B                     */
/*                                                    */
/*   X  = exp(-2.0 * pi * Fc)                         */
/*   A  = 1 - X = 1 - B                               */
/*   B  = X                                           */
/*   Fc = cutoff / rate                               */
/*                                                    */
/*                                                    */
/* Recursive single pole high-pass filter             */
/* --------------------------------------             */
/*                                                    */
/*   o[N] = A0 * i[N] + A1 * i[N-1] + B1 * o[N-1]     */
/*        = A0 * i[N] - A0 * i[N-1] + B1 * o[N-1]     */
/*        = A0 * ( i[N] - i[N-1] )  + B1 * o[N-1]     */
/*                                                    */
/*   X  = exp(-2.0 * pi * Fc)                         */
/*   A0 = (1 + X) / 2                                 */
/*   A1 = -A0                                         */
/*   B1 = X                                           */
/*   Fc = cutoff / rate                               */
/*                                                    */
/*                                                    */
/* Butterworth                                        */
/* -----------                                        */
/*     o[N] = A0 * i[N-0] + A1 * i[N-1] + A2 * i[N-2] */
/*                        - B0 * o[N-1] - B1 * o[N-2] */
/*                                                    */
/*                                                    */
/* Butterworth low-pass                               */
/* --------------------                               */
/*                                                    */
/*   c  = 1 / tan(pi * cutoff / rate)                 */
/*   a0 = 1 / (1 + sqrt(2) * c + c^2)                 */
/*   a1 = 2 * a0                                      */
/*   a2 = a0                                          */
/*   b0 = 2 * (1 - c^2) * a0                          */
/*   b1 = (1 - sqrt(2.0) * c + c^2) * a0              */
/*                                                    */
/*                                                    */
/* Butterworth high-pass                              */
/* ---------------------                              */
/*                                                    */
/*   c  = tan(pi * cutoff / rate)                     */
/*   a0 = 1 / (1 + sqrt(2) * c + c^2)                 */
/*   a1 = -2 * a0                                     */
/*   a2 = a0                                          */
/*   b0 = 2 * (c^2 - 1) * a0                          */
/*   b1 = (1 - sqrt(2) * c + c^2) * a0                */
/*                                                    */
/******************************************************/


#define REVOL(V) ((V) >> 1)
#define CLIP3(V,A,B) ( V < A ? A : ( V > B ? B : V ) )
#define CLIP(V) CLIP3(V,-32768,32767)
#define YMOUT(X) ymout(ym,X)

static inline s16 ymout(const ym_t * const ym, const int v)
{
  assert (v >= 0 && v < (1<<15) );
  return ym->ymout5[v];
}

static inline int clip(int o)
{
#ifdef DEBUG
  static int max, min;
  if ( o < min ) {
    min = o;
    msg68(ym_cat,"ym-2149: pulse -- pcm min -- %d\n", o);
  }
  if ( o > max ) {
    max = o;
    msg68(ym_cat,"ym-2149: pulse -- pcm max -- %d\n", o);
  }
  if (o < -32768) {
    msg68(ym_cat,"ym-2149: pulse -- pcm clip -- %d < -32768\n", o);
    o = -32768;
  } else if (o > 32767) {
    msg68(ym_cat,"ym-2149: pulse -- pcm clip -- %d > 32767\n", o);
    o = 32767;
  }
  return o;
#elif defined(CLIP)
  return CLIP(o);
#else
  return o;
#endif
}

/* Resample ``n'' input samples from ``irate'' to ``orate''
 * @warning irate <= 262143 or 32bit overflow
 */
static s32 * resampling(s32 * dst, const int n,
                        const uint68_t irate, const uint68_t orate)
{
  s32   * const src = dst;
  const int68_t stp = (irate << 14) / orate; /* step into source */

  if ( 0 == (stp & ((1<<14)-1)) ) {
    /* integer step: fast down-sampling/ */
    const int istp = stp >> 14;
    const int iend = n;
    int idx        = 0;
    /* forward */
    do {
      int o = REVOL(src[idx]);
      *dst++ = clip(o);
    } while ((idx += istp) < iend);
  } else {
    const int68_t end = n << 14;
    int68_t       idx = 0;

    if (stp >= 1<<14) {
      /* step > 1: down-sampling, do it forward */
      do {
        int o = REVOL(src[(int)(idx>>14)]);
        *dst++ = clip(o);
      } while ((idx += stp) < end);
    } else {
      /* step < 0: up-sampling, do backward */
      const int m = (n * orate + irate - 1) / irate; /* output samples */
      dst  = src + m - 1;
      idx  = end;
      do {
        int o = REVOL(src[(int)((idx -= stp)>>14)]);
        *dst = clip(o);
      } while (--dst != src);
      dst = src+m;
    }
  }
  return dst;
}



static void filter_dacout(ym_t * const ym)
{
  /* nothing to do ! */
}

static void filter_none(ym_t * const ym)
{
  const int n = (ym->outptr - ym->outbuf);
  if (n > 0) {
    int i;
    /* DAC in -> out */
    for (i=0; i<n; ++i)
      ym->outbuf[i] = YMOUT(ym->outbuf[i]);
    ym->outptr =
      resampling(ym->outbuf, n, ym->clock>>3, ym->hz);
  }
}

static void filter_boxcar2(ym_t * const ym)
{
  const int n = (ym->outptr - ym->outbuf) >> 1;

  if (n > 0) {
    /* DAC out 2 by 2 */
    int m = n;
    s32 * src = ym->outbuf, * dst = ym->outbuf;

    do {
      *dst++ = ( YMOUT(src[0]) + YMOUT(src[1]) ) >> 1;
      src += 2;
    } while (--m);

    ym->outptr =
      resampling(ym->outbuf, n, ym->clock>>(3+1), ym->hz);
  }
}

static void filter_boxcar4(ym_t * const ym)
{
  const int n = (ym->outptr - ym->outbuf) >> 2;

  if (n > 0) {
    int m = n;
    s32 * src = ym->outbuf, * dst = ym->outbuf;

    do {
      *dst++ = ( YMOUT(src[0]) + YMOUT(src[1]) +
                 YMOUT(src[2]) + YMOUT(src[3]) ) >> 2;
      src += 4;
    } while (--m);

    ym->outptr =
      resampling(ym->outbuf, n, ym->clock>>(3+2), ym->hz);
  }
}

/** Use 2-boxcar or 4-boxcar filter so that boxcar output rate not
    less than output sampling rate. */
static void filter_boxcar(ym_t * const ym) {
  /* Select boxcar width  */
  if (ym->hz > (ym->clock >> (3+2)))
    filter_boxcar2(ym);
  else
    filter_boxcar4(ym);
}


/* - 4-boxcar filter resamples to 62500hz
 * - Empirical lowpass filter (+adjust output level)
 * - 1-pole 25hz hipass filter.
 */
static void filter_mixed(ym_t * const ym)
{
  const int n = (ym->outptr - ym->outbuf) >> 2; /* Number of block */

  if (n > 0) {
    s32 * src = ym->outbuf, * dst = src;
    int68_t h_i1 = PULS.hipass_inp1;
    int68_t h_o1 = PULS.hipass_out1;
    int68_t l_o1 = PULS.lopass_out1;
    int m = n;

    do {
      int68_t i0,o0;

      /***********************************************************/
      /* 4-tap boxcar filter; lower sampling rate from 250Khz to */
      /* 62.5Khz; emulates half level buzz sounds.               */
      /***********************************************************/
      i0  = ( YMOUT(src[0]) + YMOUT(src[1])
              + YMOUT(src[2]) + YMOUT(src[3]) ) >> 2;
      src += 4;

      /*****************************************/
      /* Recursive single pole low-pass filter */
      /* - cutoff   : 15.625 Khz               */
      /* - sampling : 62.5 Khz                 */
      /*****************************************/
      if (1) {
        const int68_t B = 0x1a9c; /* 15 bit */
        const int68_t A = (1<<15)-B;
        l_o1 = ( (i0 * A) + l_o1 * B ) >> 15;
      } else {
        l_o1 = i0;
      }

      /******************************************/
      /* Recursive single pole high-pass filter */
      /* - cutoff   : 25 hz                     */
      /* - sampling : 62.5 Khz                  */
      /******************************************/
      if (1) {
        const int A0 = 0x7FD7; /* 15 bit */
        const int B1 = 0x7FAE; /* 15 bit */
        o0 = h_o1 = ( (l_o1 - h_i1) * A0 + (h_o1 * B1) ) >> 15;
        h_i1 = l_o1;
      } else {
        o0 = i0;
      }

      /* store */
      *dst++ = o0;

    } while (--m);

    PULS.hipass_inp1 = h_i1;
    PULS.hipass_out1 = h_o1;
    PULS.lopass_out1 = l_o1;

    ym->outptr =
      resampling(ym->outbuf, n, ym->clock>>(3+2), ym->hz);
  }
}

static void filter_1pole(ym_t * const ym)
{
  const int n = ym->outptr - ym->outbuf;

  if (n > 0) {
    s32 * src = ym->outbuf, * dst = src;

    int68_t h_i1 = PULS.hipass_inp1;
    int68_t h_o1 = PULS.hipass_out1;
    int68_t l_o1 = PULS.lopass_out1;
    int m = n;

    do {
      int68_t i0,o0;

      i0  = YMOUT(*src++);

      /*****************************************/
      /* Recursive single pole low-pass filter */
      /* - cutoff   : 15.625 Khz               */
      /* - sampling : 250 Khz                  */
      /*****************************************/
      {
        const int68_t B = 0x7408; /* 15 bit */
        const int68_t A = (1<<15)-B;
        l_o1 = ( (i0 * A) + l_o1 * B ) >> 15;
      }

      /******************************************/
      /* Recursive single pole high-pass filter */
      /* - cutoff   : 25 hz                     */
      /* - sampling : 250 Khz                   */
      /******************************************/
      {
        const int68_t A0 = 0x7FF6; /* 15 bit */
        const int68_t B1 = 0x7FEB; /* 15 bit */
        o0 = h_o1 = ( (l_o1 - h_i1) * A0 + (h_o1 * B1) ) >> 15;
        h_i1 = l_o1;
      }

      /* store */
      *dst++ = o0;

    } while (--m);

    PULS.hipass_inp1 = h_i1;
    PULS.hipass_out1 = h_o1;
    PULS.lopass_out1 = l_o1;

    ym->outptr =
      resampling(ym->outbuf, n, ym->clock>>(3+0), ym->hz);
  }
}


/* Transform 250000Hz buffer to current sampling rate.
 *
 * Using a butterworth passband filter
 *
 */
static void filter_2pole(ym_t * const ym)
{
  const int n = ym->outptr - ym->outbuf;

  if (n > 0) {
    s32 * src = ym->outbuf, * dst = src;
    int m = n;

    int68_t h_i1 = PULS.hipass_inp1;
    int68_t h_o1 = PULS.hipass_out1;

    const int68_t a0 = PULS.btw.a[0] >> 15;
    const int68_t a1 = PULS.btw.a[1] >> 15;
    const int68_t a2 = PULS.btw.a[2] >> 15;
    const int68_t b0 = PULS.btw.b[0] >> 15;
    const int68_t b1 = PULS.btw.b[1] >> 15;

    int68_t x0 = PULS.btw.x[0];
    int68_t x1 = PULS.btw.x[1];
    int68_t y0 = PULS.btw.y[0];
    int68_t y1 = PULS.btw.y[1];

    do {
      int68_t i0,o0;

      i0  = YMOUT(*src++);

      /******************************************/
      /* Recursive single pole high-pass filter */
      /* - cutoff   : 25 hz                     */
      /* - sampling : 250 Khz                   */
      /******************************************/
      {
        const int68_t A0 = 0x7FF6; /* 15 bit */
        const int68_t B1 = 0x7FEB; /* 15 bit */
        h_o1 = (((i0 - h_i1) * A0) + (h_o1 * B1)) >> 15;
        h_i1 = i0;
        i0   = h_o1;
      }

      /* Butterworth low-pass  */
      {
        o0 = (
          a0 * i0 + a1 * x0 + a2 * x1 -
          b0 * y0 - b1 * y1 ) >> 15;
        x1 = x0; x0 = i0;
        y1 = y0; y0 = o0;
      }

      *dst++ = o0;

    } while (--m);

    PULS.btw.x[0] = x0;
    PULS.btw.x[1] = x1;
    PULS.btw.y[0] = y0;
    PULS.btw.y[1] = y1;

    PULS.hipass_inp1 = h_i1;
    PULS.hipass_out1 = h_o1;

    ym->outptr =
      resampling(ym->outbuf, n, ym->clock>>3, ym->hz);
  }
}

static
int run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
  /* set pointers */
  ym->outbuf = ym->outptr = output;

  /* run the simulation */
  simulation(ym,ymcycles);

  /* post processing (filters, resample ...) */
  filters[ym->emu.puls.ifilter].filter(ym);

  /* reset event list. */
  ym->event_ptr = ym->event_buf;

  return ym->outptr - ym->outbuf;
}

static
int buffersize(const ym_t * const ym, const cycle68_t ymcycles)
{
  return ((ymcycles+7u) >> 3);
}

static
void cleanup(ym_t * const ym)
{
}

int ym_puls_setup(ym_t * const ym)
{
  int err = 0;

  /* fill callback functions */
  ym->cb_cleanup       = cleanup;
  ym->cb_reset         = reset;
  ym->cb_run           = run;
  ym->cb_buffersize    = buffersize;
  ym->cb_sampling_rate = 0;

  /* use default filter */
  PULS.ifilter        = default_filter;

  TRACE68(ym_cat,"ym-2149: filter -- *%s*\n", filters[PULS.ifilter].name);

  return err;
}

static int onchange_filter(const option68_t * opt, value68_t * val)
{
  if (val->num >= 0 && val->num < n_filters) {
    default_filter = val->num;
    return 0;
  }
  return -1;
}

/* command line options option */
/* static const char prefix[] = "sc68-"; */
#define prefix 0
static const char engcat[] = "ym-puls";
static option68_t opts[] = {
  OPT68_ENUM(prefix,"ym-filter",engcat,
             "set ym-2149 filter (pulse only)",
             f_names,sizeof(f_names)/sizeof(*f_names),1,onchange_filter)
};

#undef prefix

void ym_puls_add_options(void)
{
  const int n_opts = sizeof(opts) / sizeof(*opts);

  /* Register ym-puls options */
  option68_append(opts, n_opts);

  /* Default option values */
  option68_iset(opts+0, default_filter, opt68_NOTSET, opt68_CFG);
}
