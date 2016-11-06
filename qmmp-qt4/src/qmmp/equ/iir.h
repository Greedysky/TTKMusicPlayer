/*
 *   PCM time-domain equalizer
 *
 *   Copyright (C) 2002-2005  Felipe Rivera <liebremx at users.sourceforge.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   $Id: iir.h,v 1.12 2005/10/17 01:57:59 liebremx Exp $
 */

/*
     modifications compared to original code:
     added 48/96 kHz sampling rate support
     added 24/32bit sample size support
     added optimization
     removed glib dependency
*/

#ifndef IIR_H
#define IIR_H

//#include <glib.h>
//#include "main.h"
#include "iir_cfs.h"

/*
 * Flush-to-zero to avoid flooding the CPU with underflow exceptions
 */
#ifdef SSE_MATH
#define FTZ 0x8000
#define FTZ_ON { \
    unsigned int mxcsr; \
  __asm__  __volatile__ ("stmxcsr %0" : "=m" (*&mxcsr)); \
  mxcsr |= FTZ; \
  __asm__  __volatile__ ("ldmxcsr %0" : : "m" (*&mxcsr)); \
}
#define FTZ_OFF { \
    unsigned int mxcsr; \
  __asm__  __volatile__ ("stmxcsr %0" : "=m" (*&mxcsr)); \
  mxcsr &= ~FTZ; \
  __asm__  __volatile__ ("ldmxcsr %0" : : "m" (*&mxcsr)); \
}
#else
#define FTZ_ON
#define FTZ_OFF
#endif

/*
 * Function prototypes
 */
void init_iir(unsigned int srate, int band_num);
void clean_history();
void set_gain(int index, int chn, float val);
void set_preamp(int chn, float val);


int iir(float * d, int samples, int nch);

#ifdef ARCH_X86
__inline__ int round_trick(float floatvalue_to_round);
#endif
#ifdef ARCH_PPC
__inline__ int round_ppc(float x);
#endif

#define EQ_CHANNELS 9
#define EQ_MAX_BANDS 32

extern float preamp[EQ_CHANNELS];
extern sIIRCoefficients *iir_cf;
extern int band_count;

#ifdef BENCHMARK
extern double timex;
extern int count;
extern unsigned int blength;
#endif

#endif /* #define IIR_H */
