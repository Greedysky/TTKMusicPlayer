/**
 * @ingroup   lib_sc68
 * @file      sc68/sc68.h
 * @brief     sc68 API header.
 * @author    Benjamin Gerard
 * @date      2003/08/07
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SC68_SC68_H
#define SC68_SC68_H

#ifndef SC68_EXTERN
# ifdef __cplusplus
#  define SC68_EXTERN extern "C"
# else
#  define SC68_EXTERN extern
# endif
#endif

#ifndef SC68_API
/* Building */
# ifdef SC68_EXPORT
#  if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC)
#   define SC68_API __declspec(dllexport)
#  elif defined(HAVE_VISIBILITY)
#   define SC68_API SC68_EXTERN __attribute__ ((visibility("default")))
#  else
#   define SC68_API SC68_EXTERN
#  endif
/* Using */
# else
#  if defined(SC68_DLL)
#   define SC68_API __declspec(dllimport)
#  else
#   define SC68_API SC68_EXTERN
#  endif
# endif
#endif

/**
 * @defgroup  lib_sc68  sc68 library
 * @ingroup   sc68_dev_lib
 *
 * This API provides functions to use sc68 libraries efficiently.
 *
 * @par Multi-threading concern
 *
 * sc68 should now be thread safe. At least concerning the functions
 * with a sc68_t parameter. As no torture test has been run against
 * it this should be considered as quiet experimental.
 *
 * @par Quick start
 *
 * Here is a quiet minimalist piece of code to use basics of sc68
 * library:
 *
 * @code
 * #include <sc68/sc68.h>
 * #include <stdio.h>
 *
 * sc68_t * sc68 = 0;
 * char buffer[512*4];
 * int  n;
 *
 * // Initialise the library.
 * // You should consider using sc68_init_t struct.
 * if (sc68_init(0)) goto error;
 *
 * // Create an emulator instance.
 * // You should consider using sc68_create_t struct.
 * if (sc68 = sc68_create(0), !sc68) goto error;
 *
 * // Load an sc68 file.
 * if (sc68_load_uri(sc68, fname))
 *   goto error;
 *
 * // Set track and loop (optionnal).
 * sc68_play(sc68, 1, 1);
 *
 * // Loop until the end of disk. You can use SC68_LOOP to wait the end
 * // of the track. Notice that SC68_ERROR set all bits and make the loop
 * // break too.
 * while ( (n=sizeof(buffer)>>2), ! (sc68_process(sc68, buffer, &n) & SC68_END))
 *   fwrite(buffer, 1, n, stdout); // copy PCM to stdout
 *
 * // Close eject the current disk.
 * sc68_close(sc68);
 *
 * error:
 *
 *  // Destroy sc68 instance.
 *  sc68_destroy(sc68);
 *
 *  // Shutdown sc68 library.
 *  sc68_shutdown();
 *
 * @endcode
 *
 * @{
 */

#ifndef SC68_API
/**
 * sc68 exported symbol.
 *
 * Define special atribut for exported symbol.
 *
 * - empty: static or classic .so library
 * - __attribute__ (visibility("default"))): gcc support visibility.
 * - __declspec(dllexport): creating a win32 DLL.
 * - __declspec(dllimport): using a win32 DLL.
 */
#define SC68_API extern
#endif

/** API information. */
typedef struct _sc68_s sc68_t;

/** API disk. */
typedef void * sc68_disk_t;

/**
 * Message function.
 *
 * @param  cat   @ref msg68_cat_e "message category"
 * @param  sc68  sc68 instance
 * @param  fmt   printf like format string
 * @param  list  variable argument list
 * */
typedef void (*sc68_msg_t)();

/**
 * API initialization parameters.
 *
 *   The sc68_init_t must be properly filled before calling the
 *   sc68_init() function.
 *
 */
typedef struct {

  /** message handler. */
  sc68_msg_t msg_handler;

  /** debug mask (set bit to clear in msg68). */
  int debug_clr_mask;

  /** debug mask (set bit to set in msg68). */
  int debug_set_mask;

  /** number of arguments in command line (modified) */
  int argc;

  /** command line arguments */
  char ** argv;

  /** structure for init flags. */
  struct init_flags {
    /** set to avoid skip loading on init. */
    unsigned int no_load_config : 1;
    /** set to skip config saving on shutdown. */
    unsigned int no_save_config : 1;
  } flags;     /**< init flags (0 is always the default behaviour). */

} sc68_init_t;

/**
 * Instance creation parameters.
 */
typedef struct {
  /** sampling rate in hz (non 0 value overrides config default).
   *  The real used value is set by sc68_create() function.
   */
  unsigned int sampling_rate;

  /** short name. */
  const char * name;

  /** 68k memory size (2^log2mem). */
  int log2mem;

  /** Run  68k emulator in debug mode.
   *  bit #0: enable hardware trace and exception handler.
   *  bit #1: enable rsp server (gdb)
   */
  int emu68_debug;

  /** User private data. */
  void * cookie;

} sc68_create_t;

/**
 * metatag struct.
 * @warning  MUST match the tag68_t struct defined in tag68.h.
 */
typedef struct {
  char * key;                     /**< Tag name.  */
  char * val;                     /**< Tag value. */
} sc68_tag_t;

/**
 * Music common information.
 */
typedef struct {
  unsigned track;       /**< Current/Default track [1..99].     */
  unsigned time_ms;     /**< Track/Disk duration.               */
  char     time[12];    /**< Time in format TT MM:SS.           */
  unsigned ym:1;        /**< Music uses YM-2149 (ST).           */
  unsigned ste:1;       /**< Music uses STE specific hardware.  */
  unsigned amiga:1;     /**< Music uses Paula Amiga hardware.   */
  unsigned asid:1;      /**< aSid caps.                         */
  const char * hw;      /**< Hardware name.                     */
  int          tags;    /**< Number of tags.                    */
  sc68_tag_t * tag;     /**< Tags array                         */
} sc68_cinfo_t;

/**
 * Music information.
 */
typedef struct {
  int tracks;            /**< number of tracks [1..99].          */

  unsigned int addr;     /**< Load address.                      */
  unsigned int rate;     /**< Replay rate.                       */
  char * replay;         /**< replay name.                       */

  sc68_cinfo_t dsk;      /**< disk info.                         */
  sc68_cinfo_t trk;      /**< track info (MUST BE behind dsk).   */

  /** Predefined tags
   *
   *  @warning album must be first and _lasttag last.
   */
  char * album;          /**< Points to disk's title tag.        */
  char * title;          /**< Points to track's title tag.       */
  char * artist;         /**< Points to track's artist tag.      */
  char * format;         /**< Points to disk's format tag.       */
  char * genre;          /**< Points to track's genre tag.       */
  char * year;           /**< Points to "year" tag.              */
  char * ripper;         /**< Points to "ripper" tag.            */
  char * converter;      /**< Points to "converter" tag.         */
  char * _lasttag;       /**< Last predefined tag.               */

} sc68_music_info_t;

/** Alias type for sc68_music_info_t. */
typedef sc68_music_info_t sc68_minfo_t;

/**
 * Flags returned eturn codeby the sc68_process() functions.
 */
enum sc68_code_e {
  SC68_IDLE   = (1<<0),   /**< Set if no emulation has been runned. */
  SC68_CHANGE = (1<<1),   /**< Set if track has changed.            */
  SC68_LOOP   = (1<<2),   /**< Set if track has looped.             */
  SC68_END    = (1<<3),   /**< Set if track(s) has ended.           */
  SC68_SEEK   = (1<<4),   /**< Set if currently seeking.            */
  /* */
  SC68_OK     =  0,       /**< Success return code.                 */
  SC68_ERROR  = ~0        /**< Failure return code (all bits set).  */
};

/**
 * sc68 sampling rate values in hertz (hz).
 */
enum sc68_spr_e {
  SC68_SPR_QUERY   = -1, /**< Query default or current sampling rate. */
  SC68_SPR_DEFAULT =  0  /**< Default sampling rate.                  */
};

/**
 * sc68_play() function parameters.
 */
enum sc68_play_e {
  SC68_DSK_TRACK  =  0,  /**< special value for addressing disk. */
  SC68_DEF_LOOP   =  0,  /**< loop value for the default loop.   */
  SC68_INF_LOOP   = -1,  /**< loop value for infinite loop.      */
  SC68_DEF_TRACK  = -1,  /**< track value for the default track. */
  SC68_CUR_TRACK  = -2,  /**< track value for the current track. */
  SC68_CUR_LOOP   = -2,  /**< track value for the current track. */
};

/**
 * PCM formats.
 */
enum sc68_pcm_e {
  SC68_PCM_S16 = 1,               /**< Native 16bit signed.  */
  SC68_PCM_F32 = 2                /**< native 32bit float.   */
};

/**
 * aSIDifier flags.
 */
enum sc68_asid_e {
  SC68_ASID_OFF   = 0,
  SC68_ASID_ON    = 1,        /**< aSIDifier enable/disable.        */
  SC68_ASID_FORCE = 2,        /**< Force aSIDify unsafe track.      */
  SC68_ASID_NO_A  = 4,        /**< Disable ASIDifier for channel A. */
  SC68_ASID_NO_B  = 8,        /**< Disable ASIDifier for channel B. */
  SC68_ASID_NO_C  = 16,       /**< Disable ASIDifier for channel C. */
};

/**
 * sc68_cntl() op parameter.
 */
enum sc68_cntl_e {
  SC68_NOP = 0,      /**< No OPeration.             */
  SC68_GET_LAST,     /**< Get the last command #.   */
  SC68_GET_NAME,     /**< Get sc68 instance name.   */
  SC68_GET_TRACKS,   /**< Get number of tracks.     */
  SC68_GET_TRACK,    /**< Get current track.        */
  SC68_GET_DEFTRK,   /**< Get disk default track.   */
  SC68_GET_LOOPS,    /**< Number of loops to play.  */
  SC68_GET_LOOP,     /**< Get current loop.         */
  SC68_GET_DISK,     /**< Get a pointer to disk.    */
  SC68_GET_SPR,      /**< Get sampling rate.        */
  SC68_SET_SPR,      /**< Set sampling rate.        */
  SC68_GET_LEN,      /**< Get current track length. */
  SC68_GET_TRKLEN,   /**< Get some track length.    */
  SC68_GET_DSKLEN,   /**< Get disk length.          */
  SC68_GET_ORG,      /**< Get current track origin. */
  SC68_GET_TRKORG,   /**< Get some track origin.    */
  SC68_GET_POS,      /**< Get track position (ms).  */
  SC68_GET_DSKPOS,   /**< Get disk position (ms).   */
  SC68_GET_PLAYPOS,  /**< Get play position (ms).   */
  SC68_SET_POS,      /**< Set position.             */
  SC68_GET_PCM,      /**< Get PCM format            */
  SC68_SET_PCM,      /**< Set PCM format            */
  SC68_CAN_ASID,     /**< Get aSID caps             */
  SC68_GET_ASID,     /**< Get aSID mode             */
  SC68_SET_ASID,     /**< Set aSID mode             */
  SC68_GET_COOKIE,   /**< Get cookie (user data).   */
  SC68_SET_COOKIE,   /**< Set cookie (user data).   */
  SC68_EMULATORS,    /**< Get emulators array.      */
  SC68_CONFIG_LOAD,  /**< Load and apply config.    */
  SC68_CONFIG_SAVE,  /**< Save config               */
  SC68_ENUM_OPT,     /**< Enumerate options.        */
  SC68_GET_OPT,      /**< Get options.              */
  SC68_SET_OPT_STR,  /**< Set options (string).     */
  SC68_SET_OPT_INT,  /**< Set options (integer).    */
  SC68_DIAL,         /**< Run a dialog.             */

  /* Always last */
  SC68_CNTL_LAST     /**< Last command #.           */
};

/**
 * @name API control functions.
 * @{
 */

SC68_API
/**
 * Get version number
 */
int sc68_version(void);

SC68_API
/**
 * Get version string
 *
 */
const char * sc68_versionstr(void);

SC68_API
/**
 * Initialise sc68 API.
 *
 * @param   init  Initialization parameters.
 *
 * @return  error-code
 * @retval   0  Success
 * @retval  -1  Error
 *
 * @see sc68_shutdown()
 */
int sc68_init(sc68_init_t * init);

SC68_API
/**
 * Destroy sc68 API.
 *
 */
void sc68_shutdown(void);

SC68_API
/**
 * Create sc68 instance.
 *
 * @param   create  Creation parameters.
 *
 * @return  Pointer to created sc68 instance.
 * @retval  0  Error.
 *
 * @see sc68_destroy()
 */
sc68_t * sc68_create(sc68_create_t * create);

SC68_API
/**
 * Destroy sc68 instance.
 *
 * @param   sc68  sc68 instance to destroy.
 *
 * @see sc68_create()
 * @note  It is safe to call with null api.
 */
void sc68_destroy(sc68_t * sc68);

SC68_API
/**
 * Generic control function.
 *
 * @param  sc68  sc68 instance or 0 for general control.
 * @param  op    @ref sc68_dial_e operation
 *
 * @retval  0  success
 * @retval -1  failure
 */
int sc68_cntl(sc68_t * sc68, int op, ...);

SC68_API
/**
 * Return last error message.
 *
 * @param   sc68   sc68 instance or 0 for library messages.
 * @return  Error string.
 * @retval  0      No stacked error message.
 */
const char * sc68_error(sc68_t * sc68);

/**
 * @}
 */


/**
 * @name Music control functions.
 * @{
 */

SC68_API
/**
 * Fill PCM buffer.
 *
 *   The sc68_process() function fills the PCM buffer with the current
 *   music data. If the current track is finished and it is not the last
 *   the next one is automatically loaded. The function returns status
 *   value that report events that have occured during this pass.
 *
 * @param  sc68  sc68 instance.
 * @param  buf   PCM buffer (must be at least 4*n bytes).
 * @param  n     Pointer to number of PCM sample to fill.
 *
 * @return Process status
 *
 */
int sc68_process(sc68_t * sc68, void * buf, int * n);

SC68_API
/**
 * Set/Get current track.
 *
 *  The sc68_play() function get or set current track.
 *
 *  If track == SC68_CUR_TRACK (-1) and loop != SC68_CUR_LOOP the
 *  function returns the current track or 0 if none.
 *
 *  If track == SC68_CUR_TRACK (-1) and loop == SC68_CUR_LOOP the
 *  returns the current loop counter.
 *
 *  Forced track and loop values while disk loading (see sc68_uri)
 *  are prior to the track and loop parameters.
 *
 *  If track == SC68_DEF_TRACK, the disk default trackis used unless
 *  config "force-track" is non zero.
 *
 *  If loop == SC68_LOOP_DEF, the default number of loop is used
 *  unless config "force-loop" is non zero.
 *
 *  To avoid multi-threading issues the track is not changed directly
 *  but a change-track event is posted. This event will be processed
 *  at the next call to the sc68_process() function.
 *
 * @param  sc68   sc68 instance.
 * @param  track  track number (see sc68_play_e)
 * @param  loop   number of loop (see sc68_play_e)
 *
 * @return error code or track number.
 * @retval 0  Success or no current track or loop counter
 * @retval >0 Current track or loop counter
 * @retval -1 Failure.
 *
 * @see sc68_play_e
 * @see sc68_uri
 *
 */
int sc68_play(sc68_t * sc68, int track, int loop);

SC68_API
/**
 * Stop playing.
 *
 *    The sc68_stop() function stop current playing track. Like the
 *    sc68_play() function the sc68_stop() function does not really
 *    stop the music but send a stop-event that will be processed by
 *    the next call to sc68_process() function.
 *
 * @param  sc68    sc68 instance.
 * @return error code
 * @retval 0  Success
 * @retval -1 Failure
 */
int sc68_stop(sc68_t * sc68);

SC68_API
/**
 * Get disk/track information.
 *
 * @param  info  pointer to a sc68_music_info_t struct to be fill.
 * @param  sc68  sc68 instance
 * @param  track track number
 * @param  disk  disk to get information from (0 for current disk).
 *
 * @return error code
 * @retval  0  Success.
 * @retval -1  Failure.
 *
 */
int sc68_music_info(sc68_t * sc68, sc68_music_info_t * info,
                    int track, sc68_disk_t disk);

SC68_API
/**
 * Get disk/track meta tag.
 *
 * @param  sc68  sc68 instance
 * @param  tag   tag to look for (tag68_t::key must be set).
 * @param  track track number (0:disk tags, -1:current/default).
 * @param  disk  disk to get information from (0 for current disk).
 *
 * @return error code
 * @retval  0  Success.
 * @retval -1  Failure.
 *
 *
 * @see file68_tag_get()
 */
int sc68_tag_get(sc68_t * sc68, sc68_tag_t * tag,
                 int track, sc68_disk_t disk);


SC68_API
/**
 * Get a copy of disk/track meta tag.
 *
 * @param  sc68  sc68 instance
 * @param  key   tag to look for (tag68_t::key must be set).
 * @param  track track number (0:disk tags, -1:current/default).
 * @param  disk  disk to get information from (0 for current disk).
 *
 * @return dynamic copy of a metatag
 * @retval  0  Failure.
 *
 * @see file68_tag() for more infornmation.
 */
char * sc68_tag(sc68_t * sc68, const char * key, int track, sc68_disk_t disk);

SC68_API
/**
 * Get disk/track meta tag.
 *
 * @param  sc68  sc68 instance
 * @param  tag   result tag.
 * @param  track track number (0:disk tags, -1:current/default).
 * @param  idx   index of the metatag.
 * @param  disk  disk to get information from (0 for current disk).
 *
 * @return error code
 * @retval  0  Success.
 * @retval -1  Failure.
 *
 */
int sc68_tag_enum(sc68_t * sc68, sc68_tag_t * tag,
                  int track, int idx, sc68_disk_t disk);

/**
 * @}
 */


/**
 * @name File functions.
 * @{
 */

SC68_API
/**
 * Get official sc68 mime-type.
 * @retval  "audio/x-sc68"
 */
const char * sc68_mimetype(void);

#ifndef HAVE_VFS68
typedef struct _vfs68_t vfs68_t; /**< normally defined in sc68/file68_vfs.h */
#define HAVE_VFS68 1
#endif

SC68_API
/**
 * Create a VFS from uri.
 *
 * @param  uri     URI or path
 * @param  mode    open mode
 * @param  argc    number of supplemental arguments
 *
 * @return VFS
 * @retval 0 on error
 */
vfs68_t * sc68_vfs(const char * uri, int mode, int argc, ...);

SC68_API
/**
 * Checks if this uri is an sc68 file.
 */
int sc68_is_our_uri(const char * uri,
                    const char *exts, int * is_remote);

/**
 * Load an sc68 disk for playing.
 */
SC68_API
int sc68_load(sc68_t * sc68, vfs68_t * is);
SC68_API
int sc68_load_uri(sc68_t * sc68, const char * uri);
SC68_API
int sc68_load_mem(sc68_t * sc68, const void * buffer, int len);

/**
 * Load an sc68 disk outside the API.
 *
 * @note Free it with sc68_disk_free() function.
 */
SC68_API
sc68_disk_t sc68_load_disk(vfs68_t * is);
SC68_API
sc68_disk_t sc68_load_disk_uri(const char * uri);
SC68_API
sc68_disk_t sc68_disk_load_mem(const void * buffer, int len);
SC68_API
void sc68_disk_free(sc68_disk_t disk);

SC68_API
/**
 * Change current disk.
 *
 * @param  sc68   sc68 instance
 * @param  disk  New disk (0 does a sc68_close())
 *
 * @return error code
 * @retval 0  Success, disk has been loaded.
 * @retval -1 Failure, no disk has been loaded (occurs if disk was 0).
 *
 * @note    Can be safely call with null sc68.
 * @warning After sc68_open() failure, the disk has been freed.
 * @warning Beware not to use disk information after sc68_close() call
 *         because the disk should have been destroyed.
 */
int sc68_open(sc68_t * sc68, sc68_disk_t disk);

SC68_API
/**
 * Close current disk.
 *
 * @param  sc68  sc68 instance
 *
 * @note   Can be safely call with null sc68 or if no disk has been loaded.
 */
void sc68_close(sc68_t * sc68);

/**
 * @}
 */

/**
 * @name Dialog helper.
 *
 *   The dialog helper is meant to help handling common dialogs used
 *   by various sc68 players. It provides a system free communication
 *   method that allow to create dialogs that does not need to link
 *   with sc68 shared library.
 *
 * @{
 */

/**
 * Operation (3rd parameter) for the sc68_dial_f handler function.
 */
enum sc68_dial_e {
  SC68_DIAL_CALL,                       /**< Call special function. */

  SC68_DIAL_GETI,                       /**< Get integer value.     */
  SC68_DIAL_SETI,                       /**< Set integer value.     */
  SC68_DIAL_GETS,                       /**< Get string value.      */
  SC68_DIAL_SETS,                       /**< Set string value.      */

  SC68_DIAL_MIN,                        /**< Get minimal value.     */
  SC68_DIAL_MAX,                        /**< Get maximal value.     */
  SC68_DIAL_CNT,                        /**< Get enum count.        */

  SC68_DIAL_ENUM,                       /**< Get enum values.       */
  SC68_DIAL_DESC,                       /**< Get a description.     */
  SC68_DIAL_CAT,                        /**< Get a category.        */
};

#define SC68_DIAL_NEW    "new"      /**< key to create the dialog.  */
#define SC68_DIAL_KILL   "kill"     /**< key to destroy the dialog. */
#define SC68_DIAL_HELLO  "hello"    /**< key to start a connection. */
#define SC68_DIAL_WAIT   "wait"     /**< key to wait for dialog.    */

/**
 * Union used by the sc68_dial_f handler (4th parameter).
 */
union sc68_dialval_u {
  int i;                                /**< Integer. */
  const char * s;                       /**< String.  */
};

/**
 * Typedef for the sc68_dialval_u union.
 */
typedef union sc68_dialval_u sc68_dialval_t;

/**
 *  Dialog helper control function.
 *
 *  @param  cookie
 *  @param  key
 *  @param  op
 *  @param  val
 *
 *  @return error code
 *  @retval 0 on success
 *  @retval 1 not handled
 *  @retval -1 on error (handled but something got wrong)
 */
typedef int (*sc68_dial_f)(void *, const char *, int, sc68_dialval_t *);

/**
 * @}
 */


/**
 * @}
 */

#endif
