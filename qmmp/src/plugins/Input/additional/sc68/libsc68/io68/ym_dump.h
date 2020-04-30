/**
 * @ingroup   lib_io68
 * @file      io68/ym_dump.h
 * @brief     sc68 register dump YM-2149 emulator header.
 * @author    Benjamin Gerard
 * @date      2009/02/10
 */

/**
 * @defgroup  lib_io68_ym_dump  Dump YM engine
 * @ingroup   lib_io68_ym
 * @brief     Dump YM access to stdout.
 *
 *   The dump engine is a phony engine that will online dump to stdout
 *   all write access to the YM registers.
 *
 * @{
 */

IO68_EXTERN
/**
 *  Setup function for sc68 dump ym engine.
 *
 *    The ym_dump_setup() function sets ym dump engine for this ym
 *    emulator instance.
 *
 *  @param   ym  ym emulator instance to setup
 *  @retval   0  on success
 *  @retval  -1  on failure
 */
int ym_dump_setup(ym_t * const ym);

IO68_EXTERN
/**
 *  Get/Set sc68 dump ym engine active state.
 *
 *  @param   ym   ym emulator instance to setup
 *  @param   val  0:disable 1:enable -1:current
 *  @return  previous status
 */
int ym_dump_active(ym_t * const ym, int val);

IO68_EXTERN
/**
 *  Creates ym dump engine options.
 */
void ym_dump_add_options(void);

/**
 * YM-2149 internal data structure for original emulator.
 */
struct ym2149_dump_s
{
  uint64_t base_cycle;                  /**< cycle counter       */
  uint68_t pass;                        /**< current pass number */
  int      active;                      /**< set if active       */
};

/**
 * YM-2149 emulator instance type.
 */
typedef struct ym2149_dump_s ym_dump_t;

/**
 * @}
 */
