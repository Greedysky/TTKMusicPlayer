/*
 * @ingroup   lib_emu68_inl
 * @file      emu68/inl68_logic.h
 * @brief     68k logical instruction inlines.
 * @author    Benjamin Gerard
 * @date      2009/05/18
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef INL68_LOGIC_H
#define INL68_LOGIC_H

#include "inl68_progctrl.h"             /* for inl_tst68() */

static inline
int68_t inl_and68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  d &= s;
  inl_tst68(emu68, d);
  return d;
}

static inline
int68_t inl_orr68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  d |= s;
  inl_tst68(emu68, d);
  return d;
}

static inline
int68_t inl_eor68(emu68_t * const emu68, const int68_t s, int68_t d)
{
  d ^= s;
  inl_tst68(emu68, d);
  return d;
}

static inline
int68_t inl_not68(emu68_t * const emu68, int68_t d)
{
  d = ~d;
  inl_tst68(emu68, d);
  return d;
}

#endif
