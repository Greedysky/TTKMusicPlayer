/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs_ao.h
 * @author   Benjamin Gerard
 * @date     2007-03-08
 * @brief    AO stream header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_AO_H
#define FILE68_VFS_AO_H

#include "file68_vfs.h"

/**
 * @name     Audio VFS
 * @ingroup  lib_file68_vfs
 *
 *   Implements vfs68_t for XIPH libao (audio output).
 *
 * @{
 */

FILE68_EXTERN
/**
 * Initialize ao virtual file system.
 */
int vfs68_ao_init(void);

FILE68_EXTERN
/**
 * Shutdown ao virtual file system.
 */
void vfs68_ao_shutdown(void);

FILE68_EXTERN
/**
 * Get sampling rate.
 */
int vfs68_ao_sampling(vfs68_t * vfs);

FILE68_EXTERN
/**
 * Create audio VFS.
 *
 * @param  uri  ao stream URI starts with the audio: scheme. It is
 *              composed by "/key=value" pairs.
 *        - driver=list lists available ao driver (MSG68_INFO).
 *        - driver=[default|null|alsa|pulse|oss|esd|au|raw|wav ...]
 *        - rate=hz
 *        - format=[endian][sign][channels][format]
 *          with (first value is the default)
 *          - endian   := [n|l|b]
 *          - sign     := [-|+]
 *          - channels := [1|2]
 *          - format   := [W|B|F]
 *
 *        Others keys are used to set specific driver options as
 *        defined by libao.
 *
 *        The first non-option component of the URI is used as the
 *        start of the output path for non-live (file) drivers
 *        (wav,raw ...).
 *
 * @param  mode  Only mode supported is write (VFS68_OPEN_WRITE=2).
 * @param  spr   Sampling rate (in hertz).
 *
 * @return VFS object
 * @retval 0  on failure
 *
 */
vfs68_t * vfs68_ao_create(const char * uri, int mode, int spr);

/**
 * @}
 */

#endif
