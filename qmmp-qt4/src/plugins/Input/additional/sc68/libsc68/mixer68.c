/*
 * @file    api68.c
 * @brief   audio mixer
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
#include "mixer68.h"

/* ARM compliant version */
/* #define SWAP_16BITWORD(V) ((V^=V<<16), (V^=V>>16), (V^=V<<16)) */

#define SWAP_16BITWORD(V) (((u32)(V)>>16)+(V<<16))

/*  Mix 16-bit-stereo PCM into 16-bit-stereo with sign change.
 *
 *  PCM' = PCM ^ sign
 *
 *  sign=0          : Keep input sign,
 *  sign=0x00008000 : Change left channel sign
 *  sign=0x80000000 : Change right channel sign
 *  sign=0x80008000 : Change both channel
 */
void mixer68_stereo_16_LR(u32 * dst, u32 * src, int nb, const u32 sign)
{
  u32 *end;

  /* Optimize trivial case : same buffer, no sign change */
  if (!sign && dst == src) {
    return;
  }

  end = dst+nb;
  if (nb&1) {
    *dst++ = (*src++) ^ sign;
  }
  if (nb&2) {
    *dst++ = (*src++) ^ sign;
    *dst++ = (*src++) ^ sign;
  }
  if (dst<end) {
    do {
      *dst++ = (*src++) ^ sign;
      *dst++ = (*src++) ^ sign;
      *dst++ = (*src++) ^ sign;
      *dst++ = (*src++) ^ sign;
    } while (dst < end);
  }

}

/*  Mix 16-bit-stereo PCM into 16-bit-stereo PCM with channel swapping.
 */
void mixer68_stereo_16_RL(u32 * dst, u32 * src, int nb, const u32 sign)
{
  u32 *end;

#undef  MIX_ONE
#define MIX_ONE                                 \
  v = (*src++);                                 \
  *dst++ = SWAP_16BITWORD(v) ^ sign

  end = dst+nb;
  if (nb&1) {
    u32 v;
    MIX_ONE;
  }
  if (nb&2) {
    u32 v;
    MIX_ONE;
    MIX_ONE;
  }
  if (dst<end) {
    do {
      u32 v;
      MIX_ONE;
      MIX_ONE;
      MIX_ONE;
      MIX_ONE;
    } while (dst < end);
  }

}

/*  Mix 16-bit-stereo PCM into 32-bit-stereo-float (-norm..norm)
 */
void mixer68_stereo_FL_LR (float * dst, u32 * src, int nb,
                           const u32 sign, const float norm)
{
  const float mult = norm / 32768.0f;
  int v;
  float * const  end = dst + (nb<<1);
  if (dst < end) {
    do {
      v = (int)(s32)(*src++ ^ sign);
      *dst++ = mult * (float)(s16)(v);
      *dst++ = mult * (float)(v>>16);
    } while (dst < end);
  }
}

/*  Duplicate left channel into right channel and change sign.
 *  PCM' = ( PCM-L | (PCM-L<<16) ) ^ sign
 */
void mixer68_dup_L_to_R(u32 *dst, u32 *src, int nb,
                        const u32 sign)
{
  u32 * const end = dst+nb;
  if (nb&1) {
    u32 v;
    v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
  }
  if (nb&2) {
    u32 v;
    v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
    v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
  }
  if (dst < end) {
    do {
      u32 v;
      v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
      v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
      v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
      v = (u16)*src++; *dst++ = ((v<<16)|v) ^ sign;
    } while (dst < end);
  }

}

/*  Duplicate right channel into left channel and change sign.
 *  PCM = ( PCM-R | (PCM-R>>16) ) ^ sign
 */
void mixer68_dup_R_to_L(u32 *dst, u32 *src, int nb, const u32 sign)
{
  u32 * const end = dst+nb;

  if (nb&1) {
    u32 v;
    v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
  }
  if (nb&2) {
    u32 v;
    v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
    v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
  }
  if (dst < end) {
    do {
      u32 v;
      v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
      v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
      v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
      v = *src++&0xFFFF0000; *dst++ = ((v>>16)|v) ^ sign;
    } while (dst < end);
  }

}

/*  Blend Left and right voice :
 *  factor [0..65536], 0:blend nothing, 65536:swap L/R
 */
void mixer68_blend_LR(u32 * dst, u32 * src, int nb,
                      int factor,
                      const u32 sign_r, const u32 sign_w)
{
  u32 *end;
  int oof;

  if (factor < 0) {
    factor = 0;
  } else if (factor > 65536) {
    factor = 65536;
  }

#undef  MIX_ONE
#define MIX_ONE                                                         \
  r = (int)(s32)(*src++ ^ sign_r);                                      \
  l = r >> 16;                                                          \
  r = (int)(s16)r;                                                      \
  *dst++ = (                                                            \
            ((l*oof+r*factor)&0xFFFF0000)                               \
            |                                                           \
            ((u32)(r*oof+l*factor) >> 16)                               \
                                                                ) ^ sign_w

  oof = 65536-factor;
  end = dst+nb;
  if (nb&1) {
    int l,r;
    MIX_ONE;
  }
  if (nb&2) {
    int l,r;
    MIX_ONE;
    MIX_ONE;
  }
  if (dst < end) {
    do {
      int l,r;

      MIX_ONE;
      MIX_ONE;
      MIX_ONE;
      MIX_ONE;
    } while (dst < end);
  }

}

/*  Multiply left/right (signed) channel by ml/mr factor [-65536..65536]
 */
void mixer68_mult_LR(u32 *dst, u32 *src, int nb,
                     const int ml, const int mr,
                     const u32 sign_r, const u32 sign_w)
{
  u32 * end;

  /* Optimize some trivial case. */

  if (ml == 65536 && mr == 65536) {
    mixer68_stereo_16_LR(dst, src, nb, sign_r ^ sign_w);
    return;
  }

  if (ml==0 && mr==0) {
    mixer68_fill(dst, nb, sign_w);
    return;
  }

#undef  MIX_ONE
#define MIX_ONE                                                 \
  r = (int)(s32)(*src++ ^ sign_r);                              \
  l = (int)(s16)r;                                              \
  r = r >> 16;                                                  \
  *dst++ = ((((u32)(l*ml))>>16) | ((r*mr)&0xFFFF0000)) ^ sign_w

  end = dst+nb;

  if (nb&1) {
    int l,r;
    MIX_ONE;
  }

  if (nb&2) {
    int l,r;
    MIX_ONE;
    MIX_ONE;
  }

  if (dst < end) {
    do {
      int l,r;
      MIX_ONE;
      MIX_ONE;
      MIX_ONE;
      MIX_ONE;
    } while (dst < end);
  }

}

/*  Fill buffer sign with value (RRRRLLLL)
 */
void mixer68_fill(u32 * dst, int nb, const u32 sign)
{
  u32 * const end = dst+nb;;

  if (nb&1) {
    *dst++ = sign;
  }
  if (nb&2) {
    *dst++ = sign;
    *dst++ = sign;
  }
  if (dst < end) {
    do {
      *dst++ = sign;
      *dst++ = sign;
      *dst++ = sign;
      *dst++ = sign;
    } while (dst < end);
  }
}

void mixer68_copy(u32 * dst, u32 * src, int nb)
{
  /* Optimize trivial case : same buffer */
  if (dst == src || nb <= 0) {
    return;
  } else {
    u32 * const end = dst+nb;
    if (nb&1) {
      *dst++ = *src++;
    }
    if (nb&2) {
      *dst++ = *src++;
      *dst++ = *src++;
    }
    if (dst<end) {
      do {
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
      } while (dst < end);
    }
  }
}
