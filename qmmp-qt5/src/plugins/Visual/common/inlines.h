// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//

/*
     modifications compared to original code:
     using float format
*/

#ifndef INLINES_H
#define INLINES_H

#include "fft.h"
#include <math.h>
#include <string.h>

// *fast* convenience functions
static inline void calc_freq(short* dest, float *src)
{
    static fft_state *state = NULL;
    float tmp_out[257];
    int i;

    if (!state)
        state = fft_init();

    fft_perform(src, tmp_out, state);

    for (i = 0; i < 256; i++)
        dest[i] = ((int) sqrt(tmp_out[i + 1])) >> 8;
}

#endif // INLINES_H
