/*
 * @file    api68.c
 * @brief   sc68 API
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "io68/default.h"
#include <sc68/file68_vfs.h> /* Need vfs68.h before sc68.h */

/* libsc68 includes */
#include "sc68.h"
#include "mixer68.h"
#include "conf68.h"
#include "dial68/dial68.h"

#ifndef HAVE_BASENAME
#include "libc68.h"
#endif

/* libsc68 emulators includes */
#include "emu68/emu68.h"
#include "emu68/excep68.h"
#include "emu68/ioplug68.h"
#include "io68/io68.h"

/* file68 includes */
#include <sc68/file68.h>
#include <sc68/file68_err.h>
#include <sc68/file68_str.h>
#include <sc68/file68_uri.h>
#include <sc68/file68_rsc.h>
#include <sc68/file68_msg.h>
#include <sc68/file68_opt.h>

/* stardard includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

#define MK4CC(A,B,C,D) (((int)(A)<<24)|((int)(B)<<16)|((int)(C)<<8)|((int)(D)))


/* Bunch of constant use by sc68
 */
enum {
  /* Internal use for get_pos() and set_pos() */
  SC68_POS_PLAY, SC68_POS_TRACK,  SC68_POS_DISK,
  /* Exception handler to catch badly initialized exceptions */
  INTR_ADDR = 0x800,
  /* TOS emulator */
  TRAP_ADDR = 0x1000,
  /* # of instructions for music play code */
  TRAP_MAX_INST = 10000u,
  /* Vector used for sc68 special function break */
  BRKP_VECTOR = TRAP_VECTOR(0),
  /* Special value for sc68 special functions */
  BRKP_MAGIC = 0x5C68DB60,
  /* # of instructions for music play code */
  PLAY_MAX_INST = 1000000u,
  /* # of instructions for music init code */
  INIT_MAX_INST = 10000000u,
  /* default music time in seconds  */
  TIME_DEF = 3 * 60,
  /* sc68_t magic identifier value. */
  SC68_MAGIC = MK4CC('s','c','6','8'),
  /* disk68_t magic identifier value. */
  DISK_MAGIC = SC68_DISK_ID,
  /* Error message maximum length */
  ERRMAX = 96,
  /* Default amiga blend */
  AGA_BLEND = 0x50,
  /* Option "force-loop" off */
  CFG_LOOP_OFF = 0,
  /* Option "force-loop" infinite */
  CFG_LOOP_INF = -1,
};

/* Hardware table */
static const char * hwtable[8] = {
  "none",
  "Yamaha-2149",
  "MicroWire (STE)",
  "Yamaha-2149 & MicroWire (STE)",
  "Amiga/Paula",
  "Yamaha-2149 & Amiga/Paula",
  "MicroWire (STE) & Amiga/Paula",
  "Yamaha-2149 & MicroWire (STE) & Amiga/Paula",
};

/* Atari-ST trap emulator (see trap.s)
 */
static u8 trap_func[] = {
#include "sc68/trap68.h"
};

/* Atari-ST trap categories
 */
static const char * trap_name[16] =
{ "sc68","gemdos",0,0,0,0,0,0,0,0,0,0,0,"bios","xbios",0 };

/** Init flags, */
static struct init_flags initflags;
/* struct { */
/*   unsigned int load_oninit : 1; */
/*   unsigned int save_onexit : 1; */
/* } initflags; */

/** sc68 config */
static struct {
  unsigned int loaded      : 1;
  unsigned int allow_remote: 1;
#ifdef WITH_FORCE_HW
  unsigned int force_hw    : 1;
#endif
  int aga_blend;
#ifdef WITH_FORCE
  int force_track;
  int force_loop;
#endif
  int asid;
  int def_time_ms;
  int spr;
} config;

/** sc68 instance. */
struct _sc68_s {
  int            magic;       /**< magic identifier.                     */
  char           name[16];    /**< short name.                           */
  int            version;     /**< sc68 version.                         */
  void         * cookie;      /**< User private data.                    */
  emu68_parms_t  emu68_parms; /**< 68k emulator parameters.              */

/* Keep the following in that order !!!!
 * {
 */
  emu68_t      * emu68;       /**< 68k emulator instance.                */
  io68_t *ymio,*mwio,*shifterio,*paulaio,*mfpio;
/* } */

  ym_t         * ym;          /**< YM emulator.                          */
  mw_t         * mw;          /**< MicroWire emulator.                   */
  paula_t      * paula;       /**< Amiga emulator.                       */

  int            tobe3;       /**< free disk memory be release on close. */
  const disk68_t  * disk;     /**< Current loaded disk.                  */
  const music68_t * mus;      /**< Current playing music.                */
  int            track;       /**< Current playing track.                */
  int            track_to;    /**< Track to set (0:n/a, -1:end req).     */
  int            loop_to;     /**< Loop to set (0:default -1:infinite).  */
  int            asid;        /**< aSIDifier flags.                      */
  int            asid_timers; /**< timer assignment 4cc (0:not asid).    */
#ifdef WITH_FORCE
  int            cfg_track;   /**< from config "force-track".            */
  int            cfg_loop;    /**< from config "force-loop".             */
#endif

  unsigned int   playaddr;    /**< Current play address in 68 memory.    */
  int            seek_to;     /**< Seek to this time (-1:n/a)            */
  int            remote;      /**< Allow remote access.                  */

  struct {
    int org_ms;
    int len_ms;
  } tinfo[SC68_MAX_TRACK+1];

/** Playing time info. */
  struct {
    unsigned int def_ms;      /**< default time in ms.                   */
    unsigned int origin_ms;   /**< elapsed ms at track origin.           */
    unsigned int elapsed_ms;  /**< elapsed ms since track start.         */
  } time;

/** IRQ handler. */
  struct {
    int pc;                   /**< value of PC for last IRQ.             */
    int sr;                   /**< value of sr for lasr IRQ.             */
    int vector;               /**< what was the last IRQ type.           */
    int sysfct;               /**< Last gemdos/bios/xbios function       */
  } irq;

/** Mixer info struture. */
  struct
  {
    unsigned int   spr;          /**< Sampling rate in hz.               */
    u32          * buffer;       /**< Current PCM buffer.                */
    int            bufpos;       /**< Current PCM position.              */
    int            bufmax;       /**< buffer allocated size.             */
    int            bufreq;       /**< Required buffer size for track.    */
    int            buflen;       /**< PCM count in buffer.               */
    int            stdlen;       /**< Default number of PCM per pass.    */
    unsigned int   cycleperpass; /**< Number of 68K cycles per pass.     */
    int            aga_blend;    /**< Amiga LR blend factor [0..65535].  */

    unsigned int   pass_count;   /**< Pass counter.                      */
    unsigned int   loop_count;   /**< Loop counter.                      */
    unsigned int   pass_total;   /**< Total number of pass.              */
    unsigned int   loop_total;   /**< Total number of loop (0:infinite). */
    unsigned int   pass_2loop;   /**< Number of pass before next loop.   */
    unsigned int   pass_3loop;   /**< Reset pass_2loop after a loop.     */

  } mix;

  sc68_minfo_t     info;         /**< Disk and track info struct.        */

/* Error message */
  const char   * errstr;         /**< Last error message.                */
  char           errbuf[ERRMAX]; /**< For non-static error message.      */
};

#ifndef DEBUG_SC68_O
# ifndef DEBUG
#  define DEBUG_SC68_O 0
# else
#  define DEBUG_SC68_O 1
# endif
#endif

int sc68_cat = msg68_NEVER;
int dial_cat = msg68_NEVER;
static int           sc68_id;        /* counter for auto generated name */
static volatile int  sc68_init_flag; /* Library init flag     */
static int           sc68_spr_def = SPR_DEF;
static int           dbg68k;
static const char    not_available[] = SC68_NOFILENAME;
static char          appname[16] = "sc68";
static char          sc68_errstr[ERRMAX];

/***********************************************************************
 * Forward declarations
 **********************************************************************/

static void sc68_debug(sc68_t * sc68, const char * fmt, ...) FMT23;
static int error_addx(sc68_t * sc68, const char * fmt, ...) FMT23;
static int error_add(sc68_t * sc68, const char * fmt, const char *);
static int get_spr(const sc68_t * sc68);
static int set_spr(sc68_t * sc68, int hz);
static int get_asid(const sc68_t * sc68);
static int set_asid(sc68_t * sc68, int asid);
static int can_asid(const sc68_t * sc68, int track);
static int get_pcm_fmt(sc68_t * sc68);
static int set_pcm_fmt(sc68_t * sc68, int pcmfmt);
static int get_pos(sc68_t * sc68, int origin);
static sc68_disk_t get_dt(sc68_t * sc68, int * ptr_track, sc68_disk_t disk);
static int calc_disk_len(const disk68_t * disk, const int loop);
static unsigned int calc_track_len(const disk68_t * d, int track, int loop);
static unsigned int calc_pos(sc68_t * const sc68);
static void music_info(sc68_t * sc68, sc68_music_info_t * f,
                       const disk68_t * d, int track, int loops);

/***********************************************************************
 * Check functions
 **********************************************************************/

static inline const char * ok_int(const int err) {
  return !err ? "success" : "failure";
}

static inline const char * never_null(const char * ptr) {
  return ptr ? ptr : "(nil)";
}

static inline int overlap(unsigned a, unsigned na, unsigned b, unsigned nb)
{
  na += a; nb += b;
  return a <= b ? na > b : nb > a;
}

static inline int is_magic(const sc68_t * const sc68) {
  return sc68->magic == SC68_MAGIC;
}

static inline int is_sc68(const sc68_t * const sc68) {
  return sc68 && is_magic(sc68);
}

static inline int null_or_sc68(const sc68_t * const sc68) {
  return !sc68 || is_magic(sc68);
}

static inline int is_disk(const disk68_t * const disk) {
  return disk && disk->magic == DISK_MAGIC;
}

static inline int null_or_disk(const disk68_t * const disk) {
  return !disk || disk->magic == DISK_MAGIC;
}

static inline int has_disk(const sc68_t * const sc68) {
  return is_sc68(sc68) && is_disk(sc68->disk);
}

static inline int in_range(const disk68_t * disk, const int track) {
  return track > 0 && track <= disk->nb_mus;
}

static inline int has_track(const sc68_t * const sc68, const int track) {
  return has_disk(sc68) && in_range(sc68->disk, track);
}

static inline int in_disk(const disk68_t * disk, const int track) {
  return is_disk(disk) && in_range(disk, track);
}

static inline int trk_can_asid(const music68_t * const m)
{
  return 1
    &&  (m->hwflags & SC68_PSG)
    &&  (m->hwflags & SC68_XTD)
    && !(m->hwflags & (SC68_MFP_TA|SC68_MFP_TB|SC68_MFP_TC|SC68_MFP_TD))
    ;
}

/***********************************************************************
 * Convenience functions
 **********************************************************************/

#ifndef EXTENSION_SEP
# define EXTENSION_SEP '.'
#endif
static void appname_from_path(char *path, char * appname, int max)
{
  char * e, *s;
  int len;

  appname[0] = 0;
  if (!path || !*path) return;
  s = basename(path);
  e = strrchr(s,EXTENSION_SEP);
  if (!e || e == s)
    len = strlen(s);
  else
    len = e - s;
  if (len >= max)
    len = max-1;
  strncpy(appname, s, len);
  appname[len] = 0;
}


/***********************************************************************
 * Emulators functions
 **********************************************************************/

static int stream_read_68k(sc68_t * sc68, unsigned int dest,
                           vfs68_t * is, unsigned int sz)
{
  u8 * mem68 = emu68_memptr(sc68->emu68, dest, sz);

  if (!mem68)
    return error_addx(sc68,
                      "libsc68: %s -- %s\n",
                      "vfs error", emu68_error_get(sc68->emu68));
  return (vfs68_read(is, mem68, sz) == sz) ? 0 : -1;
}

static int init_emu68(int * argc, char ** argv)
{
  int err;

  /* Initialize emu68 */
  err = emu68_init(argc, argv);
  if (err) {
    error_add(0, "libsc68: %s\n","emu68 library *FAILED*");
    goto error;
  }

  /* Initialize chipset */
  err = io68_init(argc, argv);
  if (err) {
    error_add(0, "libsc68: %s\n","chipset library *FAILED*");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "68k and chipset emulator initialized");

error:
  return err;
}

static void safe_io68_destroy(io68_t **pio)
{
  io68_destroy(*pio);
  *pio = 0;
}

static void safe_emu68_destroy(emu68_t **pemu)
{
  emu68_destroy(*pemu);
  *pemu = 0;
}

static void safe_destroy(sc68_t * sc68)
{
  assert(sc68);
  emu68_ioplug_unplug_all(sc68->emu68);
  safe_io68_destroy(&sc68->ymio);
  safe_io68_destroy(&sc68->mwio);
  safe_io68_destroy(&sc68->shifterio);
  safe_io68_destroy(&sc68->paulaio);
  safe_io68_destroy(&sc68->mfpio);
  safe_emu68_destroy(&sc68->emu68);
  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "68k and chipset emulators destroyed");
}

#define Bpeek(EMU68,ADDR) emu68_peek((EMU68), (ADDR))

static u16 Wpeek(emu68_t* const emu68, addr68_t  addr)
{
  return (u16) ( ( (u16) Bpeek(emu68, addr+0) << 8 ) |
                 ( (u16) Bpeek(emu68, addr+1)      ) );
}

static u32 Lpeek(emu68_t* const emu68, addr68_t  addr)
{
  return (u32) ( ( (u32) Bpeek(emu68, addr+0) << 24 ) |
                 ( (u32) Bpeek(emu68, addr+1) << 16 ) |
                 ( (u32) Bpeek(emu68, addr+2) <<  8 ) |
                 ( (u32) Bpeek(emu68, addr+3)       ) );
}

/* Timer awareness */
void except_name(const int vector, char * irqname)
{
  int c = 0;
  switch (vector * 4) {
  case 0x134: c = 'A'; break;
  case 0X120: c = 'B'; break;
  case 0X114: c = 'C'; break;
  case 0X110: c = 'D'; break;
  default:    c = 0;
  }
  if (c)
    sprintf(irqname,"timer-%c", c);
  else
    emu68_exception_name(vector,irqname);
}


/* Exception handler (runs when emu68 runs in debug mode (--sc68-dbg68k=1).
 *
 * It's default behaviour is to dump the exception and to break
 * execution. That will generate an error later in the simulation loop.
 *
 * HW-trace and MFP timer interrupts are ignored.
 *
 * Know trace vectors (gemdos/xbios/bios) are dumped but not breaked.
 *
 */
static void irqhandler(emu68_t* const emu68, int vector, void * cookie)
{
  static u8 thex[16] = {
    '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
  };

  sc68_t* sc68 = cookie;
  char    irqname[32];
  const char * stname;
  int     dumpex, savest, subvec;
  u32     sr, pc, text;

  /* Store last interruption */
  sc68->irq.pc     = emu68->inst_pc;   /* or real register values ? */
  sc68->irq.sr     = emu68->inst_sr;   /* or real register values ? */
  sc68->irq.vector = vector;

  /* Exit those really fast as they happen quiet a lot, specially
   * HWTRACE_VECTOR that happem every single instruction. */
  if ( vector == HWTRACE_VECTOR ||
       vector == 0x134/4 /* Timer-A */ ||
       vector == 0X120/4 /* Timer-B */ ||
       vector == 0X114/4 /* Timer-C */ ||
       vector == 0X110/4 /* Timer-D */ )
    return;

  /* Get exception fancy name */
  except_name(vector,irqname);

  /* Init vars */
  stname = emu68_status_name(emu68->status);
  text   = 0;                           /* Debug text */
  subvec = 0;                           /* Trap number and whatnot */
  dumpex = 1;                           /* Dump */
  savest = emu68->status;               /* Save emulator status */

  switch (savest) {
  case EMU68_NRM:
    /* Assume break, set back to NRM only if the exception is known */
    emu68->status = EMU68_BRK;
    break;

  case EMU68_STP:
  case EMU68_BRK:
  case EMU68_HLT:
  case EMU68_ERR:
  case EMU68_XCT:
  default:
    break;
  }

  /***********************************************************************
   * Retrieve exception source (both pc and sr)
   */
  if ( vector < 0x100 ) {
    /* normal vectors, get sr and pc from the stack */
    sc68->irq.sr = sr = Wpeek(emu68, emu68->reg.a[7]);
    sc68->irq.pc = pc = Lpeek(emu68, emu68->reg.a[7]+2);

    if (vector >= TRAP_VECTOR(0) && vector <= TRAP_VECTOR(15)) {
      subvec = vector - TRAP_VECTOR(0);
      vector = TRAP_VECTOR(0);
      if (!subvec) {
        /* might be a breakpoint */
        uint_t fct = Lpeek(emu68, emu68->reg.a[7]+6);
        if ( (fct & ~0xF) == BRKP_MAGIC ) {
          text = Lpeek(emu68, emu68->reg.a[7]+10);
          TRACE68(sc68_cat,
                  "libsc68: !!! BREAKPOINT: $%08X/$%06X\n", fct, text);
          /* It's magic !! */
          switch (sc68->irq.sysfct = fct & 0xF) {
          case 0:                         /* function 0: just print */
          case 1:                         /* function 1: break on debug */
            emu68->status = savest;       /* no break */
            break;
          default:
            break;
          }
        }
      } else if (trap_name[subvec]) {
        sc68->irq.sysfct = Wpeek(emu68, emu68->reg.a[7]+6) & 0xFFFF;
        emu68->status    = savest;      /* Known trap: no break */
      }
    }
  } else {
    /* Special emu68 vectors, just grab cpu's reg as-is */
    sr = emu68->reg.sr;
    pc = emu68->reg.pc;

    if (vector >= HWBREAK_VECTOR && vector < HWTRACE_VECTOR) {
      subvec = vector - HWBREAK_VECTOR;
      vector = HWBREAK_VECTOR;
    }

    switch (vector) {
    case HWSTOP_VECTOR:
      if ( (sr & 0x3F00) == 0x2F00 ) {
        /* SR bit#11 (always 0) is used by our special code to catch
         * Unitialized exception.  The exception vector issued a stop
         * #$2FNN instruction with NN being the exception vector
         * number.
         */
        const int num = sr & 0xFF;
        addr68_t vaddr;
        strcpy(irqname,"NC-");
        vaddr = Lpeek(sc68->emu68, num * 4);
        if (vaddr != INTR_ADDR + num * 8)
          strcpy(irqname,"CH-");
        except_name(num,irqname+3);
        sr = Wpeek(emu68, emu68->reg.a[7]);
        pc = Lpeek(emu68, emu68->reg.a[7]+2);
        error_addx(sc68,
                   "libsc68: non-catch exception #%d (%s) from %06x",
                   num, irqname, pc);
        emu68->status = EMU68_HLT;
      } else {
        emu68->status = EMU68_STP;
      }

    case HWBREAK_VECTOR: case HWINSTOV_VECTOR:
      break;

    case HWINIT_VECTOR:
      dumpex = 0;
    case HWHALT_VECTOR:
    case HWRESET_VECTOR:
      /* Recieve the reset vector when reset instruction has just been
       * executed, in our case it happen in the uninitialized
       * exception catcher code. */
      emu68->status = EMU68_HLT;
      break;

    case HWTRACE_VECTOR:
    default:
      assert(!"Invalid hardware vector");
      break;

    }
  }

  /***********************************************************************
   * Dump exception context
   */
  if (dumpex) {
    u8 * memptr = emu68_memptr(sc68->emu68,0,0);
    const addr68_t msk = emu68->memmsk;
    const addr68_t adr = emu68->reg.a[7] & emu68->memmsk;
    const addr68_t bot = emu68->memmsk+1-16;
    char line[16 * 3];
    int i, j, l = strlen(irqname);

    if (vector == ILLEGAL_VECTOR)
      sprintf(irqname+l, "#$%02x-%02x",
              memptr[pc&emu68->memmsk], memptr[(pc+1)&emu68->memmsk]);
    else if (vector == TRAP_VECTOR(0) && trap_name[subvec])
      sprintf(irqname, "%s(%d) ($%02x)",
              trap_name[subvec], sc68->irq.sysfct, sc68->irq.sysfct);

    /* dump cpu status */
    msg68x(sc68_cat, sc68,
           "libsc68: 68k interruption -- emu68<%s> sc68<%s>\n"
           " status: in:%s out:%s\n"
           "   intr: #%02x+%02x %s\n"
           "   from: pc:%08x sr:%04x\n"
           "   regs: pc:%08x sr:%04x\n"
           "         d0:%08x d1:%08x d2:%08x d3:%08x\n"
           "         d4:%08x d5:%08x d6:%08x d7:%08x\n"
           "         a0:%08x a1:%08x a2:%08x a3:%08x\n"
           "         a4:%08x a5:%08x a6:%08x a7:%08x\n",
           emu68->name, sc68->name,
           stname, emu68_status_name(emu68->status),
           vector, subvec, irqname,
           pc, sr,
           emu68->reg.pc,   emu68->reg.sr,
           emu68->reg.d[0], emu68->reg.d[1],
           emu68->reg.d[2], emu68->reg.d[3],
           emu68->reg.d[4], emu68->reg.d[5],
           emu68->reg.d[6], emu68->reg.d[7],
           emu68->reg.a[0], emu68->reg.a[1],
           emu68->reg.a[2], emu68->reg.a[3],
           emu68->reg.a[4], emu68->reg.a[5],
           emu68->reg.a[6], emu68->reg.a[7]);

    /* dump stack */
    line[0] = 0;
    for (i = 0, j = 0; adr+i < bot && i<0x80; ++i) {
      int v = memptr[adr+i];

      line[j+0] = thex[v >> 4];
      line[j+1] = thex[v & 15];
      if (j != 3*15) {
        line[j+2] = '-';
        j += 3;
      } else {
        line[j+2] = 0;
        msg68x(sc68_cat,sc68,
               " %-6x: %s\n", (unsigned) adr + (i & ~15), line);
        j = 0;
      }
    }
    if (j) {
      line[j-1] = 0;
      msg68x(sc68_cat,sc68,
             " %-6x: %s\n", (unsigned) adr + (i & ~15), line);
    }

    /* dump debug message */
    if (text) {
      char tmp[256];
      for (i = 0; i < sizeof(tmp)-1 && (tmp[i] = memptr[(text+i)&msk]); ++i)
        ;
      tmp[i] = 0;
      msg68x(sc68_cat, sc68, "\nlibsc68: %s\n\n", tmp);
    }
  }
}

static int init68k(sc68_t * sc68, int log2mem, int emu68_debug)
{
  int err = -1;
  emu68_parms_t * const parms = &sc68->emu68_parms;

  if (sc68->emu68)
    safe_destroy(sc68);

  /* setup parameters. */
  parms->name    = "sc68/emu68";
  parms->log2mem = log2mem;
  parms->clock   = EMU68_ATARIST_CLOCK;
  parms->debug   = emu68_debug & 1;

  TRACE68(sc68_cat,
          "libsc68: init 68k -- '%s' mem:%d-bit(%dkB) clock:%uhz debug:%s\n",
          parms->name,
          parms->log2mem,parms->log2mem>10?1<<(parms->log2mem-10):0,
          parms->clock,parms->debug?"On":"Off");

  /* Do initialization. */
  sc68->emu68 = emu68_create(parms);
  if (!sc68->emu68) {
    error_add(sc68, "libsc68: %s\n", "68k emulator creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: init 68k -- %s\n",
          "CPU emulator created");

  /* Install cookie and interruption handler (debug mode only). */
  emu68_set_handler(sc68->emu68, (emu68_debug & 1) ? irqhandler : 0);
  emu68_set_cookie(sc68->emu68, sc68);

  /* Some invalid values. */
  sc68->irq.sysfct = -1;
  sc68->irq.sr     = -1;
  sc68->irq.vector = -1;
  sc68->irq.pc     = 0xDEADDAD1;

  /* Setup critical 68K registers (SR and SP) */
  sc68->emu68->reg.sr   = 0x2000;
  sc68->emu68->reg.a[7] = sc68->emu68->memmsk+1-4;

  /* Initialize chipset */
  sc68->ymio = ymio_create(sc68->emu68,0);
  sc68->ym   = ymio_emulator(sc68->ymio);
  if (!sc68->ymio) {
    error_add(sc68,"libsc68: %s","YM-2149 creation failed");
    goto error;
  }
  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- YM-2149");

  sc68->mwio = mwio_create(sc68->emu68,0);
  sc68->mw   = mwio_emulator(sc68->mwio);
  if (!sc68->mwio) {
    error_add(sc68,"libsc68: %s\n", "STE-MW creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- STE-MW");

  sc68->shifterio = shifterio_create(sc68->emu68,0);
  if (!sc68->shifterio) {
    error_add(sc68,"libsc68: %s\n","Atari Shifter creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- ST shifter");

  sc68->paulaio = paulaio_create(sc68->emu68,0);
  sc68->paula   = paulaio_emulator(sc68->paulaio);
  if (!sc68->paulaio) {
    error_add(sc68,"libsc68: %s\n","create Paula emulator failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- Amiga Paula");

  sc68->mfpio = mfpio_create(sc68->emu68);
  if (!sc68->mfpio) {
    error_add(sc68,"libsc68: %s\n","MK-68901 creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- MFP");

  err = 0;
error:
  if (err)
    safe_destroy(sc68);

  TRACE68(sc68_cat,
          "libsc68: init 68k -- %s\n",
          strok68(err));

  return err;
}

/***********************************************************************
 * Config functions
 **********************************************************************/

/* Get integer config vaue.
 * - just command line if available
 * - else get the value from the config file
 * - fallback to default otherwise.
 */
static int optcfg_get_int(const char * name, int def)
{
  option68_t * opt = option68_get(name, opt68_ALWAYS);
  if (opt && opt->type != opt68_STR) {
    if (opt->org == opt68_UDF)
      option68_iset(opt,def,opt68_ALWAYS,opt68_CFG);
    if (opt->org != opt68_UDF)
      return opt->val.num;
  }
  return def;
}

#if 0
/* Get string config value
 * @optcfg_get_int for details,
 */
static const char * optcfg_get_str(const char * name, const char * def)
{
  option68_t * opt = option68_get(name, opt68_ISSET);
  const char * v = def;

  if (opt) {
    switch (opt->type) {
    case opt68_STR:
      v = opt->val.str;
      break;
    case opt68_ENU:
      v = opt->val.num[(const char **)opt->set];
      break;
    }
  }
  return v;
}
#endif

#if 0
static void optcfg_set_int(const char * name, int v)
{
  option68_t * opt = option68_get(name, opt68_ALWAYS);
  option68_iset(opt, v, opt68_ALWAYS, opt68_APP);
}

static void optcfg_set_str(const char * name, const char * v)
{
  option68_t * opt = option68_get(name, opt68_ALWAYS);
  option68_set(opt, v, opt68_ALWAYS);
}
#endif

static void config_default(void)
{
  memset(&config,0,sizeof(config));
  config.allow_remote = 1;
  config.aga_blend    = AGA_BLEND;
#ifdef WITH_FORCE
  config.force_loop   = CFG_LOOP_DEF;
#endif
  config.def_time_ms  = TIME_DEF * 1000;
  config.spr          = SPR_DEF;
}

#define set_CONFIG(KEY,NAME) config.KEY = optcfg_get_int(NAME,config.KEY)

/* Load config for this application. */
static int config_load(const char * appname)
{
  int err;

  config_default();
  err = config68_load(appname);
  config.loaded = !err;

  set_CONFIG(allow_remote,"allow-remote");
  set_CONFIG(aga_blend,"amiga-blend");
#ifdef WITH_FORCE
  set_CONFIG(force_track,"force-track");
  set_CONFIG(force_loop,"force-loop");
#endif
#ifdef WITH_FORCE_HW
  set_CONFIG(force_hw,"force-hw");
#endif
  set_CONFIG(asid,"asid");
  config.def_time_ms = optcfg_get_int("default-time", TIME_DEF) * 1000;
  set_CONFIG(spr,"sampling-rate");

  sc68_debug(0,"libsc68: load config -- %s\n", strok68(err));
  return err;
}

/* Save config for this application. */
static int config_save(const char * appname)
{
  int err
    = config68_save(appname);
  sc68_debug(0,"libsc68: save config -- %s\n", strok68(err));
  return err;
}

#if 0
/* Update config values with current sc68 ones. */
static void config_update(const sc68_t * sc68)
{
  if (is_sc68(sc68)) {
    config.allow_remote = sc68->remote;
    config.aga_blend = sc68->mix.aga_blend >> 8;
#ifdef WITH_FORCE
    config.force_track = sc68->cfg_track < 0 ? 0 : sc68->cfg_track;
    if (sc68->cfg_loop == SC68_INF_LOOP)
      config.force_loop = CFG_LOOP_INF;
    else if (sc68->cfg_loop == SC68_DEF_LOOP)
      config.force_loop = CFG_LOOP_DEF;
    else if (sc68->cfg_loop > 0)
      config.force_loop = sc68->cfg_loop;
#endif

    /* config.asid = sc68->cfg_asid & (SC68_ASID_ON|SC68_ASID_FORCE); */
    /* if (config.asid == (SC68_ASID_ON|SC68_ASID_FORCE)) */
    /*   config.asid = SC68_ASID_FORCE; */

    config.def_time_ms = sc68->time.def_ms;
    config.spr = sc68->mix.spr;
  }

  if (config.loaded) {
    optcfg_set_int("allow-remote",  !!config.allow_remote);
    optcfg_set_int("amiga-blend",   config.aga_blend);
#ifdef WITH_FORCE
    optcfg_set_int("force-track",   config.force_track);
    optcfg_set_int("force-loop",    config.force_loop);
#endif
#ifdef WITH_FORCE_HW
    optcfg_set_int("force-hw",      config.force_hw);
#endif
    optcfg_set_int("asid",          config.asid);
    optcfg_set_int("default-time",  (config.def_time_ms+999) / 1000u);
    optcfg_set_int("sampling-rate", config.spr);
  }

  TRACE68(sc68_cat, "libsc68: %s\n", "config updated");
}

#endif

static int aga_blend(int v)
{
  v -= 128;
  return ((v << 8) | (-(v&1)&255)) + 0x8000;
}

/* Apply config to sc68 instance. */
static void config_apply(sc68_t * sc68)
{
  if (is_sc68(sc68)) {
    sc68->remote        = config.allow_remote;

    /* $$$ HAXX: reloading because it might have changed. */
    config.aga_blend    = optcfg_get_int("amiga-blend", AGA_BLEND);
    sc68->mix.aga_blend = aga_blend(config.aga_blend);
#ifdef WITH_FORCE
    sc68->cfg_track     = SC68_DEF_TRACK;
    if (config.force_track > 0)
      sc68->cfg_track = config.force_track;
    sc68->cfg_loop      = SC68_DEF_LOOP;
    if (config.force_loop == CFG_LOOP_INF)
      sc68->cfg_loop      = SC68_INF_LOOP;
    else if (config.force_loop > 0)
      sc68->cfg_loop      = config.force_loop;
#endif
    /* sc68->cfg_asid      = config.asid; */
    sc68->time.def_ms   = config.def_time_ms;
    sc68->mix.spr       = config.spr;
    TRACE68(sc68_cat,
            "libsc68: sc68<%s> %s\n",
            sc68->name, "config applied");
  }
}

/* HAXXX: should be eval when a message category is added/removed */
static void eval_debug(void)
{
  static option68_t * opt;
  /* HaXXX: force a call "debug" callback. */
  if (!opt)
    opt = option68_get("debug", opt68_ALWAYS);
  if (opt && opt->org != opt68_UDF) {
    int org = opt->org;
    char * s = strdup(opt->val.str);
    if (s) {
      option68_unset(opt);
      option68_set(opt, s, opt68_ALWAYS, org);
      free(s);
    }
  }
}


/***********************************************************************
 * API functions
 **********************************************************************/

int sc68_init(sc68_init_t * init)
{
  int err = -1;
  option68_t * opt;
  sc68_init_t dummy_init;

  static option68_t debug_options[] = {
    OPT68_BOOL("sc68-","dbg68k","sc68","run m68K in debug mode",0,0)
  };

  /* Just a stupid test to check if this host arythmetic unit use 2's
   * complement as signed representation of negative numbers.
   *
   * It should always be the case but we never know. So many things in
   * sc68 rely on that specially in the 68k emulators there is not a
   * chance it works properly otherwise.
   */
  assert( -1 == ~0 );
  if ( -1 != ~0 ) {
    err = error_add(0, "libsc68: %s\n",
                    "internal error (negative are not 2's complement)");
    goto error_no_shutdown;
  }

  if (sc68_init_flag) {
    err = error_add(0, "libsc68: %s\n","already initialized");
    goto error_no_shutdown;
  }
  sc68_errstr[0] = 0;

  if (!init) {
    memset(&dummy_init, 0, sizeof(dummy_init));
    init = &dummy_init;
  }
  sc68_cat = msg68_cat("sc68",  "sc68 library",DEBUG_SC68_O);
  dial_cat = msg68_cat("dialog","sc68 dialogs",DEBUG_SC68_O);

  /* Set debug handler and filters. */
  msg68_set_handler((msg68_t)init->msg_handler);
  msg68_set_cookie(0);
  msg68_cat_filter(init->debug_clr_mask,init->debug_set_mask);

  /* Get application name. */
  appname[0] = 0;
  if (init->argc > 0 && init->argv)
    appname_from_path(init->argv[0], appname, sizeof(appname));
  if (!appname[0])
    strcpy(appname,"sc68");
  if (init->argc > 0 && init->argv)
    init->argv[0] = appname;

  /* Intialize file68. */
  init->argc = file68_init(init->argc, init->argv);
  eval_debug();

  /* Init config module. */
  init->argc = config68_init(init->argc, init->argv);
  eval_debug();
  config_default();

  /* Setup init flags. */
  initflags = init->flags;

  /* Add and parse local options. */
  option68_append(debug_options,sizeof(debug_options)/sizeof(*debug_options));
  init->argc = option68_parse(init->argc, init->argv);

  /* Initialize emulators. */
  err = init_emu68(&init->argc, init->argv);
  eval_debug();

  /* Load config */
  if (!initflags.no_load_config)
    config_load(appname);
  else
    sc68_debug(0,"libsc68: don't load config as requested\n");


  /* Set default sampling rate. */
  set_spr(0, SC68_SPR_DEFAULT);

  opt    = option68_get("dbg68k", opt68_ISSET);
  dbg68k = opt ? opt->val.num : 0;

  sc68_init_flag = !err;

  if (err) {
    sc68_shutdown();
  }
error_no_shutdown:
  sc68_debug(0,"libsc68: initialized as '%s' -- %s\n", appname, ok_int(err));
  return -!!err;
}

void sc68_shutdown(void)
{
  /* Save config unless it was not loaded or application requested
   * otherwise. */
  if (!config.loaded && !initflags.no_save_config)
    config_save(appname);
  else
    sc68_debug(0,"libsc68: don't save config as requested\n");

  if (sc68_init_flag) {
    sc68_init_flag = 0;
    file68_shutdown();
    config68_shutdown();          /* always after file68_shutdown() */
  }
  sc68_debug(0,"libsc68: shutdowned -- %s\n",ok_int(0));

  msg68_cat_free(sc68_cat); sc68_cat = msg68_NEVER;
  msg68_cat_free(dial_cat); dial_cat = msg68_NEVER;
}

sc68_t * sc68_create(sc68_create_t * create)
{
  sc68_t      * sc68 = 0;
  sc68_create_t dummy_create;

  if (!create) {
    memset(&dummy_create, 0, sizeof(dummy_create));
    create = &dummy_create;
  }

  /* Alloc SC68 struct. */
  sc68 = calloc(sizeof(sc68_t),1);
  if (!sc68)
    goto error;

  /* Set magic */
  sc68->magic = SC68_MAGIC;

  /* User private data. */
  sc68->cookie = create->cookie;

  /* Pick a short name */
  if (create->name) {
    strncpy(sc68->name, create->name, sizeof(sc68->name));
  } else {
    snprintf(sc68->name,sizeof(sc68->name),"sc68#%02d",++sc68_id);
  }
  sc68->name[sizeof(sc68->name)-1] = 0;

  /* Apply config */
  config_apply(sc68);

  /* Override config. */
  if (create->sampling_rate) {
    sc68->mix.spr = create->sampling_rate;
  }
  if (!sc68->mix.spr) {
    sc68->mix.spr = sc68_spr_def;
  }
  if (!sc68->time.def_ms) {
    sc68->time.def_ms = TIME_DEF * 1000;
  }

  /* aSIDifier. */
  switch (config.asid) {
  case SC68_ASID_ON:
    sc68->asid = SC68_ASID_ON; break;
  case SC68_ASID_FORCE: case SC68_ASID_ON | SC68_ASID_FORCE:
    sc68->asid = SC68_ASID_ON | SC68_ASID_FORCE; break;
  case SC68_ASID_OFF:
  default:
    sc68->asid = SC68_ASID_OFF; break;
  }
  TRACE68(sc68_cat,"libsc68: config-asid=%d asid-flags=%d\n",
          config.asid,sc68->asid);

  /* Create 68k emulator and pals. */
  if (init68k(sc68, create->log2mem, create->emu68_debug | dbg68k)) {
    goto error;
  }

  /* Set IO chipsets sampling rates */
  sc68->mix.spr = set_spr(sc68, sc68->mix.spr);
  if (sc68->mix.spr <= 0) {
    error_addx(sc68, "libsc68: %s\n", "invalid sampling rate");
    goto error;
  }
  create->sampling_rate = sc68->mix.spr;
  sc68_debug(sc68,"libsc68: sampling rate -- *%dhz*\n", create->sampling_rate);

  /* Finally gets all pathes. */
  /*   rsc68_get_path(&init->shared_path, */
  /*               &init->user_path, */
  /*               &init->lmusic_path, */
  /*               &init->rmusic_path); */

  /*   debugmsg68(-1,"sc68_create: shared-path=[%s]\n",init->shared_path); */
  /*   debugmsg68(-1,"sc68_create: user-path=[%s]\n",init->user_path); */
  /*   debugmsg68(-1,"sc68_create: music-path=[%s]\n",init->lmusic_path); */
  /*   debugmsg68(-1,"sc68_create: music-rpath=[%s]\n",init->rmusic_path); */

  /*   sc68_unic = sc68; */

  sc68_debug(0,"libsc68: sc68<%s> create -- %s\n", sc68->name, ok_int(0));
  return sc68;

error:
  sc68_destroy(sc68);
  sc68 = 0;
  sc68_debug(sc68,"libsc68: create -- %s\n", ok_int(-1));
  return 0;
}

void sc68_destroy(sc68_t * sc68)
{
  if (is_sc68(sc68)) {
    free(sc68->mix.buffer);
    sc68_close(sc68);
    safe_destroy(sc68);
    sc68_debug(sc68,"libsc68: sc68<%s> destroyed\n", sc68->name);
    free(sc68);
  } else {
    assert(!sc68);                      /* catch bad magic */
  }
}

static int get_spr(const sc68_t * sc68)
{
  return sc68 ? sc68->mix.spr : sc68_spr_def;
}

static int set_spr(sc68_t * sc68, int hz)
{
  if (hz == SC68_SPR_QUERY)
    hz = get_spr(sc68);
  else {
    const int min = SPR_MIN;
    const int max = SPR_MAX;
    const int def = SPR_DEF;
    if (hz == SC68_SPR_DEFAULT)
      hz = def;
    if (hz <  min) hz = min;
    if (hz >  max) hz = max;
    if (sc68) {
      hz = ymio_sampling_rate(sc68->ymio, hz);
      hz = mwio_sampling_rate(sc68->mwio, hz);
      hz = paulaio_sampling_rate(sc68->paulaio, hz);
      sc68->mix.spr = hz;
    } else {
      sc68_spr_def = hz;
    }
  }
  return hz;
}

static int get_pcm_fmt(sc68_t * sc68)
{
  return SC68_PCM_S16;
}

static int set_pcm_fmt(sc68_t * sc68, int pcmfmt)
{
  return -(pcmfmt != SC68_PCM_S16);
}

static int get_asid(const sc68_t * sc68)
{
  return sc68 ? sc68->asid : config.asid;
}

static int dsk_can_asid(const disk68_t * const d)
{
  int i, k;
  for (k=i=0; i<d->nb_mus; ++i)
    k += trk_can_asid(d->mus + i);
  return k;
}

static int can_asid(const sc68_t * sc68, int track)
{
  assert(is_sc68(sc68));

  if (!is_disk(sc68->disk))
    return -1;
  if (track == SC68_DSK_TRACK)
    return dsk_can_asid(sc68->disk);
  if (track == SC68_CUR_TRACK)
    track = sc68->track;
  if (in_range(sc68->disk,track))
    return trk_can_asid(sc68->disk->mus + track - 1);
  return -1;
}

static int set_asid(sc68_t * sc68, int asid)
{
  if (sc68) {
    if (asid & SC68_ASID_FORCE)
      asid |= SC68_ASID_ON;
    sc68->asid = asid &= ~ (1 << (sizeof(int)*8-1));
  } else {
    if (asid & SC68_ASID_FORCE)
      asid = SC68_ASID_FORCE;
    else if (asid & SC68_ASID_ON)
      asid = SC68_ASID_ON;
    else
      asid = SC68_ASID_OFF;
    config.asid = asid;
  }
  return asid;
}

static inline unsigned int fr_to_ms(unsigned int frames, unsigned int hz) {
  return (unsigned int) ( (u64)frames * 1000u / hz );
}

static inline unsigned int ms_to_fr(unsigned int ms, unsigned int hz) {
  return (unsigned int) ( (u64)ms * hz / 1000u );
}

static void stop_track(sc68_t * sc68, const int real_stop)
{
  assert(sc68);
  assert(sc68->disk);

  if (sc68->mix.buflen) {
    msg68_warning("libsc68: discard data -- *%d pcm*\n", sc68->mix.buflen);
  }

  sc68->mus             = 0;
  sc68->track           = 0;
  sc68->seek_to         = -1;
  sc68->track_to        = 0;

  if (real_stop) {
    sc68->loop_to         = 0;   /* keep the loop_to for all tracks */
    sc68->time.origin_ms  = 0;
  } else {
    sc68->time.origin_ms += sc68->time.elapsed_ms;
    TRACE68(sc68_cat,
            "libsc68: %s -- *%d ms*\n",
            "new track origin is", sc68->time.origin_ms);
  }

  sc68->time.elapsed_ms = 0;
  sc68->mix.pass_count  = 0;
  sc68->mix.loop_count  = 0;
  sc68->mix.bufpos      = 0;
  sc68->mix.buflen      = 0;
}

static int finish(sc68_t * sc68, addr68_t pc, int sr, uint68_t maxinst)
{
  int status;
  emu68_t * const emu68 = sc68->emu68;

  emu68->reg.pc  = pc;
  emu68->reg.sr  = sr;
  emu68->reg.a[7] = emu68->memmsk+1-16;
  emu68_pushl(emu68, 0);

  status = emu68_finish(emu68, maxinst);
  while (status == EMU68_STP) {
    sc68_debug(sc68,
               "libsc68: stop #$%04X ignored @$%X\n",
               emu68->reg.sr, emu68->reg.pc);
    status = emu68_finish(emu68, EMU68_CONT);
  }

  if (status != EMU68_NRM) {
    char irqname[32];
    const char * s;

    /* Might have some error stacked in emu68. */
    while (s = emu68_error_get(emu68), s)
      error_addx(sc68,"libsc68: %s\n", s);

    if (status == EMU68_HLT && (emu68->reg.sr & 0X3F00) == 0X2F00) {
      addr68_t vaddr;

      sc68->irq.vector = emu68->reg.sr & 0xFF;
      strcpy(irqname,"NC-");            /* Non-Catched Exception */
      except_name(sc68->irq.vector,irqname+3);
      vaddr = Lpeek(emu68, sc68->irq.vector * 4);
      if (sc68->irq.sr == -1) {
        sc68->irq.sr = Wpeek(emu68, emu68->reg.a[7]);
        sc68->irq.pc = Lpeek(emu68, emu68->reg.a[7]+2);
      }
      if (vaddr != INTR_ADDR + sc68->irq.vector * 8)
        strcpy(irqname,"CH-");
      if (sc68->irq.vector >= TRAP_VECTOR(0) &&
          sc68->irq.vector <= TRAP_VECTOR(15)) {
        int n = sc68->irq.vector-TRAP_VECTOR(0);
        if (trap_name[n])
          sprintf(irqname+3, "%s-$%X", trap_name[n], sc68->irq.sysfct);
      }
    } else if (status == EMU68_BRK && !emu68->instructions) {
      strcpy(irqname,"inst-overflow");
    } else {
      except_name(sc68->irq.vector,irqname);
    }
    error_addx(sc68,
               "libsc68: pass#%d @$%X"
               " %s (%02X)"
               " $%X/$%04X irq#%d (%s) @$%X\n",
               sc68->mix.pass_count, (unsigned) pc,
               emu68_status_name(status), status,
               (unsigned) emu68->reg.pc, (unsigned) emu68->reg.sr,
               sc68->irq.vector, irqname, (unsigned) sc68->irq.pc);
  }
  return status;
}

static int reset_emulators(sc68_t * sc68, const hwflags68_t hw)
{
  u8 * memptr;
  const int base = INTR_ADDR;
  int i;

  assert(sc68);
  assert(sc68->emu68);
  assert(hw);

  emu68_ioplug_unplug_all(sc68->emu68);
  emu68_mem_reset(sc68->emu68);

  if (hw & SC68_AGA) {
    TRACE68(sc68_cat," -> %s\n","add Paula hardware");
    emu68_ioplug(sc68->emu68, sc68->paulaio);
    TRACE68(sc68_cat," -> %s\n","set PAULA as interruptible");
    emu68_set_interrupt_io(sc68->emu68, sc68->paulaio);
  } else {
    if (hw & SC68_PSG) {
      TRACE68(sc68_cat," -> %s\n","add YM hardware");
      emu68_ioplug(sc68->emu68, sc68->ymio);
    }
    if (hw & (SC68_DMA|SC68_LMC)) {
      TRACE68(sc68_cat," -> %s\n","add MW (STE) hardware");
      emu68_ioplug(sc68->emu68, sc68->mwio);
    }
    TRACE68(sc68_cat," -> %s\n","add SHIFTER hardware");
    emu68_ioplug(sc68->emu68, sc68->shifterio);
    TRACE68(sc68_cat," -> %s\n","add MFP hardware");
    emu68_ioplug(sc68->emu68, sc68->mfpio);
    TRACE68(sc68_cat," -> %s\n","set MFP as interruption");
    emu68_set_interrupt_io(sc68->emu68, sc68->mfpio);
  }
  emu68_reset(sc68->emu68);

  /* disable that we should not need it */
  if (emu68_debugmode(sc68->emu68)) {
    TRACE68(sc68_cat," -> %s\n","clear 68k memory");
    emu68_memset(sc68->emu68,0,0,0);
    /* This is done by emu68_reset() */
    /* emu68_chkset(sc68->emu68,0,0,0); */
  }
  else
    /* Clear system variables */
    emu68_memset(sc68->emu68,0x400,0,INTR_ADDR-0x400);

  memptr = emu68_memptr(sc68->emu68,0,0x1000);

  TRACE68(sc68_cat," -> exception detection code @$%06x\n",
          (unsigned)base);

  /* Install non initialized exception detection code. */
  for (i=0; i<256; ++i) {
    int vector = base + i*8;

    /* setup vector [$000..$400] */
    memptr[ (i<<2) + 0 ] = vector >> 24;
    memptr[ (i<<2) + 1 ] = vector >> 16;
    memptr[ (i<<2) + 2 ] = vector >>  8;
    memptr[ (i<<2) + 3 ] = vector;

    /* Timer-C vector (69) might be programmed without setting the
     * vector. It is legitimate as the ST system uses this timer at
     * 200hz. We could check this frequence instead of acknowledge
     * this interruption.
     */

    /* install instruction stop #$2FNN */
    memptr[ vector + 0 ] = 0x4e;
    memptr[ vector + 1 ] = 0x72    /* + (i == 69) */;
    memptr[ vector + 2 ] = 0x2F;   /* magic SR value (see handler) */
    memptr[ vector + 3 ] = i;      /* vector number                */
    memptr[ vector + 4 ] = 0x4e;   /* RESET or RTE                 */
    memptr[ vector + 5 ] = 0x70;   /* Always RESET                 */
    memptr[ vector + 6 ] = 0x4e;   /* RTE                          */
    memptr[ vector + 7 ] = 0x73;
  }

  /* Install TOS trap emulator unless AMIGA */
  if (!(hw & SC68_AGA)) {
    int status;

    TRACE68(sc68_cat," -> Load TOS trap emulator @$%06x-$%06x\n",
            (unsigned) TRAP_ADDR, (unsigned) (TRAP_ADDR+sizeof(trap_func)-1));

    /* Ensure trap emulator do not override exception handler */
    assert(!overlap(INTR_ADDR, 256*8, TRAP_ADDR, sizeof(trap_func)));

    /* Install trap emulator */
    emu68_memput(sc68->emu68, TRAP_ADDR, trap_func, sizeof(trap_func));
    sc68->emu68->cycle = 0;
    TRACE68(sc68_cat," -> Running trap init code -- $%06x ...\n",
            (unsigned) TRAP_ADDR);
    status = finish(sc68, TRAP_ADDR, 0x2300, TRAP_MAX_INST);
    if ( status != EMU68_NRM ) {
      error_addx(sc68,
                 "libsc68: abnormal 68K status %d (%s) in trap code\n",
                 status, emu68_status_name(status));
      return SC68_ERROR;
    }
  }
  return SC68_OK;
}

static int load_replay(sc68_t * sc68, const char * replay, int a0)
{
  int err, size;
  vfs68_t * is;
  char rname[256];
  assert(sc68);
  assert(replay);

  TRACE68(sc68_cat, " -> external replay -- %s\n", replay);
  strcpy(rname,"sc68://replay/");
  strcat68(rname, replay, sizeof(rname)-1);
  rname[sizeof(rname)-1] = 0;
  is = uri68_vfs(rname, 1, 0);
  err = vfs68_open(is);
  err = err || (size = vfs68_length(is), size < 0);
  err = err || stream_read_68k(sc68, a0, is, size);
  vfs68_destroy(is);
  if (err) {
    error_add(sc68,
              "libsc68: failed to load external replay -- %s",
              rname);
    return SC68_ERROR;
  }
  TRACE68(sc68_cat," -> external replay -- [%06x-%06x]\n", a0, a0+size-1);
  return a0 + ((size + 1) & ~1);
}


static int aSIDifier(sc68_t * sc68, const music68_t * m)
{
  int res = 0;
  assert(is_sc68(sc68));

  /* No YM no aSID even if forced !*/
  if ( ! ( m->hwflags & SC68_PSG ) )
    goto done;

  /* Config is OFF and instance is OFF */
  if (config.asid == SC68_ASID_OFF && sc68->asid == SC68_ASID_OFF)
    goto done;

  /* Not forced and can't safely aSid */
  if ( ! ( sc68->asid & SC68_ASID_FORCE ) &&
       ! trk_can_asid(m) )
    goto done;

  /* Default timers assignment */
  res = MK4CC('A','C','D','B');

  /* In case of forced aSID try to assign free timer first. But
   * usually it won't help much.
   */
  if (m->hwflags & SC68_XTD) {
    if (m->hwflags & SC68_MFP_TA)       /* A ? */
      res = MK4CC('B','C','D','A');
    if (m->hwflags & SC68_MFP_TC)       /* C ? */
      res = MK4CC('A','B','D','C');
    if (m->hwflags & SC68_MFP_TD)
      res = MK4CC('A','C','B','D');     /* D ? */
  }

done:
  return res;
}


static int log2hz(int hz)
{
  if (hz < 75)
    hz = 0;
  else if (hz < 150)
    hz = 1;
  else
    hz = 2;
  return hz;
}

static int run_music_init(sc68_t * sc68, const music68_t * m, int a0, int a6)
{
  int status;

  assert(is_sc68(sc68));
  assert(m);

  /* Set 68K registers. */
  sc68->emu68->reg.d[0] = m->d0;
  /* unfortunatly the ste selection is kinda unsupported stuff... */
  sc68->emu68->reg.d[1] = !(m->hwflags & (SC68_DMA|SC68_LMC));
  sc68->emu68->reg.d[2] = m->datasz;
  sc68->emu68->reg.d[6] = log2hz(m->frq);

  sc68->emu68->reg.a[0] = a0;
  sc68->emu68->reg.a[6] = a6;           /* original replay address  */
  sc68->emu68->reg.d[7] = sc68->asid_timers;

  /* Run music init code. */
  sc68->emu68->cycle = 0;
  TRACE68(sc68_cat," -> %s\n","running music init code ...");
  status = finish(sc68, sc68->playaddr, 0x2300, INIT_MAX_INST);
  if ( status != EMU68_NRM ) {
    error_addx(sc68, "libsc68: abnormal 68K status %d (%s) in init code\n",
               status, emu68_status_name(status));
    return SC68_ERROR;
  }
  return SC68_OK;
}

static int change_track(sc68_t * sc68, int track)
{
  const disk68_t  * d;
  const music68_t * m;
  int         a0,a6;
  int         force_ms;
  int         loop;

  assert(has_track(sc68, track));
  stop_track(sc68, 0);

  loop = sc68->loop_to;
  assert(loop >= -1);

  d = sc68->disk;
  m = d->mus + track - 1;

  /* Ugly hack needed by some tools. Usually not compiled */
#ifdef WITH_FORCE_HW
  if (config.force_hw && config.force_hw != m->hwflags) {
    msg68_notice("Forcing hardware %03x(%s) -> %03x(%s)\n",
                 (unsigned)m->hwflags, hwtable[m->hwflags&7],
                 (unsigned)config.force_hw, hwtable[config.force_hw&7]);
    ((music68_t *)m)->hwflags = config.force_hw;
  }
#endif

  /* Reset 68K & IO */
  if (reset_emulators(sc68, m->hwflags) != SC68_OK)
    return SC68_ERROR;

  /* Set music replay address in 68K memory. */
  sc68->playaddr = a0 = m->a0;
  TRACE68(sc68_cat," -> play address -- $%06x\n", sc68->playaddr);

  sc68->asid_timers = aSIDifier(sc68, m);
  if (sc68->asid_timers) {
    TRACE68(sc68_cat," -> aSID timers -- %c%c%c%c\n",
            (int)(u8)(sc68->asid_timers >> 24),
            (int)(u8)(sc68->asid_timers >> 16),
            (int)(u8)(sc68->asid_timers >>  8),
            (int)(u8)(sc68->asid_timers >>  0));
    if (a0 = load_replay(sc68, "asidifier", a0), a0 == SC68_ERROR)
      return SC68_ERROR;
  }
  a6 = a0;

  /* Load external replay into 68K memory. */
  if (m->replay && (a0 = load_replay(sc68, m->replay, a0), a0 == SC68_ERROR))
    return SC68_ERROR;

  /* Copy music data into 68K memory */
  if (emu68_memput(sc68->emu68, a0, (u8 *)m->data, m->datasz)) {
    error_add(sc68,"libsc68: %s\n", emu68_error_get(sc68->emu68));
    return SC68_ERROR;
  }
  TRACE68(sc68_cat," -> music data -- [%06x-%06x]\n", a0, a0+m->datasz-1);

  /* Run music init code */
  if ( run_music_init(sc68, m, a0, a6) == SC68_ERROR )
    return SC68_ERROR;

  /* Ensure sampling rate */
  if (sc68->mix.spr <= 0)
    sc68->mix.spr = sc68_spr_def;
  if (sc68->mix.spr < SPR_MIN ||
      sc68->mix.spr > SPR_MAX) {
    error_addx(sc68,
               "libsc68: %s -- %dhz\n",
               "invalid sampling rate",
               sc68->mix.spr);
    return SC68_ERROR;
  }

  /* force length if asked or if we don't have a length */
  force_ms = d->force_ms;
  if (!force_ms && !m->first_fr)
    force_ms = sc68->time.def_ms;

  assert(m->loops >= 0);
  if (loop == SC68_DEF_LOOP)
    loop = m->loops > 0 ? m->loops : SC68_INF_LOOP;

  sc68->mix.loop_count = 0;
  sc68->mix.loop_total = (loop == SC68_INF_LOOP) ? 0 : loop;
  sc68->mix.pass_count = 0;

  if (force_ms > 0) {
    TRACE68(sc68_cat,"libsc68: disk force length -- *%d ms*\n", force_ms);
    //sc68->time.track_ms  = force_ms * sc68->mix.loop_total;
    //sc68->time.disk_ms   = sc68->time.track_ms * d->nb_mus;
    sc68->mix.pass_2loop = ms_to_fr(force_ms, m->frq);
    sc68->mix.pass_3loop = sc68->mix.pass_2loop;
    sc68->mix.pass_total = sc68->mix.loop_total * sc68->mix.pass_2loop;
  } else {
    sc68->mix.pass_2loop = m->first_fr;
    sc68->mix.pass_3loop = m->loops_fr;
    sc68->mix.pass_total = sc68->mix.loop_total > 0
      ? sc68->mix.pass_2loop + (sc68->mix.loop_total-1) * sc68->mix.pass_3loop
      : 0
      ;
    //sc68->time.track_ms = fr_to_ms(sc68->mix.pass_total, m->frq);
    //sc68->time.disk_ms  = calc_disk_len(d, sc68->loop_to);
  }

  TRACE68(sc68_cat," -> loops           : %u\n", sc68->mix.loop_total);
  TRACE68(sc68_cat," -> first pass ms   : %u\n",
          fr_to_ms(sc68->mix.pass_2loop, m->frq));
  TRACE68(sc68_cat," -> loops pass ms   : %u\n",
          fr_to_ms(sc68->mix.pass_3loop, m->frq));
  TRACE68(sc68_cat," -> origin ms       : %u\n", sc68->tinfo[track].len_ms);
  TRACE68(sc68_cat," -> track ms        : %u\n", sc68->tinfo[track].len_ms);
  TRACE68(sc68_cat," -> disk ms         : %u\n", sc68->tinfo[0].len_ms);

  /* $$$$ fix start time on the fly because it is broken ! */
  // m->start_ms = sc68->time.save_ms;
  // TRACE68(sc68_cat," -> start ms        : %u\n", m->start_ms);

  TRACE68(sc68_cat," -> first frames    : %u\n", sc68->mix.pass_2loop);
  TRACE68(sc68_cat," -> loops frames    : %u\n", sc68->mix.pass_3loop);
  TRACE68(sc68_cat," -> total frames    : %u\n", sc68->mix.pass_total);


  TRACE68(sc68_cat," -> replay rate     : %u\n", m->frq);
  sc68->mix.cycleperpass =
    ( m->frq % 50u == 0 && sc68->emu68->clock == EMU68_ATARIST_CLOCK)
    ? 160256u * 50u / m->frq    /* exact value for genuine Atari ST */
    : (sc68->emu68->clock / m->frq)
    ;
  TRACE68(sc68_cat," -> cycle (exact)   : %u\n", sc68->mix.cycleperpass);

  if (m->hwflags & SC68_PSG) {
    cycle68_t cycles;
    /* Make sure ym-cycle are mutiple of 32 as required by current
     * emulator.
     *
     * $$$ I am not sure this works in all cases of frequency but it
     * is should be ok as far as cpu and ym frequency are multiple.
     *
     * $$$ In fact I am now pretty sure it does not work if
     * frequencies are not multiple.
     */
    cycles = ymio_cycle_cpu2ym(sc68->ymio,sc68->mix.cycleperpass);
    cycles = (cycles+31) & ~31;
    sc68->mix.cycleperpass = ymio_cycle_ym2cpu(sc68->ymio,cycles);

    /* verify */
    cycles = ymio_cycle_cpu2ym(sc68->ymio,sc68->mix.cycleperpass);
    TRACE68(sc68_cat," -> ym cycles       : %u [%s]\n",
            (unsigned) cycles, strok68(cycles&31));
  }
  sc68->mix.cycleperpass = (sc68->mix.cycleperpass+31) & ~31;
  TRACE68(sc68_cat," -> cycle (round)   : %u\n", sc68->mix.cycleperpass);

  if (m->frq == 60 && sc68->shifterio) {
    TRACE68(sc68_cat," -> %s\n","Force shifter to 60Hz");
    shifterio_reset(sc68->shifterio,60);
  }

  /* Compute size of buffer needed for cycleperpass length at current rate. */
  if (1) {
    u64 len;
    len  = sc68->mix.spr;
    len *= sc68->mix.cycleperpass;
    len /= sc68->emu68->clock;
    sc68->mix.stdlen = (int) len;
    TRACE68(sc68_cat," -> std buffer len  : %u\n", sc68->mix.stdlen);
  }

  /* Compute *REAL* required size (in PCM) for buffer and realloc */
  if (1) {
    sc68->mix.bufreq = (m->hwflags & SC68_PSG)
      ? ymio_buffersize(sc68->ymio, sc68->mix.cycleperpass)
      : sc68->mix.stdlen
      ;
    TRACE68(sc68_cat," -> mix buffer len  : %u\n", sc68->mix.bufreq);

    /* Should not happen. Anyway it does not hurt. */
    if ((m->hwflags & SC68_AGA) && sc68->mix.stdlen > sc68->mix.bufreq)
      sc68->mix.bufreq = sc68->mix.stdlen;

    TRACE68(sc68_cat," -> required PCM buffer size -- *%u pcm*\n",
            sc68->mix.bufreq);
    TRACE68(sc68_cat," ->  current PCM buffer size -- *%u pcm*\n",
            sc68->mix.bufmax);

    if (sc68->mix.bufreq > sc68->mix.bufmax) {
      free(sc68->mix.buffer);
      sc68->mix.bufmax = 0;
      TRACE68(sc68_cat," -> Alloc new PCM buffer -- *%u pcm*\n",
              sc68->mix.bufreq);
      sc68->mix.buffer = malloc(sc68->mix.bufreq << 2);
      if (!sc68->mix.buffer) {
        error_add(sc68,"libsc68: %s\n", strerror(errno));
        return SC68_ERROR;
      }
      sc68->mix.bufmax = sc68->mix.bufreq;
    }
  }
  TRACE68(sc68_cat," -> buffer length -- %u pcm\n", sc68->mix.bufreq);

  sc68->mus           = m;
  sc68->track         = track;

  /* Setup internal info struct */
  if (sc68_music_info(sc68, &sc68->info, track, 0))
    return SC68_ERROR;

  TRACE68(sc68_cat, "libsc68: track ready -- %s %s - %s - %s\n",
          sc68->info.trk.time,
          sc68->info.artist,
          sc68->info.album,
          sc68->info.title);

  return SC68_OK;
}

static int check_track_range(sc68_t * const sc68,
                             const disk68_t * const disk,
                             const int track)
{
  if (track < 1 || track > disk->nb_mus) {
    error_addx(sc68, "libsc68: %s -- *%d*\n","track out of range", track);
    return -1;
  }
  return 0;
}

/** Start current music of current disk.
 */
static int apply_change_track(sc68_t * const sc68)
{
  int track/* , loop */;

  assert(has_disk(sc68));

  if (track = sc68->track_to, !track)
    return SC68_OK;
  /* loop = sc68->loop_to; */
  /* Don't cleat loop_to, it's needed for next tracks */
  sc68->track_to /* = sc68->loop_to */ = 0;

  /* -1 : stop */
  if (track == -1) {
    TRACE68(sc68_cat,"libsc68: %s\n","stop requested");
    sc68->mix.buflen = 0; /* warning removal in stop_track() */
    stop_track(sc68, 1);
    sc68->time.origin_ms = 0;
    return SC68_END;
  }
  TRACE68(sc68_cat,
          "libsc68: %s -- *%02d*\n",
          "change track requested", track);

  if (check_track_range(sc68, sc68->disk, track))
    return SC68_ERROR;

  if (change_track(sc68, track/* , loop */) != SC68_OK)
    return SC68_ERROR;

  return SC68_CHANGE;
}

int sc68_process(sc68_t * sc68, void * buf16st, int * _n)
{
  int ret;

  if (!is_sc68(sc68)) {
    ret = SC68_ERROR;
  } else if (!_n) {
    /* Flush internal PCM buffer and apply change track request. */
    ret = SC68_IDLE | apply_change_track(sc68);
  } else if (!buf16st) {
    ret = SC68_ERROR;
  } else {
    int n = *_n;
    ret = (n < 0) ? SC68_ERROR : SC68_IDLE;

    while (n > 0) {
      int len;

      /* No more pcm in internal buffer ... */
      if (!sc68->mix.buflen) {
        int status;

        /* Checking for loop */
        if (sc68->mix.pass_2loop && !--sc68->mix.pass_2loop) {
          sc68->mix.pass_2loop = sc68->mix.pass_3loop;
          sc68->mix.loop_count++;
          ret |= SC68_LOOP;
        }

        /* Checking for end */
        if (sc68->mix.pass_total &&
            sc68->mix.pass_count >= sc68->mix.pass_total) {
          int next_track = sc68->track+1;
          sc68->track_to =
            (sc68->disk->force_track || next_track > sc68->disk->nb_mus)
            ? -1                        /* stop */
            : next_track                /* next track */
            ;
          sc68->seek_to  = -1;
        }

        ret |= apply_change_track(sc68);
        if (ret & (SC68_END|SC68_CHANGE)) /* exit on error|end|change */
          break;
        ret &= ~SC68_IDLE;              /* No more idle */

        /* setup aSID */
        if (sc68->asid_timers)
          sc68->emu68->mem[sc68->playaddr+17] = -!!(sc68->asid & SC68_ASID_ON);

        /* Run 68K emulator */
        status = finish(sc68, sc68->playaddr+8, 0x2300, PLAY_MAX_INST);
        if (status == EMU68_NRM) {
          /* $$$ Fix some replays (tao's intensity 200 for one) that
             assumes the music driver is running under interruption
             and do not restore the SR by themself. Need to be sure
             this does not disrupt other musics. */
          sc68->emu68->reg.sr = 0x2300;
          status = emu68_interrupt(sc68->emu68, sc68->mix.cycleperpass);
        }
        if (status != EMU68_NRM) {
          error_addx(sc68,
                     "libsc68: abnormal 68K status %d (%s) in play pass %u\n",
                     status, emu68_status_name(status),
                     sc68->mix.pass_count);
          ret = SC68_ERROR;
          break;
        }

        /* Reset pcm pointer. */
        sc68->mix.bufpos = 0;
        sc68->mix.buflen = sc68->mix.bufreq;

        /* Fill pcm buufer depending on architecture */
        if (sc68->mus->hwflags & SC68_AGA) {
          /* Amiga - Paula */
          paula_mix(sc68->paula,(s32*)sc68->mix.buffer,sc68->mix.buflen);
          mixer68_blend_LR(sc68->mix.buffer, sc68->mix.buffer, sc68->mix.buflen,
                           sc68->mix.aga_blend, 0, 0);
        } else {
          if (sc68->mus->hwflags & SC68_PSG) {
            int err =
              ymio_run(sc68->ymio, (s32*)sc68->mix.buffer,
                       sc68->mix.cycleperpass);
            if (err < 0) {
              ret = SC68_ERROR;
              sc68->mix.buflen = 0;
              break;
            }
            sc68->mix.buflen = err;
          } else {
            mixer68_fill(sc68->mix.buffer, sc68->mix.buflen=sc68->mix.bufreq, 0);
          }

          if (sc68->mus->hwflags & (SC68_DMA|SC68_LMC))
            /* STE / MicroWire */
            mw_mix(sc68->mw, (s32 *)sc68->mix.buffer, sc68->mix.buflen);
          else
            /* Else simply process with left channel duplication. */
            mixer68_dup_L_to_R(sc68->mix.buffer, sc68->mix.buffer,
                               sc68->mix.buflen, 0);
        }

        /* Advance time */
        calc_pos(sc68);
        sc68->mix.pass_count++;
      }

      assert(sc68->mix.buflen > 0);

      /* Copy to destination buffer. */
      len = sc68->mix.buflen <= n ? sc68->mix.buflen : n;
      mixer68_copy((u32 *)buf16st,sc68->mix.buffer+sc68->mix.bufpos,len);
      buf16st = (u32 *)buf16st + len;
      sc68->mix.bufpos += len;
      sc68->mix.buflen -= len;
      n                -= len;
    }
    *_n -= n;
  }
  return ret;
}

int sc68_is_our_uri(const char * uri, const char *exts, int * is_remote)
{
  assert(!"TO DO");
  return 0;
  /* return file68_is_our_uri(uri,exts,is_remote); */
}

static int load_disk(sc68_t * sc68, disk68_t * d, int free_on_close)
{
  if (!is_sc68(sc68) || !is_disk(d))
    goto error;

  if (sc68->disk) {
    error_add(sc68,"libsc68: %s","disk already loaded");
    goto error;
  }

  sc68->tobe3 = !!free_on_close;
  sc68->disk  = d;
  sc68->track = 0;
  sc68->mus   = 0;

  if (sc68_play(sc68, SC68_DEF_TRACK, SC68_DEF_LOOP) < 0)
    goto error;

  music_info(sc68, &sc68->info, d, sc68->track_to, sc68->loop_to);

  return 0;

error:
  free(d);
  return -1;
}

int sc68_load(sc68_t * sc68, vfs68_t * is)
{
  return load_disk(sc68, file68_load(is), 1);
}

int sc68_load_uri(sc68_t * sc68, const char * uri)
{
  return load_disk(sc68, file68_load_uri(uri), 1);
}

int sc68_load_mem(sc68_t * sc68, const void * buffer, int len)
{
  return load_disk(sc68, file68_load_mem(buffer, len), 1);
}


sc68_disk_t sc68_load_disk(vfs68_t * is)
{
  return (sc68_disk_t) file68_load(is);
}

sc68_disk_t sc68_load_disk_uri(const char * uri)
{
  return (sc68_disk_t) file68_load_uri(uri);
}

sc68_disk_t sc68_disk_load_mem(const void * buffer, int len)
{
  return (sc68_disk_t) file68_load_mem(buffer, len);
}

void sc68_disk_free(sc68_disk_t disk)
{
  if (is_disk(disk))
    file68_free(disk);                    /* $$$ check this */
}

int sc68_open(sc68_t * sc68, sc68_disk_t disk)
{
  if (!disk) {
    sc68_close(sc68);
    return -1; /* Not an error but notifiy no disk has been loaded */
  }
  if (!sc68) {
    return -1;
  }
  return load_disk(sc68, disk, 0);
}

void sc68_close(sc68_t * sc68)
{
  if (sc68 && sc68->disk) {
    sc68->mix.buflen = 0; /* warning removal in stop_track() */
    stop_track(sc68, 1);
    if (sc68->tobe3)
      file68_free((disk68_t *)sc68->disk);
    sc68->tobe3     = 0;
    sc68->disk      = 0;
  }
}

int sc68_play(sc68_t * sc68, int track, int loop)
{
  const disk68_t * d;

  if (!has_disk(sc68))
    return -1;
  d = sc68->disk;

  /* Legacy, better sc68_cntl()  */
  if (track == SC68_CUR_TRACK) {
    msg68x_warning(sc68, "libsc68: %s\n",
                   "deprecated use sc68_play() to get track and loops");
    return loop == SC68_CUR_LOOP
      ? sc68->mix.loop_count
      : sc68->track
      ;
  }

  /* Choose track */
  if (d->force_track > 0) {
    /* Mean track has been forced while the disk was loaded (via
     * URI) */
    track = d->force_track;
    TRACE68(sc68_cat,"libsc68: disk's forced track -- *%02d*\n",track);
  } else if (track == SC68_DEF_TRACK) {
    track = d->def_mus + 1;
#ifdef WITH_FORCE
    if (in_range(d, sc68->cfg_track)) {
      /* track forced in config only applied if default track was
       * asked and it is not out of range. */
      track = sc68->cfg_track;
      TRACE68(sc68_cat,"libsc68: config's forced track -- *%02d*\n", track);
    }
#endif
  }

  /* Unless disk has an overide. */
  if (d->force_loops) {
    loop = d->force_loops;
    TRACE68(sc68_cat,"libsc68: disk has a forced loop -- %02d\n",loop);
  }
#ifdef WITH_FORCE
  else if (loop == SC68_DEF_LOOP && sc68->cfg_loop != SC68_DEF_LOOP) {
    loop = sc68->cfg_loop;
    TRACE68(sc68_cat,"libsc68: config has a forced loop -- %02d\n", loop);
  }
#endif

  /* Check track range. */
  if (check_track_range(sc68, sc68->disk, track)) {
    return -1;
  } else {
    int i;

    sc68->tinfo[0].org_ms = 0;
    sc68->tinfo[0].len_ms = 0;
    for (i=1; i <= d->nb_mus; ++i) {
      sc68->tinfo[i].org_ms = sc68->tinfo[0].len_ms;
      sc68->tinfo[i].len_ms = calc_track_len(d,i,loop);
      sc68->tinfo[0].len_ms += sc68->tinfo[i].len_ms;
    }
  }

  /* Set change track. Real track loading occurs during process thread to
     avoid multi-threading problems. */
  sc68->track_to = track;
  sc68->seek_to  = -1;
  sc68->loop_to  = loop;

  return 0;
}

int sc68_stop(sc68_t * sc68)
{
  if (!sc68 || !sc68->disk) {
    return -1;
  }
  sc68->track_to = -1;
  sc68->seek_to  = -1;
  return 0;
}

static unsigned int calc_pos(sc68_t * const sc68)
{
  u64 ms;
  ms  = sc68->mix.pass_count;
  ms *= sc68->mix.cycleperpass * 1000u;
  ms /= sc68->emu68->clock;

  assert(ms == (u64)(unsigned int)ms);  /* check overflow */

  return sc68->time.elapsed_ms = (unsigned int) ms;
}

static int get_pos(sc68_t * sc68, int origin)
{
  int pos = -1, track;
  if ( (track = sc68->track) > 0 ) {
    switch (origin) {

    case SC68_POS_PLAY:                 /* Position since sc68_play() */
      pos = sc68->time.origin_ms;
      break;

    case SC68_POS_DISK:                 /* Position in the entire disk */
      pos = sc68->tinfo[track].org_ms;
      break;

    default:
      assert(!"invalid origin");
    case SC68_POS_TRACK:                /* Position in current track */
      pos = 0;
    }
    pos += sc68->time.elapsed_ms;
    /* Add correction for PCMs already sent in the current buffer */
    pos += (sc68->mix.bufpos * 1000u) / sc68->mix.spr;
  }
  return pos;
}

static unsigned int calc_track_len(const disk68_t * d, int track, int loop)
{
  const music68_t * m;
  unsigned force_ms;

  assert(in_disk(d,track));

  m = d->mus + track - 1;

  if (loop == SC68_DEF_LOOP)
    loop = m->loops;
  if (loop <= 0)
    return 0;                          /* return 0 on infinite loop */

  /* force length if asked or if we don't have a length */
  force_ms = d->force_ms;
  if (!force_ms && !m->first_fr)
    force_ms = config.def_time_ms;
  if (force_ms)
    return loop * force_ms;

  return fr_to_ms(m->first_fr + (loop-1) * m->loops_fr, m->frq);
}

static int calc_disk_len(const disk68_t * disk, const int loop)
{
  int i, len = 0;

  assert(is_disk(disk));
  for (i = 1; i <= disk->nb_mus; ++i)
    len += calc_track_len(disk,i,loop);
  return len;
}

static int get_len(sc68_t * sc68, int track)
{
  /* int len = -1; */

  assert(track == SC68_DSK_TRACK || has_track(sc68,track));
  assert(SC68_DSK_TRACK == 0);
  return (unsigned) track <= (unsigned) sc68->disk->nb_mus
    ? sc68->tinfo[track].len_ms
    : -1;
}


/* Get a tag. Favor track tag over disk one */
static
char * tag_get_any(const disk68_t * d, int track, const char * key)
{
  const char * val = file68_tag_get(d,track,key);
  if (!val)
    val = file68_tag_get(d,0,key);
  return (char *)val;
}

static void music_info(sc68_t * sc68, sc68_music_info_t * f, const disk68_t * d,
                       int track, int loops)
{
  const music68_t * m;
  int i, maxtag = &f->_lasttag - &f->album;

  assert(f);
  assert(in_disk(d, track));
  m = d->mus + track - 1;
  f->tracks      = d->nb_mus;
  f->addr        = m->a0;
  f->rate        = m->frq;
  f->replay      = m->replay ? m->replay : "built-in";
  f->dsk.track   = d->def_mus+1;
  f->dsk.time_ms = sc68
    ? sc68->tinfo[0].len_ms : calc_disk_len(d, loops);
  strtime68(f->dsk.time, f->tracks, (f->dsk.time_ms+999u)/1000u);
  f->dsk.ym      = !!(d->hwflags & SC68_PSG);
  f->dsk.ste     = !!(d->hwflags & (SC68_DMA|SC68_LMC));
  f->dsk.amiga   = !!(d->hwflags & SC68_AGA);
  f->dsk.asid    = dsk_can_asid(d) > 0;
  f->dsk.hw      = hwtable[f->dsk.ym+(f->dsk.ste<<1)+(f->dsk.amiga<<2)];
  f->dsk.tags    = file68_tag_count(d, 0);
  f->dsk.tag     = (sc68_tag_t *) d->tags.array;

  f->trk.track   = track;
  f->trk.time_ms = sc68
    ? sc68->tinfo[track].len_ms : calc_track_len(d, track, loops);
  strtime68(f->trk.time, track,(f->trk.time_ms+999u)/1000u);

  f->trk.ym    = !! (m->hwflags & SC68_PSG);
  f->trk.ste   = !! (m->hwflags & (SC68_DMA|SC68_LMC));
  f->trk.amiga = !! (m->hwflags & SC68_AGA);
  f->trk.asid  = trk_can_asid(m);
  f->trk.hw    = hwtable[f->trk.ym+(f->trk.ste<<1)+(f->trk.amiga<<2)];
  f->trk.tags  = file68_tag_count(d, track);
  f->trk.tag   = (sc68_tag_t *) m->tags.array;


  /* Clear all tags. */
  for (i=0; i < maxtag; ++i)
    (&f->album)[i] = 0;

  f->album     = d->tags.tag.title.val;
  f->title     = m->tags.tag.title.val;
  f->artist    = m->tags.tag.artist.val;
  f->format    = d->tags.tag.genre.val;
  f->genre     = m->tags.tag.genre.val;
  f->year      = tag_get_any(d, track, TAG68_YEAR);
  f->ripper    = tag_get_any(d, track, TAG68_RIPPER);
  f->converter = tag_get_any(d, track, TAG68_CONVERTER);

  /* Replace null pointers by empty strings. */
  for (i=0; i < maxtag; ++i)
    if (!(&f->album)[i])
      (&f->album)[i] = "";

}


int sc68_music_info(sc68_t * sc68, sc68_music_info_t * info, int track,
                    sc68_disk_t disk)
{
  const disk68_t  * d;
  int loaded;

  d = get_dt(sc68, &track, disk);
  if (!d || !info)
    return error_add(sc68, "libsc68: %s\n", "invalid parameter");

  /* Asking for the track being played by sc68, simply copy internal
   * info struct, unless off course it is the one being filled.
   */
  loaded = sc68 && d == sc68->disk;
  if (loaded && track == sc68->track && info != &sc68->info) {
    *info = sc68->info;
  } else {
    music_info(0, info, d, track, loaded ? sc68->loop_to : SC68_DEF_LOOP);
  }

  return 0;
}

/* resolve disk and track number */

static sc68_disk_t get_dt(sc68_t * sc68, int * ptr_track, sc68_disk_t disk)
{
  disk68_t * d;
  int track;

  if (disk)
    d = disk;
  else if (is_sc68(sc68) && sc68->disk)
    d = (sc68_disk_t)sc68->disk;
  else
    d = 0;
  if (!is_disk(d))
    return 0;

  switch (track = *ptr_track) {
  case SC68_DEF_TRACK:
    track = d->def_mus + 1;
    break;
  case SC68_CUR_TRACK:
    track = (sc68 && d == sc68->disk) ? sc68->track : -1;
  case SC68_DSK_TRACK:
    break;
  }
  if (track != SC68_DSK_TRACK && !in_range(d,track))
    d = 0;
  else
    *ptr_track = track;
  return d;
}

char * sc68_tag(sc68_t * sc68, const char * key, int track, sc68_disk_t disk)
{
  disk68_t * d;
  if (!key || !(d = get_dt(sc68, &track, disk)))
    return 0;
  return file68_tag(d, track, key);
}

int sc68_tag_get(sc68_t * sc68, sc68_tag_t * tag, int track,
                 sc68_disk_t disk)
{
  disk68_t * d;
  if (!tag || !(d = get_dt(sc68, &track, disk)))
    return -1;
  tag->val = (char *) file68_tag_get(d, track, tag->key);
  return -!tag->val;
}

int sc68_tag_enum(sc68_t * sc68, sc68_tag_t * tag, int track, int idx,
                  sc68_disk_t disk)
{
  disk68_t  * d;
  if (!tag || !(d = get_dt(sc68, &track, disk)))
    return -1;
  return file68_tag_enum(d, track, idx,
                         (const char **)&tag->key,
                         (const char **)&tag->val);
}

vfs68_t * sc68_vfs(const char * uri, int mode, int argc, ...)
{
  vfs68_t * vfs;
  va_list list;

  va_start(list, argc);
  vfs = uri68_vfs_va(uri, mode, argc, list);
  va_end(list);

  return vfs;
}

const char * sc68_error(sc68_t * sc68)
{
  return is_sc68(sc68)
    ? sc68->errstr
    : sc68_errstr
    ;
}

static int error_addx(sc68_t * sc68, const char * fmt, ...)
{
  va_list list;
  char * errstr;
  int n;

  n = strncmp(fmt,"libsc68: ",9) ? 0 : 9; /* skip "libsc68: " */
  errstr = is_sc68(sc68) ? sc68->errbuf : sc68_errstr;
  va_start(list, fmt);
  n = vsnprintf(errstr, ERRMAX, fmt+n, list);
  va_end(list);
  if (n > 0 && errstr[n-1] == '\n')
    errstr[n-1] = 0;
  if (sc68)
    sc68->errstr = sc68->errbuf;
  va_start(list,  fmt);
  error68_va(fmt,list);
  va_end(list);
  return -1;
}

static int error_add(sc68_t * sc68, const char * fmt, const char * errstr)
{
  if (is_sc68(sc68)) {
    sc68->errstr = errstr;
    error68x(sc68,fmt,errstr);
  } else
    error68(fmt,errstr);
  return -1;
}

static void sc68_debug(sc68_t * sc68, const char * fmt, ...)
{
  va_list list;

  assert(null_or_sc68(sc68));
  va_start(list,fmt);
  if (!sc68)
    msg68_va(sc68_cat, fmt, list);
  else if (is_magic(sc68))
    msg68x_va(sc68_cat, sc68, fmt, list);
  va_end(list);
}

/* static int track_now_or_next(const sc68_t * sc68) { */
/*   if (!is_disk(sc68)) */
/*     return -1; */
/*   if (sc68->track_to) */
/*     return sc68->track_to; */
/*   return sc68->track; */
/* } */

int sc68_cntl(sc68_t * sc68, int fct, ...)
{
  int res;
  va_list list;

  assert(null_or_sc68(sc68));

  if (!null_or_sc68(sc68))
    return -1;

  res = -1;
  va_start(list, fct);
  switch (fct) {
    /* Functions accepting null */

  case SC68_NOP:
    break;

  case SC68_GET_LAST:
    res = SC68_CNTL_LAST;
    break;

  case SC68_GET_NAME:
    *va_arg(list, char **) = sc68 ? sc68->name : appname;
    res = 0;
    break;

  case SC68_CONFIG_LOAD:
    if (!config_load(appname)) {
      config_apply(sc68);
      res = 0;
    }
    break;

  case SC68_CONFIG_SAVE:
    /* config_update(sc68); */
    if (!config_save(appname))
      res = 0;
    break;

  case SC68_ENUM_OPT: {
    option68_t * opt = option68_enum(va_arg(list, int));
    res = option68_type(opt);
    if (res != -1)
      *va_arg(list,const  char **) = opt->name;
  } break;

  case SC68_GET_OPT: {
    option68_t * opt = option68_get(va_arg(list, const char *), opt68_ISSET);
    if (opt) {
      if (opt->type == opt68_STR)
        *va_arg(list,const  char **) = opt->val.str;
      else
        *va_arg(list, int *) = opt->val.num;
      res = 0;
    }
  } break;

  case SC68_SET_OPT_STR: {
    option68_t * opt = option68_get(va_arg(list, const char *), opt68_ALWAYS);
    res =
      option68_set(opt, va_arg(list, const char *), opt68_ALWAYS, opt68_APP);
  } break;

  case SC68_SET_OPT_INT: {
    option68_t * opt = option68_get(va_arg(list, const char *), opt68_ALWAYS);
    res =
      option68_iset(opt, va_arg(list, int), opt68_ALWAYS, opt68_APP);
  } break;

  case SC68_GET_PCM:
    res = get_pcm_fmt(sc68);
    break;

  case SC68_SET_PCM:
    res = set_pcm_fmt(sc68, va_arg(list, int));
    break;

  case SC68_GET_SPR:
    res = get_spr(sc68);
    break;

  case SC68_SET_SPR:
    res = set_spr(sc68, va_arg(list, int));
    break;

  case SC68_GET_ASID:
    res = get_asid(sc68);
    break;

  case SC68_SET_ASID:
    res = set_asid(sc68, va_arg(list, int));
    break;

  case SC68_DIAL: {
    void      * data = va_arg(list, void *);
    sc68_dial_f cntl = va_arg(list, sc68_dial_f);
    res = dial68(data, cntl);
  } break;

  default:
    if (!sc68)
      break;

    switch (fct) {
      /* Functions not accepting null */

    case SC68_CAN_ASID:
      res = can_asid(sc68,va_arg(list,int));
      break;

    case SC68_GET_DISK:
      *va_arg(list, const disk68_t **)
        = is_disk(sc68->disk) ? sc68->disk : 0;
      res = 0;
      break;

    case SC68_GET_TRACKS:
      res = is_disk(sc68->disk) ? sc68->disk->nb_mus : -1;
      break;

    case SC68_GET_TRACK:
      res = sc68->track;
      break;

    case SC68_GET_DEFTRK:
      if (is_disk(sc68->disk))
        res = sc68->disk->def_mus+1;
      break;

    case SC68_GET_LOOPS:
      if (sc68->track > 0)
        res = sc68->mix.loop_total;
      break;

    case SC68_GET_LOOP:
      if (sc68->track > 0)
        res = sc68->mix.loop_count;
      break;

    case SC68_GET_LEN:
      if (sc68->track > 0)
        res = get_len(sc68, sc68->track);
      break;

    case SC68_GET_TRKLEN:
      if (is_disk(sc68->disk))
        res = get_len(sc68, va_arg(list,int));
      break;

    case SC68_GET_DSKLEN:
      if (is_disk(sc68->disk))
        res = get_len(sc68, SC68_DSK_TRACK);
      break;

    case SC68_GET_POS:
      res = get_pos(sc68, SC68_POS_TRACK);
      break;

    case SC68_GET_ORG:
      if (sc68->track > 0)
        res = sc68->tinfo[sc68->track].org_ms;
      break;

    case SC68_GET_TRKORG:
      if (is_disk(sc68->disk)) {
        int track = va_arg(list,int);
        if (in_range(sc68->disk,track))
          res = sc68->tinfo[track].org_ms;
      } break;

    case SC68_GET_DSKPOS:
      res = get_pos(sc68, SC68_POS_DISK);
      break;

    case SC68_GET_PLAYPOS:
      res = get_pos(sc68, SC68_POS_PLAY);
      break;

    case SC68_GET_COOKIE:
      *va_arg(list, void **) = sc68->cookie;
      res = 0;
      break;

    case SC68_SET_COOKIE:
      sc68->cookie = va_arg(list, void *);
      res = 0;
      break;

    case SC68_EMULATORS:
      *va_arg(list, void **) = &sc68->emu68;
      res = 0;
      break;

    case SC68_SET_POS:
    default:
      res = error_addx(sc68,
                       "libsc68: %s (%d)\n",
                       "invalid control function", fct);
      break;
    }
  }
  va_end(list);
  return res;
}

const char * sc68_mimetype(void)
{
  return SC68_MIMETYPE;
}
