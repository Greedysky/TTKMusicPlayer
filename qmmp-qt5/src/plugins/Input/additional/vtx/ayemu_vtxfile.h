#ifndef _AYEMU_vtxfile_h
#define _AYEMU_vtxfile_h

#include <stdio.h>
#include <stdint.h>

#include "ayemu_8912.h"

BEGIN_C_DECLS

/**
 * \defgroup vtxfile Functions for extract data from vtx files
 */
/*@{*/

/** structure for VTX file format handler
 * \internal
 * It stores VTX file header and current state
 * (open file pointer, extracted register data, etc).
 */
typedef struct
{
  ayemu_chip_t chiptype;	/**< Type of sound chip */
  int      stereo;		/**< Type of stereo: ABC, BCA,... */
  int      loop;		/**< song loop */
  int      chipFreq;		/**< AY chip freq (1773400 for ZX) */
  int      playerFreq;		/**< 50 Hz for ZX, 60 Hz for yamaha */
  int      year;		/**< year song composed */
  char *title;			/**< song title  */
  char *author;			/**< song author  */
  char *from;			/**< song from */
  char *tracker;		/**< tracker */
  char *comment;		/**< comment */
  int  regdata_size;	/**< size of unpacked data (need for unpack). */
  unsigned char *regdata;		/**< unpacked song data */
  size_t frames;		/**< number of AY data frames */
} ayemu_vtx_t;


/*! Load only header (song info) from vtx file.
 * Purpose: read tags in play list.
 */
EXTERN ayemu_vtx_t * ayemu_vtx_header(const char *buf, size_t size);

/*! Load song (header and unpack data). 
 *
 */
EXTERN ayemu_vtx_t * ayemu_vtx_load(const char *buf, size_t size);

EXTERN void ayemu_vtx_getframe(const ayemu_vtx_t *vtx, size_t frame_n,
			       ayemu_ay_reg_frame_t regs);

/** Free all allocaded data and structure itself.
 * You must call this for any ayemu_vtx_t pointer allocated by this lib
 * by functions ayemu_vtx_header() and ayemu_vtx_load().
 */
EXTERN void ayemu_vtx_free(ayemu_vtx_t *vtx);

/*! Load song header from file.
 * Helper (non-all platform) function
 */
EXTERN ayemu_vtx_t * ayemu_vtx_header_from_file(const char *filename);

/*! Load song header and data from file.
 * Helper (non-all platform) function
 */
EXTERN ayemu_vtx_t * ayemu_vtx_load_from_file(const char *filename);

/*@}*/

END_C_DECLS

#endif
