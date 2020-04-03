/*
 * @file    ym_dump.c
 * @brief   YM-2149 emulator - YM-2149 dump engine
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

#include <stdio.h>

#if 0
#ifdef HAVE_CONFIG_OPTION68_H
# include "config_option68.h"
#else
# include "default_option68.h"
#endif
#endif

#include "ymemul.h"
#include "emu68/assert68.h"
#include <sc68/file68_msg.h>
#include <sc68/file68_str.h>
#include <sc68/file68_opt.h>

/* command line options option */
/* static const char prefix[] = "sc68-"; */
#define prefix 0
static const char engcat[] = "ym-dump";
static option68_t opts[] = {
  OPT68_BOOL(prefix,"ym-clean-dump",engcat,
             "dump only effective bits", 0, 0)
};
#undef prefix

void ym_dump_add_options(void)
{
  const int n_opts = sizeof(opts) / sizeof(*opts);

  opts[0].hide = 1;                     /* hide -clean-dump */

  /* Register ym-dump options */
  option68_append(opts, n_opts);

  /* Default option values */
  option68_iset(opts+0, 1, opt68_NOTSET, opt68_CFG);
}

static int reset(ym_t * const ym, const cycle68_t ymcycle)
{
  ym_dump_t * const dump = &ym->emu.dump;
  dump->base_cycle = 0;
  dump->pass       = 0;
  return 0;
}

static
int buffersize(const ym_t * const ym, const cycle68_t ymcycles)
{
  return (int) (ymcycles * ym->hz / ym->clock);
}

#define CYCLE_DIGITS 10
#define  PASS_DIGITS 6

static
int run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
  static const char hex[16] = {
    '0','1','2','3','4','5','6','7',
    '8','9','A','B','C','D','E','F'
  };

  static const u8 allbits[14] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
  };

  static const u8 effbits[14] = {
    0xFF,0x0F, 0xFF,0x0F, 0xFF,0x0F,    /* pulse periods */
    0x1F,                               /* Noise */
    0x3F,                               /* Mixer (ignoring io port direction bit */
    0x1F,0x1F,0x1F,                     /* Volume */
    0xFF,0xFF,                          /* env period */
    0xF                                 /* env shape */
  };

  ym_dump_t * const dump = &ym->emu.dump;
  cycle68_t curcycle;
  u64       longcycle;
  int       i, len, ymreg[16], reg_mute, mix_mute;
  const u8 *bits = opts->val.num ? effbits : allbits;


  char tmp [128], * buf;
  ym_event_t * ptr = ym->event_buf;

  /* voice mute bit#0=A / bit#1=B / bit#2=C */
  mix_mute
    = ( ( ym->voice_mute >> 0  ) & 1 )
    | ( ( ym->voice_mute >> 5  ) & 2 )
    | ( ( ym->voice_mute >> 10 ) & 4 )
    ;

  /* mixer (reg#7) tone and noise mute */
  mix_mute = ( mix_mute | ( mix_mute << 3 ) ) ^ 077;

  /* register to ignore (1 bit per registers). Ignoring volume and
   * tone registers of muted voices. */
  reg_mute
    = ( -( ( mix_mute >> 0 ) & 1 ) & 0x103)
    | ( -( ( mix_mute >> 1 ) & 1 ) & 0x20c)
    | ( -( ( mix_mute >> 2 ) & 1 ) & 0x430)
    ;

  /* mark registers as not accessed yet. */
  for ( i = 0; i < 16; ++i ) ymreg[i] = -1;

  if (ptr == ym->event_ptr) {
    /* $$$ DIRTY TRICK: nothing happen but we style need to print at
     * least one line, so let just pretend a false access to register
     * 15. */
    ptr->ymcycle = 0;
    ptr->reg = 15;
    ptr->val = 00;
    ++ym->event_ptr;
  }

  /* Walk the events */
  while (ptr < ym->event_ptr) {
    curcycle  = ptr->ymcycle;
    longcycle = dump->base_cycle + (u64) curcycle;

    /* Walk all events at this cycle */
    do {
      assert( (unsigned int) ptr->reg < 16 );
      ymreg[ptr->reg & 15] = ptr->val & 255;
    } while (++ptr < ym->event_ptr && ptr->ymcycle == curcycle);

    buf = tmp;

    /* dump pass number */
    for ( i = (PASS_DIGITS-1)*4; i >= 0; i -= 4)
      *buf++ = hex [ 15 & (int)(dump->pass >> i) ];
    *buf++ = ' ';

    /* dump cycle number */
    for ( i = (CYCLE_DIGITS-1)*4; i >= 0; i -= 4)
      *buf++ = hex [ 15 & (int)(longcycle >> i) ];

    /* dump register list */
    for ( i = 0; i < 14; ++i ) {

      /* deal with muted voices */
      if ( reg_mute & (1<<i) )
        ymreg[i] = -1;                  /* Act as if never accessed */
      else if (i == YM_MIXER && ymreg[i] >= 0)
        ymreg[i] |= mix_mute;           /* Mask tone and noise. */

      *buf++ = "- "[!i];
      if (ymreg[i] < 0){
        *buf++ = '.';
        *buf++ = '.';
      } else {
        int v = ymreg[i] & bits[i];
        *buf++ = hex[ v >> 4 ];
        *buf++ = hex[ v & 15 ];
      }
      ymreg[i] = -1;
    }
    *buf++ = 0;

    /* Dump only if actually active */
    if (dump->active) puts(tmp);
  }

  /* Reset event list */
  ym->event_ptr = ym->event_buf;

  /* null terminated string and align to 32-bit */
  dump->base_cycle += (uint64_t) ymcycles;
  dump->pass++;

  /* clear output PCMs */
  len = buffersize(ym, ymcycles);
  for (i=0; i<len; ++i) {
    output[i] = 0;
  }
  return len;
}


static
void cleanup(ym_t * const ym)
{
}

int ym_dump_active(ym_t * const ym, int val)
{
  int code = -1;
  if (ym) {
    ym_dump_t * const dump = &ym->emu.dump;
    code = dump->active;
    if (val != -1) {
      dump->active = !!val;
    }
  }
  return code;
}

int ym_dump_setup(ym_t * const ym)
{
  ym_dump_t * const dump = &ym->emu.dump;
  int err = 0;

  /* fill callback functions */
  ym->cb_cleanup       = cleanup;
  ym->cb_reset         = reset;
  ym->cb_run           = run;
  ym->cb_buffersize    = buffersize;
  ym->cb_sampling_rate = (void*)0;
  dump->base_cycle     = 0;
  dump->active         = 1;
  dump->pass           = 0;

  return err;
}
