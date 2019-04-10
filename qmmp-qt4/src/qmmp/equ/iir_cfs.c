/*
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
 *
 *   Coefficient stuff
 *
 *   $Id: iir_cfs.c,v 1.1 2005/10/17 01:57:59 liebremx Exp $
 */

/*
     modifications compared to original code:
     added 48/96 kHz sampling rate support
     added 24/32bit sample size support
     removed glib dependency
*/

#include "iir_cfs.h"
#include <stdio.h>
#include <math.h>

/***************************
 * IIR filter coefficients *
 ***************************/
static sIIRCoefficients iir_cf10_11k_11025[10] __attribute__((aligned));
static sIIRCoefficients iir_cf10_22k_22050[10] __attribute__((aligned));
static sIIRCoefficients iir_cforiginal10_44100[10] __attribute__((aligned));
static sIIRCoefficients iir_cforiginal10_48000[10] __attribute__((aligned));
static sIIRCoefficients iir_cf10_44100[10] __attribute__((aligned));
static sIIRCoefficients iir_cf10_48000[10] __attribute__((aligned));
static sIIRCoefficients iir_cf10_96000[10] __attribute__((aligned));
static sIIRCoefficients iir_cf15_44100[15] __attribute__((aligned));
static sIIRCoefficients iir_cf15_48000[15] __attribute__((aligned));
static sIIRCoefficients iir_cf15_96000[15] __attribute__((aligned));
static sIIRCoefficients iir_cf25_44100[25] __attribute__((aligned));
static sIIRCoefficients iir_cf25_48000[25] __attribute__((aligned));
static sIIRCoefficients iir_cf25_96000[25] __attribute__((aligned));
static sIIRCoefficients iir_cf31_44100[31] __attribute__((aligned));
static sIIRCoefficients iir_cf31_48000[31] __attribute__((aligned));
static sIIRCoefficients iir_cf31_96000[31] __attribute__((aligned));

/******************************************************************
 * Definitions and data structures to calculate the coefficients
 ******************************************************************/
static const double band_f011k[] =
{ 31, 62, 125, 250, 500, 1000, 2000, 3000, 4000, 5500
};
static const double band_f022k[] =
{ 31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 11000
};
static const double band_f010[] =
{ 31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
};
static const double band_original_f010[] =
{ 60, 170, 310, 600, 1000, 3000, 6000, 12000, 14000, 16000
};
static const double band_f015[] =
{ 25,40,63,100,160,250,400,630,1000,1600,2500,4000,6300,10000,16000
};
static const double band_f025[] =
{ 20,31.5,40,50,80,100,125,160,250,315,400,500,800,
  1000,1250,1600,2500,3150,4000,5000,8000,10000,12500,16000,20000
};
static const double band_f031[] =
{ 20,25,31.5,40,50,63,80,100,125,160,200,250,315,400,500,630,800,
  1000,1250,1600,2000,2500,3150,4000,5000,6300,8000,10000,12500,16000,20000
};

#define GAIN_F0 1.0
#define GAIN_F1 GAIN_F0 / M_SQRT2

#define SAMPLING_FREQ 44100.0
#define TETA(f) (2*M_PI*(double)f/iir_bands[n].sfreq)
#define TWOPOWER(value) (value * value)

#define BETA2(tf0, tf) \
(TWOPOWER(GAIN_F1)*TWOPOWER(cos(tf0)) \
 - 2.0 * TWOPOWER(GAIN_F1) * cos(tf) * cos(tf0) \
 + TWOPOWER(GAIN_F1) \
 - TWOPOWER(GAIN_F0) * TWOPOWER(sin(tf)))
#define BETA1(tf0, tf) \
    (2.0 * TWOPOWER(GAIN_F1) * TWOPOWER(cos(tf)) \
     + TWOPOWER(GAIN_F1) * TWOPOWER(cos(tf0)) \
     - 2.0 * TWOPOWER(GAIN_F1) * cos(tf) * cos(tf0) \
     - TWOPOWER(GAIN_F1) + TWOPOWER(GAIN_F0) * TWOPOWER(sin(tf)))
#define BETA0(tf0, tf) \
    (0.25 * TWOPOWER(GAIN_F1) * TWOPOWER(cos(tf0)) \
     - 0.5 * TWOPOWER(GAIN_F1) * cos(tf) * cos(tf0) \
     + 0.25 * TWOPOWER(GAIN_F1) \
     - 0.25 * TWOPOWER(GAIN_F0) * TWOPOWER(sin(tf)))

#define GAMMA(beta, tf0) ((0.5 + beta) * cos(tf0))
#define ALPHA(beta) ((0.5 - beta)/2.0)

struct {
    sIIRCoefficients *coeffs;
    const double *cfs;
    double octave;
    int band_count;
    double sfreq;
} iir_bands[] = {
  { iir_cf10_11k_11025,     band_f011k,         1.0,     10, 11025.0 },
  { iir_cf10_22k_22050,     band_f022k,         1.0,     10, 22050.0 },
  { iir_cforiginal10_44100, band_original_f010, 1.0,     10, 44100.0 },
  { iir_cforiginal10_48000, band_original_f010, 1.0,     10, 48000.0 },
  { iir_cf10_44100,         band_f010,          1.0,     10, 44100.0 },
  { iir_cf10_48000,         band_f010,          1.0,     10, 48000.0 },
  { iir_cf10_96000,         band_f010,          1.0,     10, 96000.0 },
  { iir_cf15_44100,         band_f015,          2.0/3.0, 15, 44100.0 },
  { iir_cf15_48000,         band_f015,          2.0/3.0, 15, 48000.0 },
  { iir_cf15_96000,         band_f015,          2.0/3.0, 15, 96000.0 },
  { iir_cf25_44100,         band_f025,          1.0/3.0, 25, 44100.0 },
  { iir_cf25_48000,         band_f025,          1.0/3.0, 25, 48000.0 },
  { iir_cf25_96000,         band_f025,          1.0/3.0, 25, 96000.0 },
  { iir_cf31_44100,         band_f031,          1.0/3.0, 31, 44100.0 },
  { iir_cf31_48000,         band_f031,          1.0/3.0, 31, 48000.0 },
  { iir_cf31_96000,         band_f031,          1.0/3.0, 31, 96000.0 },
  { 0, 0, 0, 0, 0 }
};

/*************
 * Functions *
 *************/

/* Get the coeffs for a given number of bands and sampling frequency */
sIIRCoefficients* get_coeffs(int *bands, unsigned int sfreq)
{
  sIIRCoefficients *iir_cf = 0;
  switch(sfreq)
  {
    case 11025: iir_cf = iir_cf10_11k_11025;
                *bands = 10;
                break;
    case 22050: iir_cf = iir_cf10_22k_22050;
                *bands = 10;
                break;
    case 48000:
                switch(*bands)
                {
                  case 31: iir_cf = iir_cf31_48000; break;
                  case 25: iir_cf = iir_cf25_48000; break;
                  case 15: iir_cf = iir_cf15_48000; break;
                  default: iir_cf = iir_cforiginal10_48000; break;
                }
                break;
    case 96000:
               switch(*bands)
               {
                 case 31: iir_cf = iir_cf31_96000; break;
                 case 25: iir_cf = iir_cf25_96000; break;
                 case 15: iir_cf = iir_cf15_96000; break;
                 default: iir_cf = iir_cf10_96000; break;
               }
               break;

    default:
               switch(*bands)
               {
                  case 31: iir_cf = iir_cf31_44100; break;
                  case 25: iir_cf = iir_cf25_44100; break;
                  case 15: iir_cf = iir_cf15_44100; break;
                  default: iir_cf = iir_cforiginal10_44100; break;
               }
               break;
  }
  return iir_cf;
}

/* Get the freqs at both sides of F0. These will be cut at -3dB */
static void find_f1_and_f2(double f0, double octave_percent, double *f1, double *f2)
{
    double octave_factor = pow(2.0, octave_percent/2.0);
    *f1 = f0/octave_factor;
    *f2 = f0*octave_factor;
}

/* Find the quadratic root
 * Always return the smallest root */
static int find_root(double a, double b, double c, double *x0) {
  double k = c-((b*b)/(4.*a));
  double h = -(b/(2.*a));
  double x1 = 0.;
  if(-(k/a) < 0.)
    return -1;
  *x0 = h - sqrt(-(k/a));
  x1 = h + sqrt(-(k/a));
  if(x1 < *x0)
    *x0 = x1;
  return 0;
}

/* Calculate all the coefficients as specified in the bands[] array */
void calc_coeffs()
{
  int i, n;
  double f1, f2;
  double x0;

  n = 0;
  for(; iir_bands[n].cfs; n++) {
    double *freqs = (double *)iir_bands[n].cfs;
    for(i=0; i<iir_bands[n].band_count; i++)
    {

      /* Find -3dB frequencies for the center freq */
      find_f1_and_f2(freqs[i], iir_bands[n].octave, &f1, &f2);
      /* Find Beta */
      if( find_root(
            BETA2(TETA(freqs[i]), TETA(f1)),
            BETA1(TETA(freqs[i]), TETA(f1)),
            BETA0(TETA(freqs[i]), TETA(f1)),
            &x0) == 0)
      {
        /* Got a solution, now calculate the rest of the factors */
        /* Take the smallest root always (find_root returns the smallest one)
         *
         * NOTE: The IIR equation is
         *	y[n] = 2 * (alpha*(x[n]-x[n-2]) + gamma*y[n-1] - beta*y[n-2])
         *  Now the 2 factor has been distributed in the coefficients
         */
        /* Now store the coefficients */
        iir_bands[n].coeffs[i].beta = 2.0 * x0;
        iir_bands[n].coeffs[i].alpha = 2.0 * ALPHA(x0);
        iir_bands[n].coeffs[i].gamma = 2.0 * GAMMA(x0, TETA(freqs[i]));
#ifdef DEBUG
        printf("Freq[%d]: %f. Beta: %.10e Alpha: %.10e Gamma %.10e\n",
            i, freqs[i], bands[n].coeffs[i].beta,
            bands[n].coeffs[i].alpha, bands[n].coeffs[i].gamma);
#endif
      } else {
        /* Shouldn't happen */
        iir_bands[n].coeffs[i].beta = 0.;
        iir_bands[n].coeffs[i].alpha = 0.;
        iir_bands[n].coeffs[i].gamma = 0.;
        printf("  **** Where are the roots?\n");
      }
    }// for i
  }//for n
}
