/*
 * @file    ym_io.c
 * @brief   YM-2149 I/O plugin
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

#include "ym_io.h"
#include "ymemul.h"

typedef struct
{
  io68_t io;
  struct {
    int68_t  mul;
    uint68_t div;
  } clock;

  ym_t ym;
} ym_io68_t;

/* Convert cpu-cycle to ym-cycle. */
static inline
cycle68_t cycle_cputoym(const ym_io68_t * const ymio, const cycle68_t cycle)
{
  if (!ymio->clock.div) {
    return ymio->clock.mul < 0
      ? (cycle >> -ymio->clock.mul)
      : (cycle <<  ymio->clock.mul)
      ;
  } else {
    u64 cycle64 = cycle;
    cycle64 *= ymio->clock.mul;
    cycle64 /= ymio->clock.div;
    return (cycle68_t) cycle64;
  }
}

/* Convert ym-cycle to cpu-cycle. */
static inline
cycle68_t cycle_ymtocpu(const ym_io68_t * const ymio, const cycle68_t cycle)
{
  if (!ymio->clock.div) {
    return ymio->clock.mul < 0
      ? (cycle << -ymio->clock.mul)
      : (cycle >>  ymio->clock.mul)
      ;
  } else {
    u64 cycle64 = cycle;
    cycle64 *= ymio->clock.div;
    cycle64 /= ymio->clock.mul;
    return (cycle68_t) cycle64;
  }
}


static inline
int _readB(ym_io68_t * const ymio,
           const addr68_t addr, const cycle68_t ymcycle)
{
  return (addr&3)
    ? 0
    : ym_readreg(&ymio->ym, ymcycle)
    ;
}


static void ymio_readB(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  ym_io68_t * const ymio = (ym_io68_t *)io;
  cycle68_t ymcycle = cycle_cputoym(ymio,emu68->cycle);
  emu68->bus_data =
    _readB(ymio,emu68->bus_addr,ymcycle);
}

static inline
int _readW(ym_io68_t * const ymio,
           const addr68_t addr, const cycle68_t ymcycle)
{
  return (addr&3)
    ? 0
    : (ym_readreg(&ymio->ym, ymcycle)<<8)
    ;
}

static void ymio_readW(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  ym_io68_t * const ymio = (ym_io68_t *)io;
  const cycle68_t ymcycle = cycle_cputoym(ymio,emu68->cycle);
  emu68->bus_data =
    _readW(ymio,emu68->bus_addr,ymcycle);
}

static inline
int68_t _readL(ym_io68_t * const ymio,
               const addr68_t addr, const cycle68_t ymcycle)
{
  return (_readW(ymio,addr,ymcycle) << 16) | _readW(ymio,addr+2,ymcycle);
}

static void ymio_readL(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  ym_io68_t * const ymio = (ym_io68_t *)io;
  cycle68_t ymcycle = cycle_cputoym(ymio,emu68->cycle);
  emu68->bus_data =
    _readL(ymio, emu68->bus_addr, ymcycle);
}

static inline
void _writeB(ym_io68_t * const ymio,
             const addr68_t  addr, const int68_t v, const cycle68_t ymcycle)
{
  if (!(addr&2)) {
    /* CTRL register */
    ymio->ym.ctrl = (u8)v;
  } else {
    /* DATA register */
    ym_writereg(&ymio->ym, (u8)v, ymcycle);
  }
}

static void ymio_writeB(io68_t * const io)
{
  const emu68_t * const emu68 = io->emu68;
  ym_io68_t * const ymio = (ym_io68_t *)io;
  const cycle68_t ymcycle = cycle_cputoym(ymio,emu68->cycle);
  _writeB(ymio,emu68->bus_addr,emu68->bus_data,ymcycle);
}

static inline
void _writeW(ym_io68_t * const ymio,
             const addr68_t  addr, const int68_t v, const cycle68_t ymcycle)
{
  _writeB(ymio,addr,v>>8,ymcycle);
  /* that's the way it is ! */
  /* _writeB(ymio, addr+1, v, cycle); */
}

static void ymio_writeW(io68_t * const io)
{
  const emu68_t * const emu68 = io->emu68;
  ym_io68_t * const ymio = (ym_io68_t *)io;
  const cycle68_t ymcycle = cycle_cputoym(ymio,emu68->cycle);
  _writeW(ymio,emu68->bus_addr,emu68->bus_data,ymcycle);
}

static void ymio_writeL(io68_t * const io)
{
  const emu68_t * const emu68 = io->emu68;
  const addr68_t addr = emu68->bus_addr;
  const int68_t  data = emu68->bus_data;
  ym_io68_t * const ymio = (ym_io68_t *)io;
  const cycle68_t ymcycle = cycle_cputoym(ymio,emu68->cycle);
  _writeW(ymio, addr  , data>>16, ymcycle);
  _writeW(ymio, addr+2, data,     ymcycle);
}

static interrupt68_t *
ymio_interrupt(io68_t * const io, const cycle68_t cycle)
{
  return 0;
}

static cycle68_t
ymio_nextinterrupt(io68_t * const io, const cycle68_t cycle)
{
  return IO68_NO_INT;
}

static int ymio_reset(io68_t * const io)
{
  ym_io68_t * const ymio = (ym_io68_t *)io;
  const cycle68_t ymcycle = cycle_cputoym(ymio,io->emu68->cycle);
  ym_reset(&ymio->ym,ymcycle);
  return 0;
}

static void ymio_adjust_cycle(io68_t * const io, const cycle68_t cycle)
{
  ym_io68_t * const ymio = (ym_io68_t *)io;
  const cycle68_t ymcycle = cycle_cputoym(ymio,io->emu68->cycle);
  ym_adjust_cycle(&ymio->ym, ymcycle);
}


static void ymio_destroy(io68_t * const io)
{
  if (io) {
    ym_io68_t * const ymio = (ym_io68_t *)io;
    ym_cleanup(&ymio->ym);
    emu68_free(io);
  }
}

static io68_t ym_io =
{
  0,
  "YM-2149",
  0xFFFF8800, 0xFFFF88FF,
  ymio_readB,  ymio_readW,  ymio_readL,
  ymio_writeB, ymio_writeW, ymio_writeL,
  ymio_interrupt, ymio_nextinterrupt,
  ymio_adjust_cycle,
  ymio_reset,
  ymio_destroy
};

int ymio_init(int * argc, char ** argv)
{
  return ym_init(argc,argv);
}

void ymio_shutdown(void)
{
  ym_shutdown();
}

static int get_power_of_2(int v)
{
  int p,r;
  for ( p=0,r=1; r; ++p, r<<=1 ) {
    if (r == v) return p;
  }
  return -1;
}


io68_t * ymio_create(emu68_t * const emu68, ym_parms_t * const parms)
{
  ym_io68_t * ymio = 0;

  if (emu68) {
    ymio = emu68_alloc(sizeof(*ymio));
    if (ymio) {
      int s,p;
      uint68_t quotient,numerator,denominator;
      ymio->io = ym_io;
      ym_setup(&ymio->ym, parms);

      if (emu68->clock > ymio->ym.clock) {
        numerator = emu68->clock;
        denominator = ymio->ym.clock;
        s = -1;
      } else {
        numerator = ymio->ym.clock;
        denominator = emu68->clock;
        s = 1;
      }
      quotient = numerator / denominator;

      if (quotient * denominator == numerator &&
          (p = get_power_of_2(quotient), p >= 0)) {
        ymio->clock.div = 0;
        ymio->clock.mul = s*p;
      } else {
        ymio->clock.div = emu68->clock;
        ymio->clock.mul = ymio->ym.clock;
      }
    }
  }
  return &ymio->io;
}

int ymio_sampling_rate(io68_t * const io, int sampling_rate)
{
  return io
    ? ym_sampling_rate(&((ym_io68_t*)io)->ym,sampling_rate)
    : sampling_rate
    ;
}

ym_t * ymio_emulator(io68_t * const io)
{
  return io
    ? &((ym_io68_t*)io)->ym
    : 0
    ;
}

uint68_t ymio_buffersize(const io68_t * const io, const cycle68_t cycles)
{
  if (io) {
    const ym_io68_t * const ymio = (const ym_io68_t *)io;
    return ym_buffersize(&ymio->ym,cycle_cputoym(ymio,cycles));
  }
  return 0;
}

int ymio_run(const io68_t * const io, s32 * output, const cycle68_t cycles)
{
  if (io) {
    ym_io68_t * const ymio = (ym_io68_t *)io;
    return ym_run(&ymio->ym,output,cycle_cputoym(ymio,cycles));
  }
  return 0;
}

/** Convert a cpu-cycle to ym-cycle. */
cycle68_t ymio_cycle_cpu2ym(const io68_t * const io, const cycle68_t cycles)
{
  ym_io68_t * const ymio = (ym_io68_t *)io;
  return cycle_cputoym(ymio,cycles);
}

/** Convert a ym-cycle to cpu-cycle. */
cycle68_t ymio_cycle_ym2cpu(const io68_t * const io, const cycle68_t cycles)
{
  ym_io68_t * const ymio = (ym_io68_t *)io;
  return cycle_ymtocpu(ymio,cycles);
}

/** Set/Get active channels */
int ymio_active_channels(const io68_t * const io, const int clr, const int set)
{
  ym_io68_t * const ymio = (ym_io68_t *)io;
  return ymio
    ? ym_active_channels( &ymio->ym, clr, set)
    : -1
    ;
}
