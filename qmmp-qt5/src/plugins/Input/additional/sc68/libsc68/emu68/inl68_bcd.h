/*
 * @ingroup   lib_emu68_inl
 * @file      emu68/inl68_bcd.h
 * @brief     68k binary coded decimal arithmetic inlines.
 * @author    Benjamin Gerard
 * @date      2009/05/18
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef INL68_BCD_H
#define INL68_BCD_H

static inline
int inl_abcd68(emu68_t * const emu68, int a, int b)
{
  int x = ( emu68->reg.sr >> SR_X_BIT ) & 1;
  a += b + x;                           /* unadjusted result */
  b  = a;                               /* store unadjusted result */
  x  = REG68.sr & (SR_Z);               /* use x as ccr */
  if ( (a&15) > 9 )
    a += 6;
  if ( a > 0x90 ) {
    a += 0x60;
    x |= SR_C|SR_X;
  }
  a = (u8)a;
  if (a) x &= ~SR_Z;
  x |= (a&0x80) >> (7-SR_N_BIT);
  x |= ((~b)&a&0x80) >> (7-SR_V_BIT);
  SET_CCR(REG68.sr,x);

  return a;
}

static inline
int inl_sbcd68(emu68_t * const emu68, int a, int b)
{
  int x = ( emu68->reg.sr >> SR_X_BIT ) & 1;
  int r = a - b - x;
  if ( (b&0xF) + x > (a&15) )
    r -= 6;

  b  = a;                               /* store unadjusted result */
  x  = REG68.sr & (SR_Z);               /* use x as ccr */

  if ( r & 0x80 ) {
    r -= 0x60;
    x |= SR_C|SR_X;
  }

  a = (u8)r;
  if (a) x &= ~SR_Z;
  x |= (a&0x80) >> (7-SR_N_BIT);
  x |= (b&(~a)&0x80) >> (7-SR_V_BIT);

  SET_CCR(REG68.sr,x);

  return a;
}

static inline
int inl_nbcd68(emu68_t * const emu68, int a)
{
  return inl_sbcd68(emu68,0,a);
}

#endif
