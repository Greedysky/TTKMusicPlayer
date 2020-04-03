/*
 * @ingroup   lib_emu68
 * @file      emu68/inl68_arithmetic.h
 * @brief     68k arithmetical instruction inlines.
 * @author    Benjamin Gerard
 * @date      2009/05/18
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef INL68_ARITHMETIC_H
#define INL68_ARITHMETIC_H

static inline
int68_t inl_add68(emu68_t * const emu68, int68_t s, int68_t d, int68_t c)
{
  /*
    Official version from 68K RPM :
    V = (s & d & ~r) | (~s & ~d & r);  // MSBit only
    C = (s & d) | (~r & d) | (s & ~r); // MSBit only
    X = C;
    Z = !r;
    N = r;                             // MSBit only
  */
  const int68_t r = ( c += s + d );
  c  = ( ( r >> SIGN_BIT ) & ( SR_V | SR_C | SR_X | SR_N ) ) ^ SR_V;
  s  = ( ( s >> SIGN_BIT ) & ( SR_V | SR_C | SR_X ) ) ^ c;
  d  = ( ( d >> SIGN_BIT ) & ( SR_V | SR_C | SR_X ) ) ^ c;
  c &= ~SR_N;
  c |= SR_V | ( !r << SR_Z_BIT );
  c ^= s | d;
  REG68.sr = ( REG68.sr & 0xFF00 ) | c;
  return r;
}

static inline
int68_t inl_sub68(emu68_t * const emu68, int68_t s, int68_t d, int68_t c)
{
  /*
    Official version from 68K RPM :
    V = (~s & d & ~r) | (s & ~d & r);   // MSBit only
    C = (s & ~d) | (r & ~d) | (s & r);  // MSBit only
    X = C
    Z = !r;
    N = r;                              // MSBit only
  */
  const int68_t r = d - s - c;
  REG68.sr = ( REG68.sr & 0xFF00 )
    | (!r << SR_Z_BIT)
    | (( r >> SIGN_BIT ) & SR_N)
    | (((((d ^ ~r) & (s ^ r)) ^ r) >> SIGN_BIT) & (SR_C|SR_X))
    | ((((d ^ r) & (~s ^ r)) >> SIGN_BIT) & SR_V)
    ;
  return r;
}

static inline
void inl_cmp68(emu68_t * const emu68, int68_t s, int68_t d)
{
  /* Like SUB but X is not taken into account nor affected. */
  const int68_t r = d - s;
  REG68.sr = ( REG68.sr & ( 0xFF00 | SR_X ) )
    | (!r << SR_Z_BIT)
    | (( r >> SIGN_BIT ) & SR_N)
    | (((((d ^ ~r) & (s ^ r)) ^ r) >> SIGN_BIT) & SR_C)
    | ((((d ^ r) & (~s ^ r)) >> SIGN_BIT) & SR_V)
    ;
}

static inline
int68_t inl_neg68(emu68_t * const emu68, int68_t d, int68_t c)
{
  /*
    Official version from 68K RPM :
    V = (d & r)                         // MSBit only
    C = (d | r)                         // MSBit only
    X = C
    Z = !r;
    N = r;                              // MSBit only
  */
  int sr = REG68.sr & 0xFF00;
  const int68_t r = ( c = -d - c );
  sr |= !r << SR_Z_BIT;
  d >>= SIGN_BIT;
  c >>= SIGN_BIT;
  sr |= c & SR_N;
  sr |= ( d | c ) & (SR_C|SR_X);
  sr |= ( d & c ) & SR_V;
  REG68.sr = sr;
  return r;
}


static inline
int68_t inl_muls68(emu68_t * const emu68, int68_t s, int68_t d)
{
  d        = ( d >> WORD_FIX ) * ( s >> WORD_FIX );
  REG68.sr = ( REG68.sr & ( 0xFF00 | SR_X ) )
    | ( !d << SR_Z_BIT )
    | ( (d >> (31 - SR_N_BIT)) & SR_N );
  return d << LONG_FIX;
}

static inline
int68_t inl_mulu68(emu68_t * const emu68, uint68_t s, uint68_t d)
{
  d        = ( d >> WORD_FIX ) * ( s >> WORD_FIX );
  REG68.sr = ( REG68.sr & ( 0xFF00 | SR_X ) )
    | ( !d << SR_Z_BIT )
    | ( (d >> (31 - SR_N_BIT)) & SR_N );
  return d << LONG_FIX;
}

static inline
int68_t inl_divs68(emu68_t * const emu68, int68_t s, int68_t d)
{
  int sr = REG68.sr & (0xFF00 | SR_X);  /* X unaffected; C cleared */
  if ( s >>= WORD_FIX ) {
    int68_t res, rem;
    d >>= LONG_FIX;
    res = d / s;
    rem = d % s;

    if ( (s16)res != res )
      sr |= SR_V;                       /* V on overflow */
    else
      d = ( (int68_t) (u16) res ) | ( (int68_t) (u16) rem << 16 );
    sr |= !res << SR_Z_BIT;                    /* Z */
    sr |= ( res >> ( 15 - SR_N_BIT ) ) & SR_N; /* N */
    REG68.sr = sr;
    d <<= LONG_FIX;
  } else {
    /* Divide by zero */
    REG68.sr = sr;
    inl_exception68(emu68, DIVIDE_VECTOR, -1);
  }
  return d;
}

static inline
int68_t inl_divu68(emu68_t * const emu68, uint68_t s, uint68_t d)
{
  int sr = REG68.sr & (0xFF00 | SR_X);  /* X unaffected; C cleared */
  if ( s >>= WORD_FIX ) {
    uint68_t res, rem;
    d >>= LONG_FIX;
    res = d / s;
    rem = d % s;

    if ( (u16)res != res )
      sr |= SR_V;                       /* V on overflow */
    else
      d = ( res ) | ( rem << 16 );
    sr |= !res << SR_Z_BIT;                    /* Z */
    sr |= ( res >> ( 15 - SR_N_BIT ) ) & SR_N; /* N */
    REG68.sr = sr;
    d <<= LONG_FIX;
  } else {
    /* Divide by zero */
    REG68.sr = sr;
    inl_exception68(emu68, DIVIDE_VECTOR, -1);
  }
  return d;
}

static inline
int68_t inl_clr68(emu68_t * const emu68)
{
  REG68.sr = ( REG68.sr & ( 0xFF00 | SR_X ) ) | SR_Z;
  return 0;
}

#endif
