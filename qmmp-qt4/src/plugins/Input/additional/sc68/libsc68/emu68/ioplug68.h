/**
 * @ingroup   lib_emu68
 * @file      emu68/ioplug68.h
 * @brief     68k IO plugger header.
 * @author    Benjamin Gerard
 * @date      1999/03/13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_IOPLUG68_H
#define EMU68_IOPLUG68_H

#include "emu68_api.h"
#include "struct68.h"

/**
 * @defgroup  lib_emu68_ioplug  68k IO plugger
 * @ingroup   lib_emu68
 * @brief     IO plugging and mapping facilities.
 *
 *   Provide functions for warm plugging/unplugging of IO
 *   chipset. Limitations are explained in @ref lib_emu68_mem "68k
 *   memory and IO manager" detailed description.
 *
 * @{
 */

EMU68_API
/**
 * Unplug all IO.
 *
 * @param emu68  emu68 instance
 *
 *   Process emu68_ioplug_unplug() function for all pluged IO.
 *
 */
void emu68_ioplug_unplug_all(emu68_t * const emu68);

EMU68_API
/**
 * Unplug and destroy all IO.
 *
 * @param emu68  emu68 instance
 *
 *   Process emu68_ioplug_unplug() function for all pluged IO and
 *   destroy each io by calling its io68_t::destroy function.
 *
 */
void emu68_ioplug_destroy_all(emu68_t * const emu68);

EMU68_API
/**
 * Unplug an IO.
 *
 *   The emu68_ioplug_unplug() function removes an IO from pluged IO
 *   list and reset memory access handler for its area.
 *
 * @param emu68  emu68 instance
 * @param    io  IO instance to unplug.
 *
 * @return   error-code
 * @retval   0   Success
 * @retval   <0  Error (probably no matching IO)
 */
int emu68_ioplug_unplug(emu68_t * const emu68, io68_t * const io);

EMU68_API
/**
 * Plug an IO.
 *
 *   The emu68_ioplug() function add an IO to pluged IO list and add
 *   suitable memory access handlers.
 *
 * @param emu68  emu68 instance
 * @param    io  IO instance to unplug.
 */
void emu68_ioplug(emu68_t * const emu68, io68_t * const io);

/**
 * @}
 */

#endif
