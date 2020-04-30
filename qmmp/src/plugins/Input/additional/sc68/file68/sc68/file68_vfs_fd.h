/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs_fd.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    File descriptor stream header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_FD_H
#define FILE68_VFS_FD_H

#include "file68_vfs.h"

/**
 * @name     File descriptor stream
 * @ingroup  lib_file68_vfs
 *
 *   Implements vfs68_t for "un*x like" file descriptor.
 *
 * @note fd vfs optionnal scheme are "fd:", "file:", "local:", "stdin:",
 *       "stderr:" or "stdout:"
 *
 * @{
 */

FILE68_EXTERN
/**
 * Init file descriptor VFS (register fd scheme).
 *
 * @retval  0  always success
 */
int vfs68_fd_init(void);

FILE68_EXTERN
/**
 * Shutdown file descriptor VFS (unregister fd scheme).
 */
void vfs68_fd_shutdown(void);

/**
 * @}
 */

#endif
