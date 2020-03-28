/******************************************************************************
*                                                                             *
*  Copyright (C) 1992-1995 Tony Robinson                                      *
*                                                                             *
*  See the file doc/LICENSE.shorten for conditions on distribution and usage  *
*                                                                             *
******************************************************************************/

/*
 * $Id: shorten.h,v 1.4 2001/12/30 05:12:04 jason Exp $
 */

#ifndef _SHORTEN_H
#define _SHORTEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define MAGIC			"ajkg"
#define FORMAT_VERSION		2
#define MIN_SUPPORTED_VERSION	1
#define MAX_SUPPORTED_VERSION	3
#define MAX_VERSION		7

#define UNDEFINED_UINT		-1
#define DEFAULT_BLOCK_SIZE	256
#define DEFAULT_V0NMEAN	0
#define DEFAULT_V2NMEAN	4
#define DEFAULT_MAXNLPC	0
#define DEFAULT_NCHAN		1
#define DEFAULT_NSKIP		0
#define DEFAULT_NDISCARD	0
#define NBITPERLONG		32
#define DEFAULT_MINSNR          256
#define DEFAULT_MAXRESNSTR	"32.0"
#define DEFAULT_QUANTERROR	0
#define MINBITRATE		2.5

#define MAX_LPC_ORDER	64
#define CHANSIZE	0
#define ENERGYSIZE	3
#define BITSHIFTSIZE	2
#define NWRAP		3

#define FNSIZE		2
#define FN_DIFF0	0
#define FN_DIFF1	1
#define FN_DIFF2	2
#define FN_DIFF3	3
#define FN_QUIT		4
#define FN_BLOCKSIZE	5
#define FN_BITSHIFT	6
#define FN_QLPC		7
#define FN_ZERO		8
#define FN_VERBATIM     9

#define VERBATIM_CKSIZE_SIZE 5	/* a var_put code size */
#define VERBATIM_BYTE_SIZE 8	/* code size 8 on single bytes means
				 * no compression at all */
#define VERBATIM_CHUNK_MAX 256	/* max. size of a FN_VERBATIM chunk */

#define ULONGSIZE	2
#define NSKIPSIZE	1
#define LPCQSIZE	2
#define LPCQUANT	5
#define XBYTESIZE	7

#define TYPESIZE	4
#define TYPE_AU1	0	/* original lossless ulaw                    */
#define TYPE_S8	        1	/* signed 8 bit characters                   */
#define TYPE_U8         2	/* unsigned 8 bit characters                 */
#define TYPE_S16HL	3	/* signed 16 bit shorts: high-low            */
#define TYPE_U16HL	4	/* unsigned 16 bit shorts: high-low          */
#define TYPE_S16LH	5	/* signed 16 bit shorts: low-high            */
#define TYPE_U16LH	6	/* unsigned 16 bit shorts: low-high          */
#define TYPE_ULAW	7	/* lossy ulaw: internal conversion to linear */
#define TYPE_AU2	8	/* new ulaw with zero mapping                */
#define TYPE_AU3	9	/* lossless alaw                             */
#define TYPE_ALAW 	10	/* lossy alaw: internal conversion to linear */
#define TYPE_RIFF_WAVE  11	/* Microsoft .WAV files                      */
#define TYPE_EOF	12
#define TYPE_GENERIC_ULAW 128
#define TYPE_GENERIC_ALAW 129

#define POSITIVE_ULAW_ZERO 0xff
#define NEGATIVE_ULAW_ZERO 0x7f

#ifndef MAX_PATH
#define MAX_PATH 2048
#endif

#ifndef	MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef	MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#if defined(unix) && !defined(linux)
#define labs abs
#endif

#define ROUNDEDSHIFTDOWN(x, n) (((n) == 0) ? (x) : ((x) >> ((n) - 1)) >> 1)

#ifndef M_LN2
#define	M_LN2	0.69314718055994530942
#endif

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

/* BUFSIZ must be a multiple of four to contain a whole number of words */
#ifdef BUFSIZ
#undef BUFSIZ
#endif

#define BUFSIZ 512

#define V2LPCQOFFSET (1 << LPCQUANT);

#define UINT_GET(nbit, shnfile) \
  ((version == 0) ? uvar_get(nbit, shnfile) : ulong_get(shnfile))

#endif
