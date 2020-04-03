/**
 * @ingroup   lib_emu68
 * @file      emu68/excep68.h
 * @brief     68k exception and interruption definition header.
 * @author    Benjamin Gerard
 * @date      1999/13/03
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_EXCEP68_H
#define EMU68_EXCEP68_H

/**
 * @addtogroup  lib_emu68
 *
 * @{
 */

/**
 * @name  68k exceptions and interruptions
 *
 *   68K interruptions are defined by a vector and a level. The
 *   interrupt vector is a long word stored in memory at vector
 *   address. This long word is the location of the interrupt routine
 *   which is loaded in the PC register. The interrupt level is the
 *   value transfered to the IPL field of SR so that no lower leveled
 *   interruption may be triggered.
 *
 * @{
 */

enum emu68_vector_e {
  /* Virtual private vectors. */
  HWBREAK_VECTOR  = 0x100, /**< 1st hardware breakpoint, next follow.   */
  HWTRACE_VECTOR  = 0x120, /**< Report hardware trace.                  */
  HWHALT_VECTOR   = 0x121, /**< Report processor halted.                */
  HWSTOP_VECTOR   = 0x122, /**< Report processor suspended (stop).      */
  HWRESET_VECTOR  = 0x123, /**< Report IO reset (reset).                */
  HWINIT_VECTOR   = 0x124, /**< Report a hardware reset (emu68_reset).  */
  HWINSTOV_VECTOR = 0x125, /**< Report instruction count overflow.      */

  PRIVATE_VECTOR  = 0x200, /**< First private vector for users.         */

  /* Real 68k vectors. */
  RESET_VECTOR    = 0x000, /**< External (hardware) reset.              */
  RESET_SP_VECTOR = 0x000, /**< Initial interrupt stack pointer.        */
  RESET_PC_VECTOR = 0x001, /**< Initial program counter.                */
  BUSERR_VECTOR   = 0x002, /**< Access fault vector address.            */
  ADRERR_VECTOR   = 0x003, /**< Access error vector address.            */
  ILLEGAL_VECTOR  = 0x004, /**< Illegal instruction vector address.     */
  DIVIDE_VECTOR   = 0x005, /**< Integer divide by zero.                 */
  CHK_VECTOR      = 0x006, /**< Chk/Chk2 instructions vector address.   */
  TRAPV_VECTOR    = 0x007, /**< F/TRAP/V/cc istructions vector address. */
  PRIVV_VECTOR    = 0x008, /**< Privilege violation vector address.     */
  TRACE_VECTOR    = 0x009, /**< Trace vector address.                   */
  LINEA_VECTOR    = 0x00A, /**< Linea vector address.                   */
  LINEF_VECTOR    = 0x00B, /**< Linef vector address.                   */
  SPURIOUS_VECTOR = 0x018, /**< Spurious interrupt vector address.      */
  TRAP_VECTOR_0   = 0x020, /**< Trap \#n vector address.                */
};

/** Nth TRAP vector address. */
#define TRAP_VECTOR(N)   ( ( (N) & 15 ) + TRAP_VECTOR_0 )

/** Nth interrupt auto vector address. */
#define AUTO_VECTOR(N)   ( ( (N) & 07 ) + SPURIOUS_VECTOR )

/**
 * @}
 */

/**
 * @}
 */

#endif
