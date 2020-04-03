/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs_curl.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    @ref cURL stream header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_CURL_H
#define FILE68_VFS_CURL_H

#include "file68_vfs.h"

/**
 * @name     cURL stream
 * @ingroup  lib_file68_vfs
 *
 * @anchor cURL
 *
 *   @b cURL is a client-side URL transfer library. For more informations
 *   see [cURL official website](http://curl.planetmirror.com/libcurl/).
 *
 * @{
 */

FILE68_EXTERN
/**
 * Initialize curl engine.
 *
 *   The vfs68_curl_init() function initializes curl library.  It
 *   is called by the file68_init() function and everytime a new curl
 *   stream is created with the vfs68_curl_create() function.
 *
 * @return error code
 * @retval  0   success
 * @retval  -1  failure
 */
int vfs68_curl_init(void);

FILE68_EXTERN
/**
 * Shutdown curl engine.
 *
 *  The vfs68_curl_shutdoen() function shutdown curl library. It
 *  is called by the file68_shutdown() function.
 */
void vfs68_curl_shutdown(void);

/**
 * @}
 */

#endif
