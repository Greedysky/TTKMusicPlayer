/**
 * @ingroup  lib_file68
 * @file     sc68/file68_api.h
 * @author   Benjamin Gerard
 * @date     2007-02-25
 * @brief    Symbol exportation header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_API_H
#define FILE68_API_H

/**
 * @defgroup lib_file68 file68 library
 * @ingroup  sc68_dev_lib
 *
 *   file68 is a library to manipulate sc68 files and access sc68
 *   resources and much more.
 *
 * @{
 */

#ifndef FILE68_EXTERN
# ifdef __cplusplus
/** Decorate symbols exported locally. */
#   define FILE68_EXTERN extern "C"
# else
#   define FILE68_EXTERN extern
# endif
#endif

#ifndef FILE68_API
/* Building */
# ifdef FILE68_EXPORT
#  if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC)
#   define FILE68_API __declspec(dllexport)
#  elif defined(HAVE_VISIBILITY)
#   define FILE68_API FILE68_EXTERN __attribute__ ((visibility("default")))
#  else
#   define FILE68_API FILE68_EXTERN
#  endif
/* Using */
# else
#  if defined(FILE68_DLL)
#   define FILE68_API __declspec(dllimport)
#  else
#   define FILE68_API FILE68_EXTERN
#  endif
# endif
#endif

/**
 * Decorate symbols exported for public.
*/
#ifndef FILE68_API
#define FILE68_API FILE68_EXTERN
#error "FILE68_API should be defined"
#endif

/**
 * @}
 */

#endif
