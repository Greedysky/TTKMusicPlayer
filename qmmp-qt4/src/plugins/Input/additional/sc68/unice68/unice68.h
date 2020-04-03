/**
 * @ingroup  lib_unice68
 * @file     unice68.h
 * @author   Benjamin Gerard
 * @date     2003/08/06
 * @brief    ICE! depacker header
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef UNICE68_H
#define UNICE68_H

#ifndef UNICE68_EXTERN
# ifdef __cplusplus
#   define UNICE68_EXTERN extern "C"
# else
#   define UNICE68_EXTERN extern
# endif
#endif

#ifndef UNICE68_API
/* Building */
# ifdef UNICE68_EXPORT
#  if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC)
#   define UNICE68_API __declspec(dllexport)
#  elif defined(HAVE_VISIBILITY)
#   define UNICE68_API UNICE68_EXTERN __attribute__ ((visibility("default")))
#  else
#   define UNICE68_API UNICE68_EXTERN
#  endif
/* Using */
# else
#  if defined(UNICE68_DLL)
#   define UNICE68_API __declspec(dllimport)
#  else
#   define UNICE68_API UNICE68_EXTERN
#  endif
# endif
#endif

/**
 *  @defgroup  lib_unice68  unice68 library
 *  @ingroup   sc68_dev_lib
 *  @brief     ICE! 2.4 packer and depacker.
 *
 *    unice68 library provides functions to pack or depack ICE! packed
 *    buffer.  The depacker used is a rought conversion of the
 *    original ICE 2.4 routine written in 68000 assembler. ICE packer
 *    was (and may be still is) widely used on the Atari ST the Atari
 *    ST. More importantly it is the packer used by
 *    [the sndh music archive](http://dbug.kicks-ass.net/sndh/).
 *
 *  @note unice68 library is re-entrant (thread-safe).
 *
 *  @note It is pretty much impossible to implement a progressive
 *  version of the depacker. At least the entire packed file must be
 *  loaded before depacking it because data order is reversed.
 *
 *  @{
 */

#ifndef UNICE68_API
/**
 *  unice68 symbols specification.
 *
 *  Define special atributs for importing/exporting unice68 symbols.
 */
#define UNICE68_API UNICE68_EXTERN
#error "UNICE68_API should be defined"
#endif

UNICE68_API
/**
 *  Get unice68 version.
 *
 *  @return version number
 */
int unice68_version(void);

UNICE68_API
/**
 *  Get unice68 version.
 *
 *  @return version string
 */
const char * unice68_versionstr(void);

UNICE68_API
/**
 *  Get ICE depacker version.
 *
 *  @return version number (MAJOR*100+MINOR)
 */
int unice68_unice_version(void);

UNICE68_API
/**
 *  Get ICE packer version.
 *
 *  @return version number (MAJOR*100+MINOR)
 */
int unice68_ice_version(void);

UNICE68_API
/**
 *  Test ICE and get compressed and uncompresed size.
 *
 *    The unice68_depacked_size() function returns the uncompressed
 *    size of a ICE compressed buffer. If p_size is not 0 it is fill with
 *    the size of the compressed data found in header (useful for stream
 *    operation).
 *    If the value pointed by p_csize is not 0 the function assumes that it is
 *    an expected compressed size and compares it to header one. If it differs
 *    the function returns the bitwise NOT value of uncompressed data. This
 *    should be a minus value but not -1.
 *
 *  @param  buffer   buffer with at least 12 bytes of data (ice header).
 *  @param  p_csize  Optionnal pointer to store compressed size.
 *                   May be use to verify an expected compressed size.
 *                   See function documentation for more details.
 *
 *  @return uncompressed size
 *  @retval >0   Uncompressed size
 *  @retval -1   Error; not a valid ICE packed buffer
 *  @retval <0   Bitwise NOT of uncompressed size but verify failed.
 *
 */
int unice68_depacked_size(const void * buffer, int * p_csize);

UNICE68_API
/**
 *  Depack an ICE buffer into another.
 *
 *   The unice68_depacker() function depacks src input ICE compressed
 *   buffer into dst output buffer.  The dst buffer is assumed to be
 *   already allocated and large enough.
 *
 * @param  dst   output (destination) buffer (uncompressed data).
 * @param  src   input  (source)      buffer (compressed data).
 *
 * @return error code
 * @retval 0     succcess
 * @retval -1    failure
 *
 * @warning The original ICE depacker may allow to use the same buffer for
 *          compressed and uncompressed data. Anyway this has not been tested
 *          and you are encouraged to add guard bands.
 */
int unice68_depacker(void * dst, const void * src);

UNICE68_API
/**
 *  Pack a buffer with ice packer.
 *
 * @param  dst   output (destination) buffer (compressed data).
 * @param  max   output buffer size.
 * @param  src   input  (source)      buffer (uncompressed data).
 * @param  len   input buffer length.
 *
 * @return packed size
 * @retval -1    failure
 *
 * @warning  Output buffer overflow is currently not checked and will
 *           happen if the output buffer is not large enough.
 */
int unice68_packer(void * dst, int max, const void * src, int len);

/**
 * @}
 */

#endif
