/**
 * @ingroup   lib_io68
 * @file      io68/ym_io.h
 * @brief     YM-2149 emulator plugin header.
 * @author    Benjamin Gerard
 * @date      1999/03/20
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_YM_IO_H
#define IO68_YM_IO_H

#include "io68_api.h"
#include "emu68/struct68.h"
#include "ymemul.h"

/**
 * @addtogroup  lib_io68
 * @{
 */

/**
 * @name Yamaha-2149 IO plugin
 * @{
 */

IO68_EXTERN
/** Initialize YM-2149 library. */
int ymio_init(int * argc, char ** argv);

IO68_EXTERN
/** Shutdown YM-2149 library. */
void ymio_shutdown(void);

IO68_EXTERN
/**
 *  Create Yamaha-2149 IO plugin instance.
 *
 * @param  emu68  68K emulator instance
 * @param  parms  YM-2149 creation parameters
 *
 * @return created io instance
 * @retval 0  Failure
 */
io68_t * ymio_create(emu68_t * const emu68, ym_parms_t * const parms);

IO68_EXTERN
/**
 *  Get/Set sampling rate.
 *
 * @param  io  YM IO instance
 * @param  hz  0:read current sampling rate, >0:new requested sampling rate
 *
 * @return actual sampling rate
 *
 */
int ymio_sampling_rate(io68_t * const io, int hz);

IO68_EXTERN
/** Get ym-2149 emulator instance. */
ym_t * ymio_emulator(io68_t * const io);

IO68_EXTERN
/**
 *  Run ym emulator.
 *
 *  @see ym_run()
 */
int ymio_run(const io68_t * const io, s32 * output, const cycle68_t cycles);

IO68_EXTERN
/**
 *  Get required sample buffer size.
 *  @see ym_buffersize()
 */
uint68_t ymio_buffersize(const io68_t * const io, const cycle68_t cycles);

IO68_EXTERN
/**
 *  Convert a cpu-cycle to ym-cycle.
 */
cycle68_t ymio_cycle_cpu2ym(const io68_t * const io, const cycle68_t cycles);

IO68_EXTERN
/**
 * Convert a ym-cycle to cpu-cycle.
 */
cycle68_t ymio_cycle_ym2cpu(const io68_t * const io, const cycle68_t cycles);

IO68_EXTERN
/**
 * Convert a ym-cycle to cpu-cycle.
 */
int ymio_active_channels(const io68_t * const io, const int clr, const int set);


/**
 * @}
 */

/**
 * @}
 */

#endif
