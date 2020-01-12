/*-----------------------------------------------------------------------------

	ST-Sound ( YM files player library )

	Define YM types for multi-platform compilation.
	Change that file depending of your platform. Please respect the right size
	for each type.

-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ST-Sound, ATARI-ST Music Emulator
* Copyright (c) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
-----------------------------------------------------------------------------*/

#ifndef __YMTYPES__
#define __YMTYPES__

#ifdef _MSC_VER

typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else
#include <stdint.h>
#endif


#define YM_INTEGER_ONLY

//-----------------------------------------------------------
// Platform specific stuff
//-----------------------------------------------------------

#ifdef YM_INTEGER_ONLY
typedef	int64_t		yms64;
#else
typedef	float		ymfloat;
#endif

typedef	int8_t		yms8;			//  8 bits signed integer
typedef	int16_t		yms16;			// 16 bits signed integer
typedef	int32_t		yms32;			// 32 bits signed integer

typedef	uint8_t		ymu8;			//  8 bits unsigned integer
typedef	uint16_t	ymu16;			// 16 bits unsigned integer
typedef	uint32_t	ymu32;			// 32 bits unsigned integer

typedef	yms32		ymint;			// Native "int" for speed purpose. StSound suppose int is signed and at least 32bits. If not, change it to match to yms32
typedef	char		ymchar;			// 8 bits char character (used for null terminated strings)


#ifndef NULL
#define NULL	(0L)
#endif

//-----------------------------------------------------------
// Multi-platform
//-----------------------------------------------------------
typedef		int					ymbool;			// boolean ( theorically nothing is assumed for its size in StSound,so keep using int)
typedef		yms16				ymsample;		// StSound emulator render mono 16bits signed PCM samples

#define		YMFALSE				(0)
#define		YMTRUE				(!YMFALSE)

#endif

