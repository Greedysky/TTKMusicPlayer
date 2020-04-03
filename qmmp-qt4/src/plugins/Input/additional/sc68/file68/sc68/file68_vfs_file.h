/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs_file.h
 * @author   Benjamin Gerard
 * @date     2007/08/08
 * @brief    FILE stream header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_FILE_H
#define FILE68_VFS_FILE_H

#include "file68_vfs.h"

/**
 * @name     FILE stream
 * @ingroup  lib_file68_vfs
 *
 *   Implements vfs68_t for stdio.h FILE.
 *
 * @note file vfs optionnal scheme are "file:", "local:", "stdin:",
 *       "stderr:" or "stdout:"
 *
 * @{
 */

FILE68_EXTERN
/**
 * Init file VFS (register file scheme).
 *
 * @retval  0  always success
 */
int vfs68_file_init(void);

FILE68_EXTERN
/**
 * Shutdown file VFS (unregister file scheme).
 */
void vfs68_file_shutdown(void);

/**
 * @}
 */

#endif
