/**
 * @ingroup  lib_file68
 * @file     sc68/file68_reg.h
 * @author   Benjamin Gerard
 * @date     2003-08-11
 * @brief    Windows registry header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_REG_H
#define FILE68_REG_H

#ifndef FILE68_API
# include "file68_api.h"
#endif

/**
 * @defgroup  lib_file68_reg  Registry access
 * @ingroup   lib_file68
 *
 *   Provides Windows registry access functions.
 *
 * @{
 */

/**
 * Enumerate registry key type.
 *
 *  @warning Do not change order
 */
enum registry68_key_e {
  REGISTRY68_INK = -1,                  /**< INvalid Key.          */
  REGISTRY68_CRK = 0,                   /**< Classes Root Key.     */
  REGISTRY68_CUK,                       /**< Current User Key.     */
  REGISTRY68_LMK,                       /**< Local Machine Key.    */
  REGISTRY68_USK,                       /**< USers Key.            */
  REGISTRY68_PDK,                       /**< Performance Data Key. */
  REGISTRY68_CCK,                       /**< Current Config Key.   */
  REGISTRY68_DDK,                       /**< Dynamic Data Key.     */

  REGISTRY68_LST                        /**< Last element.         */
};

/**
 * Registry key type (override Microsoft HKEY type)
 */
typedef void * registry68_key_t;

FILE68_API
/**
 * Check for registry support.
 *
 * @retval  0  registry not supported
 * @retval  1  registry supported
 */
int registry68_support(void);


FILE68_API
/**
 * Get key handler for a registry root type.
 *
 * @param  rootkey  One of the M$ registry key-type/root-key/entry-point.
 * @return key handler
 * @retval  0  error (key invalid)
 */
registry68_key_t registry68_rootkey(enum registry68_key_e rootkey);

FILE68_API
/**
 * Open a named hierarchic key.
 *
 * @param hkey     Opened key handle or one of reserved registry key handles.
 * @param kname    Hierarchic key name. Slash '/' character is interpreted
 *                 as sub-key separator.
 * @param mode     bit#0:read bit#1:write
 *
 * @return registry key handle
 * @retval registry68_key_e::REGISTRY68_INK on error
 */
registry68_key_t registry68_open(registry68_key_t hkey,
                                 const char *kname, int mode);

FILE68_API
/**
 * Get a string value of a named hierarchic string key.
 *
 * @param hkey     Opened key handle or one of reserved registry key handles.
 * @param kname    Hierarchic key name. Slash '/' character is interpreted
 *                 as sub-key separator.
 * @param kdata    Returned string storage location
 * @param kdatasz  Maximum size of kdata buffer.
 *
 * @return error status
 * @retval 0  Success
 * @retval -1 Error
 */
int registry68_gets(registry68_key_t hkey, const char *kname,
                    char *kdata, int kdatasz);

FILE68_API
/**
 * Get integer value of a named hierarchic string key.
 *
 * @param hkey    Opened key handle or one of reserved registry key handles.
 * @param kname   Hierarchic key name. Slash '/' character is interpreted
 *                as sub-key separator.
 * @param kdata   Pointer to returned int.
 *
 * @return error status
 * @retval 0  Success
 * @retval -1 Error
 */
int registry68_geti(registry68_key_t hkey, const char *kname, int * kdata);


FILE68_API
/**
 * Put a string value of a named hierarchic string key.
 *
 * @param hkey     Opened key handle or one of reserved registry key handles.
 * @param kname    Hierarchic key name. Slash '/' character is interpreted
 *                 as sub-key separator.
 * @param kdata    Nul-terminated string.
 *
 * @return error status
 * @retval 0  Success
 * @retval -1 Error
 */
int registry68_puts(registry68_key_t hkey, const char * kname,
                    const char * kdata);

FILE68_API
/**
 * Put integer value of a named hierarchic string key.
 *
 * @param hkey    Opened key handle or one of reserved registry key handles.
 * @param kname   Hierarchic key name. Slash '/' character is interpreted
 *                as sub-key separator.
 * @param kdata   Integer value.
 *
 * @return error status
 * @retval 0  Success
 * @retval -1 Error
 */
int registry68_puti(registry68_key_t hkey, const char * kname, int kdata);


/**
 * @}
 */

#endif
