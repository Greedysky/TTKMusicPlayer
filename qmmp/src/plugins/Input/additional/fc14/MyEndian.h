// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#ifndef MYENDIAN_H
#define MYENDIAN_H

//#include "Config.h"
#include "MyTypes.h"

// This should never be true.
#if defined(LO) || defined(HI) || defined(LOLO) || defined(LOHI) || defined(HILO) || defined(HIHI)
  #error Redefinition of these values can cause trouble.
#endif

// For optional direct memory access.
// First value in memory/array = index 0.
// Second value in memory/array = index 1.

// For a pair of bytes/words/longwords.
#undef LO
#undef HI

// For two pairs of bytes/words/longwords.
#undef LOLO
#undef LOHI
#undef HILO
#undef HIHI

#if defined(WORDS_BIGENDIAN)
// byte-order: HI..3210..LO
  #define LO 1
  #define HI 0
  #define LOLO 3
  #define LOHI 2
  #define HILO 1
  #define HIHI 0
#else
// byte-order: LO..0123..HI
  #define LO 0
  #define HI 1
  #define LOLO 0
  #define LOHI 1
  #define HILO 2
  #define HIHI 3
#endif

union cpuLword
{
	uword w[2];  // single 16-bit low and high word
	udword l;    // complete 32-bit longword
};

union cpuWord
{
	ubyte b[2];  // single 8-bit low and high byte
	uword w;     // complete 16-bit word
};

union cpuLBword
{
	ubyte b[4];  // single 8-bit bytes
	udword l;    // complete 32-bit longword
};

// Convert high-byte and low-byte to 16-bit word.
// Used to read 16-bit words in little-endian order.
inline uword readEndian(ubyte hi, ubyte lo)
{
  return(( (uword)hi << 8 ) + (uword)lo );
}

// Convert high bytes and low bytes of MSW and LSW to 32-bit word.
// Used to read 32-bit words in little-endian order.
inline udword readEndian(ubyte hihi, ubyte hilo, ubyte hi, ubyte lo)
{
  return(( (udword)hihi << 24 ) + ( (udword)hilo << 16 ) + 
		 ( (udword)hi << 8 ) + (udword)lo );
}

// Read a little-endian 16-bit word from two bytes in memory.
inline uword readLEword(const ubyte ptr[2])
{
#if defined(WORDS_LITTLEENDIAN) && defined(OPTIMIZE_ENDIAN_ACCESS)
	return *((uword*)ptr);
#else
	return readEndian(ptr[1],ptr[0]);
#endif
}

// Write a big-endian 16-bit word to two bytes in memory.
inline void writeLEword(ubyte ptr[2], uword someWord)
{
#if defined(WORDS_LITTLEENDIAN) && defined(OPTIMIZE_ENDIAN_ACCESS)
	*((uword*)ptr) = someWord;
#else
	ptr[0] = (someWord & 0xFF);
	ptr[1] = (someWord >> 8);
#endif
}



// Read a big-endian 16-bit word from two bytes in memory.
inline uword readBEword(const ubyte ptr[2])
{
#if defined(WORDS_BIGENDIAN) && defined(OPTIMIZE_ENDIAN_ACCESS)
	return *((uword*)ptr);
#else
	return ( (((uword)ptr[0])<<8) + ((uword)ptr[1]) );
#endif
}

// Read a big-endian 32-bit word from four bytes in memory.
inline udword readBEdword(const ubyte ptr[4])
{
#if defined(WORDS_BIGENDIAN) && defined(OPTIMIZE_ENDIAN_ACCESS)
	return *((udword*)ptr);
#else
	return ( (((udword)ptr[0])<<24) + (((udword)ptr[1])<<16)
			+ (((udword)ptr[2])<<8) + ((udword)ptr[3]) );
#endif
}

// Write a big-endian 16-bit word to two bytes in memory.
inline void writeBEword(ubyte ptr[2], uword someWord)
{
#if defined(WORDS_BIGENDIAN) && defined(OPTIMIZE_ENDIAN_ACCESS)
	*((uword*)ptr) = someWord;
#else
	ptr[0] = someWord >> 8;
	ptr[1] = someWord & 0xFF;
#endif
}

// Write a big-endian 32-bit word to four bytes in memory.
inline void writeBEdword(ubyte ptr[4], udword someDword)
{
#if defined(WORDS_BIGENDIAN) && defined(OPTIMIZE_ENDIAN_ACCESS)
	*((udword*)ptr) = someDword;
#else
	ptr[0] = someDword >> 24;
	ptr[1] = (someDword>>16) & 0xFF;
	ptr[2] = (someDword>>8) & 0xFF;
	ptr[3] = someDword & 0xFF;
#endif
}



// Convert 16-bit little-endian word to big-endian order or vice versa.
inline uword convertEndianess( uword intelword )
{
	uword hi = intelword >> 8;
	uword lo = intelword & 255;
	return(( lo << 8 ) + hi );
}

// Convert 32-bit little-endian word to big-endian order or vice versa.
inline udword convertEndianess( udword inteldword )
{
	udword hihi = inteldword >> 24;
	udword hilo = ( inteldword >> 16 ) & 0xFF;
	udword hi = ( inteldword >> 8 ) & 0xFF;
	udword lo = inteldword & 0xFF;
	return(( lo << 24 ) + ( hi << 16 ) + ( hilo << 8 ) + hihi );
}

#endif  // MYENDIAN_H
