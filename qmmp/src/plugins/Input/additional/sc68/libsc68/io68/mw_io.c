/*
 * @file    mw_io.c
 * @brief   MicroWire I/O plugin (STE soundchip)
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

#include "mw_io.h"
#include "mwemul.h"
#include "emu68/struct68.h"
#include "emu68/assert68.h"

#include <sc68/file68_msg.h>
extern int mw_cat;
#define MWHD "ste-mw : "

typedef struct {
  io68_t io;
  mw_t mw;
} mw_io68_t;

static int68_t _mw_readB(mw_io68_t * const mwio, const u8 addr)
{
  const uint_t ct = mwio->mw.ct >> mwio->mw.ct_fix;

  /* Just need to be sure no one reads these word registers in byte */
  assert(addr != MW_DATA);
  assert(addr != MW_CTRL);

  switch (addr) {
  case MW_CTH:
    return (u8) ( ct >> 16 );
  case MW_CTM:
    return (u8) ( ct >> 8  );
  case MW_CTL:
    /* whatever mono/stereo the counter always return even address */
    return ct & 0xFE;
  }
  return (addr >= 0 && addr < 64)
    ? mwio->mw.map[addr]
    : 0
    ;
}

static int68_t _mw_readW(mw_io68_t * const mwio, const u8 addr)
{
  switch (addr) {
  case MW_DATA: case MW_CTRL:
    return ( mwio->mw.map[addr+0] << 8 ) + mwio->mw.map[addr+1];
    break;
  }
  return _mw_readB(mwio, mwio->io.emu68->bus_addr+1);
}

static void mwio_readB(io68_t * const io)
{
  io->emu68->bus_data =
    _mw_readB((mw_io68_t *)io, io->emu68->bus_addr);
  TRACE68(mw_cat, MWHD "read BYTE [%06x] => %02x\n",
          (unsigned)io->emu68->bus_addr & 0xFFFFFF,
          (unsigned)io->emu68->bus_data & 0xFF);
}

static void mwio_readW(io68_t * const io)
{
  io->emu68->bus_data =
    _mw_readW((mw_io68_t *)io, io->emu68->bus_addr);
  TRACE68(mw_cat, MWHD "read WORD [%06x] => %04x\n",
          (unsigned)io->emu68->bus_addr & 0xFFFFFF,
          (unsigned)io->emu68->bus_data & 0xFFFF);
}

static void mwio_readL(io68_t * const io)
{
  io->emu68->bus_data =
    ( _mw_readW((mw_io68_t *)io, io->emu68->bus_addr+0) << 16 )
    |
    ( _mw_readW((mw_io68_t *)io, io->emu68->bus_addr+2)       )
    ;
  TRACE68(mw_cat, MWHD "read LONG [%06x] => %08x\n",
          (unsigned)io->emu68->bus_addr & 0xFFFFFF,
          (unsigned)io->emu68->bus_data & 0xFFFFFFFF);
}

static void _mw_writeB(mw_io68_t * const mwio, const u8 addr, int68_t v)
{
  /* Just need to be sure no one writes these word registers in byte */
  assert(addr != MW_DATA);
  assert(addr != MW_CTRL);

  /* Skip even line in any case. */
  if ( ! ( addr & 1 ) ) {
    return;
  }

  switch (addr) {

  case MW_ACTI:
    v &= 3;                             /* ??? should we ? */

    /* Reload internal counters
     *
     * $$$ Should we do this whatever the value or only if dma is
     * started ? This is probably not a great deal !
     */
    mwio->mw.ct  = mw_counter_read(&mwio->mw,MW_BASH);
    mwio->mw.end = mw_counter_read(&mwio->mw,MW_ENDH);
    break;

  case MW_CTH: case MW_CTM: case MW_CTL:
    /* write to counter register does nothing */
    return;

  }

  if (addr >= 0 && addr < 64)
    mwio->mw.map[addr] = v;
}

static void _mw_writeW(mw_io68_t * const mwio, const u8 addr, int68_t v)
{
  if (addr == MW_CTRL) {
    mwio->mw.map[MW_CTRL+0] = v >> 8;
    mwio->mw.map[MW_CTRL+1] = v;
  } else if (addr == MW_DATA) {
    mwio->mw.map[MW_DATA+0] = v >> 8;
    mwio->mw.map[MW_DATA+1] = v;
    mw_command(&mwio->mw);
  } else if ( !(addr & 1) ) {
    _mw_writeB(mwio, addr+1, v);
  }
}

static void _mw_writeL(mw_io68_t * const mwio, const u8 addr, int68_t v)
{
  if (addr == MW_DATA) {
    /* Write both Ctrl and Data registers .... My guess is that
     * someone tries to run a command this way, which is not the
     * proper way to do it in the real world. Anyway we'll do as if it
     * works.
     */
    mwio->mw.map[MW_DATA+0] = v >> 24;
    mwio->mw.map[MW_DATA+1] = v >> 16;
    mwio->mw.map[MW_CTRL+2] = v >>  8;
    mwio->mw.map[MW_CTRL+3] = v;
    mw_command(&mwio->mw);
  } else if ( !(addr & 1) ) {
    /* Any other (even) long access are translate to word access */
    _mw_writeW(mwio, addr+0, v >> 16);
    _mw_writeW(mwio, addr+2, v);
  }
}

static void mwio_writeB(io68_t * const io)
{
  TRACE68(mw_cat, MWHD "write BYTE [%06x] <= %02x\n",
          (unsigned)io->emu68->bus_addr & 0xFFFFFF,
          (unsigned)io->emu68->bus_data & 0xFF);
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr, io->emu68->bus_data);
}

static void mwio_writeW(io68_t * const io)
{
  TRACE68(mw_cat, MWHD "write WORD [%06x] <= %04x\n",
          (unsigned)io->emu68->bus_addr & 0xFFFFFF,
          (unsigned)io->emu68->bus_data & 0xFFFF);
  _mw_writeW((mw_io68_t *)io,
             io->emu68->bus_addr, io->emu68->bus_data);
}

static void mwio_writeL(io68_t * const io)
{
  TRACE68(mw_cat, MWHD "write LONG [%06x] <= %08x\n",
          (unsigned)io->emu68->bus_addr & 0xFFFFFF,
          (unsigned)io->emu68->bus_data & 0xFFFFFFFF);
  _mw_writeL((mw_io68_t *)io,
             io->emu68->bus_addr, io->emu68->bus_data);
}

static interrupt68_t * mwio_interrupt(io68_t * const io, cycle68_t cycle)
{
  return 0;
}

static cycle68_t mwio_next_interrupt(io68_t * const io, cycle68_t cycle)
{
  return IO68_NO_INT;
}

static void mwio_adjust_cycle(io68_t * const io, cycle68_t cycle)
{
}

static int mwio_reset(io68_t * const io)
{
  return mw_reset(&((mw_io68_t *)io)->mw);
}

static void mwio_destroy(io68_t * const io)
{
  if (io) {
    mw_cleanup(&((mw_io68_t *)io)->mw);
    emu68_free(io);
  }
}

static io68_t mw_io = {
  0,
  "STE-Sound",
  0xFFFF8900, 0xFFFF8925,
  mwio_readB,  mwio_readW,  mwio_readL,
  mwio_writeB, mwio_writeW, mwio_writeL,
  mwio_interrupt, mwio_next_interrupt,
  mwio_adjust_cycle,
  mwio_reset,
  mwio_destroy,
};

int mwio_init(int * argc, char ** argv)
{
  return mw_init(argc, argv);
}

void mwio_shutdown(void)
{
  mw_shutdown();
}

io68_t * mwio_create(emu68_t * const emu68, mw_parms_t * const parms)
{
  mw_io68_t * mwio = 0;

  if (emu68) {
    mwio = emu68_alloc(sizeof(*mwio));
    if (mwio) {
      mw_setup_t setup;
      if (parms) {
        setup.parms = *parms;
      } else {
        setup.parms.engine = MW_ENGINE_DEFAULT;
        setup.parms.hz     = 0;
      }
      setup.mem     = emu68->mem;
      setup.log2mem = emu68->log2mem;
      mwio->io      = mw_io;
      mw_setup(&mwio->mw, &setup);
    }
  }
  return &mwio->io;
}

mw_t * mwio_emulator(io68_t * const io)
{
  return io
    ? &((mw_io68_t*)io)->mw
    : 0
    ;
}

int mwio_sampling_rate(io68_t * const io, int sampling_rate)
{
  return mw_sampling_rate(mwio_emulator(io), sampling_rate);
}
