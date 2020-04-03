/**
 * @ingroup  lib_file68
 * @file     sc68/file68_ice.h
 * @author   Benjamin Gerard
 * @date     2003-09-06
 * @brief    ICE loader header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_ICE_H
#define FILE68_ICE_H

#include "file68_vfs.h"

/**
 * @defgroup  lib_file68_ice  ICE loader support.
 * @ingroup   lib_file68
 *
 *   Provides functions for loading ICE stream.
 *
 * @{
 */

FILE68_API
/**
 * Get ICE! depacker version.
 *
 * @retval   1  ICE! is supported but unknown version
 * @retval   0  ICE! is not supported
 * @return  ICE! depacker version
 *
 * @see unice68_ice_version()
 */
int file68_ice_version(void);

FILE68_API
/**
 * Test ice file header magic header.
 *
 * @param  buffer  Buffer containing at least 12 bytes from ice header.
 *
 * @retval  1  buffer seems to be ice packed..
 * @retval  0  buffer is not ice packed.
 */
int file68_ice_is_magic(const void * buffer);

FILE68_API
/**
 * Load an iced stream.
 *
 *   The file68_ice_load() function loads and depack an ice packed
 *   file from a stream and returns a allocate buffer with unpacked
 *   data.
 *
 * @param  is     Stream to load (must be opened in read mode).
 * @param  ulen   Pointer to save uncompressed size.
 *
 * @return Pointer to the uncompressed data buffer.
 * @retval 0 Error
 */
void * file68_ice_load(vfs68_t * is, int * ulen);

FILE68_API
/**
 * Load an iced file.
 *
 * @param  fname    File to load.
 * @param  ulen     Pointer to save uncompressed size.
 *
 * @return Pointer to the uncompressed data buffer.
 * @retval 0 Error
 *
 * @see file68_ice_load()
 */
void * file68_ice_load_file(const char * fname, int * ulen);

/**
 * @}
 */

#endif
