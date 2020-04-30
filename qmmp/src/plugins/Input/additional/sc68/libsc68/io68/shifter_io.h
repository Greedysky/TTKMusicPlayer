/**
 * @ingroup   lib_io68
 * @file      io68/shifter_io.h
 * @author    Benjamin Gerard
 * @date      1999/06/10
 * @brief     shifter IO plugin header.
 *
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_SHIFTER_IO_H
#define IO68_SHIFTER_IO_H

#include "io68_api.h"

/**
 *  @defgroup  lib_io68_shifter  Atari-ST shifter (video chip)
 *  @ingroup   lib_io68
 *
 *    Atari-ST shifter emulator is limited to 50/60Hz detection.
 *    It is used by some player to adapt the replay speed.
 *    By default this shifter always claims to be in 50hz.
 *
 *
 *    Video address (Low byte always 0 on STF)
 *
 *    - 8201 : Hi  byte of video address
 *    - 8203 : Mid byte of video address
 *
 *    Video counter (read only)
 *
 *    - 8205 : Hi  byte of video counter
 *    - 8207 : Mid byte of video counter
 *    - 8209 : Low byte of video counter
 *
 *    Syncro mode
 *
 *    - 820A :
 *      - bit-0  0:interne(*) 1:externe
 *      - bit-1  0:60hz 1:50hz
 *
 *    Color table (16 word entries)
 *
 *    - 8240-825E :
 *      - STF 3 bit per componant 0x777
 *      - STE 4 bit per componant 0xFFF but msb is lsb
 *             (for backward compatibility)
 *
 *    Resolution
 *
 *    - 8260 (bit-1 bit-0)
 *      -  0 0 : 320x200x16
 *      -  0 1 : 640x200x2
 *      -  1 0 : 640x400x1 (70hz)
 *      -  1 1 : reserved
 *  @{
 */

IO68_EXTERN
/** Init shifter io library.
 *  @return error-code
 */
int shifterio_init(int * argc, char ** argv);

IO68_EXTERN
/** Shutdown shifter io library.
 */
void shifterio_shutdown(void);

IO68_EXTERN
/**
 *  Create shifter io instance.
 *
 *   @param   emu68  68000 emulator instance
 *   @param   hz     Initial frequency (50,60 or 70), default 50
 *
 *   @return  Created shifter instance
 *   @retval  0
 */
io68_t * shifterio_create(emu68_t * const emu68, int hz);

IO68_EXTERN
/** Reset shifter and set new vertical refresh rate. */
int shifterio_reset(io68_t * const io, int hz);

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* #ifndef _IO68_SHIFTER_IO_H_ */
