/**
 * @ingroup   lib_io68
 * @file      io68/mfp_io.h
 * @brief     MFP-68901 IO plugin header.
 * @author    Benjamin Gerard
 * @date      1999/03/20
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_MFP_IO_H
#define IO68_MFP_IO_H

#include "io68_api.h"
#include "emu68/struct68.h"

/**
 * @addtogroup  lib_io68_mfp
 * @{
 */

/**
 * @name MFP-68901 (Atari-ST timers) IO plugin
 * @{
 */

IO68_EXTERN
/**
 * Initialize MFP IO library.
 *
 *  @param  argc  pointer argument count (updated).
 *  @param  argv  argument array (updated).
 *  @return status
 *  @retval -1    on error
 *  @retval  0    on success
 */
int mfpio_init(int * argc, char ** argv);

IO68_EXTERN
/**
 * Shutdown MFP IO library.
 */
void mfpio_shutdown(void);

IO68_EXTERN
/**
 * MFP-68901 IO plugin instance.
 */
io68_t * mfpio_create(emu68_t * const emu68);

/**
 * @}
 */

/**
 * @}
 */

#endif
