/**
 * @ingroup  lib_file68
 * @file     sc68/file68.h
 * @author   Benjamin Gerard
 * @date     1998-09-03
 * @brief    Music file header.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_H
#define FILE68_H

#ifndef FILE68_API
# include "file68_api.h"
#endif
#include "file68_vfs.h"
#include "file68_tag.h"

/**
 * @defgroup  lib_file68_fil  Music file manipulation
 * @ingroup   lib_file68
 *
 *  Provides various functions for sc68 file manipulation.
 *
 * @{
 */

/**
 * SC68 (not IANA) official mime-type.
 * @see file68_idstr
 */
#define SC68_MIMETYPE "audio/x-sc68"

/**
 * SC68 file identification string definition.
 * @see file68_identifier()
 */
#define SC68_IDSTR "SC68 Music-file / (c) (BeN)jamin Gerard / SasHipA-Dev  "

/**
 * SC68 file identification string definition V2.
 * @see file68_identifier()
 */
#define SC68_IDSTR_V2 "SC68 M2"

/* Old V2 never use except for experiental files ...
#define SC68_IDSTR_V2 "SC68\0\002\377\251\337\353\321"
*/

#define SC68_NOFILENAME "n/a" /**< SC68 unknown filename or author. */

/* Only for development purpose. Do not change that. */
#ifndef SC68_DEFADDR
# define SC68_DEFADDR 0x10000
#endif

enum {
  SC68_DISK_ID   = (('d' << 24) | ('i' << 16) | ('s' << 8) | 'k'),
  SC68_LOADADDR  = SC68_DEFADDR, /**< Default load address in 68K memory. */
  SC68_MAX_TRACK = 63,           /**< Maximum track per disk. */
};

/**
 * @name  Features flag definitions for file flags (portability). 
 * @{
 */
enum  {
  SC68_PSG       = 1 << 0,   /**< YM-2149 (PSG).                  */
  SC68_DMA       = 1 << 1,   /**< STE DMA.                        */
  SC68_AGA       = 1 << 2,   /**< AMIGA/PAULA.                    */
  SC68_XTD       = 1 << 3,   /**< Has extra info (following).     */
  SC68_LMC       = 1 << 4,   /**< STE LMC-1992.                   */

  SC68_MFP_BIT   = 5,        /**< MFP timers bit position.        */
  SC68_MFP_TA    = 1 << 5,   /**< Timer-A used.                   */
  SC68_MFP_TB    = 1 << 6,   /**< Timer-B used.                   */
  SC68_MFP_TC    = 1 << 7,   /**< Timer-C used.                   */
  SC68_MFP_TD    = 1 << 8,   /**< Timer-D used.                   */

  SC68_HBL       = 1 << 9,   /**< HBL interrupt.                  */
  SC68_BLT       = 1 << 10,  /**< STE blitter.                    */
  SC68_DSP       = 1 << 11,  /**< Falcon DSP.                     */

};
/**
 * @}
 */

/**
 * @}
 */

/**
 * Hardware and features flags.
 */
typedef unsigned int hwflags68_t;

/* typedef union { */
/*   struct { */
/*     unsigned psg:1;       /\**< Require YM-2149 (ST PSG)       *\/ */
/*     unsigned dma:1;       /\**< Require STE DMA sound.         *\/ */
/*     unsigned aga:1;       /\**< Require Amiga/paula hardware.  *\/ */
/*     unsigned xtd:1;       /\**< Extended info available.       *\/ */
/*     unsigned lmc:1;       /\**< xtd: Require LMC-1992.         *\/ */
/*     unsigned mfp:4;       /\**< xtd: MFP timers (1 bit/timer). *\/ */
/*     unsigned hbl:1;       /\**< xtd: HBL interrupt.            *\/ */
/*     unsigned blt:1;       /\**< xtd: STE/Mega-ST blitter.      *\/ */
/*     unsigned dsp:1;       /\**< xtd: Falcon DSP.               *\/ */
/*   } bit;                  /\**< Flags bit field.               *\/ */
/*   unsigned clr;           /\**< All flags (clear only).        *\/ */
/* } hwflags68_t; */





/**
 * SC68 music (track) structure.
 */
typedef struct {
  unsigned int d0;       /**< D0 value to init this music.            */
  unsigned int a0;       /**< A0 Loading address. @see SC68_LOADADDR. */
  unsigned int frq;      /**< Frequency in Hz (default:50).           */

  unsigned int first_ms; /**< First loop duration in ms.              */
  unsigned int first_fr; /**< First loop duration in frames.          */
  unsigned int loops_ms; /**< Loop duration in ms (0:no loop).        */
  unsigned int loops_fr; /**< Loop duration in frames (0:no loop).    */
  int          loops;    /**< Default number of loop (-1:infinite).   */

  struct {
    unsigned   sfx  : 1; /**< Track is a sound-fx not a music.        */
    unsigned   pic  : 1; /**< Track is position independant code.     */
    unsigned   time : 1; /**< Track has time info.                    */
    unsigned   loop : 1; /**< Track has loop info.                    */
  } has;                 /**< Track flags.                            */

  char        *replay;   /**< External replay name.                   */
  hwflags68_t  hwflags;  /**< Hardware and features.                  */
  tagset68_t   tags;     /**< Meta data.                              */
  unsigned int datasz;   /**< data size in bytes.                     */
  char        *data;     /**< Music data.                             */
} music68_t;


/**
 * SC68 music disk structure.
 *
 *  The disk68_t structure is the memory representation for an SC68
 *  disk.  Each SC68 file could several music or tracks, in the limit
 *  of a maximum of 256 tracks per file. Each music is independant,
 *  but some information, including music data, could be inherit from
 *  previous track. In a general case, tracks are grouped by theme,
 *  that could be a demo or a game.
 *
 */
typedef struct {
  int          magic;       /**< Magic identifier is FILE68_MAGIC.       */
  int          def_mus;     /**< Preferred default music (default is 0). */
  int          nb_mus;      /**< Number of music track in file.          */
  unsigned int time_ms;     /**< Total time for all tracks in ms.        */
  hwflags68_t  hwflags;     /**< All tracks flags ORed.                  */
  int          hash;        /**< Caclulated hash (for sndh timedb).      */
  tagset68_t   tags;        /**< Meta tags for the disk (album)          */

  unsigned int force_track; /**< Forced track.                           */
  int          force_loops; /**< Forced loops (>0, -1:infinite).         */
  unsigned int force_ms;    /**< Forced time in ms.                      */

  music68_t    mus[SC68_MAX_TRACK]; /**< Information for each music.     */
  unsigned int datasz;      /**< data size in byte.                      */
  char        *data;        /**< points to data.                         */
  char         buffer[4];   /**< raw data. MUST be last member.          */
} disk68_t;

/**
 * @name  Meta tag functions.
 * @{
 */

FILE68_API
/**
 * Count and re-order metatags.
 *
 *  The file68_tag_count() function counts metatags and ensures no
 *  empty tag within that count.
 *
 * @param  mb     pointer to SC68 disk
 * @param  track  track number (0:disk)
 *
 * @retval -1     on error
 * @retval >=0    number of tag (in fact it should always be >= 3)
 */
int file68_tag_count(const disk68_t * mb, int track);

FILE68_API
/**
 * Enumerate metatags.
 *
 *  The file68_tag_enum() function enumerates defined metatags.
 *
 * @param  mb     pointer to SC68 disk
 * @param  track  track number (0:disk)
 * @param  idx    index of the metatag
 * @param  key    pointer to tag name
 * @param  val    pointer to tag value
 *
 * @retval -1     on error
 * @retval  0     on success
 */
int file68_tag_enum(const disk68_t * mb, int track, int idx,
                    const char ** key, const char ** val);

FILE68_API
/**
 * Get metatag from disk or track.
 *
 *  The file68_tag_get() function gets the value of a metatag.
 *
 * @param  mb     pointer to SC68 disk
 * @param  track  track number (0:disk)
 * @param  key    tag name
 *
 * @return tag value
 * @retval  0  tag is not set
 *
 * @see file68_tag_set()
 */
const char * file68_tag_get(const disk68_t * mb, int track,
                            const char * key);

FILE68_API
/**
 * Get a copy of a metatag from disk or track.
 *
 *  The file68_tag() function gets a copy of the value of a metatag.
 *
 *  Contrary to the file68_tag_get() function the file68_tag()
 *  function returns a wider range of tag. It can handle disk and
 *  track information that are avaiable in the disk68_t or music68_t
 *  structures.
 *
 *  List of sipplemental metatags:
 *
 * - "track" returns track number (01) or default track for disk
 *     query.
 * - "tracks" returns the number of tracks (01).
 *
 * @param  mb     pointer to SC68 disk
 * @param  track  track number (0:disk)
 * @param  key    tag name
 *
 * @return allocated copy of tag value
 * @retval 0  tag is not set or copy failed.
 *
 * @note  The caller have to free the returned string by itself.
 * @todo Implements the dynamic part and use it in mksc68 and info68.
 *
 * @see file68_tag_get()
 */
char * file68_tag(const disk68_t * mb, int track, const char * key);

/**
 * @}
 */


FILE68_API
/**
 * Get metatag from disk or track.
 *
 *  The file68_tag_set() function sets the value of a metatag.
 *
 * @param  mb     pointer to SC68 disk
 * @param  track  track number (0:disk)
 * @param  key    tag name
 * @param  val    tag value to set (0 to unset)
 *
 * @return tag value
 * @retval  0  tag is not set
 *
 * @see file68_tag_set()
 */
const char * file68_tag_set(disk68_t * mb, int track,
                            const char * key, const char * val);

FILE68_API
/**
 * Check if an URI is a standard sc68 one.
 *
 * @param  uri        URI to check.
 * @param  exts       extension list. (0 is default: ".sc68\0.sndh\0.snd\0").
 * @param  is_remote  fill with 0/1 if respectevely URI is a local/remote
 *                    file. May be 0.
 * @return  bool
 * @retval  0  not compatible sc68 file
 * @retval  1  sc68 compatible file
 *
 * @todo    Implements file68_is_our()
 * @warning Not implemented.
 */
int file68_is_our(const char * uri, const char * exts, int * is_remote);

/**
 * @}
 */


/**
 * @name  File load functions.
 * @{
 */

FILE68_API
/**
 * Load SC68 file from stream.
 *
 *  The file68_load() function allocates memory and loads an SC68
 *  file.  The function performs all necessary initializations in the
 *  returned disk68_t structure. A single buffer has been allocated
 *  including disk68_t structure followed by music data. It is user
 *  charge to free memory by calling file68_free() function.
 *
 * @param   is   input stream
 *
 * @return  pointer to allocated disk68_t disk structure
 * @retval  0  failure
 *
 * @see file68_save()
 */
disk68_t * file68_load(vfs68_t * is);

FILE68_API
/**
 * Load SC68 file.
 *
 * @param  uri      URI to load.
 *
 * @return  pointer to allocated disk68_t disk structure
 * @retval  0  failure
 */
disk68_t * file68_load_uri(const char * uri);

FILE68_API
/**
 * Load SC68 file mapped into memory buffer.
 *
 * @param  buffer   buffer address
 * @param  len      buffer length
 *
 * @return  pointer to allocated disk68_t disk structure
 * @retval  0  failure
 */
disk68_t * file68_load_mem(const void * buffer, int len);

/**
 * @}
 */


/**
 * @name  File save functions.
 * @{
 */

FILE68_API
/**
 * Save SC68 disk into stream.
 *
 * @param  os       output stream (must be seekable)
 * @param  mb       pointer to SC68 disk to save
 * @param  version  file version [0:default]
 * @param  gzip     gzip compression level [0:no-gzip, 1..9 or -1]
 *
 * @return error-code
 * @retval  0  success
 * @retval <0  failure
 *
 * @see file68_load()
 * @see file68_verify()
 * @see file68_diskname()
 */
int file68_save(vfs68_t * os, const disk68_t * mb,
                int version, int gzip);

FILE68_API
/**
 * Save SC68 disk into file.
 *
 * @param  uri      URI to save.
 * @param  mb       pointer to SC68 disk to save
 * @param  version  file version [0:default]
 * @param  gzip     gzip compression level [0:no-gzip, 1..9 or -1]
 */
int file68_save_uri(const char * uri, const disk68_t * mb,
                    int version, int gzip);

FILE68_API
/**
 * Save SC68 disk into memory buffer.
 *
 * @param  buffer   destination buffer.
 * @param  len      size of destination buffer.
 * @param  mb       pointer to SC68 disk to save
 * @param  version  file version [0:default]
 * @param  gzip     gzip compression level [0:no-gzip, 1..9 or -1]
 */
int file68_save_mem(const char * buffer, int len, const disk68_t * mb,
                    int version, int gzip);

/**
 * @}
 */


/**
 * @name  Memory allocation functions.
 * @{
 */

FILE68_API
/**
 * Create fresh new disk.
 *
 * @param  extra  extra bytes to allocate
 *
 * @return  a pointer to allocated disk
 * @retval  0  failure
 *
 */
disk68_t * file68_new(int extra);

FILE68_API
/**
 * Destroy loaded or allocated disk.
 *
 * @param  disk  disk to free.
 *
 * @see file68_new()
 * @see file68_load()
 */
void file68_free(const disk68_t * disk);

/**
 * @}
 */


/**
 * @name  Library functions.
 * @{
 */

FILE68_API
/**
 * Get package version string.
 *
 * @return "file68 X.Y.Z"
 */
const char * file68_versionstr(void);

FILE68_API
/**
 * Get package version number.
 *
 * @return X*100+Y*10+Z
 */
int file68_version(void);

FILE68_API
/**
 * Get identifier string.
 * @param  version  0:default 1:v1 2:v2
 * @return a static string of the identifier
 * @see SC68_IDSTR
 * @see SC68_IDSTR_V2
 */
const char * file68_identifier(int version);

FILE68_API
/**
 * Get sc68 official (no IANA) mime type.
 * @return a static string of the mimetype (should be audio/x-sc68).
 */
const char * file68_mimetype(void);

FILE68_API
/**
 * Initialize file68 library.
 *
 *  @param  argc  argument count
 *  @param  argv  argument array
 *  @return new argument count
 */
int file68_init(int argc, char **argv);

FILE68_API
/**
 * Shutdown file68 library.
 */
void file68_shutdown(void);

/**
 * @}
 */

/**
 * @}
 */

#endif
