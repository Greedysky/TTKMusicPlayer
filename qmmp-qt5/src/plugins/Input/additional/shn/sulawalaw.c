/*
 * $Id: sulawalaw.c,v 1.5 2001/12/30 05:12:04 jason Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "shorten.h"

int Sulaw2lineartab[] = {-32124, -31100, -30076, -29052, -28028, -27004,
  -25980, -24956, -23932, -22908, -21884, -20860, -19836, -18812,
  -17788, -16764, -15996, -15484, -14972, -14460, -13948, -13436,
  -12924, -12412, -11900, -11388, -10876, -10364, -9852, -9340, -8828,
  -8316, -7932, -7676, -7420, -7164, -6908, -6652, -6396, -6140, -5884,
  -5628, -5372, -5116, -4860, -4604, -4348, -4092, -3900, -3772, -3644,
  -3516, -3388, -3260, -3132, -3004, -2876, -2748, -2620, -2492, -2364,
  -2236, -2108, -1980, -1884, -1820, -1756, -1692, -1628, -1564, -1500,
  -1436, -1372, -1308, -1244, -1180, -1116, -1052, -988, -924, -876,
  -844, -812, -780, -748, -716, -684, -652, -620, -588, -556, -524,
  -492, -460, -428, -396, -372, -356, -340, -324, -308, -292, -276,
  -260, -244, -228, -212, -196, -180, -164, -148, -132, -120, -112,
  -104, -96, -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0,
  32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956, 23932, 22908,
  21884, 20860, 19836, 18812, 17788, 16764, 15996, 15484, 14972, 14460,
  13948, 13436, 12924, 12412, 11900, 11388, 10876, 10364, 9852, 9340,
  8828, 8316, 7932, 7676, 7420, 7164, 6908, 6652, 6396, 6140, 5884,
  5628, 5372, 5116, 4860, 4604, 4348, 4092, 3900, 3772, 3644, 3516,
  3388, 3260, 3132, 3004, 2876, 2748, 2620, 2492, 2364, 2236, 2108,
  1980, 1884, 1820, 1756, 1692, 1628, 1564, 1500, 1436, 1372, 1308,
  1244, 1180, 1116, 1052, 988, 924, 876, 844, 812, 780, 748, 716, 684,
  652, 620, 588, 556, 524, 492, 460, 428, 396, 372, 356, 340, 324, 308,
  292, 276, 260, 244, 228, 212, 196, 180, 164, 148, 132, 120, 112, 104,
  96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8, 0};

#ifndef Sulaw2linear
#ifdef __STDC__
int Sulaw2linear(uint8_t ulaw) {
#else
int Sulaw2linear(ulaw) uint8_t ulaw; {
#endif
  return(Sulaw2lineartab[ulaw]);
}
#endif

/* adapted by ajr for int input */
#ifdef __STDC__
uint8_t Slinear2ulaw(int sample) {
#else
uint8_t Slinear2ulaw(sample) int sample; {
#endif
/*
** This routine converts from linear to ulaw.
**
** Craig Reese: IDA/Supercomputing Research Center
** Joe Campbell: Department of Defense
** 29 September 1989
**
** References:
** 1) CCITT Recommendation G.711  (very difficult to follow)
** 2) "A New Digital Technique for Implementation of Any
**     Continuous PCM Companding Law," Villeret, Michel,
**     et al. 1973 IEEE Int. Conf. on Communications, Vol 1,
**     1973, pg. 11.12-11.17
** 3) MIL-STD-188-113,"Interoperability and Performance Standards
**     for Analog-to_Digital Conversion Techniques,"
**     17 February 1987
**
** Input: Signed 16 bit linear sample
** Output: 8 bit ulaw sample
*/

#define BIAS 0x84   /* define the add-in bias for 16 bit samples */
#define CLIP 32635

  static int exp_lut[256] = {0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
                             4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
                             5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                             5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
  int sign, exponent, mantissa;
  uint8_t ulawbyte;

  /* Get the sample into sign-magnitude. */
  if(sample < 0) {
    sign   = 0x80;
    sample = -sample;
  }
  else
    sign = 0;

  /* clip the magnitude */
  if(sample > CLIP) sample = CLIP;

  /* Convert from 16 bit linear to ulaw. */
  sample = sample + BIAS;
  exponent = exp_lut[( sample >> 7 ) & 0xFF];
  mantissa = (sample >> (exponent + 3)) & 0x0F;
  ulawbyte = ~(sign | (exponent << 4) | mantissa);

  return(ulawbyte);
}


/******************
 * ALAW starts here
 */

int Salaw2lineartab[] = {-5504, -5248, -6016, -5760, -4480, -4224,
  -4992, -4736, -7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
  -2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368, -3776, -3648,
  -4032, -3904, -3264, -3136, -3520, -3392, -22016, -20992, -24064,
  -23040, -17920, -16896, -19968, -18944, -30208, -29184, -32256,
  -31232, -26112, -25088, -28160, -27136, -11008, -10496, -12032,
  -11520, -8960, -8448, -9984, -9472, -15104, -14592, -16128, -15616,
  -13056, -12544, -14080, -13568, -344, -328, -376, -360, -280, -264,
  -312, -296, -472, -456, -504, -488, -408, -392, -440, -424, -88, -72,
  -120, -104, -24, -8, -56, -40, -216, -200, -248, -232, -152, -136,
  -184, -168, -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
  -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696, -688, -656,
  -752, -720, -560, -528, -624, -592, -944, -912, -1008, -976, -816,
  -784, -880, -848, 5504, 5248, 6016, 5760, 4480, 4224, 4992, 4736,
  7552, 7296, 8064, 7808, 6528, 6272, 7040, 6784, 2752, 2624, 3008,
  2880, 2240, 2112, 2496, 2368, 3776, 3648, 4032, 3904, 3264, 3136,
  3520, 3392, 22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
  30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136, 11008, 10496,
  12032, 11520, 8960, 8448, 9984, 9472, 15104, 14592, 16128, 15616,
  13056, 12544, 14080, 13568, 344, 328, 376, 360, 280, 264, 312, 296,
  472, 456, 504, 488, 408, 392, 440, 424, 88, 72, 120, 104, 24, 8, 56,
  40, 216, 200, 248, 232, 152, 136, 184, 168, 1376, 1312, 1504, 1440,
  1120, 1056, 1248, 1184, 1888, 1824, 2016, 1952, 1632, 1568, 1760,
  1696, 688, 656, 752, 720, 560, 528, 624, 592, 944, 912, 1008, 976,
  816, 784, 880, 848};

#ifndef Salaw2linear
#ifdef __STDC__
int Salaw2linear(uint8_t alaw) {
#else
int Salaw2linear(alaw) uint8_t alaw; {
#endif
  return(Salaw2lineartab[alaw]);
}
#endif

/* this is derived from the Sun code - it is a bit simpler and has int input */
#define QUANT_MASK      (0xf)           /* Quantization field mask. */
#define NSEGS           (8)             /* Number of A-law segments. */
#define SEG_SHIFT       (4)             /* Left shift for segment number. */
#ifdef __STDC__
uint8_t Slinear2alaw(int linear) {
#else
uint8_t Slinear2alaw(linear) int linear; {
#endif
  int	seg;
  uint8_t aval, mask;
  static int16_t seg_aend[NSEGS] = {0x1f,0x3f,0x7f,0xff,0x1ff,0x3ff,0x7ff,0xfff};

  linear = linear >> 3;

  if(linear >= 0) {
    mask = 0xd5;		/* sign (7th) bit = 1 */
  } else {
    mask = 0x55;		/* sign bit = 0 */
    linear = -linear - 1;
  }

  /* Convert the scaled magnitude to segment number. */
  for(seg = 0; seg < NSEGS && linear > seg_aend[seg]; seg++);

  /* Combine the sign, segment, and quantization bits. */
  if(seg >= NSEGS)		/* out of range, return maximum value. */
    return (uint8_t) (0x7F ^ mask);
  else {
    aval = (uint8_t) seg << SEG_SHIFT;
    if (seg < 2)
      aval |= (linear >> 1) & QUANT_MASK;
    else
      aval |= (linear >> seg) & QUANT_MASK;
    return (aval ^ mask);
  }
}
