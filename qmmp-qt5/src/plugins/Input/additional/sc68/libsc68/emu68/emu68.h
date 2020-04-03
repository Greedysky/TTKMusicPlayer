/**
 * @ingroup   lib_emu68
 * @file      emu68/emu68.h
 * @brief     68K emulator header.
 * @author    Benjamin Gerard
 * @date      1999/03/13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_EMU68_H
#define EMU68_EMU68_H

#include "emu68_api.h"
#include "struct68.h"
#include "error68.h"
#include "cc68.h"
#include "srdef68.h"
#include "mem68.h"

/**
 * @defgroup  lib_emu68  68k emulator library
 * @ingroup   lib_sc68
 * @brief     The 68k emulator library.
 */

/**
 * @defgroup  lib_emu68_core  68k emulator core
 * @ingroup   lib_emu68
 * @brief     The core of the 68k emulator.
 */

/**
 * @addtogroup  lib_emu68
 * @{
 */

/**
 * @name  Library init functions
 * @{
 */

EMU68_API
/**
 * Init EMU68 library.
 *
 *   The emu68_init() function initializes the EMU68 library. It
 *   should be call once before any other emu68 call. The function
 *   will perform internal setup. EMU68 behaviour can be adjust by
 *   passing arguments in the argv[] array. Parsed arguments are
 *   removed and the remaining number of argument is returned.
 *
 * @retval  -1     on error
 * @retval   0     on success
 *
 * @see emu68_shutdown()
 */
int emu68_init(int * argc, char ** argv);

EMU68_API
/**
 * Shutdown 68K emulator.
 *
 *   The emu68_shutdown() function shutdown the EMU68 library. It must
 *   be call at the end and further more calls are forbidden except
 *   for emu68_init(). All emulator instances created should have been
 *   killed before.
 *
 * @see emu68_init()
 */
void emu68_shutdown(void);

/**
 * @}
 */


/**
 * @name EMU68 instance functions
 * @{
 */

/**
 * Atari ST clock (as written on cristal clock chip).
 */
enum {
  EMU68_ATARIST_CLOCK = (8010613u&~3u)
};

/**
 * 68k emulator instance creation parameters. This structure have to
 * be filled before calling the emu68_create() function to customize
 * the instance. Members set to zero are replaced by default values.
 */
typedef struct {
  const char * name;  /**< Identifier name.                         */
  int log2mem;        /**< Memory amount (value of the power of 2). */
  int clock;          /**< CPU clock frequency (in hz).             */
  int debug;          /**< Run in debug mode (0:off).               */
} emu68_parms_t;

EMU68_API
/**
 * Create a 68k emulator instance.
 *
 *   The emu68_create() function creates an instance of the 68k
 *   emulator.  The logmem parameter is the size of the 68K memory
 *   expressed in power of 2. Valid values are in the range 17 to 24
 *   (inclusive) respectively 128 Kb to 16 Mb. Members set to zero
 *   will be replaced by the default value; alternatively if null
 *   pointer is passed the whole default parameters is applied (512KB
 *   at 8Mhz).
 *
 * @param   parms  Creation parameters or null pointer.
 * @return         emu68 instance
 * @retval  0      on error
 */
emu68_t * emu68_create(emu68_parms_t * const parms);

EMU68_API
/**
 * Duplicate a 68k emulator instance.
 *
 *   The emu68_dup() function creates an new instance of the 68k
 *   emulator which is a duplicate of the given emu68 instance.
 *
 * @param  emu68  emulator instance to duplicate
 * @param  name   duplicate emulator name [0:auto]
 *
 * @return        duplicated emu68 instance
 * @retval  0     on error
 *
 * @todo Duplicate attached IO is currently not supported since IO
 *       modules do not have a dup() function.
 */
emu68_t * emu68_duplicate(emu68_t * emu68, const char * name);

EMU68_API
/**
 * Destroy a 68k emulator instance.
 * @param  emu68  emulator instance
 */
void emu68_destroy(emu68_t * const emu68);

EMU68_API
/**
 * Hardware Reset.
 *
 *   Perform following operations:
 *   - PC = 0
 *   - SR = 2700
 *   - A7 = end of mem - 4
 *   - All registers cleared
 *   - All IO reseted
 *
 * @param  emu68  emulator instance
 */
void emu68_reset(emu68_t * const emu68);

/**
 * @}
 */


/*  /\  ============================================================  /\  */
/* <  > ============================================================ <  > */
/*  \/  ============================================================  \/  */


/**
 * @name  Exception and Interruption control functions.
 *
 *    EMU68 has a very limited interrupt handler. In fact only one
 *    source of interruption is used which is enought for sc68
 *    needs. The emu68_set_interrupt_io() function selects the given
 *    IO chip as the candidate to interruption.
 *
 *    Exception can be trapped and notified by calling specified
 *    handler function.
 *
 * @{
 */

EMU68_API
/**
 * Set new interrupt IO.
 *
 * @param  emu68  emulator instance
 * @param  io     pointer to the only io that could possibly interrupt
 * @return        pointer to previous interrupt IO
 */
io68_t * emu68_set_interrupt_io(emu68_t * const emu68, io68_t * const io);

EMU68_API
/**
 * Set user-data pointer.
 *
 * @param  emu68   emulator instance
 * @param  cookie  user-data pointer
 * @return previous user-data pointer
 */
void * emu68_set_cookie(emu68_t * const emu68, void * cookie);

EMU68_API
/**
 * Get user-data pointer.
 *
 * @param  emu68   emulator instance
 * @return  current user-data pointer
 */
void * emu68_get_cookie(emu68_t * const emu68);

EMU68_API
/**
 * Set user handler.
 *
 * @param  emu68  emulator instance
 * @param  hdl    user handler (0:do not update)
 * @return previous handler
 */
emu68_handler_t emu68_set_handler(emu68_t * const emu68, emu68_handler_t hdl);

EMU68_API
/**
 * Get exception name.
 *
 * @param  vector  Exception vector number
 * @param  buffer  Buffer to store name (0:use internal static buffer)
 * @return exception name copied in buffer (never nul)
 */
const char * emu68_exception_name(unsigned int vector, char * buffer);

/**
 * @}
 */


/**
 * @name  Cycle counter access functions.
 * @{
 */

EMU68_API
/**
 * Set internal cycle counter.
 *
 * @param  emu68  emulator instance
 * @param  cycle  cycle number
 */
void emu68_set_cycle(emu68_t * const emu68, cycle68_t cycle);

EMU68_API
/**
 * Get internal cycle counter.
 *
 * @param  emu68  emulator instance
 */
cycle68_t emu68_get_cycle(emu68_t * const emu68);

/**
 * @}
 */


/*  /\  ============================================================  /\  */
/* <  > ============================================================ <  > */
/*  \/  ============================================================  \/  */


/**
 * @name  EMU68 on-board memory access
 * @{
 */

EMU68_API
/**
 * Check and get a valid 68k onboard memory block.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of memory block to access
 * @param  size   size in byte of the memory block
 *
 * @return  Pointer to onboard memory block
 * @retval  0  Failure
 */
u8 * emu68_memptr(emu68_t * const emu68, addr68_t addr, uint68_t size);

EMU68_API
/**
 * Check and get a valid 68k onboard memory access control block.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of memory block to access
 * @param  size   size in byte of the memory block
 *
 * @return  pointer to onboard memory block
 * @retval  0  Failure
 */
u8 * emu68_chkptr(emu68_t * const emu68, addr68_t addr, uint68_t size);


EMU68_API
/**
 * Read a byte from 68K onboard memory.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of byte to access
 *
 * @see emu68_poke()
 */
int emu68_peek(emu68_t * const emu68, addr68_t addr);

EMU68_API
/**
 * Read a byte from 68K access control memory.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of byte to access
 *
 * @see emu68_chkpoke()
 */
int emu68_chkpeek(emu68_t * const emu68, addr68_t addr);


EMU68_API
/**
 * Write a byte in 68K onboard memory.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of byte to access
 * @param  byte   byte value
 *
 * @see emu68_peek()
 */
int emu68_poke(emu68_t * const emu68, addr68_t addr, int68_t byte);

EMU68_API
/**
 * Write a byte in 68K access control memory.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of byte to access
 * @param  byte   byte value
 *
 * @see emu68_peek()
 */
int emu68_chkpoke(emu68_t * const emu68, addr68_t addr, int68_t byte);

EMU68_API
/**
 * Write a memory block in 68K on-board memory.
 *
 *   The function copy a memory block in 68K on-board memory after verifying
 *   that the operation access valid 68K memory.
 *
 * @param  emu68  emulator instance
 * @param  dst    address of memory block to access in the 68k memory
 * @param  src    pointer to the source buffer
 * @param  size   size in byte of the memory block
 *
 * @see emu68_memget()
 * @see emu68_memvalid()
 */
int emu68_memput(emu68_t * const emu68,
                 addr68_t dst, const u8 * src, uint68_t size);

EMU68_API
/**
 * Read a 68K on-board memory block.
 *
 *   The function copy a 68K on-board memory to a memory location after
 *   verifying that the operation access valid 68K memory.
 *
 * @param  emu68  emulator instance
 * @param  dst    pointer to the destination buffer
 * @param  src    address of memory block to access in the 68k memory
 * @param  size   size in byte of the memory block
 *
 * @see emu68_memput()
 * @see emu68_memvalid()
 */
int emu68_memget(emu68_t * const emu68,
                 u8 * dst, addr68_t src, uint68_t size);

EMU68_API
/**
 * Fill a 68k on board memory block with a value.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of 68K memory block to access
 * @param  byte   byte value to write
 * @param  size   size in byte of the memory block
 * @return error-code
 * @retval  0 on success
 * @retval -1 on error
 */
int emu68_memset(emu68_t * const emu68,
                 addr68_t addr, u8 byte, uint68_t size);

EMU68_API
/**
 * Fill a 68k access control memory block with a value.
 *
 * @param  emu68  emulator instance
 * @param  addr   address of 68K memory block to access
 * @param  byte   byte value to write
 * @param  size   size in byte of the memory block
 * @return error-code
 * @retval  0 on success
 * @retval -1 on error
 */
int emu68_chkset(emu68_t * const emu68,
                 addr68_t addr, u8 byte, uint68_t size);

EMU68_API
/**
 * Push 32-bit long word.
 * @param  emu68  emulator instance
 * @param  val    value to push.
 */
void emu68_pushl(emu68_t * const emu68, int68_t val);

EMU68_API
/**
 * Push 16-bit word.
 * @param  emu68  emulator instance
 * @param  val    value to push.
 */
void emu68_pushw(emu68_t * const emu68, int68_t val);

EMU68_API
/**
 * Pop 32-bit long word.
 * @param  emu68  emulator instance
 * @return poped 32-bit value
 */
int68_t emu68_popl(emu68_t * const emu68);

EMU68_API
/**
 * Pop 16-bit word.
 * @param  emu68  emulator instance
 * @return poped 16-bit value
 */
int68_t emu68_popw(emu68_t * const emu68);

EMU68_API
/**
 * Compute CRC32 of emu68 object (registers + memory).
 * @param  emu68   emulator instance
 * @return crc32
 */
uint68_t emu68_crc32(emu68_t * const emu68);

/**
 * @}
 */


/**
 * @name  Execution functions
 * @{
 *
 * @todo Describe execution loop here ...
 *
 */

/**
 * Execution status code.
 *
 *   The emu68_status_e:: values
 */
enum emu68_status_e {
  EMU68_ERR  = -1,          /**< Execution failed.                          */
  EMU68_NRM  = 0x00,        /**< Execution running normally.                */
  EMU68_STP  = 0x01,        /**< Execution stopped by the stop instruction. */
  EMU68_HLT  = 0x12,        /**< Execution halted (double fault or user).   */
  EMU68_BRK  = 0x13,        /**< Execution break by user.                   */
  EMU68_XCT  = 0x24         /**< Execution in exception.                    */
};

EMU68_API
/**
 * Get status name.
 *
 * @param  status  one of the emu68_status_e value.
 * @return status name
 */
const char * emu68_status_name(enum emu68_status_e status);

EMU68_API
/**
 * Execute one instruction. If the inst parameter is EMU68_STEP
 * emu68_t::framechk is cleared and emu68_t::status is set to
 * EMU68_NRM for a clean new run.
 *
 * @param  emu68     emulator instance
 * @param  inst      EMU68_STEP or EMU68_CONT
 * @return @ref emu68_status_e "execution status"
 */
int emu68_step(emu68_t * const emu68, uint68_t inst);

/**
 * Various constant.
 */
enum emu68_const_e {
  EMU68_STEP = 0, /**< Start a fresh run (emu68_step())               */
  EMU68_INF  = 0, /**< No instruction limit emu68_finish()            */
  EMU68_CONT = -1 /**< Continue a run (emu68_step() or emu68_finish() */
};

EMU68_API
/**
 * Execute until RTS (Return To Subroutine).
 *
 *   The execution might be breaked by user or stop instruction or
 *   some kind of error. It might be continued by using EMU68_CONT as
 *   instruction counter.
 *
 * @param  emu68           emulator instance
 * @param  instructions    max instruction to execute 0: no limit,
 *                         EMU68_CONT: continue a broken run

 * @return @ref emu68_status_e "execution status"
 */
int emu68_finish(emu68_t * const emu68, uint68_t instructions);

EMU68_API
/**
 * Execute interruptions with given cycle interval.
 *
 * @param  emu68   emulator instance
 * @param  cycles  interval within to excute interruptions
 *
 * @return @ref emu68_status_e "execution status"
 */
int emu68_interrupt(emu68_t * const emu68, cycle68_t cycles);

/**
 * @}
 */

/**
 * @name  Breakpoint functions.
 * @{
 */

EMU68_API
/**
 * Kill all existing breakpoints.
 */
void emu68_bp_delall(emu68_t * const emu68);

EMU68_API
/**
 * Delete a existing breakpoint.
 *
 * @param  emu68  emulator instance
 * @param  id      breakpoint identifier
 */
void emu68_bp_del(emu68_t * const emu68, int id);

EMU68_API
/**
 * Set/Create a breakpoint.
 *
 * @param  emu68  emulator instance
 * @param  id      breakpoint identifier (-1:find free breakpooint)
 * @param  addr    breakpoint address
 * @param  count   breakpoint countdown
 * @param  reset   breakpoint countdown reset (0:remove after break)
 * @return breakpoint identifier
 * @retval -1 on error
 */
int emu68_bp_set(emu68_t * const emu68, int id,
                 addr68_t addr, uint68_t count, uint68_t reset);

EMU68_API
/**
 * Find breakpoint.
 *
 * @param  emu68  emulator instance
 * @param  addr   breakpoint address
 * @return breakpoint identifier
 * @retval -1 on error
 */
int emu68_bp_find(emu68_t * const emu68, addr68_t addr);

/**
 * @}
 */

/**
 * @name  Version checking functions
 * @{
 */

EMU68_API
/**
 * Get debug mode.
 *
 * @param  emu68  emulator instance
 *
 * @retval  0  normal mode
 * @retval  1  debug mode
 * @retval -1  error
 */
int emu68_debugmode(emu68_t * const emu68);

/**
 * @}
 */

/**
 * @}
 */

#endif
