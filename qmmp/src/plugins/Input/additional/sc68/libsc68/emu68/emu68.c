/*
 * @file    emu68/emu68.c
 * @brief   emu68 API
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

#include "struct68.h"
#include "srdef68.h"
#include "excep68.h"
#include "assert68.h"

#include "emu68.h"
#include "ioplug68.h"
#include "io68/io68.h"

#include "macro68.h"

#include "inl68_exception.h"
#include <string.h>
#include <stdio.h>

EMU68_EXTERN linefunc68_t *line_func[1024];

/* ,-----------------------------------------------------------------.
 * |                     Internal struct access                      |
 * `-----------------------------------------------------------------'
 */

/*  Set new interrupt IO
 *  return previous pointer
 */
io68_t * emu68_set_interrupt_io(emu68_t * const emu68, io68_t * const io)
{
  io68_t * oldio = 0;
  if (emu68) {
    oldio = emu68->interrupt_io;
    emu68->interrupt_io = io;
  }
  return oldio;
}

void emu68_set_registers(emu68_t * emu68, const reg68_t * r, int mask)
{
  if (emu68 && r) {
    int i;
    if ( mask & ( 1 << REG68_US_IDX ) ) REG68.usp = r->usp;
    if ( mask & ( 1 << REG68_PC_IDX ) ) REG68.pc  = r->pc;
    if ( mask & (1  << REG68_SR_IDX ) ) REG68.sr  = r->sr;
    for ( i=0; i<8; i++ )
      if ( mask & ( 1 << ( REG68_D0_IDX + i ) ) ) REG68.d[i] = r->d[i];
    for ( i=0; i<8; i++)
      if ( mask & ( 1 << ( REG68_A0_IDX + i ) ) ) REG68.a[i] = r->a[i];
  }
}

void emu68_get_registers(const emu68_t * const emu68, reg68_t * r, int mask)
{
  if (emu68 && r) {
    int i;
    if ( mask & ( 1 << REG68_US_IDX ) ) r->usp = REG68.usp;
    if ( mask & ( 1 << REG68_PC_IDX ) ) r->pc  = REG68.pc;
    if ( mask & ( 1 << REG68_SR_IDX ) ) r->sr  = REG68.sr;
    for ( i=0; i<8; i++ )
      if ( mask & ( 1 << ( REG68_D0_IDX + i ) ) ) r->d[i] = REG68.d[i];
    for ( i=0; i<8; i++ )
      if ( mask & ( 1 << ( REG68_A0_IDX + i ) ) ) r->a[i] = REG68.a[i];
  }
}

void emu68_set_cycle(emu68_t * const emu68, cycle68_t cycle)
{
  emu68->cycle = cycle;
}

cycle68_t emu68_get_cycle(emu68_t * const emu68)
{
  return emu68->cycle;
}

void * emu68_set_cookie(emu68_t * const emu68, void * cookie)
{
  void * old = 0;
  if (emu68) {
    old = emu68->cookie;
    emu68->cookie = cookie;
  }
  return old;
}

void * emu68_get_cookie(emu68_t * const emu68)
{
  return emu68
    ? emu68->cookie
    : 0
    ;
}

emu68_handler_t emu68_set_handler(emu68_t * const emu68, emu68_handler_t hdl)
{
  emu68_handler_t old = emu68->handler;
  emu68->handler = hdl;
  return old;
}

emu68_handler_t emu68_get_handler(emu68_t * const emu68)
{
  return emu68
    ? emu68->handler
    : 0
    ;
}

const char * emu68_exception_name(unsigned int vector, char * buf)
{
  static const char * xtra_names[] = {
    "hw-trace", "hw-halt", "hw-stop", "hw-reset", "hw-init", "hw-iovf"
  };

  static const char * xcpt_names[] = {
    "reset-sp", "reset-pc", "bus-error", "addr-error",
    "illegal", "0-divide", "chk", "trapv", "privv",
    "trace", "linea", "linef"
  };

  static char tmp[16];
  if (!buf) buf = tmp;

  switch ( vector & ~0xCFF ) {
  case 0x000:
    if (vector < sizeof(xcpt_names)/sizeof(*xcpt_names))
      strcpy(buf,xcpt_names[vector]);
    else if (vector >= TRAP_VECTOR(0) && vector <= TRAP_VECTOR(15))
      sprintf(buf,"trap#%02d",vector-TRAP_VECTOR_0);
    else
      sprintf(buf,"vector#%02x",vector);
    break;

  case 0x100:
    vector -= 0x100;
    if (vector < 0x20)
      sprintf(buf,"hw-brkp#%02d",vector);
    else if (vector-0x20 < sizeof(xtra_names)/sizeof(*xtra_names))
      strcpy(buf,xtra_names[vector-0x20]);
    else
      sprintf(buf,"special#%02x",vector);
    break;

  case 0x200:
    sprintf(buf,"private#%02x", vector-0x200);
    break;
  default:
    sprintf(buf,"invalid#%d", vector);
    break;
  }
  return buf;
}

/* ,-----------------------------------------------------------------.
 * |                      Onboard memory access                      |
 * `-----------------------------------------------------------------'
 */

u8 * emu68_memptr(emu68_t * const emu68, addr68_t dst, uint68_t sz)
{
  u8 * ptr = 0;
  if (emu68) {
    const uint68_t adr = dst;
    const uint68_t bot = MEMMSK68+1;
    const uint68_t end = adr+sz;
    if (adr >= bot || end > bot || adr > end) {
      emu68_error_add(emu68,
                      "invalid memory range [$%06x..$%06x] > $%06x",
                      adr,end,bot);
    } else {
      ptr = emu68->mem + dst;
    }
  }
  return ptr;
}

u8 * emu68_chkptr(emu68_t * const emu68, addr68_t dst, uint68_t sz)
{
  u8 * ptr = emu68_memptr(emu68,dst,sz);
  if (ptr && emu68->chk) {
    ptr = emu68->chk + (ptr - emu68->mem);
  }
  return ptr;
}

/* Peek & Poke */
int emu68_peek(emu68_t * const emu68, addr68_t addr)
{
  return emu68
    ? emu68->mem[addr & MEMMSK68]
    : -1
    ;
}

int emu68_chkpeek(emu68_t * const emu68, addr68_t addr)
{
  return (emu68 && emu68->chk)
    ? emu68->chk[addr & MEMMSK68]
    : -1
    ;
}

int emu68_poke(emu68_t * const emu68, addr68_t addr, int68_t v)
{
  return emu68
    ? (emu68->mem[addr & MEMMSK68] = v)
    : -1
    ;
}

int emu68_chkpoke(emu68_t * const emu68, addr68_t addr, int68_t v)
{
  return (emu68 && emu68->chk)
    ? (emu68->chk[addr & MEMMSK68] = v)
    : -1
    ;
}

/*  Write memory block to 68K on board memory
 */
int emu68_memput(emu68_t * const emu68, addr68_t dst, const u8 *src, uint68_t sz)
{
  u8 * ptr = emu68_memptr(emu68,dst,sz);
  if (ptr) {
    memcpy(ptr,src,sz);
    return 0;
  }
  return -1;
}

/*  Read memory block from 68K on board memory
 */
int emu68_memget(emu68_t * const emu68, u8 *dst, addr68_t src, uint68_t sz)
{
  u8 * ptr = emu68_memptr(emu68,src,sz);
  if (ptr) {
    memcpy(dst,ptr,sz);
  }
  return -!ptr;
}

int emu68_memset(emu68_t * const emu68, addr68_t dst, u8 val, uint68_t sz)
{
  u8 * ptr;

  if (!emu68) {
    ptr = 0;
  } else {
    if (!sz) {
      sz = emu68->memmsk + 1 - dst;
    }
    ptr = emu68_memptr(emu68, dst, sz);
    if (ptr)
      memset(ptr,val,sz);
  }
  return -!ptr;
}

int emu68_chkset(emu68_t * const emu68, addr68_t dst, u8 val, uint68_t sz)
{
  u8 * ptr;

  if (!emu68) {
    ptr = 0;
  } else {
    if (!sz) {
      sz = emu68->memmsk+1-dst;
    }
    ptr = emu68_chkptr(emu68, dst, sz);
    if (ptr)
      memset(ptr,val,sz);
  }
  return -!ptr;
}

static uint_t crc32b(uint_t crc, u8 * ptr, int len)
{
  u8 * end = ptr + len;
  /* crc = (u32)~crc; */
  while (ptr < end) {
    int j;
    crc ^= *ptr++;
    for (j = 7; j >= 0; j--) {
      const int mask = -(int)(crc & 1);
      crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
  }
  /* crc = (u32)~crc; */
  return crc;
}

uint68_t emu68_crc32(emu68_t * const emu68)
{
  uint68_t crc = 0;
  if ( emu68 ) {
    int i;
    u8 tmp [ 18 * 4 + 2 ];              /* serialize registers */
    s32 * reg32 = emu68->reg.d;
    for ( i = 0; i < 18*4; i += 4 ) {
      unsigned int v = reg32[i>>2];
      tmp[i+0] = v >> 24;
      tmp[i+1] = v >> 16;
      tmp[i+2] = v >> 8;
      tmp[i+3] = v;
    }
    tmp[i++] = emu68->reg.sr >> 8;
    tmp[i++] = emu68->reg.sr;
    assert ( i == sizeof(tmp) );

    crc = crc32b(0xFFFFFFFF, tmp, i);
    crc = crc32b(crc, emu68->mem, emu68->memmsk+1);
  }
  return crc;
}

/* Process a single instruction emulation. */
static inline void step68(emu68_t * const emu68)
{
  int line,opw,reg9;
  u8 * mem;

  assert( emu68->status == EMU68_NRM );
  emu68->inst_pc = REG68.pc;
  /* Save the sr and rise the trace exception if needed. */
  if ( ((emu68->inst_sr = emu68->reg.sr) & SR_T) ) {
    inl_exception68(emu68, TRACE_VECTOR, -1);
    if (emu68->status != EMU68_NRM)
      return;
  }

  /* TODO: check address valid */
  mem = emu68->mem + (REG68.pc & (MEMMSK68 & ~1));
  REG68.pc += 2;
  opw  = (mem[0]<<8) | mem[1];

 /* 68000 OP-WORD format :
  *  1111 0000 0000 0000 ( LINE  )
  *  0000 0001 1111 1000 ( OP    )
  *  0000 0000 0000 0111 ( REG 0 )
  *  0000 1110 0000 0000 ( REG 9 )
  */

  /*                       LINE RG9 OPCODE  RG0 */
  /*                       -------------------- */
  line = opw & 0170000; /* 1111 000 000-000 000 */
  opw -= line;          /* 0000 111 111-111 111 */
  reg9 = opw &   07000; /* 0000 111 000-000 000 */
  opw -= reg9;          /* 0000 000 111-111 111 */
  reg9 >>= 9;           /* reg9 */
  line |= opw<<3;
  line >>= 6;
  opw  &=  7;           /* 0000 000 000-000 111 */
  (line_func[line])(emu68, reg9, opw);
}

static inline int valid_bp(const unsigned int id) {
  return id < 31u;
}

static inline int free_bp(const emu68_t * const emu68, const int id) {
  return !emu68->breakpoints[id].count;
}

/* Run a single step emulation with all program controls. */
static inline int controlled_step68(emu68_t * const emu68)
{
  assert( emu68->status == EMU68_NRM );

  /* running in debug mode. */
  if (emu68->chk) {
    int       id;
    addr68_t  addr;
     /* HardWare TRACE exception */
    inl_exception68(emu68, HWTRACE_VECTOR, -1);
    if (emu68->status != EMU68_NRM)
      return emu68->status;
     /* HardWare BREAKPOINT exception: the memory is flagged with a
      * breakpoint number and that breakpoint is active.
      */
    addr = REG68.pc & MEMMSK68;
    id = (emu68->chk[addr] >> 3) - 1;
    if (valid_bp(id) && emu68->breakpoints[id].count) {
      assert( addr == emu68->breakpoints[id].addr );
      if (! --emu68->breakpoints[id].count ) {
        /* breakpoint count reachs 0, it is an actual break:
         * - run the appropriate exception
         * - reset the breakpoint; delete it if neccessary
         */
        if (! (emu68->breakpoints[id].count = emu68->breakpoints[id].reset))
          emu68->chk[addr] &= EMU68_A;
        inl_exception68(emu68, HWBREAK_VECTOR+id, -1);
        if (emu68->status != EMU68_NRM)
          return emu68->status;
      }
    }
    chkframe(emu68, REG68.pc, EMU68_X);
  }

  /* Execute 68K instruction. */
  step68(emu68);

  /* Instruction countdown */
  if ( emu68->instructions && !--emu68->instructions )
    if (emu68->status == EMU68_NRM) {
      emu68->status = EMU68_BRK;
      inl_exception68(emu68, HWINSTOV_VECTOR, -1);
    }

  return emu68->status;
}

static void loop68(emu68_t * const emu68)
{
  assert( ! (emu68->finish_sp & 1 ) );
  assert( emu68->status == EMU68_NRM );

  while ( controlled_step68(emu68) == EMU68_NRM &&
          emu68->finish_sp >= (addr68_t) REG68.a[7] )
    ;
}

const char * emu68_status_name(enum emu68_status_e status)
{
  switch (status) {
  case EMU68_ERR: return "error";
  case EMU68_NRM: return "ok";
  case EMU68_STP: return "stop";
  case EMU68_HLT: return "halt";
  case EMU68_BRK: return "break";
  case EMU68_XCT: return "exception";
  }

  assert(!"unknown emu68 status");

  return "unknown";
}

/* Execute one instruction. */
int emu68_step(emu68_t * const emu68, uint68_t inst)
{
  int rc = EMU68_ERR;

  assert (emu68);
  assert (inst == EMU68_STEP || inst == EMU68_CONT);

  if (emu68) {
    if (inst == EMU68_STEP) {
      emu68->frm_chk_fl = 0;
      emu68->status = EMU68_NRM;
    }

    switch (emu68->status) {
    case EMU68_NRM:
      controlled_step68(emu68);
    case EMU68_BRK:
    case EMU68_STP:
    case EMU68_HLT:
      rc = emu68->status;
      break;

    default:
      assert(!"unexpected value for emu68_t::status");
      break;
    }
  }
  return rc;
}

int emu68_finish(emu68_t * const emu68, uint68_t instructions)
{
  io68_t *io;

  assert (emu68);
  if (!emu68)
    return EMU68_ERR;


  if (instructions != EMU68_CONT) {
    emu68->finish_sp    = REG68.a[7];
    emu68->frm_chk_fl   = 0;
    emu68->instructions = instructions;
  }

  if (emu68->cycle) {
    for (io=emu68->iohead; io; io=io->next) {
      io->adjust_cycle(io, emu68->cycle);
    }
    emu68->cycle = 0;
  }

  emu68->status = EMU68_NRM;
  /* assert ( emu68->status == EMU68_NRM ); */
  loop68(emu68);

  return emu68->status;
}

/* Execute interruptions. */
int emu68_interrupt(emu68_t * const emu68, cycle68_t cycleperpass)
{
  assert(emu68);
  if (!emu68)
    return EMU68_ERR;

  assert ( emu68->status == EMU68_NRM || emu68->status == EMU68_STP );

  /* Clear STOP mode. If interrupt is running it means execution is
   * not stopped anymore.
   */
  emu68->status = EMU68_NRM;

  /* Get interrupt IO (MFP) if any */
  if (emu68->interrupt_io) {
    for ( ;; ) {
      const int ipl = ( REG68.sr >> SR_I_BIT ) & 7;
      interrupt68_t * t =
        emu68->interrupt_io->interrupt(emu68->interrupt_io, cycleperpass);
      if (!t)
        break;
      emu68->cycle = t->cycle;
      if (t->level > ipl) {
        inl_exception68(emu68, t->vector, t->level);
        if (emu68->status != EMU68_NRM)

        /* $$$ ignore break in interrupt atm as we can't resume
         * properly
         */
        if (emu68->status == EMU68_BRK)
          emu68->status = EMU68_NRM;

        emu68->finish_sp = (addr68_t) REG68.a[7];
        loop68(emu68);
      }
    }
  }
  emu68->cycle = cycleperpass;

  return emu68->status;
}


/* Get debug mode.  */
int emu68_debugmode(emu68_t * const emu68)
{
  assert(emu68);
  return emu68
    ? !!emu68->chk
    : -1
    ;
}

/* ,-----------------------------------------------------------------.
 * |                           Breakpoints                           |
 * `-----------------------------------------------------------------'
 */

static void bp_reset(emu68_t * const emu68)
{
  int id;
  assert(emu68);

  for ( id=0; valid_bp(id); ++id ) {
    emu68->breakpoints[id].addr  = 0;
    emu68->breakpoints[id].count = 0;
    emu68->breakpoints[id].reset = 0;
  }
}

void emu68_bp_delall(emu68_t * const emu68)
{
  int id;
  assert(emu68);

  for ( id=0; valid_bp(id); ++id ) {
    emu68_bp_del(emu68, id);
  }
}

void emu68_bp_del(emu68_t * const emu68, int id)
{
  assert(emu68);

  if (emu68 && valid_bp(id)) {
    if (emu68->chk && !free_bp(emu68,id)) {
      const addr68_t addr = emu68->breakpoints[id].addr & MEMMSK68;
      emu68->chk[addr] &= EMU68_A;
    }
    emu68->breakpoints[id].addr  = 0;
    emu68->breakpoints[id].count = 0;
    emu68->breakpoints[id].reset = 0;
  }
}

int emu68_bp_set(emu68_t * const emu68, int id,
                 addr68_t addr, uint68_t count, uint68_t reset)
{
  assert(emu68);

  if (!emu68) {
    id = -1;
  } else {
    if ( id == -1 )
      for ( id=0; valid_bp(id) && !free_bp(emu68,id); ++id)
        ;
    if ( valid_bp(id) ) {
      emu68->breakpoints[id].addr  = addr &= MEMMSK68;
      emu68->breakpoints[id].count = count;
      emu68->breakpoints[id].reset = reset;
      if (emu68->chk)
        emu68->chk[addr] = (emu68->chk[addr] & EMU68_A) | ((id+1)<<3);
    } else {
      id = -1;
    }
  }
  return id;
}

int emu68_bp_find(emu68_t * const emu68, addr68_t addr)
{
  int id = -1;

  assert(emu68);
  if (emu68) {
    int i;
    for ( i=0; valid_bp(i); ++i ) {
      if ( !free_bp(emu68, i) &&
          !( (emu68->breakpoints[i].addr ^ addr) & MEMMSK68 ) ) {
        id = i;
        break;
      }
    }
  }
  return id;
}


/* ,-----------------------------------------------------------------.
 * |                     Emulator init functions                     |
 * `-----------------------------------------------------------------'
 */

extern io68_t * mem68_io(void);

static emu68_parms_t def_parms;

emu68_t * emu68_create(emu68_parms_t * const parms)
{
  emu68_t * emu68 = 0;
  int memsize, membyte;
  emu68_parms_t * const p = parms ? parms : &def_parms;

  if (!p->log2mem) {
    p->log2mem = def_parms.log2mem;
  }
  if (p->log2mem < 16 || p->log2mem > 24) {
    emu68_error_add(emu68,
                    "invalid requested amount of memory -- 2^%d",
                    p->log2mem);
    goto error;
  }

  if (!p->clock) {
    p->clock = def_parms.clock;
  }
  if (p->clock<500000u || p->clock>60000000u) {
    emu68_error_add(emu68,
                    "invalid clock frequency -- %u", p->clock);
    goto error;
  }

  memsize = 1 << p->log2mem;
  membyte = sizeof(emu68_t) + (memsize << !!p->debug);
  emu68   = emu68_alloc(membyte);
  if (!emu68)
    goto error;

  memset(emu68,0,sizeof(emu68_t));
  /* memset(emu68,0,membyte); */ /* this is too heavy just don't */

  strncpy(emu68->name,p->name?p->name:"emu68",sizeof(emu68->name)-1);
  emu68->clock = p->clock;

  emu68->log2mem = p->log2mem;
  emu68->memmsk  = memsize-1;
  emu68->chk     = p->debug ? emu68->mem + memsize + 8 : 0;
  emu68_mem_init(emu68);
  /* Notice that emu68_reset() triggers the HWVECTOR_INIT exception
   * but it won't be catch by the user as as at this point it is null.
   */
  emu68_reset(emu68);

error:
  return emu68;
}

emu68_t * emu68_duplicate(emu68_t * emu68src, const char * dupname)
{
  emu68_parms_t parms;
  emu68_t * emu68 = 0;

  assert(!"This function needs rework");
  return 0;

  assert(emu68src);
  if (!emu68src)
    goto error;

  /* Create an instance with the same parameters */
  parms.name    = dupname;
  parms.log2mem = emu68->log2mem;
  parms.clock   = emu68->clock;
  parms.debug   = !!emu68->chk;
  emu68 = emu68_create(&parms);
  if (!emu68)
    goto error;

  /* Copy registers and status */
  emu68->reg          = emu68src->reg;
  emu68->cycle        = emu68src->cycle;

  /* Copy memory access control stuff */
  emu68->frm_chk_fl   = emu68src->frm_chk_fl;
  emu68->fst_chk      = emu68src->fst_chk;
  emu68->lst_chk      = emu68src->lst_chk;

  emu68->instructions = emu68src->instructions;
  emu68->finish_sp    = emu68src->finish_sp;
  emu68->status       = emu68src->status;

  /* Copy memory */
  memcpy(emu68->mem, emu68src->mem, 1<<emu68->log2mem);
  if (emu68->chk)
    memcpy(emu68->chk, emu68src->chk, 1<<emu68->log2mem);

  /* Copy breakpoints */
  memcpy(emu68->breakpoints, emu68src->breakpoints,
         sizeof(emu68->breakpoints));

  /* TODO: all related IO stuff */

error:
  return emu68;
}

/* Destroy emulator instance. */
void emu68_destroy(emu68_t * const emu68)
{
  if (emu68) {
    emu68_ioplug_destroy_all(emu68);
    emu68_mem_destroy(emu68);
    emu68_free(emu68);
  }
}

/* 68000 Hardware Reset
 * - PC = 0
 * - SR = 2700
 * - A7 = end of mem - 4
 * - All registers cleared
 * - All IO reseted
 */
void emu68_reset(emu68_t * const emu68)
{
  assert(emu68);
  if (emu68) {
    io68_t * io;

    /* Reset IOs */
    for (io=emu68->iohead; io; io=io->next)
      io68_reset(io);
    io68_reset(emu68->memio);
    if (&emu68->ramio != emu68->memio)
      io68_reset(&emu68->ramio);
    if (&emu68->errio != emu68->memio)
      io68_reset(&emu68->errio);

    /* Reset breakpoints. */
    bp_reset(emu68);

    /* Reset registers. */
    memset(REG68.d,0,sizeof(REG68.d)*2);
    REG68.pc     = 0;
    REG68.sr     = 0x2700;
    REG68.a[7]   = MEMMSK68+1-4;
    REG68.usp    = REG68.a[7];

    /* Reset internals */
    emu68->nerr         = 0;
    emu68->cycle        = 0;
    emu68->frm_chk_fl   = 0;
    emu68->instructions = 0;
    emu68->finish_sp    = -1;
    emu68->status       = EMU68_NRM;
    emu68->inst_sr      = emu68->inst_pc = -1;

    /* Reset memory access control flags */
    if (emu68->chk)
      memset(emu68->chk, 0, emu68->memmsk+1);

    /* Notify init is complete */
    inl_exception68(emu68, HWINIT_VECTOR, -1);
  }
}

/* ,-----------------------------------------------------------------.
 * |                    Library init functions                       |
 * `-----------------------------------------------------------------'
 */

/* Library initialize. */
int emu68_init(int * argc, char ** argv)
{
  def_parms.name    = 0;
  def_parms.log2mem = 20;               /* log2mem: 2^20 => 1 mB */
  def_parms.clock   = EMU68_ATARIST_CLOCK;
  def_parms.debug   = 0;

  /* $$$ TODO: parse argument */
  return 0;
}

/* Library shutdown. */
void emu68_shutdown(void)
{
}
