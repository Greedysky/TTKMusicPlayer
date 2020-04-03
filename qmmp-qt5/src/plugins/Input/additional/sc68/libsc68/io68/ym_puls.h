/**
 * @ingroup   lib_io68
 * @file      io68/ym_puls.h
 * @brief     sc68 pulse YM-2149 emulator header.
 * @author    Benjamin Gerard
 * @date      2009/02/10
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

/**
 * @defgroup  lib_io68_ym_puls  Pulse (sc68 legacy) YM engine.
 * @ingroup   lib_io68_ym
 * @{
 */

IO68_EXTERN
/**
 *  Setup function for sc68 pulse ym engine.
 *
 *    The ym_puls_setup() function sets ym pulsinal engine for this ym
 *    emulator instance.
 *
 *  @param   ym  ym emulator instance to setup
 *  @retval   0  on success
 *  @retval  -1  on failure
 */
int ym_puls_setup(ym_t * const ym);

IO68_EXTERN
/**
 *  Creates ym pulse engine options.
 *
 */
void ym_puls_add_options(void);

/**
 * Type definition for the pulse engine filter function.
 */
typedef void (*ym_puls_filter_t)(ym_t * const);

/**
 * YM-2149 internal data structure for pulse emulator.
 */
struct ym2149_puls_s
{
  /**
   *  @name  Envelop generator
   *  @{
   */
  int envel_ct;                        /**< Envelop period counter. */
  int envel_idx;                       /**< Envelop index [0..96].  */
  /**
   * @}
   */

  /**
   * @name  Noise generator
   * @{
   */
  int noise_bit;        /**< Noise generator 17-bit shift register. */
  int noise_ct;         /**< Noise generator period counter.        */
  /**
   * @}
   */

  /**
   * @name  Tone generator
   * @{
   */
  int voice_ctA;                 /**< Canal A sound period counter. */
  int voice_ctB;                 /**< Canal B sound period counter. */
  int voice_ctC;                 /**< Canal C sound period counter. */
  int levels;                    /**< Square level [C<<10|B<<5|A].  */
  /**
   * @}
   */

  /**
   * @name  Filter
   * @{
   */
  int68_t hipass_inp1;                /**< high pass filter input.  */
  int68_t hipass_out1;                /**< high pass filter output. */
  int68_t lopass_out1;                /**< low pass filter output.  */

  /** 2-poles butterworth filter. */
  struct {
    int68_t x[2];                       /**< @nodoc */
    int68_t y[2];                       /**< @nodoc */
    int68_t a[3];                       /**< @nodoc */
    int68_t b[2];                       /**< @nodoc */
  } btw;
  /**
   * @}
   */

  int ifilter;                         /**< filter function to use. */

};

/**
 * YM-2149 emulator instance type.
 */
typedef struct ym2149_puls_s ym_puls_t;

/**
 * @}
 */
