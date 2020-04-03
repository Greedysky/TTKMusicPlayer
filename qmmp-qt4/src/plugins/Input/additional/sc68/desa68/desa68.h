/**
 * @ingroup   lib_desa68
 * @file      desa68.h
 * @brief     Motorola 68K disassembler header.
 * @author    Benjamin Gerard
 * @date      1999-03-17
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef DESA68_H
#define DESA68_H

#ifndef DESA68_EXTERN
# ifdef __cplusplus
#   define DESA68_EXTERN extern "C"
# else
#   define DESA68_EXTERN extern
# endif
#endif

#ifndef DESA68_API
/* Building */
# ifdef DESA68_EXPORT
#  if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC)
#   define DESA68_API __declspec(dllexport)
#  elif defined(HAVE_VISIBILITY)
#   define DESA68_API DESA68_EXTERN __attribute__ ((visibility("default")))
#  else
#   define DESA68_API DESA68_EXTERN
#  endif
/* Using */
# else
#  if defined(DESA68_DLL)
#   define DESA68_API __declspec(dllimport)
#  else
#   define DESA68_API DESA68_EXTERN
#  endif
# endif
#endif

/**
 * @defgroup lib_desa68 desa68 library
 * @ingroup  sc68_dev_lib
 *
 * desa68 is a standalone library to disassemble 68000 machine code.
 *
 * Additionally desa68 classes instructions by category; along with it
 * determines possible program branch. Together with it helps to
 * source machine code.
 *
 * Optionnally the disassembler may disassemble with symbol instead of
 * absolute address or long immediat. A supplemental control is
 * available to choose the range of address that must be disassembled
 * as symbol.
 *
 * A complete example of how to use it may be found in the @ref
 * prg_sourcer68 sourcer68 directory. This library is also used by the
 * deprecated @ref prg_debug68 debug68 and @ref prg_cdbg68 cdbg68
 * programs.
 *
 * Memory access
 *
 *   There is 2 ways for the disassembler to access instruction data
 *   bytes. The legacy method and the new user callback method. The
 *   callback field desa68_t::memget is set to a function that is
 *   called by the disassembler or each bytes. The legacy method is a
 *   buffer pointed by desa68_t::memptr of desa68_t::memlen bytes
 *   starting at address/origin desa68_t::memorg. Internally the
 *   legacy method calls its own callback so in term of performance
 *   it's mostly the same (depending on the user callback
 *   complexity). When using the callback method desa68_t::memptr can
 *   be null. It's also possible not to set desa68_t::memorg and
 *   desa68_t::memlen fileds, only if a user callback is used for
 *   symbols or the disassembler is set not to generated symbols.
 *
 * Symbol lookup
 *
 *   Just as memory access there is 2 ways for the disassembler to
 *   replace an address value by a symbol. The legacy method and the
 *   new user callback method. The callback field desa68_t::symget is
 *   set to a function that is called by the disassembler or each time
 *   it needs to determine if a value is a symbol. The legacy method
 *   is limited to simple generated symbol (L123456). If the
 *   DESA68_SYMBOL_FLAG is set in desa68_t::flags and the value is in
 *   the memory range, an address will be replaced by its
 *   symbol. There is an additionnal range for immediat long to be
 *   replace defined by the 2 fields desa68_t::immsym_min and
 *   desa68_t::immsym_max.
 *
 * @{
 */

#ifndef DESA68_API
# error "DESA68_API should be defined"
#endif

/**
 * Disassembly option flags.
 * @anchor desa68_opt_flags
 *
 * Use bitwise OR operation with these values to set the the
 * desa68_parm_t::flags in order to configure the disassembler.
 *
 * @note DESA68_ASCII_FLAG, DESA68_ALNUM_FLAG and DESA68_GRAPH_FLAG
 * are mutualy exclusive.
 */
enum {
  /**
   * Disassemble with symbol.
   *
   * DESA68_SYMBOL_FLAG bit controls the general symbolic
   * disassembly. If it is set all valid candidats are transformed to
   * symbol. A valid candidat is an address within the memory range or
   * an immediat higher or equal to desa68_t::immsym_min and lower
   * than desa68_t::immsym_max.
   */
  DESA68_SYMBOL_FLAG = (1<<0),

  /**
   * Force source operand to be a valid candidat for symbol.
   */
  DESA68_SRCSYM_FLAG = (1<<1),

  /**
   * Force destination operand to be a valid candidat for symbol.
   */
  DESA68_DSTSYM_FLAG = (1<<2),

  /**
   * Disassemble immediate values to ASCII chars.
   *
   * DESA68_ASCII_FLAG is set to enable the conversion of immediate
   * values to a corresponding quoted string if all its bytes belong
   * to a special char set (alpha-numeric, space and a few other chars
   * [-_!.#]).
   *
   * @warning The name of this flag cab be misleading.
   * @see DESA68_ALNUM_FLAG
   * @see DESA68_GRAPH_FLAG
   */
  DESA68_ASCII_FLAG = (1<<3),

  /**
   * Disassemble immediate values to alpha-numeric chars.
   *
   * DESA68_ALNUM_FLAG works like DESA68_ASCII_FLAG but for a pure
   * alpha-numeric charset.
   *
   * @see DESA68_ASCII_FLAG
   * @see DESA68_GRAPH_FLAG
   */
  DESA68_ALNUM_FLAG = (1<<4),

  /**
   * Disassemble immediate values to graphic chars.
   *
   * DESA68_GRAPH_FLAG works like DESA68_ASCII_FLAG but for a graphical
   * charset [32 to 126].
   *
   * @see DESA68_ASCII_FLAG
   * @see DESA68_ALNUM_FLAG
   */
  DESA68_GRAPH_FLAG = DESA68_ASCII_FLAG|DESA68_ALNUM_FLAG,

  /**
   * Lowercase disassembly.
   */
  DESA68_LCASE_FLAG = (1<<5),

};

/**
 * Instruction types
 * @anchor desa68_inst_types
 *
 * These values are used by the desa68_t::itype to help determine what
 * kind of instruction was disassenbled.
 *
 */
enum {
  /* An error occured. */
  DESA68_ERR = -1,

  /** Invalid instruction (disassembled as data). */
  DESA68_DCW = 0,

  /** Generic instruction. */
  DESA68_INS,

  /** Branch always instruction (bra/jmp). */
  DESA68_BRA,

  /** Subroutine (bsr/jsr) or Conditionnal branch instruction (bcc/dbcc). */
  DESA68_BSR,

  /** Return from subroutine/Interruption instruction (rts/rte). */
  DESA68_RTS,

  /** Software interrupt instruction (trap/chk). */
  DESA68_INT,

  /** Nop instruction. */
  DESA68_NOP
};

/**
 * Error flags.
 * @anchor desa68_error_flags
 */
enum {
  /** Output writer failed. */
  DESA68_ERR_OUT  = 1,

  /** Attempt to read word or long at odd address. */
  DESA68_ERR_ODD  = 2,

  /** Memory access failed. */
  DESA68_ERR_MEM  = 4,

  /** Uncommon byte immediate MSB. */
  DESA68_ERR_IMM  = 8
};

/**
 * Operand types.
 */

/**
 * Reference type.
 * @anchor desa68_ref_types
 * @warning  DO NOT CHANGE ORDER.
 */
enum {
  DESA68_OP_B,                          /**< Byte memory access. */
  DESA68_OP_W,                          /**< Word memory access. */
  DESA68_OP_L,                          /**< Long memory access. */
  DESA68_OP_A,                          /**< Address Loaded.     */
  DESA68_OP_NDEF = 255                  /**< Not defined.        */
};

/**
 * 68k registers identifier (8bit).
 *
 * @warning DO NOT CHANGE ORDER.
 */
enum {
  DESA68_REG_DN  = 0,                 /**< First data register.     */
  DESA68_REG_AN  = 8,                 /**< First address register.  */
  DESA68_REG_SP  = 15,                /**< Stack pointer.           */
  DESA68_REG_USP,                     /**< User stack pointer.      */
  DESA68_REG_CCR,                     /**< Code condition register. */
  DESA68_REG_SR,                      /**< Status register.         */
  DESA68_REG_PC,                      /**< Program counter.         */
  DESA68_REG_LAST,                    /**< Not real registers.      */
 };

/**
 * Symbol type for the desa68_t::symget() type argument.
 */
enum {
  DESA68_SYM_NDEF,       /**< Undefined.                            */
  DESA68_SYM_DABW,       /**< destination operand absolute address. */
  DESA68_SYM_DABL,       /**< destination operand absolute address. */
  DESA68_SYM_SABW,       /**< source operand absolute address.      */
  DESA68_SYM_SABL,       /**< source operand absolute address.      */
  DESA68_SYM_SIMM,       /**< source operand immediat value.        */
  DESA68_SYM_SPCR,       /**< source operand pc relativ address.    */
  DESA68_SYM_SPCI,       /**< source operand pc indexed address.    */

  DESA68_SYM_LAST
};

/**
 * type for the 68K disassemble pass parameters structure.
 */
typedef struct desa68_parm_s desa68_t;

/**
 * 68K disassemble pass parameters.
 *
 * The desa68_t data structure contains the information necessary to
 * disassemble 68K instructions.
 *
 * There are 3 categories of fields in this structure.
 * - Input parameters; Must be set before calling the desa68() function.
 * - Output parameters; Information on disassembled instruction
 *                          filled by desa68() function.
 * - Internal fields (start with an underscore "_").
 *
 * @note The desa68_t::pc field is both input and output since it
 *       is use to set the address of the instruction to decode and
 *       returns with the value of the next one.
 */
struct desa68_parm_s
{
  /**
   * @name Input parameters.
   *
   * These parameters must be set before calling the desa68() functions.
   *
   * @{
   */

  /** User private data (cookie). */
  void          *user;

  /** Function to read memory (or null). */
  int          (*memget)(desa68_t *, unsigned int addr, int flag);
  /** 68K memory buffer. */
  unsigned char *memptr;

  /** Start address of the memory buffer. */
  unsigned int   memorg;

  /** Size of memory. */
  unsigned int   memlen;

  /** 68k memory addressing space (normally 24 bits). */
  unsigned int   memmsk;

  /**
   * Address (Offset in mem) of instruction to disassemble; Returns
   * with the address of the next instruction.
   */
  unsigned int   pc;
  int            flags;  /**< @ref desa68_opt_flags "Disassemble options" */

  /**
   * A function to test for graphical chars. If not set (null) an
   * appropriate function is selected according to desa68_t::flags
   * DESA68_GRAPH_FLAG family flags.
   */
  int          (*ischar)(desa68_t *, int);

  /** Writer function (0:use default writer). */
  void         (*strput)(desa68_t *, int);
  /** String used by default writer (can be 0) */
  char          *str;
  /** Size of the buffer pointed by str (forced to 0 on null pointer). */
  int            strmax;

  /** Output symbol function (or 0). */
  const char * (*symget)(desa68_t *, unsigned int addr, int type);

  /**
   * Minimum value to select long immediate as symbol or address.
   * @see DESA68_SYMBOL_FLAG for more details
   * @see immsym_max
   */
  unsigned int   immsym_min;

  /**
   * Maximum value to select long immediate as symbol or address.
   * @see DESA68_SYMBOL_FLAG for more details
   * @see immsym_min
   */
  unsigned int   immsym_max;

  /**
   * @}
   */


  /**
   * @name Output parameters.
   *
   * These parameters are setted by the desa68() functions.
   * @{
   */

  unsigned int  regs; /**< used registers bit mask.                 */
  struct desa68_ref {
    unsigned int type; /**< reference types. @see desa68_ref_types  */
    unsigned int addr; /**< reference address (if type is defined). */
  }
  sref,                       /**< source operand reference.        */
  dref;                       /**< destination operand reference.   */

  unsigned char itype; /**< Instruction type. @see desa68_inst_types */
  unsigned char error; /**< Error flags. @see desa68_error_flags     */

  /**
   * Output char count.
   */
  unsigned int out;

  /**
   * @}
   */


  /**
   * @name Miscellaneous internal variables.
   * @internal
   * @{
   */

  /**
   * Intermediat opcode decoding.
   */
  unsigned int  _pc;       /**< pc origin at start of disassembly. */
  signed   int  _w;        /**< Last decoded word (sign extended). */
  unsigned int  _opw;      /**< First decoded word (opcode).       */
  unsigned char _reg0;     /**< bit 2:0 of opcode word. */
  unsigned char _mode3;    /**< bit 5:3 of opcode word. */
  unsigned char _opsz;     /**< bit 7:6 of opcode word. */
  unsigned char _mode6;    /**< bit 8:6 of opcode word. */
  unsigned char _reg9;     /**< bit B:9 of opcode word. */
  unsigned char _line;     /**< bit F:C of opcode word. */
  unsigned char _adrm0;    /**< _mode3+_reg0. */
  unsigned char _adrm6;    /**< _mode6+_reg9. */
  int           _quote;    /**< set when quoting. */
  char          _tmp[16];  /**< small scratch pad */

  /**
   * @}
   */

};

DESA68_API
/**
 * Disassemble a single 68000 instruction.
 *
 * @param  d  Pointer to disassemble pass parameter structure.
 * @return @ref desa68_inst_types instruction type
 */
int desa68(desa68_t *d);


DESA68_API
/**
 * Get version number.
 *
 * @return X*100+Y*10+Z.
 */
int desa68_version(void);


DESA68_API
/**
 * Get version string.
 *
 * @return version string.
 */
const char * desa68_versionstr(void);

/**
 * @}
 */

#endif
