/*
 * @file    ym_emul.c
 * @brief   YM-2149 emulator
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
# include "config.h"
#endif
#include "default.h"
#include "ymemul.h"
#include "emu68/assert68.h"
#include <sc68/file68_msg.h>
#include <sc68/file68_opt.h>
#include <string.h>

#ifndef BREAKPOINT68
# define BREAKPOINT68 assert(!"breakpoint")
#endif

#ifndef DEBUG_YM_O
# define DEBUG_YM_O 0
#endif
#define YMHD "ym-2149: "
int ym_cat = msg68_DEFAULT;            /* For debug message */
int ym_default_chans = 7;              /* Active channels */
int ym_output_level = 0xCAFE;          /* PCM scaling */
int ym_cur_volmodel = YM_VOL_QUERY;    /* Invalid value */

#include "ym_linear_table.c"
#include "ym_atarist_table.c"

/** 3 channels output table.
 *  Using a table for non linear mixing.
 */
static s16 ymout5[32*32*32];

/* ,-----------------------------------------------------------------.
 * |                         Yamaha reset                            |
 * `-----------------------------------------------------------------'
 */

int ym_reset(ym_t * const ym, const cycle68_t ymcycle)
{
  int ret = -1;

  static const struct ym2149_reg_s init_regs = {
    0xff, 0x0f, 0xff, 0x0f, 0xff, 0x0f, /* tone period A,B,C */
    0x1f, 0xff,                         /* noise period & mixer */
    0x00, 0x00, 0x00,                   /* Volume A,B,C */
    0xFF, 0xFF,                         /* envelop period */
    0x0A,                               /* envelop shape */
    0,0                                 /* I/O port a,b */
  };

  if (ym) {
    /* Reset registers */
    ym->shadow.name = ym->reg.name = init_regs;
    ym->ctrl = 0;

    /* Run emulator specific reset callback. */
    if (ym->cb_reset) {
      ym->cb_reset(ym,ymcycle);
    }

    /* Reset event lists */
    ym->event_ptr = ym->event_buf;
    ym->event_ovf = 0;

    ret = 0;
  }

  return ret;
}


/* ,-----------------------------------------------------------------.
 * |                          Yamaha init                            |
 * `-----------------------------------------------------------------'
 */

/* Select default engine */
#ifndef YM_ENGINE
# define YM_ENGINE YM_ENGINE_BLEP
#endif

/* Select default volume table */
#ifndef YM_VOLMODEL
# define YM_VOLMODEL YM_VOL_ATARIST
#endif

/* Default parameters */
static ym_parms_t default_parms;

/* Max output level for volume tables. */


static const char f_pulse[]  = "pulse";
static const char f_blep[]   = "blep";
static const char f_dump[]   = "dump";
static const char * f_engines[] = { f_blep, f_pulse, f_dump };

static const char f_atari[]  = "atari";
static const char f_linear[] = "linear";
static const char * f_volumes[] = { f_atari, f_linear };

static int onchange_engine(const option68_t *opt, value68_t * val)
{
  static int engs[3] = { YM_ENGINE_BLEP, YM_ENGINE_PULS, YM_ENGINE_DUMP };
  assert(val->num >= 0 && val->num < 3);
  if (val->num >= 0 && val->num < 3) {
    ym_engine(0, engs[val->num]);
    return 0;
  }
  return -1;
}

static int onchange_volume(const option68_t *opt, value68_t * val)
{
  static int vols[2] = { YM_VOL_ATARIST, YM_VOL_LINEAR };
  assert(val->num >= 0 && val->num < 2);
  if (val->num >= 0 && val->num < 2) {
    default_parms.volmodel = vols[val->num];
    return 0;
  }
  return -1;
}

static int onchange_ymchans(const option68_t *opt, value68_t * val)
{
  val->num = ym_default_chans = val->num & 7;
  return 0;
}

/* Command line options */
/* static const char prefix[] = "sc68-"; */
#define prefix 0

static const char engcat[] = "ym-2149";
static option68_t opts[] = {
  OPT68_ENUM(prefix,"ym-engine",engcat,"set ym-2149 engine",
             f_engines,sizeof(f_engines)/sizeof(*f_engines),
             1,onchange_engine),
  OPT68_ENUM(prefix,"ym-volmodel",engcat,
             "set ym-2149 volume model",
             f_volumes,sizeof(f_volumes)/sizeof(*f_volumes),
             1,onchange_volume),
  OPT68_IRNG(prefix,"ym-chans",engcat,
             "set ym-2149 active channel {bit#0:A,bit#1:B,bit#2:C}",
             0,7,1,onchange_ymchans)
};
#undef prefix

static const char * ym_engine_name(int emul);
static const char * ym_volmodel_name(int model);

/* Force output level in valid range */
static void check_output_level(void)
{
  if (ym_output_level < 0)
    ym_output_level = 0;
  else if (ym_output_level >= 0x10000)
    ym_output_level = 0xFFFF;
}


int ym_init(int * argc, char ** argv)
{
  /* Debug */
  ym_cat = msg68_cat("ym","ym-2149 emulator",DEBUG_YM_O);

  /* Setup default */
  default_parms.engine   = YM_ENGINE;
  default_parms.volmodel = YM_VOLMODEL;
  default_parms.clock    = YM_CLOCK_ATARIST;
  default_parms.hz       = SPR_DEF;

  /* Register ym options */
  option68_append(opts,sizeof(opts)/sizeof(*opts));

  /* Default option values */
  option68_set (opts+0, ym_engine_name(default_parms.engine),
                opt68_NOTSET, opt68_CFG);
  option68_set (opts+1, ym_volmodel_name(default_parms.volmodel),
                opt68_NOTSET, opt68_CFG);
  option68_iset(opts+2, ym_default_chans, opt68_NOTSET, opt68_CFG);

  /* Add ym-puls options */
  ym_puls_add_options();

  /* Add ym-dump options */
  ym_dump_add_options();

  /* Add ym-blep options */
  ym_blep_add_options();

  /* Parse options. */
  *argc = option68_parse(*argc,argv);

  check_output_level();

  /* Set volume table (unique for all instance) */
  switch (default_parms.volmodel) {
  case YM_VOL_LINEAR:
    ym_create_5bit_linear_table(ymout5, ym_output_level);
    ym_cur_volmodel = YM_VOL_LINEAR;
    break;
  case YM_VOL_DEFAULT:
  case YM_VOL_ATARIST:
  default:
    ym_create_5bit_atarist_table(ymout5, ym_output_level);
    ym_cur_volmodel = YM_VOL_ATARIST;
  }

  return 0;
}

void ym_shutdown(void)
{
}


/* ,-----------------------------------------------------------------.
 * |                         Run emulation                           |
 * `-----------------------------------------------------------------'
 */
int ym_run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
  if (!ymcycles) {
    return 0;
  }

  /*
   * Currently ymcycles must be multiple of 32.
   * 32 cycles will generate 4 samples @ 250000hz. It helps to
   * simplify the first filter code (half-level buzz trick).
   */
  if ( (ymcycles&31) || !output)  {
    return -1;
  }

  return ym->cb_run(ym,output,ymcycles);
}


/* ,-----------------------------------------------------------------.
 * |                         Write YM register                       |
 * `-----------------------------------------------------------------'
 */

#define ELTOF(A) ( sizeof(A) / sizeof(*A) )

void ym_writereg(ym_t * const ym,
                 const int val, const cycle68_t ymcycle)
{
  const int reg = ym->ctrl;
  const ym_event_t * const event_end = ym->event_buf+ELTOF(ym->event_buf);

  if ( (unsigned int)reg < 16 ) {
    assert( reg >= 0 && reg < 16 );
    ym->shadow.index[reg] = val;
    if (ym->event_ptr >= event_end)
      ++ym->event_ovf;
    else {
      ym->event_ptr->ymcycle = ymcycle;
      ym->event_ptr->reg = reg;
      ym->event_ptr->val = val;
      ++ym->event_ptr;
    }
  }
}


/* ,-----------------------------------------------------------------.
 * |                  Adjust YM-2149 cycle counters                  |
 * `-----------------------------------------------------------------'
 */
void ym_adjust_cycle(ym_t * const ym, const cycle68_t ymcycles)
{
  if (ym && ymcycles) {
    ym_event_t * event;

    /* Should not be run before events have been flushed or
     * processed. It's not really an error, but with the current
     * implementation it should not be happening. */
    assert(ym->event_ptr == ym->event_buf);

    /* Do the job anyway */
    for (event = ym->event_buf; event < ym->event_ptr; ++event) {
      assert(event->ymcycle >= ymcycles);
      event->ymcycle -= ymcycles;
    }
  }
}


/* ,-----------------------------------------------------------------.
 * |                  Yamaha get activated voices                    |
 * `-----------------------------------------------------------------'
 */

extern const int ym_smsk_table[];       /* declared in ym_puls.c */
int ym_active_channels(ym_t * const ym, const int clr, const int set)
{
  int v = 0;
  if (ym) {
    const int voice_mute = ym->voice_mute;
    v = ( voice_mute & 1 ) | ( (voice_mute>>5) & 2 ) | ( (voice_mute>>10) & 4);
    v = ( (v & ~clr ) | set ) & 7;
    ym->voice_mute = ym_smsk_table[v];
    TRACE68(ym_cat, YMHD "active channels -- *%c%c%c*\n",
            (v&1)?'A':'.', (v&2)?'B':'.', (v&4)?'C':'.');
  }
  return v;
}

/* ,-----------------------------------------------------------------.
 * |                        Engine selection                         |
 * `-----------------------------------------------------------------'
 */

static
const char * ym_engine_name(int emul)
{
  switch (emul) {
  case YM_ENGINE_PULS:    return f_pulse;
  case YM_ENGINE_BLEP:    return f_blep;
  case YM_ENGINE_DUMP:    return f_dump;
  }
  return 0;
}

int ym_engine(ym_t * const ym, int engine)
{
  switch (engine) {

  case YM_ENGINE_QUERY:
    /* Get current value. */
    engine = ym ? ym->engine : default_parms.engine;
    break;

  default:
    /* Invalid values */
  case YM_ENGINE_DEFAULT:
    /* Default values */
    engine = default_parms.engine;
  case YM_ENGINE_PULS:
  case YM_ENGINE_BLEP:
  case YM_ENGINE_DUMP:
    /* Valid values */
    if (!ym) {
      default_parms.engine = engine;
      TRACE68(ym_cat,
              YMHD "default engine -- *%s*\n",
              ym_engine_name(engine));
    } else {
      ym->engine = engine;
    }
    break;
  }
  return engine;
}


/* ,-----------------------------------------------------------------.
 * |                           Master clock                          |
 * `-----------------------------------------------------------------'
 */

int ym_clock(ym_t * const ym, int clock)
{
  switch (clock) {

  case YM_CLOCK_QUERY:
    clock = ym ? ym->clock : default_parms.clock;
    break;

  case YM_CLOCK_DEFAULT:
    clock = default_parms.clock;

  default:
    clock = YM_CLOCK_ATARIST;
    if (!ym) {
      default_parms.clock = clock;
      TRACE68(ym_cat,
              YMHD "default clock -- *%u*\n",
              (unsigned int) clock);
    } else {
      clock = ym->clock;
    }
    break;
  }
  return clock;
}


/* ,-----------------------------------------------------------------.
 * |                           Volume model                          |
 * `-----------------------------------------------------------------'
 */

static
const char * ym_volmodel_name(int model)
{
  switch (model) {
  case YM_VOL_LINEAR:  return f_linear;
  case YM_VOL_ATARIST: return f_atari;
  }
  return 0;
}

int ym_volume_model(ym_t * const ym, int model)
{
  /* Set volume table (currently unique for all instances) */

  switch (model) {

  case YM_VOL_QUERY:
    model = default_parms.volmodel;
    break;

  default:
  case YM_VOL_DEFAULT:
    model = default_parms.volmodel;
  case YM_VOL_LINEAR:
  case YM_VOL_ATARIST:
    assert(model == YM_VOL_LINEAR || model == YM_VOL_ATARIST);
    if (ym)
      ym->volmodel = model;             /* does nothing ATM */
    if (model != ym_cur_volmodel) {
      ym_cur_volmodel = model;
      check_output_level();
      if ( model == YM_VOL_LINEAR )
        ym_create_5bit_linear_table(ymout5, ym_output_level);
      else
        ym_create_5bit_atarist_table(ymout5, ym_output_level);
      TRACE68(ym_cat,
              YMHD "set volume table -- *%s $%04x*\n",
              ym_volmodel_name(model), ym_output_level);
    }
    break;
  }
  return model;
}


/* ,-----------------------------------------------------------------.
 * |                        Output sampling rate                     |
 * `-----------------------------------------------------------------'
 */

int ym_sampling_rate(ym_t * const ym, const int chz)
{
  int hz = chz;
  switch (hz) {

  case YM_VOL_QUERY:
    hz = ym ? ym->hz : default_parms.hz;
    break;

  case YM_VOL_DEFAULT:
    hz = default_parms.hz;

  default:
    if (hz < SPR_MIN) hz = SPR_MIN;
    if (hz > SPR_MAX) hz = SPR_MAX;
    if (ym->cb_sampling_rate)
      hz = ym->cb_sampling_rate(ym,hz);
    if (ym)
      ym->hz = hz;
    else
      default_parms.hz = hz;
    TRACE68(ym_cat,
            YMHD "%s sampling rate -- *%dhz*\n",
            ym ? "select" : "default", hz);
  }
  return hz;
}


/* ,-----------------------------------------------------------------.
 * |               Configure default or emulator instance            |
 * `-----------------------------------------------------------------'
 */

int ym_configure(ym_t * const ym, ym_parms_t * const parms)
{
  assert(ym);
  assert(parms);
  parms->engine   = ym_engine(ym, parms->engine);
  parms->volmodel = ym_volume_model(ym, parms->volmodel);
  parms->clock    = ym_clock(ym, parms->clock);
  parms->hz       = ym_sampling_rate(ym, parms->hz);
  return 0;
}


/* ,-----------------------------------------------------------------.
 * |                  Setup / Cleanup emulator instance              |
 * `-----------------------------------------------------------------'
 */

int ym_setup(ym_t * const ym, ym_parms_t * const parms)
{
  ym_parms_t * const p = parms ? parms : &default_parms;
  int err = -1;

  /* engine */
  if (p->engine == YM_ENGINE_DEFAULT) {
    p->engine = default_parms.engine;
  }

  /* sampling rate */
  if (p->hz == 0) {
    p->hz = default_parms.hz;
  }

  /* clock */
  switch (p->clock) {
  case YM_CLOCK_ATARIST:
    break;
  case YM_CLOCK_DEFAULT:
  default:
    p->clock = default_parms.clock;
  }

  TRACE68(ym_cat,
          YMHD "setup -- engine:%d rate:%d clock:%d level:%d\n",
          p->engine,p->hz,p->clock,256);

  if (ym) {
    ym->ymout5      = ymout5;
    ym->clock       = p->clock;
    ym->voice_mute  = ym_smsk_table[7 & ym_default_chans];
    /* clearing sampling rate callback ensure requested rate to be in
       valid range. */
    ym->cb_sampling_rate = 0;
    ym_sampling_rate(ym, p->hz);
    ym->engine = p->engine;

    switch (p->engine) {
    case YM_ENGINE_PULS:
      err = ym_puls_setup(ym);
      break;

    case YM_ENGINE_BLEP:
      err = ym_blep_setup(ym);
      break;

    case YM_ENGINE_DUMP:
      err = ym_dump_setup(ym);
      break;

    default:
      assert(!"invalid ym-engine");
      err = -1;
    }
    if (!err)
      TRACE68(ym_cat,
              YMHD "engine -- *%s*\n", ym_engine_name(ym->engine));

    /* at this point specific sampling rate callback can be call */
    ym_sampling_rate(ym, ym->hz);
  }

  /* Just for info print */
  ym_active_channels(ym,0,0);

  return err ? err : ym_reset(ym, 0);
}


/** Destroy an Yamaha-2149 emulator instance.
 */
void ym_cleanup(ym_t * const ym)
{
  TRACE68(ym_cat,YMHD "%s\n", "cleanup");
  if (ym) {
    if (ym->event_ovf) {
      msg68_critical(YMHD "write access buffer has overflow -- *%u*\n",
                     ym->event_ovf);
      assert(!"YM event list overflow");
    }
    if (ym->cb_cleanup)
      ym->cb_cleanup(ym);
  }
}

/** Get required output buffer size.
 */
uint68_t ym_buffersize(const ym_t * const ym, const cycle68_t ymcycles)
{
  return ym->cb_buffersize(ym,ymcycles);
}
