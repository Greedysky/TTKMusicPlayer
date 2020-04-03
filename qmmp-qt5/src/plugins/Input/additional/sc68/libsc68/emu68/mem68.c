/*
 * @file    emu68/mem68.c
 * @brief   68K memory and IO access
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

/* generated config include */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "mem68.h"
#include "error68.h"
#include "emu68.h"
#include "io68/io68.h"
#include "assert68.h"

/* ,--------------------------------------------------------.
 * |                                                        |
 * |            68000 memory access functions               |
 * |                                                        |
 * |   These functions don't check address/bus access nor   |
 * |   and privilege access violation.                      |
 * |                                                        |
 * `--------------------------------------------------------'
 */


/* ,--------------------------------------------------------.
 * |                      Debug IO                          |
 * `--------------------------------------------------------'
 */

static void memchk_rb(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const addr68_t addr = emu68->bus_addr;
  assert(emu68->chk);
  emu68->bus_data = emu68->mem[addr&MEMMSK68];
  chkframe_b(emu68, EMU68_R);
}

static void memchk_rw(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const u8 * const mem = emu68->mem+(emu68->bus_addr&MEMMSK68);
  assert(emu68->chk);
  emu68->bus_data = (mem[0]<<8) + mem[1];
  chkframe_w(emu68, EMU68_R);
}

static void memchk_rl(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const u8 * const mem = emu68->mem+(emu68->bus_addr&MEMMSK68);
  assert(emu68->chk);
  emu68->bus_data = (mem[0]<<24) + (mem[1]<<16) + (mem[2]<<8) + mem[3];
  chkframe_l(emu68, EMU68_R);
}

static void memchk_wb(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const addr68_t addr = emu68->bus_addr;
  assert(emu68->chk);
  emu68->mem[addr&MEMMSK68] = emu68->bus_data;
  chkframe_b(emu68, EMU68_W);
}

static void memchk_ww(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  u8 * mem = emu68->mem + (emu68->bus_addr&MEMMSK68);
  int68_t v = emu68->bus_data;
  assert(emu68->chk);
  mem[1] = v; v>>=8; mem[0] = v;
  chkframe_w(emu68, EMU68_W);
}

static void memchk_wl(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  u8 * mem = emu68->mem + (emu68->bus_addr&MEMMSK68);
  int68_t v = emu68->bus_data;
  assert(emu68->chk);
  mem[3] = v; v>>=8; mem[2] = v; v>>=8; mem[1] = v; v>>=8; mem[0] = v;
  chkframe_l(emu68, EMU68_W);
}

/* ,--------------------------------------------------------.
 * |                      Fault IO                          |
 * `--------------------------------------------------------'
 */

static void fault_rab(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  assert(emu68);
  emu68_error_add(emu68, "Invalid byte R access pc:$%06x $%08x",
                  (unsigned)(u32)emu68->inst_pc,
                  (unsigned)(u32)emu68->bus_addr);
  emu68->bus_data = -1;
  emu68->status = EMU68_HLT;
}

static void fault_raw(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  assert(emu68);
  emu68_error_add(emu68, "Invalid word R access pc:$%06x $%08x",
                  (unsigned)(u32)emu68->inst_pc,
                  (unsigned)(u32)emu68->bus_addr);
  emu68->bus_data = -1;
  emu68->status = EMU68_HLT;
}

static void fault_ral(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  assert(emu68);
  emu68_error_add(emu68, "Invalid long R access pc:$%06x $%08x",
                  (unsigned)(u32)emu68->inst_pc,
                  (unsigned)(u32)emu68->bus_addr);
  emu68->bus_data = -1;
  emu68->status = EMU68_HLT;
}

static void fault_wab(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  assert(emu68);
  emu68_error_add(emu68, "Invalid byte W access pc:$%06x $%08x <- $%02x",
                  (unsigned)(u32)emu68->inst_pc,
                  (unsigned)(u32)emu68->bus_addr,
                  (unsigned)(u8)emu68->bus_data);
  emu68->status = EMU68_HLT;
}

static void fault_waw(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  assert(emu68);
  emu68_error_add(emu68, "Invalid word W access pc:$%06x $%08x <- $%02x",
                  (unsigned)(u32)emu68->inst_pc,
                  (unsigned)(u32)emu68->bus_addr,
                  (unsigned)(u16)emu68->bus_data);
  emu68->status = EMU68_HLT;
}

static void fault_wal(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  assert(emu68);
  emu68_error_add(emu68, "Invalid long W access pc:$%06x $%08x <- $%02x",
                  (unsigned)(u32)emu68->inst_pc,
                  (unsigned)(u32)emu68->bus_addr,
                  (unsigned)(u32)emu68->bus_data);
  emu68->status = EMU68_HLT;
}

static void nop_rwa(io68_t * const io) {}

static void no_destroy(io68_t * const io) {}

static const io68_t fault_io = {
  0,"Fault",0,0,
  fault_rab,fault_raw,fault_ral,
  fault_wab,fault_waw,fault_wal,
  0,0,0,0,no_destroy
};

static const io68_t ram_io = {
  0,"RAM",0,0,
  memchk_rb,memchk_rw,memchk_rl,
  memchk_wb,memchk_ww,memchk_wl,
  0,0,0,0,no_destroy
};

static const io68_t nop_io = {
  0,"NOP",0,0,
  nop_rwa,nop_rwa,nop_rwa,
  nop_rwa,nop_rwa,nop_rwa,
  0,0,0,0,no_destroy
};


/* ,--------------------------------------------------------.
 * |                   Read functions                       |
 * `--------------------------------------------------------'
 */

void mem68_read_b(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;

  if (mem68_is_io(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->r_byte(io);
  } else if (!emu68->memio) {
    emu68->bus_data = emu68->mem[addr&MEMMSK68];
  } else {
    emu68->memio->r_byte(emu68->memio);
  }
}

void mem68_read_w(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (mem68_is_io(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->r_word(io);
  } else if (!emu68->memio) {
    const u8 * const mem = emu68->mem+(addr&MEMMSK68);
    emu68->bus_data = (mem[0]<<8) + mem[1];
  } else {
    emu68->memio->r_word(emu68->memio);
  }
}

void mem68_read_l(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (mem68_is_io(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->r_long(io);
  } else if (!emu68->memio) {
    const u8 * const mem = emu68->mem+(addr&MEMMSK68);
    emu68->bus_data = (mem[0]<<24) + (mem[1]<<16) + (mem[2]<<8) + mem[3];
  } else {
    emu68->memio->r_long(emu68->memio);
  }
}

/* ,--------------------------------------------------------.
 * |                   Write functions                      |
 * `--------------------------------------------------------'
 */

void mem68_write_b(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (mem68_is_io(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->w_byte(io);
  } else if (!emu68->memio) {
    emu68->mem[addr&MEMMSK68] = emu68->bus_data;
  } else {
    emu68->memio->w_byte(emu68->memio);
  }
}

void mem68_write_w(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (mem68_is_io(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->w_word(io);
  } else if (!emu68->memio) {
    u8 * mem = emu68->mem + (addr&MEMMSK68);
    int68_t v = emu68->bus_data;
    mem[1] = v; v>>=8; mem[0] = v;
  } else {
    emu68->memio->w_word(emu68->memio);
  }
}

void mem68_write_l(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (mem68_is_io(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->w_long(io);
  } else if (!emu68->memio) {
    u8 * mem = emu68->mem + (addr&MEMMSK68);
    int68_t v = emu68->bus_data;
    mem[3] = v; v>>=8; mem[2] = v; v>>=8; mem[1] = v; v>>=8; mem[0] = v;
  } else {
    emu68->memio->w_long(emu68->memio);
  }
}


/* Read 68000 (PC)+ word
 * - This version assume PC is in 68000 memory
 *   as long I don't try to make 68000 execute
 *   @ IO address, I assume it is not possible !
 */
int68_t mem68_nextw(emu68_t * const emu68)
{
  const addr68_t addr = REG68.pc;
  io68_t * const io =
    mem68_is_io(addr)
    ? emu68->mapped_io[(u8)((addr)>>8)]
    : emu68->memio
    ;
  REG68.pc += 2;

  if (!io) {
    u8 * const mem = emu68->mem + ( addr & MEMMSK68 );
    return ( (int68_t) (s8)mem[0] << 8 ) | mem[1];
  } else {
    emu68->bus_addr = addr;
    io->r_word(io);
    return (int68_t)(s16)emu68->bus_data;
  }
}

int68_t mem68_nextl(emu68_t * const emu68)
{
  const addr68_t addr = REG68.pc;
  io68_t * const io =
    mem68_is_io(addr)
    ? emu68->mapped_io[(u8)((addr)>>8)]
    : emu68->memio
    ;
  REG68.pc += 4;

  if (!io) {
    u8 * const mem= emu68->mem + ( addr & MEMMSK68 );
    return
      ( (int68_t)(s8)mem[0] << 24 ) |
      ( (int68_t)    mem[1] << 16 ) |
      ( (int68_t)    mem[2] <<  8 ) |
      (              mem[3]       ) ;
  } else {
    emu68->bus_addr = addr;
    io->r_long(io);
    return (int68_t)(s32)emu68->bus_data;
  }
}

void mem68_pushw(emu68_t * const emu68, const int68_t v)
{
  REG68.a[7] -= 2;
  write_W(REG68.a[7],v);
}

void mem68_pushl(emu68_t * const emu68, const int68_t v)
{
  REG68.a[7] -= 4;
  write_L(REG68.a[7],v);
}

int68_t mem68_popl(emu68_t * const emu68)
{
  int68_t v = (s32)read_L(REG68.a[7]);
  REG68.a[7] += 4;
  return v;
}

int68_t mem68_popw(emu68_t * const emu68)
{
  s32 v = read_W(REG68.a[7]);
  REG68.a[7] += 2;
  return v;
}

void emu68_pushl(emu68_t * const emu68, int68_t val)
{
  if (emu68)
    mem68_pushl(emu68, val);
}

void emu68_pushw(emu68_t * const emu68, int68_t val)
{
  if (emu68)
    mem68_pushw(emu68, val);
}


int68_t emu68_popl(emu68_t * const emu68)
{
  return emu68
    ? mem68_popl(emu68)
    : -1
    ;
}

int68_t emu68_popw(emu68_t * const emu68)
{
  return emu68
    ? mem68_popw(emu68)
    : -1
    ;
}

/* Init memory quick acces table for SC68
 */
void emu68_mem_init(emu68_t * const emu68)
{
  if (emu68) {
    /* Copy RAM and Fault IOs for private use. */
    emu68->ramio = ram_io;
    emu68->ramio.emu68 = emu68;
    emu68->ramio.addr_lo = 0;
    emu68->ramio.addr_hi = emu68->memmsk;

    emu68->errio = fault_io;
    emu68->errio.emu68 = emu68;
    emu68->errio.addr_lo = mem68_is_io(0xFFFFFF);
    emu68->errio.addr_hi = 0xFFFFFFFF;

    emu68->nopio = nop_io;
    emu68->nopio.emu68 = emu68;
    emu68->nopio.addr_lo = mem68_is_io(0xFFFFFF);
    emu68->nopio.addr_hi = 0xFFFFFFFF;

    emu68->memio = (emu68->chk) ? &emu68->ramio : 0;
  }
  emu68_mem_reset(emu68);
}

void emu68_mem_destroy(emu68_t * const emu68)
{
  if (emu68) {
    emu68->errio.emu68 = 0;
    emu68->ramio.emu68 = 0;
    io68_destroy(emu68->memio);
    emu68->memio = 0;
  }
}

/* Reset memory quick acces table
 */
void emu68_mem_reset(emu68_t * const emu68)
{
  if (emu68) {
    int i;
    for(i=0; i<256; i++)
      emu68_mem_reset_area(emu68, i);
  }
}

/* Reset memory acces control area to normal state :
 */
void emu68_mem_reset_area(emu68_t * const emu68, u8 area)
{
  emu68->mapped_io[area] =
    emu68->chk ? &emu68->errio : &emu68->nopio;
}
