/*
 * @ingroup   lib_emu68_inl
 * @file      emu68/inl68_shifting.h
 * @brief     68k bit shifting inlines.
 * @author    Benjamin Gerard
 * @date      2009/05/18
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef INL68_SHIFTING_H
#define INL68_SHIFTING_H

static inline
void inl_swap68(emu68_t * const emu68, int dn)
{
  uint68_t v = (u32) REG68.d[dn];
  REG68.d[dn] = v = ( v >> 16 ) + ( (int68_t)(u16)v << 16 );
  REG68.sr = ( REG68.sr & ( 0xFF00 | SR_X ) )
    | ( !v << SR_Z_BIT )
    | ( ( v >> (31 - SR_N_BIT) ) & SR_N )
    ;
}

static inline
int68_t inl_lsl68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  int ccr;

  s &= 63;
  inl_addcycle68(emu68, 2*s);
  if (--s < 0) {
    ccr = REG68.sr & SR_X;              /* X unaffected on no shift */
  } else if (s > SIGN_BIT) {
    d   = 0;
    ccr = 0;
  } else {
    d <<= s;
    ccr = ( d >> SIGN_BIT ) & ( SR_X | SR_C ); /* X & C */
    d <<= 1;
  }
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = (REG68.sr & 0xFF00) | ccr;
  return d;
}

static inline
int68_t inl_lsr68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  int ccr;

  s &= 63;
  inl_addcycle68(emu68, 2*s);
  if (--s < 0) {
    ccr = REG68.sr & SR_X;              /* X unaffected on no shift */
  } else if (s > SIGN_BIT) {
    d   = 0;                            /* shift > word width; X is 0 */
    ccr = 0;
  } else {
    const uint68_t m = NRM_MSK(l);
    d >>= s;
    ccr = -(int)( ( d >> ( SIGN_BIT - l ) ) & 1 ) & ( SR_X | SR_C ); /* X & C */
    d = ( d >> 1 ) & m;
  }
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = (REG68.sr & 0xFF00) | ccr;
  return d;
}

static inline
int68_t inl_asl68(emu68_t * const emu68, int68_t d, int s, const int l)
{
  int ccr;

  s &= 63;
  inl_addcycle68(emu68, 2*s);
  if (--s < 0) {
    ccr = REG68.sr & SR_X;              /* X unaffected on no shift */
  } else if (s > l) {
    ccr = -!!d & SR_V;                  /* V */
    d   = 0;
  } else {
    int68_t r = d;
    d  <<= s;
    ccr  = ( d >> SIGN_BIT ) & ( SR_X | SR_C ); /* X & C */
    d  <<= 1;
    ccr |= -( r != ( (d >> s) >> 1 ) ) & SR_V; /* V on shift can't be revert */
  }
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = ( REG68.sr & 0xFF00 ) | ccr;
  return d;
}

static inline
int68_t inl_asr68(emu68_t * const emu68, int68_t d, int s, const int l)
{
  int ccr;

  s &= 63;
  inl_addcycle68(emu68, 2*s);
  if (--s < 0) {
    ccr = REG68.sr & SR_X;              /* X unaffected on no shift */
  } else if (s > l) {
    d >>= SIGN_BIT;                 /* shift > word width; X is MSB */
    ccr = (int) d & ( SR_C | SR_X );    /* X & C is MSB */
  } else {
    const uint68_t m = NRM_MSK(l);
    d >>= s;
    ccr = -( ( d >> ( SIGN_BIT - l ) ) & 1 ) & ( SR_X | SR_C ); /* X & C */
    d   = ( d >> 1 ) & m;
  }
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = (REG68.sr & 0xFF00) | ccr;
  return d;
}

static inline
int68_t inl_rol68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  int ccr = REG68.sr & (0xFF00 | SR_X); /* X unaffected */

  if (s &= 63) {
    const uint68_t m = NRM_MSK(l);
    inl_addcycle68(emu68, 2*s);
    d    = ( ( d << ( s & l ) ) | ( d >> ( -s & l ) ) ) & m;
    ccr |= -(int)( ( d >> (SIGN_FIX - l) ) & 1 ) & SR_C; /* C is LSB */
  }
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = ccr;
  return d;
}

static inline
int68_t inl_ror68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  int ccr = REG68.sr & (0xFF00 | SR_X); /* X unaffected */

  if (s &= 63) {
    const uint68_t m = NRM_MSK(l);
    inl_addcycle68(emu68, 2*s);
    d    = ( ( d >> ( s & l ) ) | ( d << ( -s & l ) ) ) & m;
    ccr |= ( d >> ( SIGN_BIT - SR_C_BIT ) ) & SR_C; /* C is MSB */
  }
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = ccr;
  return d;
}

static inline
int68_t inl_roxl68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  int ccr = REG68.sr & ( 0xFF00 | SR_X ); /* X is unaffected on no shift */

  if (s &= 63) {
    const uint68_t m = NRM_MSK(l);
    inl_addcycle68(emu68, 2*s);
    s %= (l+2);                         /* s := [0 .. l+1] */
    if (--s >= 0) {                     /* s := [0 .. l] */
      uint68_t x, c, r = d;

      r <<= s;
      c   = (ccr >> SR_X_BIT) & 1;            /* old X */
      x   = r >> SIGN_BIT;                    /* new X */
      ccr = -(int)x & SR_X;
      r <<= 1;
      r  |= c << ( SIGN_BIT - l + s );
      d >>= 1;
      d >>= l - s;
      d  |= r;
      d  &= m;
    }
  }

  ccr |= (ccr & SR_X) >> (SR_X_BIT - SR_C_BIT); /* C is X whatever shift */
  ccr |= -!d & SR_Z;
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N;
  REG68.sr = ccr;
  return d;
}

static inline
int68_t inl_roxr68(emu68_t * const emu68, uint68_t d, int s, const int l)
{
  int ccr = REG68.sr & ( 0xFF00 | SR_X ); /* X unaffected on no shift */

  s &= 63;
  inl_addcycle68(emu68, 2*s);
  if (s) {
    const uint68_t m = NRM_MSK(l);
    s %= l + 2;                         /* s := [0 .. l+1] */
    if (--s >= 0) {                     /* s := [0 .. l] */
      uint68_t r = d;
      uint68_t x, c;

      d >>= s;
      c   = (ccr >> SR_X_BIT) & 1;            /* old X */
      x   = (d >> (SIGN_BIT-l)) & 1;          /* new X */
      ccr = -(int)x & SR_X;
      d >>= 1;
      d  |= c << (SIGN_BIT-s);
      r <<= 1;
      r <<= l-s;
      d  |= r;
      d  &= m;
    }
  }
  ccr |= (ccr & SR_X) >> (SR_X_BIT - SR_C_BIT);   /* C is X */
  ccr |= -!d & SR_Z;                              /* Z */
  ccr |= ( d >> ( SIGN_BIT - SR_N_BIT ) ) & SR_N; /* N */
  REG68.sr = ccr;
  return d;
}

#endif
