/**
 * @ingroup   lib_io68
 * @file      io68/paula_io.h
 * @brief     Paula IO plugin header.
 * @author    Benjamin Gerard
 * @date      1998/06/18
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_PAULA_IO_H
#define IO68_PAULA_IO_H

#include "emu68/struct68.h"
#include "paulaemul.h"

/**
 * @addtogroup  lib_io68_aga
 * @{
 */

/**
 * @name Paula (Amiga soundchip) IO plugin
 * @{
 */

IO68_EXTERN
/**
 * Initialize paula library.
 */
int paulaio_init(int * argc, char ** argv);

IO68_EXTERN
/**
 * Shutdown paula library.
 */
void paulaio_shutdown(void);

IO68_EXTERN
/**
 * Create paula io instance.
 *
 * @param   emu68  68000 emulator instance
 * @param   parms  Paula parameters
 *
 * @return  Created shifter instance
 * @retval  0
 */
io68_t * paulaio_create(emu68_t * const emu68, paula_parms_t * const parms);

IO68_EXTERN
/**
 * Get/Set sampling rate.
 *
 * @param  io  Paula IO instance
 * @param  hz  @ref paula_hz_e "sampling rate" in hz
 * @return     Actual @ref paula_hz_e "sampling rate".
 */
int paulaio_sampling_rate(io68_t * const io, int hz);

IO68_EXTERN
/**
 * Get paula emulator instance.
 */
paula_t * paulaio_emulator(io68_t * const io);

/**
 * @}
 */

/**
 * @}
 */

#endif
