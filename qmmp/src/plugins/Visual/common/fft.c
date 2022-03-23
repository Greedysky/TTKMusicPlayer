/* fft.c: Iterative implementation of a FFT
 * Copyright (C) 1999 Richard Boulton <richard@tartarus.org>
 * Convolution stuff by Ralph Loader <suckfish@ihug.co.nz>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/*
 * TODO
 * Remove compiling in of FFT_BUFFER_SIZE?  (Might slow things down, but would
 * be nice to be able to change size at runtime.)
 * Finish making / checking thread-safety.
 * More optimisations.
 */

/*
     modifications compared to original code:
     using float format for input data
*/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "fft.h"
#include <math.h>
#ifndef PI
#ifdef M_PI
#define PI M_PI
#else
#define PI            3.14159265358979323846    /* pi */
#endif
#endif

/* ########### */
/* # Structs # */
/* ########### */

struct _struct_fft_state {
    /* Temporary data stores to perform FFT in. */
    float real[FFT_BUFFER_SIZE];
    float imag[FFT_BUFFER_SIZE];
};

/* ############################# */
/* # Local function prototypes # */
/* ############################# */

static void fft_prepare(const float *input, float *re, float *im);
static void fft_calculate(float *re, float *im);
static void fft_output(const float *re, const float *im, float *output);
static int reverseBits(unsigned int initial);

/* #################### */
/* # Global variables # */
/* #################### */

/* Table to speed up bit reverse copy */
static unsigned int bitReverse[FFT_BUFFER_SIZE];

/* The next two tables could be made to use less space in memory, since they
 * overlap hugely, but hey. */
static float sintable[FFT_BUFFER_SIZE / 2];
static float costable[FFT_BUFFER_SIZE / 2];

/* ############################## */
/* # Externally called routines # */
/* ############################## */

/* --------- */
/* FFT stuff */
/* --------- */

/*
 * Initialisation routine - sets up tables and space to work in.
 * Returns a pointer to internal state, to be used when performing calls.
 * On error, returns NULL.
 * The pointer should be freed when it is finished with, by fft_close().
 */
fft_state *
fft_init(void)
{
    fft_state *state;
    unsigned int i;

    state = (fft_state *) malloc(sizeof(fft_state));
    if(!state)
        return 0;

    for(i = 0; i < FFT_BUFFER_SIZE; ++i) {
        bitReverse[i] = reverseBits(i);
    }
    for(i = 0; i < FFT_BUFFER_SIZE / 2; ++i) {
        float j = 2 * PI * i / FFT_BUFFER_SIZE;
        costable[i] = cos(j);
        sintable[i] = sin(j);
    }

    return state;
}

/*
 * Do all the steps of the FFT, taking as input sound data (as described in
 * sound.h) and returning the intensities of each frequency as floats in the
 * range 0 to ((FFT_BUFFER_SIZE / 2) * 32768) ^ 2
 *
 * FIXME - the above range assumes no frequencies present have an amplitude
 * larger than that of the sample variation.  But this is false: we could have
 * a wave such that its maximums are always between samples, and it's just
 * inside the representable range at the places samples get taken.
 * Question: what _is_ the maximum value possible.  Twice that value?  Root
 * two times that value?  Hmmm.  Think it depends on the frequency, too.
 *
 * The input array is assumed to have FFT_BUFFER_SIZE elements,
 * and the output array is assumed to have (FFT_BUFFER_SIZE / 2 + 1) elements.
 * state is a (non-NULL) pointer returned by fft_init.
 */
void
fft_perform(const float *input, float *output, fft_state * state)
{
    /* Convert data from sound format to be ready for FFT */
    fft_prepare(input, state->real, state->imag);

    /* Do the actual FFT */
    fft_calculate(state->real, state->imag);

    /* Convert the FFT output into intensities */
    fft_output(state->real, state->imag, output);
}

/*
 * Free the state.
 */
void
fft_close(fft_state * state)
{
    if(state)
        free(state);
}

/* ########################### */
/* # Locally called routines # */
/* ########################### */

/*
 * Prepare data to perform an FFT on
 */
static void
fft_prepare(const float *input, float *re, float *im)
{
    unsigned int i;
    float *realptr = re;
    float *imagptr = im;

    /* Get input, in reverse bit order */
    for(i = 0; i < FFT_BUFFER_SIZE; ++i) {
        *realptr++ = input[bitReverse[i]] * 32767.0;
        *imagptr++ = 0;
    }
}

/*
 * Take result of an FFT and calculate the intensities of each frequency
 * Note: only produces half as many data points as the input had.
 * This is roughly a consequence of the Nyquist sampling theorm thingy.
 * (FIXME - make this comment better, and helpful.)
 *
 * The two divisions by 4 are also a consequence of this: the contributions
 * returned for each frequency are split into two parts, one at i in the
 * table, and the other at FFT_BUFFER_SIZE - i, except for i = 0 and
 * FFT_BUFFER_SIZE which would otherwise get float (and then 4* when squared)
 * the contributions.
 */
static void
fft_output(const float *re, const float *im, float *output)
{
    float *outputptr = output;
    const float *realptr = re;
    const float *imagptr = im;
    float *endptr = output + FFT_BUFFER_SIZE / 2;

#ifdef DEBUG
    unsigned int i, j;
#endif

    while(outputptr <= endptr) {
        *outputptr = (*realptr * *realptr) + (*imagptr * *imagptr);
        outputptr++;
        realptr++;
        imagptr++;
    }
    /* Do divisions to keep the constant and highest frequency terms in scale
     * with the other terms. */
    *output /= 4;
    *endptr /= 4;

#ifdef DEBUG
    printf("Recalculated input:\n");
    for(i = 0; i < FFT_BUFFER_SIZE; ++i) {
        float val_real = 0;
        float val_imag = 0;
        for(j = 0; j < FFT_BUFFER_SIZE; ++j) {
            float fact_real = cos(-2 * j * i * PI / FFT_BUFFER_SIZE);
            float fact_imag = sin(-2 * j * i * PI / FFT_BUFFER_SIZE);
            val_real += fact_real * re[j] - fact_imag * im[j];
            val_imag += fact_real * im[j] + fact_imag * re[j];
        }
        printf("%5d = %8f + i * %8f\n", i,
               val_real / FFT_BUFFER_SIZE, val_imag / FFT_BUFFER_SIZE);
    }
    printf("\n");
#endif
}

/*
 * Actually perform the FFT
 */
static void
fft_calculate(float *re, float *im)
{
    unsigned int i, j, k;
    unsigned int exchanges;
    float fact_real, fact_imag;
    float tmp_real, tmp_imag;
    unsigned int factfact;

    /* Set up some variables to reduce calculation in the loops */
    exchanges = 1;
    factfact = FFT_BUFFER_SIZE / 2;

    /* Loop through the divide and conquer steps */
    for(i = FFT_BUFFER_SIZE_LOG; i != 0; --i) {
        /* In this step, we have 2 ^ (i - 1) exchange groups, each with
         * 2 ^ (FFT_BUFFER_SIZE_LOG - i) exchanges
         */
        /* Loop through the exchanges in a group */
        for(j = 0; j != exchanges; ++j) {
            /* Work out factor for this exchange
             * factor ^ (exchanges) = -1
             * So, real = cos(j * PI / exchanges),
             *     imag = sin(j * PI / exchanges)
             */
            fact_real = costable[j * factfact];
            fact_imag = sintable[j * factfact];

            /* Loop through all the exchange groups */
            for(k = j; k < FFT_BUFFER_SIZE; k += exchanges << 1) {
                int k1 = k + exchanges;
                /* newval[k]  := val[k] + factor * val[k1]
                 * newval[k1] := val[k] - factor * val[k1]
                 **/
#ifdef DEBUG
                printf("%d %d %d\n", i, j, k);
                printf("Exchange %d with %d\n", k, k1);
                printf("Factor %9f + i * %8f\n", fact_real, fact_imag);
#endif
                /* FIXME - potential scope for more optimization here? */
                tmp_real = fact_real * re[k1] - fact_imag * im[k1];
                tmp_imag = fact_real * im[k1] + fact_imag * re[k1];
                re[k1] = re[k] - tmp_real;
                im[k1] = im[k] - tmp_imag;
                re[k] += tmp_real;
                im[k] += tmp_imag;
#ifdef DEBUG
                for(k1 = 0; k1 < FFT_BUFFER_SIZE; ++k1) {
                    printf("%5d = %8f + i * %8f\n", k1, real[k1], imag[k1]);
                }
#endif
            }
        }
        exchanges <<= 1;
        factfact >>= 1;
    }
}

static int
reverseBits(unsigned int initial)
{
    unsigned int reversed = 0, loop;
    for(loop = 0; loop < FFT_BUFFER_SIZE_LOG; ++loop) {
        reversed <<= 1;
        reversed += (initial & 1);
        initial >>= 1;
    }
    return reversed;
}
