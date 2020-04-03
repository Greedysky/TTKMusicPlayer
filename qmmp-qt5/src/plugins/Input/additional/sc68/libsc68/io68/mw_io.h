/**
 * @ingroup   lib_io68
 * @file      io68/mw_io.h
 * @brief     STE sound IO plugin header.
 * @author    Benjamin Gerard
 * @date      1999/03/20
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_MW_IO_H
#define IO68_MW_IO_H

#include "io68_api.h"
#include "mwemul.h"

/**
 * @addtogroup  lib_io68_mw
 * @{
 */

/**
 * @name Microwire/LMC (STE sound) IO plugin.
 * @{
 */

IO68_EXTERN
/**
 * Init the mwio library.
 *
 *   The mwio_init() function setup the microwire IO plugin and the
 *   microwire emulator library by calling the mw_init() function.
 *
 * @param argc  pointer to argument count
 * @param argv  arguemnt array
 * @return      error status
 * @retval   0  on success
 * @retval  -1  on success
 */
int mwio_init(int * argc, char ** argv);

IO68_EXTERN
/**
 * Shutdown mwio library.
 */
void mwio_shutdown(void);

IO68_EXTERN
/**
 * Create a microwire plugin instance.
 */
io68_t * mwio_create(emu68_t * const emu68, mw_parms_t * const parms);

IO68_EXTERN
/**
 * Get/Set sampling rate.
 *
 * @param  io  MW IO instance
 * @param  hz  @ref mw_hz_e "sampling rate"
 *
 * @return current @ref mw_hz_e "sampling rate"
 *
 */
int mwio_sampling_rate(io68_t * const io, int hz);

IO68_EXTERN
/**
 * Get/Set emulator engine.
 *
 * @param  io      MW IO instance
 * @param  engine  @ref mw_engine_e "MW engine descriptor"
 *
 * @return @ref mw_engine_e "MW engine descriptor"
 */
uint68_t mwio_engine(io68_t * const io, int engine);

IO68_EXTERN
/**
 * Get microwire emulator instance attached to the mwio plugin.
 */
mw_t * mwio_emulator(io68_t * const io);

/**
 * @}
 */

/**
 * @}
 */

#endif
