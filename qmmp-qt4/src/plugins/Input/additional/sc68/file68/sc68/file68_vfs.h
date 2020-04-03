/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Stream interface header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_H
#define FILE68_VFS_H


#ifndef FILE68_API
# include "file68_api.h"
#endif


/**
 * @defgroup  lib_file68_vfs  Virtual File System.
 * @ingroup   lib_file68
 *
 *   Provides access functions for generic I/O streams.
 *
 * @{
 */

/**
 * VFS type.
 */
#ifndef HAVE_VFS68
typedef struct _vfs68_t vfs68_t;
#define HAVE_VFS68 1  /**< defined if type vfs68_t is known */
#endif

/**
 * @name Generic access functions.
 * @{
 */

FILE68_API
/**
 * Get stream name.
 *
 * @param  vfs  stream
 *
 * @return stream name
 * @retval 0 Failure.
 */
const char * vfs68_filename(vfs68_t *vfs);

FILE68_API
/**
 * Open stream.
 *
 * @param  vfs  stream
 *
 * @return error code
 * @retval 0   Success
 * @retval -1  Failure
 */
int vfs68_open(vfs68_t *vfs);

FILE68_API
/**
 * Close stream.
 *
 * @param  vfs  stream
 *
 * @return error code
 * @retval 0   Success
 * @retval -1  Failure
 */
int vfs68_close(vfs68_t *vfs);

FILE68_API
/**
 * Read data from stream.
 *
 * @param  vfs   stream
 * @param  data  destination buffer
 * @param  len   number of byte to read
 *
 * @return number of byte read
 * @retval -1 Failure.
 */
int vfs68_read(vfs68_t *vfs, void * data, int len);

FILE68_API
/**
 * Write data into stream.
 *
 * @param  vfs   stream
 * @param  data  destination buffer
 * @param  len   number of byte to read
 *
 * @return number of byte written
 * @retval -1 Failure.
 */
int vfs68_write(vfs68_t *vfs, const void * data, int len);

FILE68_API
/**
 * Get stream length.
 *
 * @param  vfs  stream
 *
 * @return number of bytes.
 * @retval -1 Failure.
 */
int vfs68_length(vfs68_t *vfs);

FILE68_API
/**
 * Get stream current position.
 *
 * @param  vfs  stream
 *
 * @return stream position
 * @retval -1 Failure.
 */
int vfs68_tell(vfs68_t *vfs);

FILE68_API
/**
 * Set stream relative position.
 *
 * @param  vfs     stream
 * @param  offset  displacement from current position
 *
 * @return Absolute position after seeking
 * @retval -1 Failure.
 *
 * @see vfs68_seek_to()
 */
int vfs68_seek(vfs68_t *vfs, int offset);

FILE68_API
/**
 * Set stream absolute position.
 *
 * @param  vfs  stream
 * @param  pos  position to reach
 *
 * @return Absolute position after seeking
 * @retval -1 Failure.
 *
 * @see vfs68_seek()
 */
int vfs68_seek_to(vfs68_t *vfs, int pos);

FILE68_API
/**
 * Close and destroy stream.
 *
 * @param  vfs  stream
 *
 */
void vfs68_destroy(vfs68_t *vfs);

FILE68_API
/**
 * Read a '\\0' or '\\n' terminated string.
 *
 * @param  vfs     stream
 * @param  buffer  destination buffer
 * @param  max     destination buffer size
 *
 * @return number of char read
 * @retval -1  Failure.
 */
int vfs68_gets(vfs68_t *vfs, char * buffer, int max);

FILE68_API
/**
 * Read next character.
 *
 * @param  vfs  stream
 *
 * @return char value [0..255]
 * @retval -1  EOF or error.
 */
int vfs68_getc(vfs68_t *vfs);

FILE68_API
/**
 * Write a '\\0' terminated string.
 *
 * @param  vfs  stream
 * @param  s    string
 *
 * @return number of char written
 * @retval -1  Failure.
 */
int vfs68_puts(vfs68_t *vfs, const char * s);

FILE68_API
/**
 * Write a character.
 *
 * @param  vfs  stream
 * @param  c    char [0..255]
 *
 * @return error code
 * @retval  0  Success
 * @retval -1  Failure
 */
int vfs68_putc(vfs68_t *vfs, const int c);

/**
 * @}
 */

/**
 * @}
 */

#endif
