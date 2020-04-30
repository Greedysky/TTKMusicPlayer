/**
 * @ingroup   lib_io68
 * @file      io68/mwemul.h
 * @author    Benjamin Gerard
 * @date      1999/03/20
 * @brief     STE sound emulator header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_MWEMUL_H
#define IO68_MWEMUL_H

#include "io68_api.h"
#include "emu68/type68.h"

/**
 * @defgroup  lib_io68_mw  STE sound emulator
 * @ingroup   lib_io68
 * @brief     STE (MicroWire/LMC/DMA) emulator.
 *
 * Hardware I/O registers
 *
 *  - $FF8900 (W) : DMA sound control register
 *  - $FF8903 (B) : Frame Start Hi
 *  - $FF8905 (B) : Frame Start Mi
 *  - $FF8907 (B) : Frame Start Lo
 *  - $FF8909 (B) : Frame Count Hi
 *  - $FF890B (B) : Frame Count Mi
 *  - $FF890D (B) : Frame Count Lo
 *  - $FF890F (B) : Frame End Hi
 *  - $FF8911 (B) : Frame End Mi
 *  - $FF8913 (B) : Frame End Lo
 *  - $FF8920 (W) : Sound Mode Control (frequency, mono/stereo)
 *  - $FF8922 (B) : Microwire Data Register
 *  - $FF8924 (B) : Microwire Mask Register
 *
 *
 * Microwire and LMC 1992 commands
 *
 *  Commands format is XX YYY ZZZZZZ.
 *
 *  - XX is the chipset address (only 10 is the only one valid).
 *  - YYY is the command number/identifier.
 *  - ZZZZZZ is the command data/parameter.
 *
 *  Commands are
 *
 *  - 000 ....DD Mixer
 *          - 00 : DMA sound only
 *          - 01 : DMA sound + input 1 YM (FR) + DMA (-12dB)
 *          - 10 : DMA sound + input 2 DMA sound only (LPF)
 *          - 11 : DMA sound + input 3 (N/C) -> DMA sound only
 *
 *  - 001 XXD DDD Bass (2dB step)
 *        - 0 000 : -12 dB
 *        - 0 110 :   0 dB
 *        - 1 100 : +12 dB
 *
 *  - 010 XXD DDD Treble (2dB step)
 *        - 0 000 : -12 dB
 *        - 0 110 :   0 dB
 *        - 1 100 : +12 dB
 *
 *  - 011 DDD DDD Master volume (2dB step)
 *      - 000 000 : -80 dB
 *      - 010 100 : -40 dB
 *      - 101 XXX :   0 dB
 *
 *  - 100 XDD DDD Right channel volume (2dB step)
 *       - 00 000 : -40 dB
 *       - 01 010 : -20 dB
 *       - 10 1XX :   0 dB
 *
 *  - 101 XDD DDD  Left channel volume (2dB step)
 *       - 00 000 : -40 dB
 *       - 01 010 : -20 dB
 *       - 10 1XX :   0 dB
 *
 * @{
 */

/**
 * DMA/Micro-Wire registers.
 */
enum mw_reg_e {
  MW_ACTI = 0x01,        /**< Microwire enabled */
  MW_BASH = 0x03,        /**< Microwire sample start address, bit 16-23 */
  MW_BASM = (MW_BASH+2), /**< Microwire sample start address, bit 8-15  */
  MW_BASL = (MW_BASH+4), /**< Microwire sample start address, bit 0-7   */
  MW_CTH  = 0x09,        /**< Microwire sample counter, bit 16-23 */
  MW_CTM  = (MW_CTH+2),  /**< Microwire sample counter, bit 8-15  */
  MW_CTL  = (MW_CTH+4),  /**< Microwire sample counter, bit 0-7   */
  MW_ENDH = 0x0f,        /**< Microwire sample end address, bit 16-23 */
  MW_ENDM = (MW_ENDH+2), /**< Microwire sample end address, bit 8-15  */
  MW_ENDL = (MW_ENDH+4), /**< Microwire sample end address, bit 0-7   */
  MW_MODE = 0x21,        /**< Microwire playing mode     */
  MW_DATA = 0x22,        /**< Microwire data register    */
  MW_CTRL = 0x24         /**< Microwire control register */
};

/**
 * Microwire/LMC mixer modes.
 */
enum mw_mixer_e {
  MW_MIXER_DB12     = 0,                /**< -12dB atttenuation. */
  MW_MIXER_BOTH     = 1,                /**< normal blend.       */
  MW_MIXER_STE      = 2,                /**< STE only.           */
  MW_MIXER_RESERVED = 3,                /**< Reserved.           */
};

/**
 * Microwire emulator engines.
 */
enum mw_engine_e {
  MW_ENGINE_QUERY   = -1, /**< Query default or current engine.         */
  MW_ENGINE_DEFAULT = 0,  /**< Use default engine.                      */
  MW_ENGINE_SIMPLE,       /**< Simple engine without interpolation.     */
  MW_ENGINE_LINEAR        /**< Simple engine with linear interpolation. */
};

/**
 * Microwire sampling rate is given in hertz (hz).
 */
enum mw_hz_e {
  MW_HZ_QUERY   = -1,                   /**< Query current value.   */
  MW_HZ_DEFAULT = 0                     /**< Default sampling rate. */
};

/**
 * LMC register values are mostly unsigned bytes.
 */
enum mw_lmc_e {
  MW_LMC_QUERY  = -1,                   /**< Query current value.   */
};

/**
 * Microwire configuration parameters.
 */
typedef struct {
  int engine;       /**< @ref mw_engine_e "Microwire emulator engine". */
  int hz;           /**< @ref mw_hz_e     "Microwire sampling rate".   */
} mw_parms_t;

/**
 *Microwire setup structure.
 */
typedef struct {
  mw_parms_t parms; /**< configuration parameters.           */
  u8 * mem;         /**< 68K memory buffer.                  */
  int log2mem;      /**< 68K memory buffer size (2^log2mem). */
} mw_setup_t;

/**
 * Type used by internal counters.
 *
 *   This type must be an unsigned of at least 32-bit. Values are
 *   always fixed point. The amount of shifting is calculated so that
 *   the most useful bit is the most signifiant bit. Therefore these
 *   counters get an automatic natural modulo.
 */
typedef uint68_t mwct_t;

/**
 * Microwire emulator instance.
 */
typedef struct {
  u8 map[0x40];   /**< STE register array.                          */
  mwct_t ct;      /**< DMA current location (ct_fix fixed point).   */
  mwct_t end;     /**< DMA end point location (ct_fix fixed point). */

  /** LMC chip. */
  struct
  {
    u8 master; /**< Master volume.             */
    u8 left;   /**< Left volume.               */
    u8 right;  /**< Right volume.              */
    u8 lr;     /**< Left/Right average volume. */
    u8 high;   /**< High pass filter.          */
    u8 low;    /**< Low pass filter.           */
    u8 mixer;  /**< Mixer mode.                */
    u8 align;  /**< reserved for struct align. */
  } lmc;
  const int * db_conv;       /**< Current decibel conversion table. */

  int engine;         /**< @ref mw_engine_e "Emulator engine".      */
  int hz;             /**< @ref mw_hz_e "Sampling rate" (in hz).    */
  int ct_fix;         /**< fixed point for automatic memory modulo. */
  const u8 * mem;     /**< 68000 memory buffer.                     */
  int log2mem;        /**< Size of 68K memory (2^log2mem).          */

} mw_t;


static inline
mwct_t mw_counter_read(const mw_t * const mw, const int n)
{
  return
    (mwct_t) (
      ( mw->map[n] << 16 ) + ( mw->map[n+2] << 8 ) + mw->map[n+4] )
    << mw->ct_fix;
}

static inline
void mw_counter_write(mw_t * const mw, const int n, const mwct_t v)
{
  uint_t vnorm = v >> mw->ct_fix;
  mw->map[n+0] = vnorm >> 16;
  mw->map[n+2] = vnorm >>  8;
  mw->map[n+4] = vnorm;
}

/**
 * @name  Microwire emulator functions
 * @{
 */

IO68_EXTERN
/**
 * Microwire emulator setup.
 *
 *   The mw_init() function must be call before any other function. It
 *   mainly setups internal tables.
 *
 * @return error-code
 * @retval  0 on success
 * @retval -1 on error (should never happen)
 *
 * @see mw_shutdown()
 */
int mw_init(int * argc,char ** argv);

IO68_EXTERN
/**
 * Microwire emulator shutdown.
 *
 * @see mw_init()
 */
void mw_shutdown(void);

IO68_EXTERN
/**
 * Setup microwire instance.
 *
 * @param   mw     microwire instance
 * @param   setup  setup configuration
 * @return         error status
 * @retval   0     on success
 * @retval  -1     on error
 */
int mw_setup(mw_t * const mw, mw_setup_t * const setup);

IO68_EXTERN
/**
 * Cleanup microwire instance.
 */
void mw_cleanup(mw_t * const mw);

IO68_EXTERN
/**
 * Set/Get microwire sampling rate.
 *
 *   The mw_sampling_rate() function set microwire emulator sampling
 *   rate.
 *
 * @param   mw  microwire instance (0 for set or get library default)
 * @param   hz  @ref mw_hz_e "sampling rate"
 * @return      @ref mw_hz_e "sampling rate"
 */
int mw_sampling_rate(mw_t * const mw, int hz);

IO68_EXTERN
/**
 * Set/Get microwire emulator engine.
 *
 * @param   mw      microwire instance (0 for set or get library default)
 * @param   engine  @ref mw_engine_e "engine descriptor"
 * @return          @ref mw_engine_e "engine descriptor"
 */
int mw_engine(mw_t * const mw, int engine);

IO68_EXTERN
/**
 * Microwire reset.
 *
 *   The mw_reset() reset function restore the chip on to its default
 *   configuation or what it is believed to be.
 *
 * @param   mw   microwire instance
 * @return       error status (always success)
 * @retval   0   on success
 * @retval  -1   on error
 */
int mw_reset(mw_t * const mw);

IO68_EXTERN
/**
 * Execute microwire emulation.
 *
 *   The mw_mix() function processes sample mixing with current
 *   internal parameters for n samples. Mixed samples are stored in a
 *   large enough (at least n) 32 bit buffer pointed by b. This buffer
 *   have to be previously filled with the YM-2149 samples. Typically
 *   it is the YM-2149 emulator output buffer. This allows Microwire
 *   emulator to honnor the LMC mixer mode.iven LMC mode. This
 *   porocess include the mono to stereo expansion. The mem68 starting
 *   pointer locates the 68K memory buffer where samples are stored to
 *   allow DMA fetch emulation.
 *
 * @param  mw     microwire instance
 * @param  out    pointer to YM-2149 source sample directly used for
 *                microwire output mixing.
 * @param  n      number of sample to mix in out buffer
 *
 * @see YM_mix()  @see YM_get_buffer()
 */
void mw_mix(mw_t * const mw, s32 * out, int n);

/**
 * @}
 */


/**
 * @name  Microwire LMC functions
 * @{
 */


IO68_EXTERN
/**
 * Set/Get LMC mixer mode.
 *
 *   The mw_lmc_mixer() function selects or retrieves the function
 *   used by the chip to blend the YM-2149 signal and the digital
 *   signal produce by microwire chip.
 *
 * @param   mw    microwire instance
 * @param   mode  @ref mw_mixer_e "mixer mode"
 * @return        @ref mw_mixer_e "mixer mode"
 */
int mw_lmc_mixer(mw_t * const mw, int mode);

IO68_EXTERN
/**
 * Set/Get LMC master volume.
 *
 * @param   mw  microwire instance
 * @param   n   @ref mw_lmc_e "volume" in range [0..40] for [-80..0] dB
 * @return      master @ref mw_lmc_e "volume"
 *
 * @see mw_lmc_left()
 * @see mw_lmc_right()
 */
int mw_lmc_master(mw_t * const mw, int n);

IO68_EXTERN
/**
 * Set/Get LMC left channel volume.
 *
 * @param   mw  microwire instance
 * @param   n   @ref mw_lmc_e "volume" in range [0..20] for [-40..0] dB
 * @return      left @ref mw_lmc_e "volume"
 *
 * @see mw_lmc_master()
 * @see mw_lmc_right()
 */
int mw_lmc_left(mw_t * const mw, int n);

IO68_EXTERN
/**
 * Set/Get LMC right channel volume.
 *
 * @param   mw  microwire instance
 * @param   n   @ref mw_lmc_e "volume" in range [0..20] for [-40..0] dB
 * @return      right @ref mw_lmc_e "volume"
 *
 * @see mw_lmc_master()
 * @see mw_lmc_left()
 */
int mw_lmc_right(mw_t * const mw, int n);

IO68_EXTERN
/**
 * Set/Get high pass filter.
 *
 * @param   mw  microwire instance

 * @param    n  @ref mw_lmc_e "high pass filter" in range [0..12] for
 *              [-12..0] dB
 * @return      @ref mw_lmc_e "high pass filter"
 *
 * @see      mw_lmc_low()
 * @warning  Filters are not supported by this microwire emulator.
 */
int mw_lmc_high(mw_t * const mw, int n);

IO68_EXTERN
/**
 * Set/Get low pass filter.
 *
 * @param   mw  microwire instance
 * @param    n  @ref mw_lmc_e "low pass filter" in range [0..12]
 *              for [-12..0] dB
 * @return      @ref mw_lmc_e "low pass filter"
 *
 * @see      mw_lmc_high()
 * @warning  Filters are not supported by this microwire emulator.
 */
int mw_lmc_low(mw_t * const mw,int n);

IO68_EXTERN
/**
 * Parse and execute a microwire command.
 *
 * @param   mw  microwire instance
 * @retval  0 on success (a command was found)
 * @retval -1 on error
 */
int mw_command(mw_t * const mw);

/**
 * @}
 */

/**
 * @}
 */

#endif
