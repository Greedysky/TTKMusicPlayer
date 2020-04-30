/**
 * @ingroup   lib_io68
 * @file      io68/io68.h
 * @author    Benjamin Gerard
 * @date      1999/03/20
 * @brief     all io68 headers.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_IO68_H
#define IO68_IO68_H

#include "io68_api.h"

#include "ym_io.h"
#include "mw_io.h"
#include "shifter_io.h"
#include "mfp_io.h"
#include "paula_io.h"

/**
 * @defgroup  lib_io68  Chipset emulators.
 * @ingroup   lib_sc68
 *
 * @{
 */

IO68_API
/**
 * Initialize the io68 library.
 *
 *   The io68_init() function setup chipset emulator engines.
 *
 * @param   argc  pointer to argument count.
 * @param   argv  argument array.
 * @retval  0 on success
 * @retval -1 on error
 */
int io68_init(int * argc, char ** argv);

IO68_API
/**
 * Shutdown the io68 library.
 *
 *   The io68_shutdown() function cleanup emulator engines. It should
 *   call the corresponding shutdown function for each existing
 *   chipset emulators. It should be call only after all chipset
 *   instances have been released.
 */
void io68_shutdown(void);

IO68_API
/**
 * Destroy an chipset instance.
 *
 *   The io68_destroy() function calls the io68_t::destroy() callback
 *   function or if it is null just free the memory.
 *
 *  @param  io  chipset instance
 */
void io68_destroy(io68_t * const io);

IO68_API
/**
 * Reset chipset.
 *
 *   The io68_reset() function calls the io68_t::reset() callback
 *   function.
 *
 *  @param  io  chipset instance
 *  @retval  0  on success
 *  @retval -1  on failure
 */
int io68_reset(io68_t * const io);

/**
 * @}
 */

#endif
