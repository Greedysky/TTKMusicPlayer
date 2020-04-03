/**
 * @ingroup   lib_emu68
 * @file      emu68/struct68.h
 * @brief     Struture definitions header.
 * @author    Benjamin Gerard
 * @date      1999-03-13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_STRUCT68_H
#define EMU68_STRUCT68_H

#include "type68.h"

/**
 * @addtogroup lib_emu68_types
 * @{
 */

/**
 * IO no pending interruption return value.
 *
 *   The next_int function of IO plugin must return IO68_NO_INT when
 *   no interruption has been triggered.
 */
#define IO68_NO_INT (0x80000000)

/** Convenience macro to access reg68 in the current emu68 instance. */
#define REG68 (emu68->reg)

/** Mask for memory overflow. */
#define MEMMSK68 (emu68->memmsk)


/**
 * @name  Memory access caller type
 * @{
 */

/** Write memory function. */
typedef void (*memwfunc68_t)(emu68_t * const);

/** Write IO chip function. */
typedef void (*iomemfunc68_t)(io68_t * const);

/**
 * @}
 */

/** First level (16 lines) decoder function. */
typedef void (linefunc68_t)(emu68_t * const, int, int);

#ifndef EMU68_MONOLITIC
/** @nodoc */
# define DECL_LINE68(N)                                 \
  void N(emu68_t * const emu68, int reg9, int reg0)
/** @nodoc */
# define DECL_STATIC_LINE68(N) static DECL_LINE68(N)
#else
/** @nodoc */
# define DECL_LINE68(N)                                         \
  static void N(emu68_t * const emu68, int reg9, int reg0)
/** @nodoc */
# define DECL_STATIC_LINE68(N) DECL_LINE68(N)
#endif


/**
 * 68K interruption exception structure.
 */
typedef struct
{
  int       vector;              /**< Interrupt vector number       */
  int        level;              /**< Interrupt level [0..7].       */
  cycle68_t  cycle;              /**< Cycle the interruption falls. */
} interrupt68_t;


/**
 * IO emulator plugin structure.
 *
 *   All 68K IO must have a filled io68_t structure to be warm plug or
 *   unplug with ioplug interface.
 *
 */
struct io68_s
{
  struct io68_s * next;             /**< IO list; pointer to next.       */
  char name[32];                    /**< IO identifier name.             */
  addr68_t addr_lo;                 /**< IO mapping area start address.  */
  addr68_t addr_hi;                 /**< IO mapping area end address.    */

  iomemfunc68_t r_byte;             /**< IO read byte function.          */
  iomemfunc68_t r_word;             /**< IO read word function.          */
  iomemfunc68_t r_long;             /**< IO read long function.          */

  iomemfunc68_t w_byte;             /**< IO write byte function.         */
  iomemfunc68_t w_word;             /**< IO write word function.         */
  iomemfunc68_t w_long;             /**< IO write long function.         */

  /** IO interruption function claim. */
  interrupt68_t* (*interrupt)(io68_t * const, const cycle68_t);
  /** IO get next interruption cycle. */
  cycle68_t      (*next_interrupt)(io68_t * const, const cycle68_t);
  /** IO adjust cycle function.       */
  void           (*adjust_cycle)(io68_t * const, const cycle68_t);
  /** IO reset function.              */
  int            (*reset)(io68_t * const);
  /** Destructor. */
  void           (*destroy)(io68_t * const);

  /** Emulator this IO is attached to. */
  emu68_t * emu68;
};


/**
 * 68K internal registers.
 */
typedef struct
{
  s32 d[8];                    /**< 68K data registers.      */
  s32 a[8];                    /**< 68K address registers.   */
  s32 usp;                     /**< 68K User Stack Pointers. */
  s32 pc;                      /**< 68K Program Counter.     */
  int sr;                      /**< 68K Status Register.     */
} reg68_t;

/**
 * 68k register index.
 * @{
 */
enum emu68_regidx_e {
  REG68_D0_IDX,                /**< D0 register index.  */
  REG68_D1_IDX,                /**< D1 register index.  */
  REG68_D2_IDX,                /**< D2 register index.  */
  REG68_D3_IDX,                /**< D3 register index.  */
  REG68_D4_IDX,                /**< D4 register index.  */
  REG68_D5_IDX,                /**< D5 register index.  */
  REG68_D6_IDX,                /**< D6 register index.  */
  REG68_D7_IDX,                /**< D7 register index.  */
  REG68_A0_IDX,                /**< A0 register index.  */
  REG68_A1_IDX,                /**< A1 register index.  */
  REG68_A2_IDX,                /**< A2 register index.  */
  REG68_A3_IDX,                /**< A3 register index.  */
  REG68_A4_IDX,                /**< A4 register index.  */
  REG68_A5_IDX,                /**< A5 register index.  */
  REG68_A6_IDX,                /**< A6 register index.  */
  REG68_A7_IDX,                /**< A7 register index.  */
  REG68_US_IDX,                /**< USP register index. */
  REG68_PC_IDX,                /**< PC register index.  */
  REG68_SR_IDX,                /**< SR register index.  */
};

#define REG68_DN_IDX(N) (REG68_D0_IDX+(N&7)) /**< Dn register index.  */
#define REG68_AN_IDX(N) (REG68_A0_IDX+(N&7)) /**< An register index.  */
/**
 * @}
 */

/**
 * Exception trapping handler.
 *
 *   The emu68_handler_t handler is called by EMU68 when an exception
 *   occurs. It does include interruption triggered by IO chip as well
 *   as software exception like TRACE, ILLEGAL, ZERO DIVIDE, CHK,
 *   TRAP, RESET ... and special emulator interrupts.
 *
 *  @param  emu68   emulator instance
 *  @param  vector  exception vector number
 *  @param  cookie  user-data pointer
*/
typedef void (*emu68_handler_t)(emu68_t* const emu68, int vector, void * cookie);

/** Breakpoint definition. */
typedef struct {
  addr68_t addr;                        /**< Breakpoint address.          */
  uint68_t count;                       /**< Break countdown; 0:disable   */
  uint68_t reset;                       /**< Reset countdown after break. */
} emu68_bp_t;

/** 68K Emulator struct. */
struct emu68_s {
  char name[32];                        /**< Identifier.            */

  /* Error. */
  char err[4][128];                     /**< Error message stack.   */
  int  nerr;                            /**< Error counter.         */

  reg68_t   reg;                     /**< 68000 internal registers. */

  int       inst_pc;               /**< PC of executed instruction. */
  int       inst_sr;               /**< SR of executed instruction. */

  cycle68_t cycle;                   /**< Internal cycle counter.   */
  uint68_t  clock;                   /**< Master clock frequency.   */

  /* Exception trapping. */
  emu68_handler_t   handler;         /**< Exception trap handler.   */
  void            * cookie;          /**< User data.                */

  int      status;                      /**< Execution status.      */
  uint68_t instructions;                /**< Instruction countdown. */
  addr68_t finish_sp;                   /**< Finish Stack Pointer.  */

  /* IO chips. */
  int      nio;                       /**< # IO plug in IO-list.    */
  io68_t * iohead;                    /**< Head of IO-list.         */
  io68_t * interrupt_io;              /**< Current interuptible IO. */
  io68_t * mapped_io[256];            /**< IO areas.                */
  io68_t * memio;                     /**< IO to access memory.     */
  io68_t   ramio; /**< IO used only in debug mode (access control). */
  io68_t   errio; /**< IO used for invalid address in debug mode.   */
  io68_t   nopio; /**< IO used for invalid address in normal mode.  */

  /* Memory access. */
  addr68_t bus_addr;        /**< bus address for memory access.     */
  int68_t  bus_data;        /**< bus data for memory access.        */

  int      frm_chk_fl;      /**< ORed chk change for current frame. */
  struct {
    unsigned pc;                        /**< Pc register value  */
    unsigned ad;                        /**< Addr               */
    int      fl;                        /**< Flags.             */
  } fst_chk /**< first check. */, lst_chk /**< last check. */;
  u8       * chk;           /**< Access-Control-Memory buffer.      */

  emu68_bp_t breakpoints[31];           /**< Hardware breakpoints.  */

  /* Onboard memory. */
  addr68_t memmsk;     /**< Onboard memory mask (2^log2mem-1).      */
  int      log2mem;    /**< Onboard memory buffer size (2^log2mem). */
  u8       mem[32];    /**< Onboard memory buffer.
                            @note   Must be last in struct.         */
};

static inline
void inl_setcycle68(emu68_t * const emu68, const cycle68_t n)
{
#ifdef EMU68CYCLE
  emu68->cycle = n;
#endif
}

static inline
void inl_addcycle68(emu68_t * const emu68, const cycle68_t n)
{
#ifdef EMU68CYCLE
  emu68->cycle += n;
#endif
}

#ifndef emu68_alloc
# include <stdlib.h>
# define emu68_alloc(size) malloc(size) /**< used instead of malloc(). */
# define emu68_free(size)  free(size)   /**< used instead of free().   */
#endif

/**
 * @}
 */

#endif
