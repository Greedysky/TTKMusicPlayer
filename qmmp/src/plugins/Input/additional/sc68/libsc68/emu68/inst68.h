/**
 * @ingroup   lib_emu68
 * @file      emu68/inst68.h
 * @brief     68k instructions header.
 * @author    Benjamin Gerard
 * @date      1999/03/13
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_INST68_H
#define EMU68_INST68_H

#include "emu68_api.h"
#include "type68.h"

/**
 * @defgroup   lib_emu68_inst  68k Instructions
 * @ingroup    lib_emu68_core
 * @{
 */

/**
 * @name Arithmetical instruction functions
 *
 *   The integer arithmetic operations include four basic operations:
 *   ADD, SUB, MUL, and DIV.  They also include CMP, CMPM, CMP2, CLR,
 *   and NEG. The instruction set includes ADD, CMP, and SUB
 *   instructions for both address and data operations with all
 *   operand sizes valid for data operations. Address operands consist
 *   of 16 or 32 bits. The CLR and NEG instructions apply to all sizes
 *   of data operands. Signed and unsigned MUL and DIV instructions
 *   include:
 * - Word multiply to produce a long product.
 * - Long divided by a word divisor (word quotient, word remainder).
 *
 * @{
 */

EMU68_EXTERN
/**
 * 68K ADD/X instruction.
 *
 *  The add68() function performs addition and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized source operand
 *  @param  d      normalized destination operand
 *  @param  c      normalized carry
 *  @return        normalized result
 *  @retval        d + s + c
 */
int68_t add68(emu68_t * const emu68, const int68_t s, int68_t d, int68_t c);

EMU68_EXTERN
/**
 * 68K SUB/X instruction.
 *
 *  The sub68() function performs substraction and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized source operand
 *  @param  d      normalized destination operand
 *  @param  c      normalized carry
 *  @return        normalized result
 *  @retval        d - s - c
 */
int68_t sub68(emu68_t * const emu68, const int68_t s, int68_t d, int68_t c);

EMU68_EXTERN
/**
 * 68K CMP instruction.
 *
 *  The cmp68() function performs substraction d minus s and updates
 *  SR flags accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized source operand
 *  @param  d      normalized destination operand
 */
void cmp68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * 68k NEG/X instruction.
 *
 *  The neg68() function performs 2nd complement and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  d      normalized destination operand
 *  @param  c      normalized carry
 *  @return        normalized result
 *  @retval        -d - c
 */
int68_t neg68(emu68_t * const emu68, int68_t d, int68_t c);

EMU68_EXTERN
/**
 * 68K MULS instruction.
 *
 *  The muls68() function performs signed multiplication and updates
 *  SR flags accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized word source operand
 *  @param  d      normalized word destination operand
 *  @return        normalized long result
 *  @retval        d * s
 */
int68_t muls68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * 68K MULU instruction.
 *
 *  The mulu68() function performs unsigned multiplication and updates
 *  SR flags accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized word source operand
 *  @param  d      normalized word destination operand
 *  @return        normalized long result
 *  @retval        d * s
 */
int68_t mulu68(emu68_t * const emu68, const uint68_t s, uint68_t d);

EMU68_EXTERN
/**
 * 68K DIVS instruction.
 *
 *  The divs68() function performs signed division and updates SR
 *  flags accordingly. In case of divide by zero the corresponding
 *  exception is triggered. In case of overflow the result is the
 *  destination unchanged; additionnally V SR flag is set.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized word source operand
 *  @param  d      normalized long destination operand
 *  @return        result
 *  @retval        MSW:d%s LSW:d/s
 *  @retval        unchanged d in case of overflow
 */
int68_t divs68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * 68K DIVU instruction.
 *
 *  The divu68() function performs unsigned division and updates SR
 *  flags accordingly. In case of divide by zero the corresponding
 *  exception is triggered. In case of overflow the result is the
 *  destination unchanged; additionnally V SR flag is set.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized word source operand
 *  @param  d      normalized long destination operand
 *  @return        result
 *  @retval        MSW:d%s LSW:d/s
 *  @retval        unchanged d in case of overflow
 */
int68_t divu68(emu68_t * const emu68, const uint68_t s, uint68_t d);

EMU68_EXTERN
/**
 * 68K CLR instruction.
 *
 *  The clr68() function clears and updates SR flags accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @retval        0
 */
int68_t clr68(emu68_t * const emu68);

/**
 * @}
 */

/**
 * @name Logical instruction functions
 *
 *  The logical operation instructions (AND, OR, EOR, and NOT) perform
 *  logical operations with all sizes of integer data operands.
 *
 *  @{
 */

EMU68_EXTERN
/**
 * 68K AND instruction.
 *
 *  The and68() function performs bitwise AND and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized source operand
 *  @param  d      normalized destination operand
 *  @return        normalized result
 *  @retval        d & s
*/
int68_t and68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * 68K OR instruction.
 *
 *  The orr68() function performs bitwise OR and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized source operand
 *  @param  d      normalized destination operand
 *  @return        normalized result
 *  @retval        d | s
*/
int68_t orr68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * 68K EOR instruction.
 *
 *  The eor68() function performs bitwise OR and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance.
 *  @param  s      normalized source operand
 *  @param  d      normalized destination operand
 *  @return        normalized result
 *  @retval        d ^ s
*/
int68_t eor68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * 68K NOT instruction.
 *
 *  The not68() function performs bitwise NOT (aka 1st complement) and
 *  updates SR flags accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @return        normalized result
 *  @retval        ~d
 */
int68_t not68(emu68_t * const emu68, int68_t d);

/**
 * @}
 */


/**
 * @name  Bit manipulation functions
 *
 *  BTST, BSET, BCLR, and BCHG are bit manipulation instructions. All
 *  bit manipulation operations can be performed on either registers
 *  or memory. The bit number is specified either as immediate data or
 *  in the contents of a data register. Register operands are 32 bits
 *  long, and memory operands are 8 bits long. Table 3-6 summarizes
 *  bit manipulation operations; Z refers to the zero bit of the CCR.
 *
 * @{
 */

EMU68_EXTERN
/**
 * 68K BTST instruction (Bit TeST).
 *
 *  The btst68() function performs a bit test and updates SR flags
 *  accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  val    value to test
 *  @param  bit    bit number
 */
void btst68(emu68_t * const emu68, const int68_t val, const int bit);

EMU68_EXTERN
/**
 * 68K BSET instruction (Bit SET).
 *
 *  The bset68() function performs a bit test and set and updates SR
 *  flags accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  val    value to test
 *  @param  bit    bit number
 *  @return        result
 *  @retval        val | (1<<bit)
 */
int68_t bset68(emu68_t * const emu68, const int68_t val, const int bit);

EMU68_EXTERN
/**
 * 68K BCLR instruction (Bit CLeaR).
 *
 *  The bclr68() function performs a bit test and clear and updates SR
 *  flags accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  val    value to test
 *  @param  bit    bit number
 *  @return        result
 *  @retval        val & ~(1<<bit)
 */
int68_t bclr68(emu68_t * const emu68, const int68_t val, const int bit);

EMU68_EXTERN
/**
 * 68K BCHG instruction (Bit CHanGe).
 *
 *  The bchg68() function performs a bit test and change and updates SR
 *  flags accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  val    value to test
 *  @param  bit    bit number
 *  @return        result
 *  @retval        val ^ (1<<bit)
 */
int68_t bchg68(emu68_t * const emu68, const int68_t val, const int bit);

/**
 * @}
 */


/**
 * @name  Program control instructions
 *
 *  A set of subroutine call and return instructions and conditional
 *  and unconditional branch instructions perform program control
 *  operations. Also included are test operand instructions (TST and
 *  FTST), which set the integer or floating-point condition codes for
 *  use by other program and system control instructions. NOP forces
 *  synchronization of the internal pipelines. Table 3-9 summarizes
 *  these instructions.
 *
 * @{
 */

EMU68_EXTERN
void rts68(emu68_t * const emu68);

EMU68_EXTERN
void rte68(emu68_t * const emu68);

EMU68_EXTERN
void rtr68(emu68_t * const emu68);

EMU68_EXTERN
void bsr68(emu68_t * const emu68, const addr68_t addr);

EMU68_EXTERN
void (* const bcc68[])(emu68_t * const, const addr68_t);

EMU68_EXTERN
void jmp68(emu68_t * const emu68, const addr68_t addr);

EMU68_EXTERN
void jsr68(emu68_t * const emu68, const addr68_t addr);

EMU68_EXTERN
void nop68(emu68_t * const emu68);

EMU68_EXTERN
void tst68(emu68_t * const emu68, const int68_t a);

EMU68_EXTERN
int68_t tas68(emu68_t * const emu68, const int68_t a);

EMU68_EXTERN
void (* const dbcc68[])(emu68_t * const emu68, const int dn);

EMU68_EXTERN
int (* const scc68[])(emu68_t * const emu68);


/**
 * @}
 */


/**
 * @name  Data move instructions
 *
 *  The MOVE instructions with their associated addressing modes are
 *  the basic means of transferring and storing addresses and
 *  data. MOVE instructions transfer byte, word, and long-word
 *  operands from memory to memory, memory to register, register to
 *  memory, and register to register. MOVE instructions transfer word
 *  and long-word operands and ensure that only valid address
 *  manipulations are executed.
 *
 *  Data move and associated instructions are MOVEM, MOVEP, MOVEQ,
 *  EXG, LEA, PEA, LINK, and UNLK.
 *
 * @{
 */

EMU68_EXTERN
/**
 * 68K LEA instruction (load effective address).
 *
 *  The lea68() function returns effective address.
 *
 *  @param  emu68  68k emulator instance
 *  @param  mode   addressing mode [0..7]
 *  @param  reg    address register [0..7]
 *  @return effective address
 */
addr68_t lea68(emu68_t * const emu68, const  int mode, const int reg);

EMU68_EXTERN
/**
 * 68K PEA instruction (push effective address).
 *
 *  The pea68() function pushs effective address into the stack.
 *
 *  @param  emu68  68k emulator instance
 *  @param  mode   addressing mode [0..7]
 *  @param  reg    address register [0..7]
 *  @return effective address
 */
addr68_t pea68(emu68_t * const emu68, const  int mode, const int reg);

EMU68_EXTERN
void exg68(emu68_t * const emu68, const int reg0, const int reg9);

EMU68_EXTERN
void link68(emu68_t * const emu68, const int reg);

EMU68_EXTERN
void unlk68(emu68_t * const emu68, const int reg);

/* EMU68_EXTERN */
/* void movem68(emu68_t * const emu68, const int reg0, const int reg9); */

/* EMU68_EXTERN */
/* void movep68(emu68_t * const emu68, const int reg0, const int reg9); */

/**
 * @}
 */


/**
 * @name  Shifting instructions
 *
 *  The ASR, ASL, LSR, and LSL instructions provide shift operations
 *  in both directions. The ROR, ROL, ROXR, and ROXL instructions
 *  perform rotate (circular shift) operations, with and without the
 *  CCR extend bit (X-bit). All shift and rotate operations can be
 *  performed on either registers or memory.  Register shift and
 *  rotate operations shift all operand sizes. The shift count can be
 *  specified in the instruction operation word (to shift from 1 – 8
 *  places) or in a register (modulo 64 shift count).  Memory shift
 *  and rotate operations shift word operands one bit position
 *  only. The SWAP instruction exchanges the 16-bit halves of a
 *  register. Fast byte swapping is possible by using the ROR and ROL
 *  instructions with a shift count of eight, enhancing the
 *  performance of the shift/rotate instructions. Table 3-5 is a
 *  summary of the shift and rotate operations. In Table 3-5, C and X
 *  refer to the C-bit and X- bit in the CCR.
 *
 *  @{
 */

EMU68_EXTERN
/**
 * SWAP high and low words.
 *
 *  The swap68() function performs a MSW LSW swapping and updates SR
 *  flags accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      unnormalized destination operand
 *  @return        result
 */
void swap68(emu68_t * const emu68, const int d);

EMU68_EXTERN
/**
 * Logical Shift Left.
 *
 *  The lsl68() function performs logical (unsigned) shift left and
 *  updates SR flags accordingly.
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 *  @retval        d << s
 */
int68_t lsl68(emu68_t * const emu68, uint68_t d, int s, const int l);

EMU68_EXTERN
/**
 * Logical Shift Right.
 *
 *  The lsr68() function performs logical (unsigned) shift right and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 *  @retval        d >> s
 */
int68_t lsr68(emu68_t * const emu68, uint68_t d, int s, const int l);

EMU68_EXTERN
/**
 * Arithmetic Shift Left.
 *
 *  The asl68() function performs arithmetic (signed) shift left and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 *  @retval        d << s
 */
int68_t asl68(emu68_t * const emu68, int68_t d, int s, const int l);

EMU68_EXTERN
/**
 * Arithmetic Shift Right.
 *
 *  The asr68() function performs arithmetic (signed) shift right and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 *  @retval        d >> s
 */
int68_t asr68(emu68_t * const emu68, int68_t d, int s, const int l);

EMU68_EXTERN
/**
 * ROtation Left.
 *
 *  The rol68() function performs bit rotation to the left and updates
 *  SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 */
int68_t rol68(emu68_t * const emu68, uint68_t d, int s, const int l);

EMU68_EXTERN
/**
 * ROtation Right.
 *
 *  The ror68() function performs bit rotation to the right and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 */
int68_t ror68(emu68_t * const emu68, uint68_t d, int s, const int l);

EMU68_EXTERN
/**
 * ROtation eXtended Left.
 *
 *  The roxl68() function performs extended bit rotation to the left
 *  and updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 */
int68_t roxl68(emu68_t * const emu68, uint68_t d, int s, const int l);

EMU68_EXTERN
/**
 * ROtation eXtended Right.
 *
 *  The roxr68() function performs extended bit rotation to the right
 *  and updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance
 *  @param  d      normalized destination operand
 *  @param  s      source operand (shift count)
 *  @param  l      word length in bit minus one (7, 15 or 31)
 *  @return        normalized result
 */
int68_t roxr68(emu68_t * const emu68, uint68_t d, int s, const int l);

/**
 * @}
 */


/**
 * @name  Binary Coded Decimal (BCD) instructions
 *
 *  Three instructions support operations on binary-coded decimal
 *  (BCD) numbers. The arithmetic operations on packed BCD numbers are
 *  ABCD, SBCD, and NBCD.
 *
 * @{
 */

EMU68_EXTERN
/**
 * Negate Binary Coded Decimal with extend.
 *
 *  The nbcd68() function performs BCD negating with extend and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance.
 *  @param  d      destination operand
 *  @return        result
 *  @retval        0 - d,10 - X
 */
int68_t nbcd68(emu68_t * const emu68, int68_t d);

EMU68_EXTERN
/**
 * Addition Binary Coded Decimal with extend.
 *
 *  The abcd68() function performs BCD addition with extend and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance.
 *  @param  d      destination operand
 *  @return        result
 *  @retval        s,10 + d,10 + X
 */
int68_t abcd68(emu68_t * const emu68, const int68_t s, int68_t d);

EMU68_EXTERN
/**
 * Substract Binary Coded Decimal with extend.
 *
 *  The sbcd68() function performs BCD substraction with extend and
 *  updates SR flags accordingly
 *
 *  @param  emu68  68k emulator instance.
 *  @param  d      destination operand
 *  @param  s      source operand
 *  @return        result
 *  @retval        d,10 - s,10 - X
 */
int68_t sbcd68(emu68_t * const emu68, const int68_t s, int68_t d);

/**
 * @}
 */


/**
 * @name Exception functions
 * @{
 */

EMU68_EXTERN
void buserror68(emu68_t * const emu68, const int addr, const int mode);

EMU68_EXTERN
void exception68(emu68_t * const emu68, const int vector, const int level);

/**
 * @}
 */


/**
 * @name  System Control Instructions
 *
 *  Privileged and trapping instructions as well as instructions that
 *  use or modify the CCR provide system control operations. FSAVE and
 *  FRESTORE save and restore the nonuser visible portion of the FPU
 *  during context switches in a virtual memory or multitasking
 *  system. The conditional trap instructions, which use the same
 *  conditional tests as their corresponding program control
 *  instructions, allow an optional 16- or 32-bit immediate operand to
 *  be included as part of the instruction for passing parameters to
 *  the operating system. These instructions cause the processor to
 *  flush the instruction pipe. Table 3-10 summarizes these
 *  instructions. See 3.2 Integer Unit Condition Code Computation for
 *  more details on condition codes.
 *
 * @{
 */

EMU68_EXTERN
void chk68(emu68_t * const emu68, const int68_t a, const int68_t b);

EMU68_EXTERN
void illegal68(emu68_t * const emu68);

EMU68_EXTERN
void linea68(emu68_t * const emu68);

EMU68_EXTERN
void linef68(emu68_t * const emu68);

EMU68_EXTERN
void trap68(emu68_t * const emu68, const int n);

EMU68_EXTERN
void trapv68(emu68_t * const emu68);

EMU68_EXTERN
void andtosr68(emu68_t * const emu68, int68_t v);

EMU68_EXTERN
void orrtosr68(emu68_t * const emu68, int68_t v);

EMU68_EXTERN
void eortosr68(emu68_t * const emu68, int68_t v);

EMU68_EXTERN
void reset68(emu68_t * const emu68);

EMU68_EXTERN
void stop68(emu68_t * const emu68);

/**
 * @}
 */


/**
 * @}
 */

#endif
