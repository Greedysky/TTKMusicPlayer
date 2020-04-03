/**
 * @ingroup   lib_emu68
 * @file      emu68/error68.h
 * @brief     Error message stack header.
 * @author    Benjamin Gerard
 * @date      1999/03/13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_ERROR68_H
#define EMU68_ERROR68_H

#include "emu68_api.h"
#include "struct68.h"

/**
 * @defgroup  lib_emu68_err  Error message stack
 * @ingroup   lib_emu68
 *
 *   EMU68 error handling consists on a fixed size stack of
 *   messages. When an EMU68 function fails, it stores a description
 *   message for the error and returns -1. If error stack is full, the
 *   oldest stacked message is removed.
 *
 * @{
 */

EMU68_API
/**
 * Push formatted error message.
 *
 *   The emu68_error_add() function formats error message and pushes
 *   it in error stack.  On overflow the older message is lost.
 *
 * @param  emu68   emulator instance.
 * @param  format  printf() like format string.
 *
 * @return error-code
 */
int emu68_error_add(emu68_t * const emu68, const char * format, ...);

EMU68_API
/**
 * Pop last error message.
 *
 *   Retrieve and remove last error message from error stack.
 *
 * @param  emu68   emulator instance.
 *
 * @return  Last pushed error message.
 * @retval  0  Empty message stack, no more message.
 */
const char * emu68_error_get(emu68_t * const emu68);

/**
 * @}
 */

#endif
