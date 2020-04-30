/**
 * @ingroup  lib_file68
 * @file     sc68/file68_chk.h
 * @author   Benjamin Gerard
 * @date     2011-10-02
 * @brief    Chunk definition header file.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_CHK_H
#define FILE68_CHK_H

/**
 * @addtogroup   lib_file68
 * @{
 */

/**
 * SC68 file chunk header.
 */
typedef struct
{
  char id[4];   /**< Must be "SC??".            */
  char size[4]; /**< Size in bytes (MSB first). */
} chunk68_t;

/**
 * @name SC68 file chunk definitions.
 * @{
 */

#define CH68_CHUNK     "SC"    /**< Chunk identifier.                    */
#define CH68_BASE      "68"    /**< Start of file.                       */
#define CH68_FNAME     "FN"    /**< File name (album).                   */
#define CH68_DEFAULT   "DF"    /**< Album default music.                 */
#define CH68_MUSIC     "MU"    /**< Music (track) section start.         */
#define CH68_MNAME     "MN"    /**< Track name (title).                  */
#define CH68_ANAME     "AN"    /**< Track or album artist.               */
#define CH68_CNAME     "CN"    /**< Track or album original artist.      */
#define CH68_D0        "D0"    /**< Track 'd0' value.                    */
#define CH68_AT        "AT"    /**< Track load address.                  */
#define CH68_TIME      "TI"    /**< Track length in seconds. @deprecated */
#define CH68_FRAME     "FR"    /**< Track length in frames.              */
#define CH68_FRQ       "FQ"    /**< Track replay rate in Hz.             */
#define CH68_LOOP      "LP"    /**< Track number of loop.                */
#define CH68_LOOPFR    "LF"    /**< Track loop length in frames.         */
#define CH68_TYP       "TY"    /**< Track HW feature flag.               */
#define CH68_YEAR      "YY"    /**< Track publishing year.               */
#define CH68_COPYRIGHT "CR"    /**< Track or album copyright owner.      */
#define CH68_TAG       "TG"    /**< Track or album Meta tag.             */
#define CH68_REPLAY    "RE"    /**< Track external replay.               */
#define CH68_UTF8      "U8"    /**< String are UTF-8 encoded.            */
#define CH68_ALIGN     "32"    /**< Chunks are 32bit aligned.            */
#define CH68_SFX       "FX"    /**< Track is a sound-fx.                 */
#define CH68_MDATA     "DA"    /**< Music data.                          */
#define CH68_EOF       "EF"    /**< End of file.                         */
#define CH68_NULL      "\0\0"  /**< Null.                                */

/**
 * @}
 */

/**
 * @}
 */

#endif
