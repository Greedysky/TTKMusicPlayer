//
// Audio Overload SDK
//
// Fake ao.h to set up the general Audio Overload style environment
//

#ifndef __AO_H
#define __AO_H

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#define AO_SUCCESS					1
#define AO_FAIL						0
#define AO_FAIL_DECOMPRESSION		-1

#define MAX_DISP_INFO_LENGTH		256
#define AUDIO_RATE					(44100)

enum
{
	COMMAND_NONE = 0,
	COMMAND_PREV,
	COMMAND_NEXT,
	COMMAND_RESTART,
	COMMAND_HAS_PREV,
	COMMAND_HAS_NEXT,
	COMMAND_GET_MIN,
	COMMAND_GET_MAX,
	COMMAND_JUMP
};

#if 0
/* Compiler defines for Xcode */
#ifdef __BIG_ENDIAN__
	#undef LSB_FIRST
#endif

#ifdef __LITTLE_ENDIAN__
	#define LSB_FIRST	1
#endif
#endif

#ifdef WORDS_BIGENDIAN
	#undef LSB_FIRST
#else
	#define LSB_FIRST	1
#endif

typedef unsigned char ao_bool;

#ifdef __GNUC__
#include <stddef.h>	// get NULL
#include <stdbool.h>

#ifndef nil
#define nil NULL
#endif

#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define xmalloc(a) malloc(a)

#endif

#ifdef _MSC_VER
#include <stddef.h>	// get NULL
#include <wchar.h> // for off_t

#ifndef nil
#define nil NULL
#endif

#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define true (1)
#define false (0)

#define xmalloc(a) malloc(a)

#define strcasecmp _strcmpi

#endif

#ifndef PATH_MAX
#define PATH_MAX	2048
#endif

typedef struct
{
	char title[9][MAX_DISP_INFO_LENGTH];
	char info[9][MAX_DISP_INFO_LENGTH];
} ao_display_info;

typedef unsigned char		uint8;
typedef unsigned char		UINT8;
typedef signed char			int8;
typedef signed char			INT8;
typedef unsigned short		uint16;
typedef unsigned short		UINT16;
typedef signed short		int16;
typedef signed short		INT16;
typedef signed int			int32;
typedef unsigned int		uint32;
#ifdef LONG_IS_64BIT
typedef signed long             int64;
typedef unsigned long           uint64;
#else
typedef signed long long	int64;
typedef unsigned long long	uint64;
#endif

typedef int8 s8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

#ifndef INLINE
#if defined(_MSC_VER)
#define INLINE __forceinline
#elif defined(__GNUC__)
#define INLINE __inline__
#elif defined(_MWERKS_)
#define INLINE inline
#elif defined(__powerc)
#define INLINE inline
#else
#define INLINE
#endif
#endif

#if LSB_FIRST
static INLINE u16 BFLIP16(u16 x)
{
 return x;
}
#else
static INLINE u16 BFLIP16(u16 x)
{
 return( ((x>>8)&0xFF)| ((x&0xFF)<<8) );
}
#endif

#ifdef WIN32
#ifndef _BASETSD_H
typedef signed int			INT32;
typedef unsigned int		UINT32;
typedef signed long long	INT64;
typedef unsigned long long	UINT64;
#endif
#else
typedef signed int			INT32;
typedef unsigned int		UINT32;
#ifdef LONG_IS_64BIT
typedef signed long         INT64;
typedef unsigned long       UINT64;
#else
typedef signed long long	INT64;
typedef unsigned long long	UINT64;
#endif
#endif

#if LSB_FIRST
#define LE16(x) (x)
#define LE32(x) (x)

#ifndef __ENDIAN__ /* Mac OS X Endian header has this function in it */
static unsigned long INLINE Endian32_Swap(unsigned long addr)
{
	unsigned long res = (((addr&0xff000000)>>24) |
		 ((addr&0x00ff0000)>>8) |
		 ((addr&0x0000ff00)<<8) |
		 ((addr&0x000000ff)<<24));

	return res;
}
#endif

#else

static unsigned short INLINE LE16(unsigned short x)
{
	unsigned short res = (((x & 0xFF00) >> 8) | ((x & 0xFF) << 8));
	return res;
}

static unsigned long INLINE LE32(unsigned long addr)
{
	unsigned long res = (((addr&0xff000000)>>24) |
		 ((addr&0x00ff0000)>>8) |
		 ((addr&0x0000ff00)<<8) |
		 ((addr&0x000000ff)<<24));

	return res;
}

#endif

int ao_get_lib(char *filename, uint8 **buffer, uint64 *length);

int ao_identify (char *buffer);

void *ao_start (uint32 type, const char *name, uint8 *buffer, uint32 size);

int ao_stop (uint32 type, void *handle);

int ao_get_info (uint32 type, void *handle, ao_display_info *info);

int ao_decode (uint32 type, void *handle, int16 *buffer, uint32 size);

int ao_command (uint32 type, void *handle, int32 command, int32 param);

void ao_getlibpath (const char *path, const char *libname, char *libpath, int size);

#endif // AO_H
