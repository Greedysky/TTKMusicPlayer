/**
 * @ingroup  lib_file68
 * @file     sc68/file68_vfs_def.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Generic stream definition header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_VFS_DEF_H
#define FILE68_VFS_DEF_H

#include "file68_vfs.h"

/**
 * @name     Open modes
 * @ingroup  lib_file68_vfs
 * @{
 */
enum {
  VFS68_OPEN_READ_BIT = 0,              /**< Read open mode bit. */
  VFS68_OPEN_WRITE_BIT = 1,             /**< Write open mode bit. */
  VFS68_SLAVE_BIT = 7,                  /**< Slave mode bit. */
  VFS68_OPEN_READ = 1<<VFS68_OPEN_READ_BIT, /**< Read open mode value. */
  VFS68_OPEN_WRITE = 1<<VFS68_OPEN_WRITE_BIT, /**< Write open mode value. */
  VFS68_OPEN_MASK= VFS68_OPEN_READ|VFS68_OPEN_WRITE, /**< Open modes mask. */
  VFS68_SLAVE = 1<<VFS68_SLAVE_BIT,     /**< Slave mode value. */
};

/**
 * Test SLAVE mode.
 */
#define VFS68_IS_SLAVE(V) (((V)>>VFS68_SLAVE_BIT)&1)

/**
 * Test if any open flags is set (returns 0 or 1).
 */
#define VFS68_IS_OPEN(V) (!!((V)&VFS68_OPEN_MASK))

/**
 * Test if READ open flags is set (returns 0 or 1).
 */
#define VFS68_IS_OPEN_READ(V) (((V)>>VFS68_OPEN_READ_BIT)&1)

/**
 * Test if WRITE open flags is set (returns 0 or 1).
 */
#define VFS68_IS_OPEN_WRITE(V) (((V)>>VFS68_OPEN_WRITE_BIT)&1)

/**
 * @}
 */

/**
 * @name     Stream function prototypes.
 * @ingroup  lib_file68_vfs
 * @{
 */
typedef const char * (* vfs68_name_t) (vfs68_t *);
typedef int  (* vfs68_open_t)    (vfs68_t *);
typedef int  (* vfs68_close_t)   (vfs68_t *);
typedef int  (* vfs68_length_t)  (vfs68_t *);
typedef int  (* vfs68_flush_t)   (vfs68_t *);
typedef int  (* vfs68_tell_t)    (vfs68_t *);
typedef int  (* vfs68_seek_t)    (vfs68_t *, int);
typedef int  (* vfs68_read_t)    (vfs68_t *, void *, int);
typedef int  (* vfs68_write_t)   (vfs68_t *, const void *, int);
typedef void (* vfs68_destroy_t) (vfs68_t *);
/**
 * @}
 */

/**
 * Generic stream structure.
 *
 *   Set of functions needed to handle a stream.
 *
 * @ingroup  lib_file68_vfs
 */
struct _vfs68_t {
  vfs68_name_t    name;    /**< Get stream name.        */
  vfs68_open_t    open;    /**< Open stream.            */
  vfs68_close_t   close;   /**< Close stream.           */
  vfs68_read_t    read;    /**< Read data from stream.  */
  vfs68_write_t   write;   /**< Write data to stream.   */
  vfs68_flush_t   flush;   /**< Flush data.             */
  vfs68_length_t  length;  /**< Get stream data length. */
  vfs68_tell_t    tell;    /**< Get stream position.    */
  vfs68_seek_t    seekf;   /**< Seek forward.           */
  vfs68_seek_t    seekb;   /**< Seek backward.          */
  vfs68_destroy_t destroy; /**< Destructor.             */
};

#endif
