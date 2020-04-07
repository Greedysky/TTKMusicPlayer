/*
 * Z80~Ex, ZILoG Z80 CPU emulator.
 *
 * by Boo-boo [boo_boo(^at^)inbox.ru]
 * contains code from the FUSE project (http://fuse-emulator.sourceforge.net)
 * Released under GNU GPL v2
 *
 */

#ifndef _Z80EX_COMMON_H_INCLUDED
#define _Z80EX_COMMON_H_INCLUDED

#ifdef _MSC_VER
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

#define USE_TIMINGS 0

#if defined(__SYMBIAN32__)
typedef unsigned char Z80EX_BYTE;
typedef signed char Z80EX_SIGNED_BYTE;
typedef unsigned short Z80EX_WORD;
typedef unsigned int Z80EX_DWORD;
#elif defined(__GNUC__)
#include <stdint.h>
typedef uint8_t Z80EX_BYTE;
typedef int8_t Z80EX_SIGNED_BYTE;
typedef uint16_t Z80EX_WORD;
typedef uint32_t Z80EX_DWORD;
#elif defined(_MSC_VER)
typedef unsigned __int8 Z80EX_BYTE;
typedef signed __int8 Z80EX_SIGNED_BYTE;
typedef unsigned __int16 Z80EX_WORD;
typedef unsigned __int32 Z80EX_DWORD;
#else
typedef unsigned char Z80EX_BYTE;
typedef signed char Z80EX_SIGNED_BYTE;
typedef unsigned short Z80EX_WORD;
typedef unsigned int Z80EX_DWORD;
#endif

#endif
