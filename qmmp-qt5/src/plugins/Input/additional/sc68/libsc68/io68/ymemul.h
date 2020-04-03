/**
 * @ingroup   lib_io68
 * @file      io68/ymemul.h
 * @brief     YM-2149 emulator header.
 * @author    Benjamin Gerard
 * @date      1998/06/24
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_YM_EMUL_H
#define IO68_YM_EMUL_H

#include "io68_api.h"
#include "emu68/emu68.h"

/* Need these values in ymemul.c and ym_puls.c */
enum {
  YM_OUT_MSK_A   = 0x001F,
  YM_OUT_MSK_B   = 0x03E0,
  YM_OUT_MSK_C   = 0x7C00,
  YM_OUT_MSK_ALL = 0x7FFF
};

/**
 * @defgroup   lib_io68_ym  YM-2149 emulator
 * @ingroup    lib_io68
 *
 * The YM-2149 (Atari-ST soundchip) emulator.
 *
 * @{
 */


/**
 * @name YM-2149 registers.
 * @{
 */
enum {
  YM_BASEPERL = 0,  /**< YM-2149 LSB period base (chan A).      */
  YM_BASEPERH = 1,  /**< YM-2149 MSB period base (chan A).      */
  YM_BASEVOL  = 8,  /**< YM-2149 volume base register (chan A). */
  YM_NOISE    = 6,  /**< Noise period.                          */
  YM_MIXER    = 7,  /**< Mixer control.                         */
  YM_ENVL     = 11, /**< Volume envelop LSB period.             */
  YM_ENVH     = 12, /**< Volume envelop MSB period.             */
  YM_ENVTYPE  = 13, /**< Volume envelop wave form.              */
  YM_ENVSHAPE = 13, /**< Alias for YM_ENVTYPE.                  */
};
#define YM_PERL(N) (YM_BASEPERL+(N)*2) /**< Canal N LSB period.    */
#define YM_PERH(N) (YM_BASEPERH+(N)*2) /**< Canal N MSB period.    */
#define YM_VOL(N)  (YM_BASEVOL+(N))    /**< Canal N volume.        */
/**
 * @}
 */


/**
 * @name YM-2149 internal register access.
 * @{
 */

/**
 * YM event (write access) struture.
 */
struct ym_event_s
{
  cycle68_t ymcycle;          /**< CPU cycle this access occurs. */
  u8 reg;                     /**< YM register to write into.    */
  u8 val;                     /**< Value to write.               */
};

/**
 * YM write access type.
 */
typedef struct ym_event_s ym_event_t;

/**
 * @}
 */

/**
 * YM-2149 internal register mapping.
 */
struct ym2149_reg_s {
  u8 per_a_lo;      /**< (0) LSB tone period channel A */
  u8 per_a_hi;      /**< (1) MSB tone period channel A */
  u8 per_b_lo;      /**< (2) LSB tone period channel B */
  u8 per_b_hi;      /**< (3) MSB tone period channel B */
  u8 per_c_lo;      /**< (4) LSB tone period channel C */
  u8 per_c_hi;      /**< (5) MSB tone period channel C */
  u8 per_noise;     /**< (6) Noise period              */
  u8 ctl_mixer;     /**< (7) Mixer control             */
  u8 vol_a;         /**< (8) Volume/envelop channel A  */
  u8 vol_b;         /**< (9) Volume/envelop channel B  */
  u8 vol_c;         /**< (A) Volume/envelop channel C  */
  u8 per_env_lo;    /**< (B) LSB envelop period        */
  u8 per_env_hi;    /**< (C) MSB envelop period        */
  u8 env_shape;     /**< (D) Envelop shape             */
  u8 io_a;          /**< (E) IO port A                 */
  u8 io_b;          /**< (F) IO port B                 */
};

/**
 * Access YM-2149 internal register by name or by index.
 */
typedef union ym_reg_u {
  struct ym2149_reg_s name;     /**< ym registers by name.  */
  u8 index[16];                 /**< ym registers by index. */
} ym_reg_t;

/**
 * YM-2149 emulation engine identifiers.
 */
enum ym_engine_e {
  YM_ENGINE_QUERY   = -1, /**< Query current or default engine.             */
  YM_ENGINE_DEFAULT = 0,  /**< Use default mode.                            */
  YM_ENGINE_PULS,         /**< sc68 original (pulse) emulation.             */
  YM_ENGINE_BLEP,         /**< Antti Lankila's Band Limited Step synthesis. */
  YM_ENGINE_DUMP          /**< Dummy register dump.                         */
};

/**
 * YM-2149 volume model identifiers.
 */
enum ym_vol_e {
  YM_VOL_QUERY   = -1, /**< Query current or default volume model. */
  YM_VOL_DEFAULT = 0,  /**< Use default volume model.              */
  YM_VOL_ATARIST,      /**< Atari-ST volume table.                 */
  YM_VOL_LINEAR        /**< Linear mixing volume table.            */
};

/**
 * Sampling rate special values.
 */
enum ym_hz_e {
  YM_HZ_QUERY   = -1,  /**< Query current or default sampling rate. */
  YM_HZ_DEFAULT = 0    /**< Default sampling rate. */
};

/**
 * YM master clock frequency special values.
 */
enum ym_clock_e {
  /** Query current or default master clock frequency. */
  YM_CLOCK_QUERY   = 1,
  /** Default master clock frequency. */
  YM_CLOCK_DEFAULT = 0,
  /** Atari-ST YM-2149 master clock is about 2Mz. */
  YM_CLOCK_ATARIST = EMU68_ATARIST_CLOCK/4u,
};

/**
 * Type definition for struct ym_s.
 */
typedef struct ym_s ym_t;

#include "ym_puls.h" /* data structure for puls ym emulator. */
#include "ym_blep.h" /* data structure for blep ym emulator.  */
#include "ym_dump.h" /* data structure for dump ym emulator.  */

/**
 * Base struct for all YM emulators.
 */
struct ym_s {

  /**
   * @name Interface
   * @{
   */
  void (*cb_cleanup)       (ym_t * const);
  int  (*cb_reset)         (ym_t * const, const cycle68_t);
  int  (*cb_run)           (ym_t * const, s32 *, const cycle68_t);
  int  (*cb_buffersize)    (const ym_t *, const cycle68_t);
  int  (*cb_sampling_rate) (ym_t * const, const int);
  /**
   * @}
   */

  /**
   * @name Internal YM registers
   * @{
   */
  u8 ctrl;                    /**< Control (working) register.           */
  ym_reg_t reg;               /**< YM registers.                         */
  ym_reg_t shadow;            /**< Shadow YM registers (for reading).    */
  /**
   * @}
   */

  s16    * ymout5;            /**< DAC lookup table                      */
  uint_t   voice_mute;        /**< Mask muted voices.                    */
  uint_t   hz;                /**< Sampling rate.                        */
  uint68_t clock;             /**< Master clock frequency in Hz.         */

  /**
   * @name  Events (Write access) storage.
   * @{
   */
  ym_event_t  *event_ptr;       /**< Current number of entry in event. */
  unsigned int event_ovf;       /**< count overflows.                  */
  ym_event_t   event_buf[1600]; /**< Pre-allocated YM memory access.   */
  /**
   * @}
   */

  /**
   * @name  Output
   * @{
   */
  s32 * outbuf;             /**< output buffer given to ym_run()         */
  s32 * outptr;             /**< generated sample pointer (into outbuf)  */
  /**
   * @}
   */

  int engine;               /**< @ref ym_engine_e "engine type". */
  int volmodel;             /**< @ref ym_vol_e "volume model".   */

  /**
   * union of all YM emulators private data.
   */
  union emu_u {
    ym_puls_t puls; /**< PULS YM emulator data. */
    ym_blep_t blep; /**< BLEP YM emulator data. */
    ym_dump_t dump; /**< DUMP YM emulator data. */
  } emu;            /**< Engine private data. */
};

/**
 * YM-2149 setup structure.
 *
 *   This structure is passed to the ym_init() and ym_setup()
 *   functions. The caller have to fill it with desired values before
 *   calling these functions. Each field can be set to a nul value in
 *   order to use the default value. The function will set the actual
 *   values before returning.
 *
 */
typedef struct
{
  int engine;        /**< @ref ym_engine_e "emulator engine".       */
  int volmodel;      /**< @ref ym_vol_e "volume model".             */
  int clock;         /**< @see ym_clock_e "master clock frequency". */
  int hz;            /**< Sampling rate in Hz.                      */
} ym_parms_t;

/**
 * @name  Initialization functions
 * @{
 */

IO68_EXTERN
/**
 * Create an Yamaha-2149 emulator instance.
 *
 * @param  ym
 * @param  parms
 *
 * @return  error-code
 * @retval   0  Success
 * @retval  -1  Failure
 *
 * @see  ym_destroy()
 */
int ym_setup(ym_t * const ym, ym_parms_t * const parms);

IO68_EXTERN
/**
 * Destroy an Yamaha-2149 emulator instance.
 *
 *   @param  ym  ym emulator instance to destroy
 */
void ym_cleanup(ym_t * const ym);

IO68_EXTERN
/**
 * Perform an Yamaha-2149 hardware reset.
 *
 *   The ym_reset() reset function perform a YM-2149 reset. It
 *   performs following operations :
 *   - all register zeroed
 *   - mixer is set to 077 (no sound and no noise)
 *   - envelop shape is set to 0xA (/\)
 *   - control register is set to 0
 *   - internal periods counter are zeroed
 *
 * @param  ym       YM-2149 emulator instance.
 * @param  ymcycle  ym cycle the reset has occured.
 *
 * @return error-code (always success)
 * @retval  0  Success
 * @retval -1  Failure
 */
int ym_reset(ym_t * const ym, const cycle68_t ymcycle);

IO68_EXTERN
/**
 * Yamaha-2149 first one first initialization.
 *
 *   The ym_init() function must be call before any other ym_
 *   function.  If params is non nul then all non nul fields will be
 *   used as the new default value for this field at ym_setup()
 *   function call.
 *
 * @param  argc pointer to argument count (updated)
 * @param  argv array of arguments string (updated)
 *
 * @return error-code (always success)
 * @retval 0  Success
 *
 * @see ym_reset()
 */
int ym_init(int * argc, char ** argv);

IO68_EXTERN
/**
 * Shutdown the ym-2149 library.
 *
 *  The ym_shutdown() function should be call after all created ym
 *  emulator instances have been clean up.
 *
 */
void ym_shutdown(void);

/**
 * @}
 */


/**
 * @name  Emulation functions
 * @{
 */

IO68_EXTERN
/**
 * Execute Yamaha-2149 emulation.
 *
 *   The ym_run() function execute Yamaha-2149 emulation for a given
 *   number of cycle. The output buffer
 *
 *   @warning The requested output buffer size may seem larger than
 *   neccessary but it is not. Internally the emulator may need some
 *   extra place (for oversampling...). Always call ym_buffersize() to
 *   allocate this buffer large enough.
 *
 * @param  ym        YM-2149 emulator instance.
 * @param  output    Output sample buffer.
 * @param  ymcycles  Number of cycle to mix.
 *
 * @return  Number of sample in output mix-buffer
 * @retval  -1  Failure
 *
 * @see ym_buffersize()
 */
int ym_run(ym_t * const ym, s32 * output, const cycle68_t ymcycles);


IO68_EXTERN
/**
 * Get required output buffer size.
 *
 *   The ym_buffersize() function returns the minimum size in
 *   PCM(32bit) of the output buffer for generating ymcycles
 *   cycles. It should be use to ensure the buffer is large enough
 *   before calling ym_run() function.
 *
 * @param  ym        YM-2149 emulator instance.
 * @param  ymcycles  Number of ym-cycles to generate
 *
 * @return Minimum size in PCM (32bit) of output buffer to generate
 *         ymcycles samples.
 *
 */
uint68_t ym_buffersize(const ym_t * const ym, const cycle68_t ymcycles);

IO68_EXTERN
/**
 * Change YM cycle counter base.
 *
 *   The ym_adjest() function allow to corrige the internal cycle
 *   counter to prevent overflow. Because the number of cycle could
 *   grow very quickly, it is neccessary to get it down from time to
 *   time.
 *
 * @param  ym        YM-2149 emulator instance.
 * @param  ymcycles  Number of cpu cycle to substract to current the
 *                    cycle counter.
 *
 */
void ym_adjust_cycle(ym_t * const ym, const cycle68_t ymcycles);

/**
 * @}
 */


/**
 * @name  YM-2149 register access functions
 * @{
 */

IO68_EXTERN
/**
 * Write in YM-2149 register.
 *
 *   The ym_writereg() function performs a write access to an YM-2149
 *   internal register.  The YM-2149 emulator does not really write
 *   register but store changes in separate list depending of the
 *   register nature and dependencies. There are 3 lists of update
 *   (sound, noise and envelop). This method allow to perform a very
 *   efficient cycle precise emulation. For this reason the YM-2149
 *   registers should be read by ym_readreg() function.
 *
 * @param  ym       YM-2149 emulator instance.
 * @param  val      Value to write.
 * @param  ymcycle  YM cycle this access has occurred.
 *
 * @see ym_readreg();
 */
void ym_writereg(ym_t * const ym, const int val, const cycle68_t ymcycle);


static inline
/**
 * Read a YM-2119 register.
 *
 *   The ym_readreg() function must be call to read an YM-2149
 *   register. For the reasons explained in YM_writereg(), register
 *   must not be read directly.
 *
 * @param  ym       YM-2149 emulator instance.
 * @param  ymcycle  CPU cycle number this access has occurred.
 *
 * @return  Register value at given cycle
 *
 * @see ym_writereg();
 */
int ym_readreg(ym_t * const ym, const cycle68_t ymcycle)
{
  const int reg = ym->ctrl;
  return (reg>=0 && reg<16)
    ? ym->shadow.index[reg]
    : 0;
}

/**
 * @}
 */


/**
 * @name  YM-2149 emulator configuration functions.
 * @{
 */

IO68_EXTERN
/**
 * Get/Set active channels status.
 *
 *   The ym_active_channels() function allows to activate/desactivate
 *   separarely channels (voices) of the YM-2149.
 *
 *   For both on and off parameters:
 *   -bit#0: canal A (0:no change 1:change to on or off)
 *   -bit#1: canal B (0:no change 1:change to on or off)
 *   -bit#2: canal C (0:no change 1:change to on or off)
 *
 *   How it works:
 *
 *   The ym_active_channels() function performs off parameter before on
 *   parameter. The formula looks like ``result=(current&^off)|on''.
 *
 *   How to use:
 *
 *   -Get current value            : ym_active_channels(ym,0,0);
 *   -Mute all channels            : ym_active_channels(ym,7,0);
 *   -Active all channels          : ym_active_channels(ym,0,7);
 *   -Active some channel channels : ym_active_channels(ym,0,x);
 *
 * @param  ym   YM-2149 emulator instance
 * @param  off  Mute the channels for bit set to 1
 * @param  on   Active the channels for bit set to 1
 *
 * @return  new active-voices status
 * @retval  -1 Failure
 * @retval   0 All voice muted
 * @retval   1 Only channel A
 * @retval   7 all channels active
 *
 */
int ym_active_channels(ym_t * const ym, const int off, const int on);

IO68_EXTERN
/**
 * Set/Get configuration.
 */
int ym_configure(ym_t * const ym, ym_parms_t * const parms);

IO68_EXTERN
/**
 * Get/Set sampling rate.
 *
 * @param  ym   YM-2149 emulator instance
 * @param  hz   frequency in hz or @ref ym_hz_e "special values".
 *
 * @return Actual sampling rate (may differs from requested).
 * @retval -1 on error
 */
int ym_sampling_rate(ym_t * const ym, const int hz);

IO68_EXTERN
/**
 * Set or get Yamaha-2149 emulator engine.
 *
 * @param  ym      YM-2149 emulator instance
 * @param  engine  @ref ym_engine_e "engine identifier" value

 * @return @ref ym_engine_e "engine identifier".
 * @retval -1 on error
 */
int ym_engine(ym_t * const ym, int engine);

IO68_EXTERN
int ym_volume_model(ym_t * const ym, int model);

IO68_EXTERN
int ym_clock(ym_t * const ym, int clock);

/**
 * @}
 */

/**
 *  @}
 */

#endif
