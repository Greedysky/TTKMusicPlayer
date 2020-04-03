/**
 * @ingroup   lib_emu68
 * @file      emu68/srdef68.h
 * @brief     Status Register (SR) definition header.
 * @date      1999/13/03
 * @author    Benjamin Gerard
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_SRDEF68_H
#define EMU68_SRDEF68_H

/**
 * @defgroup  lib_emu68_srdef  Status Register (SR) definitions
 * @ingroup   lib_emu68
 *
 *   68K status register definitions.
 *   The SR is composed of condition code register (CCR) located in
 *   the LSB and privileged processor status in MSB. EMU68 does not
 *   currently handle supervisor and trace mode. Internal processor level is
 *   partially managed. Each SR bit is defined by its bit number (SR_x_BIT)
 *   and the corresponding value (SR_x) where x is one of C,V,Z,N,X,S or T.
 *   SR_IPL_BIT is used to locate the less significant bit position of the 3
 *   IPL bits. Macros are available to help with SR bit manipulations.
 *
 *  SR := T.S. .IPL ...X NZVC
 *
 * @{
 */

/**
 * @name  SR bit definitions.
 * @{
 */

enum {
  SR_C_BIT  = 0   /**< Carry bit number.          */,
  SR_V_BIT  = 1   /**< Overflow bit number.       */,
  SR_Z_BIT  = 2   /**< Zero bit number.           */,
  SR_N_BIT  = 3   /**< Negative bit number.       */,
  SR_X_BIT  = 4   /**< eXtended carry bit number. */,
  SR_5_BIT  = 5   /**< Bit#5.                     */,
  SR_6_BIT  = 6   /**< Bit#6.                     */,
  SR_7_BIT  = 7   /**< Bit#7.                     */,

  SR_I_BIT  = 8   /**< Internal Processor Level bit number. */,
  SR_I0_BIT = 8   /**< Internal Processor Level bit#0.      */,
  SR_I1_BIT = 9   /**< Internal Processor Level bit#1.      */,
  SR_I2_BIT = 10  /**< Internal Processor Level bit#2.      */,

  SR_B_BIT  = 11  /**< Bit#11.                         */,
  SR_M_BIT  = 12  /**< Master Interrupt State (68020). */,
  SR_S_BIT  = 13  /**< Superuser bit number.           */,
  SR_T0_BIT = 14  /**< Trace enable bit#0 (68020).     */,
  SR_T1_BIT = 15  /**< Trace enable bit#1 (68020).     */,
  SR_T_BIT  = 15  /**< Trace bit number.               */,
};

enum {
  SR_C  = (1<<SR_C_BIT)  /**< Carry value.          */,
  SR_V  = (1<<SR_V_BIT)  /**< Overflow value.       */,
  SR_Z  = (1<<SR_Z_BIT)  /**< Zero value.           */,
  SR_N  = (1<<SR_N_BIT)  /**< Negative value.       */,
  SR_X  = (1<<SR_X_BIT)  /**< eXtended carry value. */,
  SR_5  = (1<<SR_5_BIT)  /**< bit#5 always 0.       */,
  SR_6  = (1<<SR_6_BIT)  /**< bit#6 always 0.       */,
  SR_7  = (1<<SR_7_BIT)  /**< bit#7 always 0.       */,

  SR_I  = (7<<SR_I_BIT)  /**< IPL mask.    */,
  SR_I0 = (1<<SR_I0_BIT) /**< IPL mask #0. */,
  SR_I1 = (1<<SR_I1_BIT) /**< IPL mask #1. */,
  SR_I2 = (1<<SR_I2_BIT) /**< IPL mask #2. */,

  SR_B  = (1<<SR_B_BIT)  /**< bit#11 always 0.                */,
  SR_M  = (1<<SR_M_BIT)  /**< Master Interrupt State (68020). */,
  SR_S  = (1<<SR_S_BIT)  /**< Superuser value.                */,
  SR_T  = (1<<SR_T_BIT)  /**< Trace value.                    */,
  SR_T0 = (1<<SR_T0_BIT) /**< Trace 0 (68020).                */,
  SR_T1 = (1<<SR_T1_BIT) /**< Trace 1 (68020).                */,
};

/**
 * @}
 */


/**
 * @name  SR manipulations.
 * @{
 */

/**
 * Get CCR value.
 * @param  SR  current SR value
 * @return CCR value
 * @retval SR&255
 */
#define GET_CCR(SR) ( (u8) (SR) )

/**
 * Set CCR in SR value.
 * @param  SR  current SR value
 * @param  CCR ccr value [0..255]
 * @return new SR value
 * @retval ( SR & ~255 ) | CCR
 */
#define SET_CCR(SR,CCR) (SR) = ( ( (SR) & ~255 ) | (CCR) )

/**
 * Get IPL in SR value.
 * @param  SR  current SR value
 * @return IPL value
 * @retval ( SR >> SR_I_BIT ) & 7
 */
#define GET_IPL(SR) ( ( (SR) >> SR_I_BIT ) & 7 )

/**
 * Change IPL value of SR.
 * @param  SR   current SR value
 * @param  IPL  new IPL value [0..7]
 * @return new SR value
 * @retval ( SR & ~SR_I ) | ( IPL << SR_IPL_BIT )
 */
#define SET_IPL(SR,IPL) (SR) = ( ( (SR) & ~SR_I ) | ( (IPL) << SR_I_BIT ) )

/**
 * @}
 */

/**
 * @}
 */

#endif
