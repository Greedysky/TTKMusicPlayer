#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#ifndef _WIN32
#define __stdcall
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define V2TYPES

typedef int               sInt;
typedef unsigned int      sUInt;
typedef sInt              sBool;
typedef char              sChar;

typedef int8_t     sS8;
typedef int16_t    sS16;
typedef int32_t     sS32;
typedef int64_t  sS64;

typedef uint8_t     sU8;
typedef uint16_t    sU16;
typedef uint32_t     sU32;
typedef uint64_t  sU64;

typedef float             sF32;
typedef double            sF64;

#define sTRUE             1
#define sFALSE            0

template<class T> inline T sMin(const T a, const T b) { return (a<b)?a:b;  }
template<class T> inline T sMax(const T a, const T b) { return (a>b)?a:b;  }
template<class T> inline T sClamp(const T x, const T min, const T max) { return sMax(min,sMin(max,x)); }

#endif
