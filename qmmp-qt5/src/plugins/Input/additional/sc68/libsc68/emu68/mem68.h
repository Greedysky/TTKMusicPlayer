/**
 * @ingroup   lib_emu68
 * @file      emu68/mem68.h
 * @brief     68k memory and IO manager header.
 * @author    Benjamin Gerard
 * @date      1999/03/13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_MEM68_H
#define EMU68_MEM68_H

#include "emu68_api.h"
#include "struct68.h"

/**
 * @defgroup  lib_emu68_mem  68k memory and IO manager
 * @ingroup   lib_emu68
 * @brief     Handling memory and I/O access.
 *
 *   EMU68 memory manager assumes that all addresses in the lowest
 *   half part of address space are memory access. A simple bit test
 *   over most signifiant bit (23) of address allow to choose beetween
 *   memory or eventual IO access. In case of memory access, address
 *   is masked to fit available 68K onboard memory. Overflow does NOT
 *   generate address error.  IO access are performed towards quick
 *   access tables. There are 6 acccess tables: for each read and
 *   write access in 3 sizes (byte, word and long).  Each of this 6
 *   tables has 256 entries filled with a pointer to suitable
 *   function. At init time, the entries of all tables are initialized
 *   to access 68K onboard memory. When an IO is plugged by user, it
 *   is mapped somewhere in 68K address space. EMU68 memory manager
 *   get bit 8 to 15 of address to make an index to be used in the
 *   suitable table (R/W for B/W/L).
 *
 *   Featuring :
 *   - Onboard memory byte, word and long read/write access.
 *   - Optimized IO warm mapping/unmapping.
 *   - Optionnal (compile time) enhanced memory control with RWX access
 *     tag and hardware breakpoints.
 *
 *   Limitations :
 *   - For optimization purposes IO must be mapped in high half memory
 *     (bit 23 of address setted).
 *   - Two IO can not shared the same memory location for bit 8 to 15.
 *     Conflicts could be resolved by creating an intermediate IO which
 *     dispatches to others. This mechanism has not been implemented yet,
 *     so users must do it themself if needed.
 *
 *   Atari-ST & Amiga IO areas :
 *   - @p  FF8800-FF88FF : YM2149 (ST)
 *   - @p  FF8900-FF89FF : STE DMA and LMC1992
 *   - @p  FF8200-FF82FF : Shifter (ST)
 *   - @p  FFFA00-FFFAFF : MFP (ST)
 *   - @p  DFF000-DFF0DF : Paula (AMIGA)
 *
 * @{
 */

/**
 * Memory access flags for emu68_t::chk (debug mode only).
 */
enum {
  EMU68_R = 0x01,    /**< Memory location has been read.            */
  EMU68_W = 0x02,    /**< Memory location has been written.         */
  EMU68_X = 0x04,    /**< Memory location has been executed.        */
  EMU68_A = EMU68_R|EMU68_W|EMU68_X,   /**< All memory access bits. */
};

/**
 * @name  Memory/IO quick access tables.
 * @{
 */


/**
 * @}
 */

/**
 * @name Effective address calculation tables.
 *
 *   The get_ab[bwl] tables are used by EMU68 to calculate operand
 *   effective address. Each of them is indexed by operand addressing
 *   mode. Each entry is a pointer to a function which do everything
 *   neccessary to update processor state (e.g. address register
 *   increment or decrement). reg parameter is register number coded
 *   in the three first bit (0 to 2) of 68k opcode. When the mode is
 *   7, register parameter is used as an index in a second level
 *   function table for extended addressing mode.
 *
 * @{
 */

EMU68_EXTERN
/**
 * Byte operand effective address calculation function table.
 */
addr68_t (*const get_eab68[8])(emu68_t * const,int reg);

EMU68_EXTERN
/**
 * Word operand effective address calculation function table.
 */
addr68_t (*const get_eaw68[8])(emu68_t * const,int reg);

EMU68_EXTERN
/**
 * Long operand effective address calculation function table.
 */
addr68_t (*const get_eal68[8])(emu68_t * const,int reg);


#define get_EAB(MODE,REG) get_eab68[MODE](emu68,REG)
#define get_EAW(MODE,REG) get_eaw68[MODE](emu68,REG)
#define get_EAL(MODE,REG) get_eal68[MODE](emu68,REG)


/**
 * @name  68K onboard memory access.
 * @{
 */

EMU68_EXTERN
/**
 * Read memory byte.
 */
void mem68_read_b(emu68_t * const emu68);

EMU68_EXTERN
/**
 * Read memory word.
 */
void mem68_read_w(emu68_t * const emu68);

EMU68_EXTERN
/**
 * Read memory long.
 */
void mem68_read_l(emu68_t * const emu68);

EMU68_EXTERN
/**
 * Write memory byte.
 */
void mem68_write_b(emu68_t * const emu68);

EMU68_EXTERN
/**
 * Write memory word.
 */
void mem68_write_w(emu68_t * const emu68);

EMU68_EXTERN
/**
 * Write memory long.
 */
void mem68_write_l(emu68_t * const emu68);

static inline uint68_t _read_B(emu68_t * const emu68,
                               const addr68_t addr)
{
  emu68->bus_addr = addr;
  mem68_read_b(emu68);
  return (u8) emu68->bus_data;
}

static inline uint68_t _read_EAB(emu68_t * const emu68,
                                 const int mode, const int reg)
{
  emu68->bus_addr = get_eab68[mode](emu68,reg);
  mem68_read_b(emu68);
  return (u8) emu68->bus_data;
}


static inline uint68_t _read_W(emu68_t * const emu68,
                               const addr68_t addr)
{
  emu68->bus_addr = addr;
  mem68_read_w(emu68);
  return (u16) emu68->bus_data;
}

static inline uint68_t _read_EAW(emu68_t * const emu68,
                                 const int mode, const int reg)
{
  emu68->bus_addr = get_eaw68[mode](emu68,reg);
  mem68_read_w(emu68);
  return (u16) emu68->bus_data;
}


static inline uint68_t _read_L(emu68_t * const emu68,
                               const addr68_t addr)
{
  emu68->bus_addr = addr;
  mem68_read_l(emu68);
  return (u32) emu68->bus_data;
}

static inline uint68_t _read_EAL(emu68_t * const emu68,
                                 const int mode, const int reg)
{
  emu68->bus_addr = get_eal68[mode](emu68,reg);
  mem68_read_l(emu68);
  return (u32) emu68->bus_data;
}

/**
 * Read memory byte.
 */
#define read_B(ADDR) _read_B(emu68,(ADDR))

/**
 * Read memory word.
 */
#define read_W(ADDR) _read_W(emu68,(ADDR))

/**
 * Read memory long.
 */
#define read_L(ADDR) _read_L(emu68,(ADDR))

/**
 * Read memory byte from AE mode.
 */
#define read_EAB(MODE,PARM) _read_EAB(emu68,(MODE),(PARM))

/**
 * Read memory word from AE mode.
 */
#define read_EAW(MODE,PARM) _read_EAW(emu68,(MODE),(PARM))

/**
 * Read memory long from AE mode.
 */
#define read_EAL(MODE,PARM) _read_EAL(emu68,(MODE),(PARM))

static inline void _write_B(emu68_t * const emu68,
                            const addr68_t addr, const int68_t v)
{
  emu68->bus_addr = addr;
  emu68->bus_data = v;
  mem68_write_b(emu68);
}

static inline void _write_EAB(emu68_t * const emu68,
                              const int mode, const int reg, const int68_t v)
{
  emu68->bus_addr = get_eab68[mode](emu68,reg);
  emu68->bus_data = v;
  mem68_write_b(emu68);
}


static inline void _write_W(emu68_t * const emu68, addr68_t addr, int68_t v)
{
  emu68->bus_addr = addr;
  emu68->bus_data = v;
  mem68_write_w(emu68);
}

static inline void _write_EAW(emu68_t * const emu68,
                              const int mode, const int reg, const int68_t v)
{
  emu68->bus_addr = get_eaw68[mode](emu68,reg);
  emu68->bus_data = v;
  mem68_write_w(emu68);
}


static inline void _write_L(emu68_t * const emu68, addr68_t addr, int68_t v)
{
  emu68->bus_addr = addr;
  emu68->bus_data = v;
  mem68_write_l(emu68);
}

static inline void _write_EAL(emu68_t * const emu68,
                              const int mode, const int reg, const int68_t v)
{
  emu68->bus_addr = get_eal68[mode](emu68,reg);
  emu68->bus_data = v;
  mem68_write_l(emu68);
}


/**
 * Write memory byte.
 */
#define write_B(ADDR,V) _write_B(emu68,(ADDR),(V))

/**
 * Write memory word.
 */
#define write_W(ADDR,V) _write_W(emu68,(ADDR),(V))

/**
 * Write memory long.
 */
#define write_L(ADDR,V) _write_L(emu68,(ADDR),(V))

/**
 * Write memory byte at EA mode.
 */
#define write_EAB(MODE,PARM,V) _write_EAB(emu68,(MODE),(PARM),(V))

/**
 * Write memory word at EA mode.
 */
#define write_EAW(MODE,PARM,V) _write_EAW(emu68,(MODE),(PARM),(V))

/**
 * Write memory long at EA mode.
 */
#define write_EAL(MODE,PARM,V) _write_EAL(emu68,(MODE),(PARM),(V))

/**
 * @}
 */


/**
 * @name Instruction read.
 * @{
 */
EMU68_EXTERN
int68_t mem68_nextw(emu68_t * const emu68);  /**< Decode word and update PC */
EMU68_EXTERN
int68_t mem68_nextl(emu68_t * const emu68);  /**< Decode long and update PC */

/**
 * mem68_nextw() convenience macro.
 */
#define get_nextw() mem68_nextw(emu68)

/**
 * mem68_nextl() convenience macro.
 */
#define get_nextl() mem68_nextl(emu68)

/**
 * @}
 */


/**
 * @name Stack access.
 * @{
 */

EMU68_EXTERN
/**
 * Push long.
 */
void mem68_pushl(emu68_t * const emu68, const int68_t v);

/**
 * Push word.
 */
EMU68_EXTERN
void mem68_pushw(emu68_t * const emu68, const int68_t v);

EMU68_EXTERN
/**
 * Pop long.
 */
int68_t mem68_popl(emu68_t * const emu68);

EMU68_EXTERN
/**
 * Pop word.
 */
int68_t mem68_popw(emu68_t * const emu68);

/**
 * mem68_pushl() convenience macro.
 */
#define pushl(V) mem68_pushl(emu68,(V));

/**
 * mem68_pushw() convenience macro.
 */
#define pushw(V) mem68_pushw(emu68,(V));

/**
 * mem68_popl() convenience macro.
 */
#define popl() mem68_popl(emu68);

/**
 * mem68_popw() convenience macro.
 */
#define popw() mem68_popw(emu68);

/**
 * @}
 */


/**
 * Test for direct memory access or IO quick table access
 */
static inline int mem68_is_io(const addr68_t addr) {
  return addr & 0x800000;
}

/**
 * Set memory access check flags.
 */
static inline void chkframe(emu68_t * const emu68,
                            addr68_t addr, const int flags)
{
  int chgchk, newchk;
  /* assert( ! mem68_is_io(addr) ); */
  addr &= MEMMSK68;
  chgchk  = emu68->chk[addr];           /* current value */
  newchk  = chgchk | flags;             /* new value */
  chgchk ^= newchk;                     /* what's changed ? */
  if (chgchk) {
    emu68->lst_chk.pc = emu68->inst_pc;
    emu68->lst_chk.ad = addr;
    emu68->lst_chk.fl = chgchk;
    if (!emu68->frm_chk_fl)
      emu68->fst_chk = emu68->lst_chk;
    emu68->frm_chk_fl |= chgchk;
    emu68->chk[addr] = newchk;
  }
}

static inline void chk_buseven(emu68_t * const emu68)
{
  if (emu68->bus_addr & 1)
    ;
}

static inline void chkframe_b(emu68_t * const emu68, const int flags)
{
  chkframe(emu68, emu68->bus_addr, flags);
}

static inline void chkframe_w(emu68_t * const emu68, const int flags)
{
  chk_buseven(emu68);
  chkframe(emu68, emu68->bus_addr+0, flags);
  chkframe(emu68, emu68->bus_addr+1, flags);
}

static inline void chkframe_l(emu68_t * const emu68, const int flags)
{
  chk_buseven(emu68);
  chkframe(emu68, emu68->bus_addr+0, flags);
  chkframe(emu68, emu68->bus_addr+1, flags);
  chkframe(emu68, emu68->bus_addr+2, flags);
  chkframe(emu68, emu68->bus_addr+3, flags);
}


EMU68_EXTERN
/**
 *  Init memory quick access table.
 *
 *    The emu68_mem_init() function must be call at init time.
 *    Currently this function only call the emu68_mem_reset()
 *    function.
 *
 *  @see emu68_mem_reset()
 */
void emu68_mem_init(emu68_t * const emu68);

EMU68_EXTERN
/**
 *  Destroy memory quick access table.
 */
void emu68_mem_destroy(emu68_t * const emu68);

EMU68_EXTERN
/**
 *  Reset memory quick access table.
 *
 *    The emu68_mem_reset() function restores all memory access to
 *    default. All mapped IO will be discard and replace by onboard
 *    memory access.
 */
void emu68_mem_reset(emu68_t * const emu68);

EMU68_EXTERN
/**
 *  Add a new memory access control area (for new IO).
 *
 *  @param  emu68     emu68 instance
 *  @param  area      which area (bit 16 to 23 of address) to change
 *  @param  read_bwl  read function table (byte, word and long in this order)
 *  @param  write_bwl idem read_bwl for write access.
 *
 * @see emu68_mem_reset_area()
 */
void emu68_mem_new_area(emu68_t * const emu68, u8 area,
                        memfunc68_t *read_bwl,
                        memfunc68_t *write_bwl);

EMU68_EXTERN
/**
 *  Reset memory access control area to default state.
 *
 *  @param  emu68     emu68 instance
 *  @param  area      which area (bit 16 to 23 of address) to reset
 *
 *  @see emu68_mem_new_area()
 */
void emu68_mem_reset_area(emu68_t * const emu68, u8 area);

/**
 * @}
 */

#endif
