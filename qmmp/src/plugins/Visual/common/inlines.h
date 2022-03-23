// Copyright (C) 2000-2001 Brad Hughes <bhughes@trolltech.com>
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
    static fft_state *state = nullptr;
    float tmp_out[257];
    int i;

    if(!state)
    {
        state = fft_init();
    }

    fft_perform(src, tmp_out, state);

    for(i = 0; i < 256; ++i)
    {
        dest[i] = ((int) sqrt(tmp_out[i + 1])) >> 8;
    }
}

static inline void stereo_from_multichannel(float *l,
                                              float *r,
                                              float *s,
                                              long cnt, int chan)
{
    if(chan == 1)
    {
        memcpy(l, s, cnt * sizeof(float));
        memcpy(r, s, cnt * sizeof(float));
        return;
    }

    while(cnt > 0)
    {
        l[0] = s[0];
        r[0] = s[1];
        s += chan;
        l++;
        r++;
        cnt--;
    }
}



static inline void mono_from_multichannel(float *l,
                                            float *s,
                                            long cnt, int chan)
{
    if(chan == 1)
    {
        memcpy(l, s, cnt * sizeof(float));
        return;
    }

    while(cnt > 0)
    {
        l[0] = s[0];
        s += chan;
        l++;
        cnt--;
    }
}

#endif
