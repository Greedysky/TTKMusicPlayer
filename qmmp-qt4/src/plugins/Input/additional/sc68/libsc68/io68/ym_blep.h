/**
 * @ingroup   lib_io68
 * @file      io68/ym_blep.h
 * @brief     YM-2149 BLEP synthesis engine
 * @author    Antti Lankila
 * @date      2009-01-30
 */

/* Copyright (c) 200?-2015 Antti Lankila */

/**
 * @defgroup  lib_io68_ym_blep  BLEP YM engine
 * @ingroup   lib_io68_ym
 * @brief     Band Limited Step YM engine.
 *
 *   The dump engine is a phony engine that will online dump to stdout
 *   all write access to the YM registers.
 *
 * @{
 */

IO68_EXTERN
/**
 * Setup function for ym blep synthesis engine.
 *
 *    The ym_blep_setup() function sets ym blep synthesis engine for
 *    this ym emulator instance.
 *
 *  @param    ym  ym emulator instance to setup
 *  @retval   0  on success
 *  @retval  -1  on failure
 */
int ym_blep_setup(ym_t * const ym);

IO68_EXTERN
/**
 *  Creates ym blep engine options.
 */
void ym_blep_add_options(void);

enum {
  MAX_BLEPS = 256                       /**< @nodoc */
};

/** @nodoc */
typedef struct {
  s32 count;                            /**< @nodoc */
  u32 event;                            /**< @nodoc */
  u16 flip_flop;                        /**< @nodoc */
  u16 tonemix;                          /**< @nodoc */
  u16 noisemix;                         /**< @nodoc */
  u16 envmask;                          /**< @nodoc */
  u16 volmask;                          /**< @nodoc */
} ym_blep_tone_t;

/** @nodoc */
typedef struct {
  u16 stamp;                            /**< @nodoc */
  s16 level;                            /**< @nodoc */
} ym_blep_blep_state_t;

/** @nodoc */
typedef struct {
  /* sampling parameters */
  u32 cycles_per_sample;             /**< 8 bit fixed point ym clocks */
  u32 cycles_to_next_sample;         /**< @nodoc */

  /* subsystem states */
  ym_blep_tone_t tonegen[3];            /**< @nodoc */
  u32 noise_event;                      /**< @nodoc */
  s32 noise_count;                      /**< @nodoc */
  u32 noise_state;                      /**< @nodoc */
  u16 noise_output;                     /**< @nodoc */

  u32 env_event;                        /**< @nodoc */
  s32 env_count;                        /**< @nodoc */
  u8 env_state;                         /**< @nodoc */
  u16 env_output;                       /**< @nodoc */

  /* blep stuff */
  s16 global_output_level;              /**< @nodoc */
  u32 blep_idx;                         /**< @nodoc */
  u16 time;                             /**< @nodoc */
  s32 hp;                               /**< @nodoc */

  /** @nodoc */
  ym_blep_blep_state_t blepstate[MAX_BLEPS];
} ym_blep_t;

/**
 * @}
 */
