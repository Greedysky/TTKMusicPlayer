/**
 * @ingroup   lib_sc68
 * @file      dial68.h
 * @brief     sc68 dialog helper file.
 * @author    Benjamin Gerard
 * @date      2014/07/20
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SC68_DIAL68_H
#define SC68_DIAL68_H

#include "sc68/sc68.h"
#include "sc68/file68_msg.h"

#define P "dial68 : "                /**< @nodoc */
SC68_EXTERN int dial_cat;            /**< dialog message categorie. */

/**
 *  @defgroup  lib_dial68  dialog helper
 *  @ingroup   lib_sc68
 *
 *  This module prodives functions to handle sc68 dialogs.
 *
 *  @{
 */

/**
 * Type for dial68_s structure.
 */
typedef struct dial68_s dial68_t;

/**
 * Dialog base structure.
 */
struct dial68_s {
  int         dial;                     /**< magic number.          */
  int         size;                     /**< sizeof() + extra.      */
  void      * data;                     /**< user data.             */
  sc68_dial_f cntl;                     /**< user control function. */
};

SC68_EXTERN
/**
 *  Create and run a new dialog.
 *
 *  @param  data  user private data (cookie).
 *  @param  cntl  user control function.
 *
 *  @return dialog return value
 *  @retval -1 the dialog failed
 *  @retval -2 user hit cancel or close
 */
int dial68(void * data, sc68_dial_f cntl);


SC68_EXTERN
/**
 * The dialog frontend (called by dial68()) function.
 *
 *  @param  data  user private data (cookie), normally a dial68_t child.
 *  @param  cntl  user control function.
 *
 *  @return dialog return value
 *  @retval -1 the dialog failed
 *  @retval -2 user hit cancel or close
 */
int dial68_frontend(void * data, sc68_dial_f cntl);

/**
 * @}
 */

#endif
