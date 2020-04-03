/**
 * @ingroup   lib_io68
 * @file      io68/paulaemul.h
 * @brief     Paula emulator header.
 * @author    Benjamin Gerard
 * @date      1998/07/18
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_PAULAEMUL_H
#define IO68_PAULAEMUL_H

#include "io68_api.h"

#include "emu68/struct68.h"

/**
 *  @defgroup  lib_io68_aga  Amiga Paula emulator.
 *  @ingroup   lib_io68
 *  @brief     Paula emulator (Amiga soundchip).
 *
 *  @todo Write something about paula and about this emulator right here !
 *
 *
 *  @par About Amiga hardware registers
 *
 *  Amiga hardware registers could have a different address for read
 *  and write access. It is the case for DMA control register
 *  (DMACON).
 *
 *  Registers could be (B)yte or (W)ord wide.
 *
 *
 *  @par SET/CLR bit
 *
 *  Some hardware registers work with a SET/CLR bit. When writing
 *  these registers the value is not copied as is. The operation
 *  depends on the value of S the most signifiant bit (#15).
 *
 * - If S is 1 set-bit have to be set (bitwise OR).
 * - If S is 0 set-bit have to be clear (bitwise NAND).
 *
 *
 *  @par Register map
 *
 *  @code
 *  // NAME SZ  ADDR
 *
 *  VHPOSR  (B) DFF006 [xxxxxxxx] Vertical/Horizontal electron bean position.
 *
 *  INTREQR (W) DFF01E [-x---DCBA--------] Interrupt request read.
 *  INTREQW (W) DFF09C [-x---DCBA--------] Interrupt request write.
 *
 *  INTENAR (W) DFF01C [-M---DCBA--------] Interrupt enable read (M=Master).
 *  INTENAW (W) DFF09A [SM---DCBA--------] Interrupt enable write (M=Master).
 *
 *  DMACONW (W) DFF096 [S00000E00000DBCA] DMA control register write (E=Enable).
 *  DMACONR (W) DFF002 [000000E00000DBCA] DMA control register read (E=Enable).
 *
 *  VOICEA      DFF0A0
 *  VOICEB      DFF0B0
 *  VOICEC      DFF0C0
 *  VOICED      DFF0D0
 *
 *  VOICEX
 *  +0 (L) START  [00000000 00000xxx xxxxxxxx xxxxxxx0] start address (even).
 *  +4 (W) LENGTH [xxxxxxxx xxxxxxxx] Length in word (0=010000).
 *  +6 (W) PERIOD [0000xxxx xxxxxxxx] Period (in paula cycle).
 *  +8 (W) VOLUME [0xxxxxxx] Volume [0-64] (presume value > 64 => 64).
 *  @endcode
 *
 *
 *  @{
 */

/**
 * Amiga hardware regiosters.
 */
enum amiga_regs_e {

  /** Amiga Vertical/Horizontal electron bean position */
  PAULA_VHPOSR    = 0x06,

  /**
   * @name Amiga interrupt registers.
   *
   * All hardware registers involved with interruption handling use the
   * same bit organisation :
   *  - bit 7   Audio channel A
   *  - bit 8   Audio channel B
   *  - bit 9   Audio channel C
   *  - bit 10  Audio channel D
   *  - bit 14  Master interrupt
   * @{
   */
  PAULA_INTREQR   = 0x1E,       /**< Interruption request read      */
  PAULA_INTREQRH  = 0x1E,       /**< Interruption request read MSB  */
  PAULA_INTREQRL  = 0x1F,       /**< Interruption request read LSB  */

  PAULA_INTREQ    = 0x9C,       /**< Interruption request write     */
  PAULA_INTREQH   = 0x9C,       /**< Interruption request write MSB */
  PAULA_INTREQL   = 0x9D,       /**< Interruption request write LSB */

  PAULA_INTENAR   = 0x1C,       /**< Interruption enable read       */
  PAULA_INTENARH  = 0x1C,       /**< Interruption enable read MSB   */
  PAULA_INTENARL  = 0x1D,       /**< Interruption enable read LSB   */

  PAULA_INTENA    = 0x9A,       /**< Interruption enable write      */
  PAULA_INTENAH   = 0x9A,       /**< Interruption enable write MSB  */
  PAULA_INTENAL   = 0x9B,       /**< Interruption enable write LSB  */
  /**
   * @}
   */

  /**
   * @name Amiga DMA registers.
   *
   *  Amiga DMA control register bits :
   *  - bit 0  Audio DMA channel A
   *  - bit 1  Audio DMA channel B
   *  - bit 2  Audio DMA channel C
   *  - bit 3  Audio DMA channel D
   *  - bit 9  General DMA
   *
   * @{
   */
  PAULA_DMACONR   = 0x02,               /**< DMA control read      */
  PAULA_DMACONRH  = 0x02,               /**< DMA control read MSB  */
  PAULA_DMACONRL  = 0x03,               /**< DMA control read LSB  */
  PAULA_DMACON    = 0x96,               /**< DMA control write     */
  PAULA_DMACONH   = 0x96,               /**< DMA control write MSB */
  PAULA_DMACONL   = 0x97,               /**< DMA control write LSB */
  /**
   * @}
   */

  /**
   * @name The Audio & Disk Control Registers.
   *
   * Bits:
   * - 07   USE3PN    Use audio channel 3 to modulate nothing.
   * - 06   USE2P3    Use audio channel 2 to modulate period of channel 3.
   * - 05   USE1P2    Use audio channel 1 to modulate period of channel 2.
   * - 04   USE0P1    Use audio channel 0 to modulate period of channel 1.
   * - 03   USE3VN    Use audio channel 3 to modulate nothing.
   * - 02   USE2V3    Use audio channel 2 to modulate volume of channel 3.
   * - 01   USE1V2    Use audio channel 1 to modulate volume of channel 2.
   * - 00   USE0V1    Use audio channel 0 to modulate volume of channel 1.
   *
   * @note If both period/volume are modulated on the same channel, the
   * period and volume will be alternated. First word xxxxxxxx V6-V0,
   * Second word P15-P0 (etc).
   *
   * @{
   */
  PAULA_ADKCON    = 0x9E,          /**< Audio, disk, control write. */
  PAULA_ADKCONR   = 0x10,          /**< Audio, disk, control read.  */
  PAULA_ADKCONRH  = PAULA_ADKCONR+0,
  PAULA_ADKCONRL  = PAULA_ADKCONR+1,
  /**
   * @}
   */

  /**
   * @name Amiga Paula registers.
   * @{
   */
  PAULA_VOICEA   = 0xA0,        /**< Paula channel A register base. */
  PAULA_VOICEB   = 0xB0,        /**< Paula channel B register base. */
  PAULA_VOICEC   = 0xC0,        /**< Paula channel C register base. */
  PAULA_VOICED   = 0xD0,        /**< Paula channel D register base. */
  /**
   * @}
   */
};

#define PAULA_VOICE(I) ((0xA+(I))<<4) /**< Paula channel I register base. */

/**
 * Amiga Paula Clock frequencies.
 */
enum {
  PAULA_PAL_FRQ     = 3546897u, /**< Paula PAL clock frequency.     */
  PAULA_NTSC_FRQ    = 3579545u, /**< Paula NTSC clock frequency.    */
};

/**
 * Type used by internal counters.
 *
 *   This type must be an unsigned of at least 32-bit. Values are
 *   always fixed point. The amount of shifting is calculated so that
 *   the most useful bit is the most signifiant bit. Therefore these
 *   counters have a automatic natural modulo.
 */
typedef uint68_t plct_t;

/** Must be same size than plct_t but signed. */
typedef int68_t signed_plct_t;

/** Paula voice data structure. */
typedef struct {
  plct_t adr;   /**< current sample counter (<<paula_t::ct_fix). */
  plct_t start; /**< loop address.                               */
  plct_t end;   /**< end address (<<paula_t::ct_fix).            */
} paulav_t;

/** Paula emulator data structure. */
typedef struct {
  u8       map[256];   /**< Paula regiters mapping.              */
  paulav_t voice[4];   /**< Paula voices (4 channels).           */
  int      engine;     /**< @ref paula_engine_e "Engine id".     */
  int      ct_fix;     /**< Fixed precision used by counters in
                          order to have a natural memory access
                          modulo.                                */
  int      clock;      /**< Clock type. @see paula_clock_e.      */
  plct_t   clkperspl;  /**< (clock<<paula_t::ct_fix)/hz.         */
  uint_t   hz;         /**< Sampling rate in hz,                 */
  int    * chansptr;   /**< Pointer to enabled channels mask.    */

  const u8 * mem;      /**< 68K memory buffer.                   */
  int        log2mem;  /**< 68K memory buffer size (2^log2mem).  */

  int      dmacon;     /**< Shadow DMACON. */
  int      intena;     /**< Shadow INTENA. */
  int      intreq;     /**< Shadow INTREQ. */
  int      adkcon;     /**< Shadow ADKCON. */
  int      vhpos;      /**< Shadow VHPOSR. */
} paula_t;

/**
 * @name  Initialization functions.
 * @{
 */

/** available emulation modes. */
enum paula_engine_e {
  PAULA_ENGINE_QUERY   = -1, /**< Query current or default paula engine. */
  PAULA_ENGINE_DEFAULT = 0,  /**< Default engine.                        */
  PAULA_ENGINE_SIMPLE,       /**< Without interpolation.                 */
  PAULA_ENGINE_LINEAR        /**< With linear interpolation.             */
};

/** clock type. */
enum paula_clock_e {
  PAULA_CLOCK_QUERY   = -1, /**< Query current or default clock */
  PAULA_CLOCK_DEFAULT = 0,
  PAULA_CLOCK_PAL,
  PAULA_CLOCK_NTSC
};

/** sampling rate value in hertz. */
enum paula_hz_e {
  PAULA_HZ_QUERY   = -1, /**< Query current or default sampling rate */
  PAULA_HZ_DEFAULT = 0   /**< Default sampling rate                  */
};

/** Paula instance creaation parameters. */
typedef struct {
  int engine; /**< Default @ref paula_engine_e "emulation mode".  */
  int clock;  /**< Default @ref paula_clock_e "clock type".       */
  int hz;     /**< Default @ref paula_hz_e "sampling rate" in Hz. */
} paula_parms_t;

/** setup data. */
typedef struct {
  paula_parms_t parms;     /**< Paula params.                       */
  const u8 *    mem;       /**< 68K memory buffer.                  */
  int           log2mem;   /**< 68K memory buffer size (2^log2mem). */
} paula_setup_t;

IO68_EXTERN
/**
 * Init paula library.
 *
 *   The paula_init() must be call prior to all other paula functions.
 *
 * @return  error-code (always success)
 * @return  0  Success
 * @return  -1 Failure
 *
 * @see paula_shutdown()
 */
int paula_init(int * argc, char ** argv);

IO68_EXTERN
/**
 * Shutdown paula library.
 *
 * @see paula_init()
 */
void paula_shutdown(void);

IO68_EXTERN
/**
 * Paula hardware reset.
 *
 *   The paula_reset() reset function perform a Paula reset.
 *   It performs following operations :
 *   - all registers zeroed
 *   - all internal voices set to dummy 2 samples len address.
 *   - general DMA enabled
 *   - all audio DMA disabled
 *   - interrupt master enabled
 *   - all audio interrupt disbled
 *
 * @return error-code (always success)
 * @return  0  Success
 */
int paula_reset(paula_t * const paula);

IO68_EXTERN
/**
 * Paula emulator instance setup.
 *
 * @return error-code (always success)
 * @return  0  Success
 *
 * @see paula_reset()
 */
int paula_setup(paula_t * const paula, paula_setup_t * const setup);

IO68_EXTERN
/**
 * Paula emulator instance cleanup.
 */
void paula_cleanup(paula_t * const paula);

IO68_EXTERN
/**
 * Set/Get paula emulator type.
 *
 *   The paula_emulation() function set and get the paula emulator
 *   type.
 *
 *   If engine is paula_engine_e::PAULA_ENGINE_QUERY the function
 *   returns the current value without modifying anything. Else the
 *   function set the current paula engine according to the value of
 *   engine. If paula parameter is NULL pointer the function works
 *   with the default parameters rather than a actual paula instance
 *   meaning it will read or write the default setting (addressed by
 *   paula_engine_e::PAULA_ENGINE_DEFAULT).
 *
 * @param   paula   Paula emulator instance.
 * @param   engine  @ref paula_engine_e "emulator type".
 * @return  an @ref paula_engine_e "emulator type".
 */
int paula_engine(paula_t * const paula, int engine);

IO68_EXTERN
/**
 * Set/Get paula clock type.
 *
 *   The paula_clock() function is similar to the paula_engine()
 *   function except it concerns clock rate.
 *
 * @param   paula  Paula emulator instance.
 * @param   clock  @ref paula_clock_e "clock type".
 * @return  a @ref paula_clock_e "clock type".
 */
int paula_clock(paula_t * const paula, int clock);

IO68_EXTERN
/**
 * Set/Get paula emulator sampling rate.
 *
 *   The paula_sampling_rate() function is similar to the
 *   paula_engine() function except it concerns sampling rate.
 *
 * @param   paula  Paula emulator instance.
 * @param   hz     @ref paula_hz_e "sampling rate" in Hz.
 * @return  actual @ref paula_hz_e "sampling rate" in Hz.
 */
int paula_sampling_rate(paula_t * const paula, int hz);

/**
 * @}
 */


/**
 * @name  Emulation functions
 * @{
 */

IO68_EXTERN
/**
 * Execute Paula emulation.
 *
 *   The paula_mix() function processes sample mixing with current
 *   internal parameters for n samples. Mixed samples are stored in a
 *   large enough (at least n) 32 bit pcm buffer pointed by b. mem68
 *   is a pointer to the 68K memory buffer. The Paula emulator assume
 *   that this buffer is at least the size of the Amiga "chip"
 *   RAM. This implies at leat 512Kb and PCM data must be in the first
 *   512Kb.
 *
 * @param  paula   Paula emulator instance
 * @param  splbuf  Destination 32-bit sample buffer
 * @param  n       Number of sample to mix in splbuf buffer
 *
 */
void paula_mix(paula_t * const paula, s32 * splbuf, int n);

/**
 * @}
 */

/**
 * @}
 */

#endif
