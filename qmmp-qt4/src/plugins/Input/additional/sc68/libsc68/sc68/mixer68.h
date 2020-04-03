/**
 * @ingroup   lib_sc68
 * @file      sc68/mixer68.h
 * @brief     audio mixer header.
 * @author    Benjamin Gerard
 * @date      1999/05/17
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef MIXER68_H
#define MIXER68_H

#include "emu68/type68.h"

#ifndef SC68_EXTERN
# ifdef __cplusplus
#  define SC68_EXTERN extern "C"
# else
#  define SC68_EXTERN
# endif
#endif

/**
 * @defgroup  lib_sc68_mixer  Simple audio mixer
 * @ingroup   lib_sc68
 *
 *   sc68 audio mixer provides functions for computing audio data PCM.
 *   Almost all functions work on 16 bit stereo PCM. Some functions
 *   require signed input to work properly. All functions allow to
 *   change PCM sign. Functions that require signed input perform a
 *   double sign change operation which allow to have any kind of
 *   input PCM.
 *
 *   Internaly PCM are read as 32bit integer. This implies that buffer
 *   must be properly aligned (depending on the architecture). Left
 *   channel is stored the less signifiant 16 bit and right the most
 *   signifiant 16 bit. Ensure the audio device use the same byte
 *   order.
 *
 * @{
 */

/**
 * @name  Constants for PCM sign transformation.
 * @{
 */
#define MIXER68_SAME_SIGN                 0x00000000 /**< No channel.    */
#define MIXER68_CHANGE_LEFT_CHANNEL_SIGN  0x00008000 /**< Left channel.  */
#define MIXER68_CHANGE_RIGHT_CHANNEL_SIGN 0x80000000 /**< Right channel. */
#define MIXER68_CHANGE_SIGN               0x80008000 /**< Both channels. */
/**
 * @}
 */

SC68_EXTERN
/**
 * Copy 16-bit-stereo PCM with optionnal sign change.
 *
 * @param  dst  dstination PCM buffer
 * @param  src   Source PCM buffer (can be same as dst).
 * @param  nb    Number of PCM
 * @param  sign  Sign transformation.
 */
void mixer68_stereo_16_LR(u32 * dst, u32 * src, int nb,
                          const u32 sign);

SC68_EXTERN
/**
 * Copy 16-bit-stereo PCM with channel swapping and optionnal sign
 * change.
 *
 * @note  Sign change occurs after channel swapping.
 *
 * @param  dst   Destination PCM buffer
 * @param  src   Source PCM buffer (can be same as dst).
 * @param  nb    Number of PCM
 * @param  sign  Sign transformation.
 */
void mixer68_stereo_16_RL(u32 * dst, u32 * src, int nb,
                          const u32 sign);

SC68_EXTERN
/**
 * Copy 16-bit-stereo PCM into normalized float-stereo (-norm..+norm).
 *
 * @note     Sign change occurs before float transformation.
 * @warning  PCM are assumed to be signed after sign transform.
 *
 * @param  dst  dstination PCM buffer
 * @param  src   Source PCM buffer (can be same as dst).
 * @param  nb    Number of PCM
 * @param  sign  Sign transformation.
 * @param  norm  float absolute range (normalization).
 *
 * @note   Minus norm causes a sign/phase inversion.
 */
void mixer68_stereo_FL_LR(float * dst, u32 * src, int nb,
                          const u32 sign, const float norm);

SC68_EXTERN
/**
 * Copy left channel of 16-bit stereo PCM into L/R channels with
 * optionnal sign change.
 *
 * @param  dst   Destination PCM buffer
 * @param  src   Source PCM buffer (can be same as dst).
 * @param  nb    Number of PCM
 * @param  sign  Sign transformation.
 */
void mixer68_dup_L_to_R(u32 * dst, u32 * src, int nb, const u32 sign);

SC68_EXTERN
/**
 * Copy right channel of 16-bit stereo PCM into L/R channels
 * with optionnal sign change.
 *
 * @param  dst   Destination PCM buffer
 * @param  src   Source PCM buffer (can be same as dst).
 * @param  nb    Number of PCM
 * @param  sign  Sign transformation.
 */
void mixer68_dup_R_to_L(u32 * dst, u32 * src, int nb, const u32 sign);

SC68_EXTERN
/**
 * Copy 16-bit-stereo PCM with L/R blending and optionnal sign change.
 *
 *  This function performs following transformations in this order :
 *  - Read 32 bit value from src.
 *  - Apply sign_r transformation; it is a bitwise EOR.
 *  - Apply blending (here PCM are asuumed to be signed).
 *  - Apply sign_w transformation; it is a bitwise EOR too.
 *  - Store 32 bit value.
 *
 * @warning  As the blending occurs on signed PCM the sign_r and
 *           the sign_w must be setted properly.
 *
 * @param  dst     Destination PCM buffer
 * @param  src     Source PCM buffer (can be same as dst).
 * @param  nb      Number of PCM
 * @param  factor  Blending factor from [0..65536].
 *                 - 0, blend nothing.
 *                 - 65536, Swap L/R.
 *                 - other value, linear blending.
 * @param  sign_r  sign transform for input PCM.
 * @param  sign_w  sign transform for output PCM.
 */
void mixer68_blend_LR(u32 * dst, u32 * src, int nb,
                      int factor,
                      const u32 sign_r, const u32 sign_w);

SC68_EXTERN
/**
 * Copy 16-bit-stereo PCM with L/R amplitude factor and optionnal sign
 * change.
 *
 *  This function performs following transformations in this order :
 *  - Read 32 bit value from src.
 *  - Apply sign_r transformation; it is a bitwise EOR.
 *  - Apply signed modulation to both channels.
 *  - Apply sign_w transformation; it is a bitwise EOR too.
 *  - Store 32 bit value.
 *
 * @warning  As the modulation occurs on signed PCM the sign_r and
 *           the sign_w must be setted properly.
 *
 * @note     Amplitude factors (ml and mr) can be minus to invert phase.
 *
 * @param  dst     Destination PCM buffer
 * @param  src     Source PCM buffer (can be same as dst).
 * @param  nb      Number of PCM
 * @param  ml      Left channel factor from [-65536..65536].
 * @param  mr      Like ml but for right channel.
 * @param  sign_r  sign transform for input PCM.
 * @param  sign_w  sign transform for output PCM.
 */
void mixer68_mult_LR(u32 * dst, u32 * src, int nb,
                     const int ml, const int mr,
                     const u32 sign_r, const u32 sign_w);

SC68_EXTERN
/**
 * Fill 16-bit-stereo buffer with sign value (RRRRLLLL).
 *
 * @param  dst     Destination PCM buffer
 * @param  nb      Number of PCM
 * @param  sign    PCM value written (right channel is MSW).
 */
void mixer68_fill(u32 * dst, int nb,
                  const u32 sign);

SC68_EXTERN
/**
 * Copy 16-bit-stereo buffer.
 *
 * @param  dst  Destination PCM buffer
 * @param  src  Source PCM buffer
 * @param  nb   Number of PCM
 */
void mixer68_copy(u32 * dst, u32 * src, int nb);

/**
 * @}
 */

#endif
