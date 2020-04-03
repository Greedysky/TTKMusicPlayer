/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs_z.h
 * @author   Benjamin Gerard
 * @date     2003-10-06
 * @brief    Zlib stream header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_Z
#define FILE68_VFS_Z

#include "file68_vfs.h"

/**
 * @name     Z stream
 * @ingroup  lib_file68_vfs
 *
 * @anchor zlib
 *
 *   Implements vfs68_t for Z library.  Z is a famous
 *   compression/decompression library. For more informations see
 *   <a href="http://www.gzip.org">gzip website</a>.
 *
 * @todo      deflate (compression) mode.
 * @{
 */

/**
 * gzip options.
 */
typedef struct {
  unsigned int gzip:1;     /**< Read/Write gzip file format.                */
  unsigned int level:4;    /**< Compression level [0..9] or -1 for default. */
  unsigned int strategy:3; /**< Compression strategy (0 is default).        */
  unsigned int name:1;     /**< Include original name to gzip.              */
} vfs68_z_option_t;

/**
 * Default gzip option.
 */
FILE68_EXTERN
const vfs68_z_option_t vfs68_z_default_option;

FILE68_EXTERN
/**
 * Create a @ref zlib "Z" stream.
 *
 * @param  is     Stream to compress/decompress.
 * @param  mode   bit-0: read access, bit-1: write access.
 * @param  opt    gzip options.
 *
 * @return stream
 * @retval 0 on error
 */
vfs68_t * vfs68_z_create(vfs68_t * is, int mode,
                                 const vfs68_z_option_t opt);

FILE68_EXTERN
/**
 * Init gzip vfs.
 */
int vfs68_z_init(void);

FILE68_EXTERN
/**
 * Shutdown gzip vfs.
 */
void vfs68_z_shutdown(void);

/**
 * @}
 */

#endif
