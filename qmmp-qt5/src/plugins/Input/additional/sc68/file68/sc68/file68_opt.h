/**
 * @ingroup  lib_file68
 * @file     sc68/file68_opt.h
 * @brief    Command line option manipulation header.
 * @author   Benjamin Gerard
 * @date     2009-02-04
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_OPT_H
#define FILE68_OPT_H

#ifndef FILE68_API
# include "file68_api.h"
#endif

/**
 * @defgroup  lib_file68_opt  Options manipulation
 * @ingroup   lib_file68
 *
 *   Provides command line options manipulation functions.
 *
 * @{
 */

/**
 * Option value types.
 */
enum option68_type_e {
  opt68_BOL = 0,                        /**< Value type boolean. */
  opt68_STR,                            /**< Value type string.  */
  opt68_INT,                            /**< Value type integer. */
  opt68_ENU,                            /**< Value type enum.    */
  opt68_TYP                             /**< Number of types.    */
};

/**
 * Option value origin (sorted by priority).
 */
enum option68_org_e {
  opt68_UDF  = 0,            /**< Option not defined (set).      */
  opt68_CFG,                 /**< Option set by config.          */
  opt68_ENV,                 /**< Option set by environment var. */
  opt68_CLI,                 /**< Option set by command line.    */
  opt68_APP,                 /**< Option set by application.     */
  opt68_ORG                  /**< Number of origin.              */
};

/**
 * Option set polocies.
 */
enum option68_set_e {
  opt68_NEVER  = 0,    /**< Never set.                              */
  opt68_ALWAYS,        /**< Always set.                             */
  opt68_NOTSET,        /**< If not already set.                     */
  opt68_ISSET,         /**< If is already set.                      */
  opt68_PRIO,          /**< If origin priority is higher or same .  */
};

/**
 * Option type.
 */
typedef struct option68_s option68_t;

/**
 * option value.
 */
typedef union {
  const char * str;                /**< Value for string argument.  */
  int          num;                /**< Value for integer argument. */
} value68_t;

/**
 * Options help display function.
 */
typedef void (*option68_help_t)(void * cookie,
                                const char * option,
                                const char * envvar,
                                const char * values,
                                const char * desc);

/**
 * Callback on change value.
 *
 * @retval  0  Accept new value
 */
typedef int (*option68_cb_t)(const option68_t *, value68_t *);

/**
 * Macro to declare a generic option.
 */
#define OPT68(PREFIX,NAME,CAT,DESC,SET,NSET,MIN,MAX,TYPE,SAVE,ONCHG)    \
  { PREFIX,NAME,CAT,DESC,ONCHG,MIN,MAX,SET,NSET,TYPE,SAVE,0,opt68_UDF,{0} }

/**
 * Macro to declare a boolean option
 */
#define OPT68_BOOL(PREFIX,NAME,CAT,DESC,SAVE,ONCHG)             \
  OPT68(PREFIX,NAME,CAT,DESC,0,0,-1,0,opt68_BOL,SAVE,ONCHG)

/**
 * Macro to declare a simple integer option.
 */
#define OPT68_INTE(PREFIX,NAME,CAT,DESC,SAVE,ONCHG)             \
  OPT68(PREFIX,NAME,CAT,DESC,0,0,0,0,opt68_INT,SAVE,ONCHG)

/**
 * Macro to declare a integer enum option.
 */
#define OPT68_ENUM(PREFIX,NAME,CAT,DESC,SET,NSET,SAVE,ONCHG)            \
  OPT68(PREFIX,NAME,CAT,DESC,SET,NSET,0,(NSET)-1,opt68_ENU,SAVE,ONCHG)

/**
 * Macro to declare a simple string option.
 */
#define OPT68_STRG(PREFIX,NAME,CAT,DESC,SAVE,ONCHG)             \
  OPT68(PREFIX,NAME,CAT,DESC,0,0,0,0,opt68_STR,SAVE,ONCHG)

/**
 * Macro to declare an integer option with a set of values.
 */
#define OPT68_ISET(PREFIX,NAME,CAT,DESC,SET,NSET,SAVE,ONCHG)            \
    OPT68(PREFIX,NAME,CAT,DESC,SET,NSET,0,0,opt68_INT,SAVE,ONCHG)

/**
 * Macro to declare an string option with a set of values.
 */
#define OPT68_SSET(PREFIX,NAME,CAT,DESC,SET,NSET,SAVE,ONCHG)            \
  OPT68(PREFIX,NAME,CAT,DESC,SET,NSET,0,0,opt68_STR,SAVE,ONCHG)

/**
 * Macro to declare an integer option within a range.
 */
#define OPT68_IRNG(PREFIX,NAME,CAT,DESC,MIN,MAX,SAVE,ONCHG)             \
    OPT68(PREFIX,NAME,CAT,DESC,0,0,MIN,MAX,opt68_INT,SAVE,ONCHG)

/**
 * Command line option description and parsing info.
 */
struct option68_s {
  const char    * prefix;      /**< Key prefix.                     */
  const char    * name;        /**< Key name (bare).                */
  const char    * cat;         /**< Category name.                  */
  const char    * desc;        /**< Short description.              */

  option68_cb_t   onchange;    /**< Call on value change.           */

  int             min;         /**< Mininal for integer values.     */
  int             max;         /**< Maximal for integer values.     */

  const void    * set;         /**< Set of acceptable values.       */

  unsigned int    sets : 5;    /**< Size of set                     */
  unsigned int    type : 2;    /**< @see option68_type_e.           */
  unsigned int    save : 1;    /**< Save in config.                 */
  unsigned int    hide : 1;    /**< Hide from user.                 */
  unsigned int    org  : 3;    /**< @see option68_org_e.            */
  value68_t       val;         /**< Option value.                   */

  int             prefix_len;  /**< length of option68_t::prefix.   */
  int             name_len;    /**< length of option68_t::name.     */
  option68_t    * next;        /**< Chain to next option.           */
};

FILE68_API
/**
 * Print defined options.
 *
 * @param  cookie  User data used as 1st argument for fct
 * @param  fct     Function call for each selected option
 * @param flags    filter selection. At the moment only bit#0 is
 *                 used to unhide options. Other bits are reserved
 *                 for future use and must be clear.
 */
void option68_help(void * cookie, option68_help_t fct, int flags);


FILE68_API
/**
 * Append option definitions.
 *
 * @param  options  Array of options
 * @param  n        Array size
 *
 * @note   Options are not internally copied.
 */
int option68_append(option68_t * options, int n);

FILE68_API
/**
 * Parse command line options.
 *
 * @param  argc     argument count
 * @param  argv     arguments
 * @retval remaining argc
 */
int option68_parse(int argc, char ** argv);

FILE68_API
/**
 * Get option type.
 *
 * @param   opt      option
 * @return  one of option68_e value
 * @retval  option68_ERR on error
 */
int option68_type(const option68_t * opt);

FILE68_API
/**
 * Enumerate options.
 *
 * @param   idx     options index (0-based).
 * @return  option
 * @retval  0       not found
 */
option68_t * option68_enum(int idx);

FILE68_API
/**
 * Get option by name.
 *
 * @param   key     argument count
 * @param   set     set policy (@see option68_set_e)
 * @return  option
 * @retval  0        not found
 */
option68_t * option68_get(const char * key, int set);

FILE68_API
/**
 * Test if option has been set.
 *
 * @param   opt  option to test
 * @retval  1    option has been set
 * @retval  0    option has not been set
 */
int option68_isset(const option68_t * opt);

FILE68_API
/**
 * Set option.
 *
 * @param   opt  option to set
 * @return  option origin (@see option68_org_e)
 */
int option68_org(const option68_t * opt);

FILE68_API
/**
 * Set option (integer and boolean only).
 *
 * @param   opt  option to set
 * @param   val  integer value
 * @param   set  set policy (@see option68_set_e)
 * @param   org  origin (@see option68_org_e)
 * @retval  0    on success
 * @retval -1    on failure
 */
int option68_iset(option68_t * opt, int val, int set, int org);

FILE68_API
/**
 * Set option.
 *
 * @param   opt  option to set
 * @param   str  integer value
 * @param   set  set policy (@see option68_set_e)
 * @param   org  origin (@see option68_org_e)
 * @retval  0    on success
 * @retval -1    on failure
 */
int option68_set(option68_t * opt, const char * str, int set, int org);

FILE68_API
/**
 * Unset option.
 *
 * @param   opt  option to unset
 * @retval  0    on success
 * @retval -1    on failure
 */
int option68_unset(option68_t * opt);


FILE68_API
/**
 * Get associate environment variable value.
 *
 * @param   opt  option
 * @param   set  set policy (@see option68_set_e)
 * @retval  0    on error (or envvar does not exist)
 */
const char * option68_getenv(option68_t * opt, int set);

/**
 * @}
 */

#endif
