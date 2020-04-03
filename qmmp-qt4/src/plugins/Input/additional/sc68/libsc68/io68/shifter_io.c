/*
 * @file    shifter_io.c
 * @brief   Atari ST shifter I/O plugin (50/60hz and resolution)
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

#include "emu68/struct68.h"

typedef struct {
  io68_t io;
  u8 data_0a;
  u8 data_60;
} shifter_io68_t;

static int68_t _shifter_readB(shifter_io68_t * const shifterio,
                              addr68_t addr)
{
  switch ((u8)addr) {
  case 0x0A:
    return shifterio->data_0a;
  case 0x60:
    return shifterio->data_60;
  }
  return 0;
}

static void _shifter_writeB(shifter_io68_t * const shifterio,
                            addr68_t addr, int68_t v)
{
  switch ((u8)addr) {
  case 0x0A:
    shifterio->data_0a = v;
    break;
  case 0x60:
    shifterio->data_60 = v;
    break;
  }
}


static void shifter_readB(io68_t * const io68)
{
  emu68_t * const emu68 = io68->emu68;
  emu68->bus_data =
    _shifter_readB((shifter_io68_t*)io68, emu68->bus_addr);
}


static void shifter_readW(io68_t * const io68)
{
  emu68_t * const emu68 = io68->emu68;
  io68->emu68->bus_data =
    0
    | (_shifter_readB((shifter_io68_t*)io68, emu68->bus_addr+0)<<8)
    | (_shifter_readB((shifter_io68_t*)io68, emu68->bus_addr+1)   );
}

static void shifter_readL(io68_t * const io68)
{
  emu68_t * const emu68 = io68->emu68;
  emu68->bus_data =
    0
    | (_shifter_readB((shifter_io68_t*)io68, emu68->bus_addr+0)<<24)
    | (_shifter_readB((shifter_io68_t*)io68, emu68->bus_addr+1)<<16)
    | (_shifter_readB((shifter_io68_t*)io68, emu68->bus_addr+2)<<8 )
    | (_shifter_readB((shifter_io68_t*)io68, emu68->bus_addr+3)    );
}

static void shifter_writeB(io68_t * const io68)
{
  emu68_t * const emu68 = io68->emu68;
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr, emu68->bus_data);
}

static void shifter_writeW(io68_t * const io68)
{
  emu68_t * const emu68 = io68->emu68;
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr+0, emu68->bus_data>>8);
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr+1, emu68->bus_data   );
}

static void shifter_writeL(io68_t * const io68)
{
  emu68_t * const emu68 = io68->emu68;
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr+0, emu68->bus_data>>24);
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr+1, emu68->bus_data>>16);
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr+2, emu68->bus_data>> 8);
  _shifter_writeB((shifter_io68_t*)io68,
                  emu68->bus_addr+3, emu68->bus_data    );
}

static interrupt68_t *shifter_interrupt(io68_t * const io,
                                        const cycle68_t cycle)
{
  return 0;
}

static cycle68_t shifter_next_interrupt(io68_t * const io,
                                        const cycle68_t cycle)
{
  return IO68_NO_INT;
}

static void _shifter_reset(shifter_io68_t * const shifter, const int hz)
{
  switch (hz) {
  case 60:
    shifter->data_0a = 0xfc;
    shifter->data_60 = 0x00;
    break;
  case 70:
    shifter->data_0a = 0xfe;
    shifter->data_60 = 0x02;
    break;
  case 50:
  default:
    shifter->data_0a = 0xfe;
    shifter->data_60 = 0x00;
  }
}

static int shifter_reset(io68_t * const io)
{
  if (io) {
    _shifter_reset((shifter_io68_t *)io, 50);
    return 0;
  } else {
    return -1;
  }
}

static void shifter_adjust_cycle(io68_t * const io, const cycle68_t cycle)
{
}

static void shifter_destroy(io68_t * const io)
{
  emu68_free(io);
}

static io68_t const shifter_io =
{
  0,
  "Shifter",
  0xFFFF8200, 0xFFFF82FF,
  shifter_readB,  shifter_readW,  shifter_readL,
  shifter_writeB, shifter_writeW, shifter_writeL,
  shifter_interrupt, shifter_next_interrupt,
  shifter_adjust_cycle,
  shifter_reset,
  shifter_destroy
};

int shifterio_init(int * argc, char ** argv)
{
  argc = argc; argv = argv;
  return 0;
}

void shifterio_shutdown(void)
{
}

io68_t * shifterio_create(emu68_t * const emu68, int hz)
{
  shifter_io68_t * const io =
    emu68
    ? emu68_alloc(sizeof(*io))
    : 0
    ;

  if (io) {
    io->io = shifter_io;
    _shifter_reset(io, hz);
  }
  return &io->io;
}

int shifterio_reset(io68_t * const io, int hz)
{
  if (io) {
    _shifter_reset((shifter_io68_t *)io, hz);
    return 0;
  } else {
    return -1;
  }
}
