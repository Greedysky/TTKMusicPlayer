/**
 * @ingroup  lib_file68
 * @file     sc68/file68_err.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Error message header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_ERR_H
#define FILE68_ERR_H

#ifndef FILE68_API
# include "file68_api.h"
#endif
#include <stdarg.h>


/**
 * @defgroup  lib_file68_err  Error message
 * @ingroup   lib_file68
 *
 *   Provides error message facilities.
 *
 * @{
 */

FILE68_API
/**
 * Print a formatted error message.
 *
 *   The error68() function prints an error message via the
 *   msg68_error() function.
 *
 * @param  format   printf() like format string
 *
 * @return error-code
 * @retval -1
 */
int error68(const char * format, ... );

FILE68_API
/**
 * Extended version of error68() function.
 *
 * @param  cookie   user parameter
 * @param  format   printf() like format string
 *
 * @return error-code
 * @retval -1
 *
 * @see error68()
 */
int error68x(void * cookie, const char * format, ... );

FILE68_API
/**
 * Print a formatted error message (variable argument version).
 *
 * @param  format   printf() like format string
 * @param  list     variable argument list
 *
 * @see error68()
 */
int error68_va(const char * format, va_list list);

FILE68_API
/**
 * Extended version of error68() function.
 *
 * @param  cookie   user parameter
 * @param  format   printf() like format string
 * @param  list     variable argument list
 *
 * @return error-code
 * @retval -1
 *
 * @see error68()
 */
int error68x_va(void * cookie, const char * format, va_list list);

/**
 * @}
 */

#endif
