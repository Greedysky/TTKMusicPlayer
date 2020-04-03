/**
 * @ingroup   lib_emu68
 * @file      emu68/cc68.h
 * @brief     Condition code header.
 * @author    Benjamin Gerard
 * @date      1999/03/13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_CC68_H
#define EMU68_CC68_H

#include "emu68_api.h"
#include "srdef68.h"

/**
 * @addtogroup   lib_emu68_core
 * @{
 */

/**
 * Code Conditions.
 */
enum {
  CC68_TR = 0x0,  CC68_FA = 0x1,  CC68_HI = 0x2,  CC68_LS = 0x3,
  CC68_CC = 0x4,  CC68_CS = 0x5,  CC68_NE = 0x6,  CC68_EQ = 0x7,
  CC68_VC = 0x8,  CC68_VS = 0x9,  CC68_PL = 0xA,  CC68_MI = 0xB,
  CC68_GE = 0xC,  CC68_LT = 0xD,  CC68_GT = 0xE,  CC68_LE = 0xF
};

/**
 * @name  Condition tests.
 *
 *   Theses macros perform conditionnal tests depending on CCR value.
 *
 * @return  1  condition true
 * @return  0  condition false
 * @{
 */

/**
 * 0000 True.
 * @param  sr  current SR value
 * @retval always true
 */
static inline int inl_is_cc0(const int sr)
{
  return 1;
}

/**
 * 0001 False.
 * @param  sr  current SR value
 * @return always false
 */
static inline int inl_is_cc1(const int sr)
{
  return 0;
}

/**
 * 0010 High.
 * @param  sr  current SR value
 * @retval ¬(C|Z) <-> ¬C&¬Z
 */
static inline int inl_is_cc2(const int sr)
{
  return ~
    ( ( sr >> SR_C_BIT )
      |
      ( sr >> SR_Z_BIT )
      ) & 1;
}

/**
 * 0011 Low or Same.
 * @param  sr  current SR value
 * @return (C|Z)
 */
static inline int inl_is_cc3(const int sr)
{
  return
    ( ( sr >> SR_C_BIT )
      |
      ( sr >> SR_Z_BIT )
      ) & 1;
}
/* #define IS_LS(SR) inl_is_cc3(SR) */

/**
 * 0100 Carry Clear.
 * @param  sr  current SR value
 * @return ¬C
 */
static inline int inl_is_cc4(const int sr)
{
  return ~
    ( sr >> SR_C_BIT ) & 1;
}

/**
 * 0101 Carry Set.
 * @param  sr  current SR value
 * @return C
 */
static inline int inl_is_cc5(const int sr)
{
  return
    ( sr >> SR_C_BIT ) & 1;
}

/**
 * 0110 Not Equal (not zero).
 * @param  sr  current SR value
 * @return ¬Z
 */
static inline int inl_is_cc6(const int sr)
{
  return ~
    ( sr >> SR_Z_BIT ) & 1;
}

/**
 * 0111 Equal (zero).
 * @param  sr  current SR value
 * @return Z
 */
static inline int inl_is_cc7(const int sr)
{
  return
    ( sr >> SR_Z_BIT ) & 1;
}

/**
 * 1000 Overflow Clear.
 * @param  sr  current SR value
 * @return ¬V
 */
static inline int inl_is_cc8(const int sr)
{
  return ~
    ( sr >> SR_V_BIT ) & 1;
}

/**
 * 1001 Overflow Set.
 * @param  sr  current SR value
 * @return V
 */
static inline int inl_is_cc9(const int sr)
{
  return
    ( sr >> SR_V_BIT ) & 1;
}

/**
 * 1010 Plus.
 * @param  sr  current SR value
 * @return ¬N
 */
static inline int inl_is_ccA(const int sr)
{
  return ~
    ( sr >> SR_N_BIT ) & 1;
}

/**
 * 1011 Minus.
 * @param  sr  current SR value
 * @return N
 */
static inline int inl_is_ccB(const int sr)
{
  return
    ( sr >> SR_N_BIT ) & 1;
}

/**
 * 1100 Greater or Equal.
 * @param  sr  current SR value
 * @return ¬(N^V)
 */
static inline int inl_is_ccC(const int sr)
{
  return ~
    ( ( sr >> SR_N_BIT )
      ^
      ( sr >> SR_V_BIT )
      ) & 1;
}

/**
 * 1101 Less Than.
 * @param  sr  current SR value
 * @return N^V
 */
static inline int inl_is_ccD(const int sr)
{
  return
    ( ( sr >> SR_N_BIT )
      ^
      ( sr >> SR_V_BIT )
      ) & 1;
}

/**
 * 1110 Greater Than.
 * @param  sr  current SR value
 * @return ¬(Z|(N^V))
 */
static inline int inl_is_ccE(const int sr)
{
  return ~
    ( ( sr >> SR_Z_BIT )
      |
      ( ( sr >> SR_N_BIT )
        ^
        ( sr >> SR_V_BIT ) )
      ) & 1;
}

/**
 * 1111 Less or Equal.
 * @param  sr  current SR value
 * @return (Z|(N^V))
 */
static inline int inl_is_ccF(const int sr)
{
  return
    ( ( sr >> SR_Z_BIT )
      |
      ( ( sr >> SR_N_BIT )
        ^
        ( sr >> SR_V_BIT ) )
      ) & 1;
}

/**
 * @}
 */

/**
 *  @}
 */

#endif
