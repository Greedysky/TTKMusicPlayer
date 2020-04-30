//
// ARM7 processor emulator - interpreter core
// version 1.6 / 2008-02-16
// (c) Radoslaw Balcewicz
//

#include "arm7.h"
#include "arm7i.h"
#include "dc_hw.h"

  //--------------------------------------------------------------------------
  // definitions and macros

 /** PC is being incremented after every instruction fetch, so we adjust for
 that on all stores and jumps. */
#define PC_ADJUSTMENT (-4)

  /** Memory access routines. */
#include "arm7memil.c"

  /** Bit shifts compatible with IA32. */
#define SHL(w, k) (((UINT32)(w)) << (k))
#define SHR(w, k) (((UINT32)(w)) >> (k))
#define SAR(w, k) (((INT32)(w)) >> (k))
#define ROR(w, k) (SHR (w, k) | SHL (w, 32 - (k)))

  /** Byte rotation for unaligned 32-bit read. */
#define RBOD(w, i) (ROR (w, (i) * 8))

  /** Data processing macros. */
#define NEG(i) ((i) & (1 << 31))
#define POS(i) (~(i) & (1 << 31))
#define ADDCARRY(a, b, c) \
  ((NEG (a) & NEG (b)) |\
  (NEG (a) & POS (c)) |\
  (NEG (b) & POS (c))) ? 1 : 0;
#define ADDOVERFLOW(a, b, c) \
  ((NEG (a) & NEG (b) & POS (c)) |\
  (POS (a) & POS (b) & NEG (c))) ? 1 : 0;
#define SUBCARRY(a, b, c) \
  ((NEG (a) & POS (b)) |\
  (NEG (a) & POS (c)) |\
  (POS (b) & POS (c))) ? 1 : 0;
#define SUBOVERFLOW(a, b, c)\
  ((NEG (a) & POS (b) & POS (c)) |\
  (POS (a) & NEG (b) & NEG (c))) ? 1 : 0;
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // private functions
	   
  /** Condition EQ. */
static int R_WEQ (struct sARM7 *cpu);
  /** Condition NE. */
static int R_WNE (struct sARM7 *cpu);
  /** Condition CS. */
static int R_WCS (struct sARM7 *cpu);
  /** Condition CC. */
static int R_WCC (struct sARM7 *cpu);
  /** Condition MI. */
static int R_WMI (struct sARM7 *cpu);
  /** Condition PL. */
static int R_WPL (struct sARM7 *cpu);
  /** Condition VS. */
static int R_WVS (struct sARM7 *cpu);
  /** Condition VC. */
static int R_WVC (struct sARM7 *cpu);
  /** Condition HI. */
static int R_WHI (struct sARM7 *cpu);
  /** Condition LS. */
static int R_WLS (struct sARM7 *cpu);
  /** Condition GE. */
static int R_WGE (struct sARM7 *cpu);
  /** Condition LT. */
static int R_WLT (struct sARM7 *cpu);
  /** Condition GT. */
static int R_WGT (struct sARM7 *cpu);
  /** Condition LE. */
static int R_WLE (struct sARM7 *cpu);
  /** Condition AL. */
static int R_WAL (struct sARM7 *cpu);
  /** Undefined condition. */
static int R_Wxx (struct sARM7 *cpu);

  /** Calculates barrel shifter output. */
static UINT32 WyliczPrzes (struct sARM7 *cpu);
  /** Logical shift left. */
static UINT32 LSL_x (struct sARM7 *cpu, UINT32 w, int i);
  /** Logical shift right. */
static UINT32 LSR_x (struct sARM7 *cpu, UINT32 w, int i);
  /** Arithmetic shift right. */
static UINT32 ASR_x (struct sARM7 *cpu, UINT32 w, int i);
  /** Rotate right. */
static UINT32 ROR_x (struct sARM7 *cpu, UINT32 w, int i);
  /** Rotate right extended. */
static UINT32 RRX_1 (struct sARM7 *cpu, UINT32 w);

  /** Group 00x opcodes. */
static void R_G00x (struct sARM7 *cpu);
  /** Multiply instructions. */
static void R_MUL_MLA (struct sARM7 *cpu);
  /** Single data swap. */
static void R_SWP (struct sARM7 *cpu);
  /** PSR Transfer. */
static void R_PSR (struct sARM7 *cpu);
  /** Data processing instructions. */
static void R_DP (struct sARM7 *cpu);
  /** Data processing result writeback. */
static void R_WynikDP (struct sARM7 *cpu, ARM7_REG w);
  /** Data processing flags writeback. */
static void R_FlagiDP (struct sARM7 *cpu, ARM7_REG w);
  /** Single data transfer. */
static void R_SDT (struct sARM7 *cpu);
  /** Rozkaz "Undefined". */
static void R_Und (struct sARM7 *cpu);
  /** Block Data Transfer. */
static void R_BDT (struct sARM7 *cpu);
  /** Block load instructions. */
static void R_LDM (struct sARM7 *cpu, int Rn, UINT32 adres);
  /** Block store instructions. */
static void R_STM (struct sARM7 *cpu, int Rn, UINT32 adres);
  /** Branch/Branch with link. */
static void R_B_BL (struct sARM7 *cpu);
  /** Group 110 opcodes. */
static void R_G110 (struct sARM7 *cpu);
  /** Group 111 opcodes. */
static void R_G111 (struct sARM7 *cpu);

#ifdef ARM7_THUMB
  /** Halfword and Signed Data Transfer. */
static void R_HSDT (struct sARM7 *cpu);
#endif
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // private data

  /** Flag testing functions for conditional execution. */
static int (*s_tabWar [16]) (struct sARM7 *cpu) = {R_WEQ, R_WNE, R_WCS, R_WCC, R_WMI, R_WPL,
 R_WVS, R_WVC, R_WHI, R_WLS, R_WGE, R_WLT, R_WGT, R_WLE, R_WAL, R_Wxx};
  /** Handler table for instruction groups. */
static void (*s_tabGrup [8]) (struct sARM7 *cpu) = {R_G00x, R_G00x, R_SDT, R_SDT, R_BDT,
 R_B_BL, R_G110, R_G111};
  /** Data processing instructions split to arithmetic and logical. */
static int s_tabAL [16] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
 FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE};

  /** Cycles it took for current instruction to complete. */
static int s_cykle;
  //--------------------------------------------------------------------------


  // public functions


  //--------------------------------------------------------------------------
  /** Single step, returns number of burned cycles. */
int ARM7i_Step (struct sARM7 *cpu)
  {
  cpu->kod = arm7_read_32 (cpu, cpu->Rx [ARM7_PC] & ~3);

  // we increment PC here, and if there's a load from memory it will simply
  // overwrite it (all PC modyfing code should be aware of this)
  cpu->Rx [ARM7_PC] += 4;
  s_cykle = 2;
  // condition test and group selection
  if (s_tabWar [(cpu->kod >> 28) & 15] (cpu))
    s_tabGrup [(cpu->kod >> 25) & 7] (cpu);
  return s_cykle;
  }
  //--------------------------------------------------------------------------


  // private functions


  //--------------------------------------------------------------------------
  /** Condition EQ. */
int R_WEQ (struct sARM7 *cpu)
  {
  // "Z set"
  return cpu->Rx [ARM7_CPSR] & ARM7_CPSR_Z;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition NE. */
int R_WNE (struct sARM7 *cpu)
  {
  // "Z clear"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_Z);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition CS. */
int R_WCS (struct sARM7 *cpu)
  {
  // "C set"
  return cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition CC. */
int R_WCC (struct sARM7 *cpu)
  {
  // "C clear"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition MI. */
int R_WMI (struct sARM7 *cpu)
  {
  // "N set"
  return cpu->Rx [ARM7_CPSR] & ARM7_CPSR_N;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition PL. */
int R_WPL (struct sARM7 *cpu)
  {
  // "N clear"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_N);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition VS. */
int R_WVS (struct sARM7 *cpu)
  {
  // "V set"
  return cpu->Rx [ARM7_CPSR] & ARM7_CPSR_V;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition VC. */
int R_WVC (struct sARM7 *cpu)
  {
  // "V clear"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_V);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition HI. */
int R_WHI (struct sARM7 *cpu)
  {
  // "C set and Z clear"
  return (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) &&\
 !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_Z);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition LS. */
int R_WLS (struct sARM7 *cpu)
  {
  // "C clear or Z set"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ||\
 (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_Z);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition GE. */
int R_WGE (struct sARM7 *cpu)
  {
  // "N equals V"
  return (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_N) &&\
 (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_V) || !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_N) &&\
 !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_V);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition LT. */
int R_WLT (struct sARM7 *cpu)
  {
  // "N not equal to V"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_N) &&\
 (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_V) || (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_N) &&\
 !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_V);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition GT. */
int R_WGT (struct sARM7 *cpu)
  {
  // "Z clear AND (N equals V)"
  return !(cpu->Rx [ARM7_CPSR] & ARM7_CPSR_Z) && R_WGE (cpu);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition LE. */
int R_WLE (struct sARM7 *cpu)
  {
  // "Z set OR (N not equal to V)"
  return (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_Z) || R_WLT (cpu);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Condition AL. */
int R_WAL (struct sARM7 *cpu)
  {
  // "(ignored)"
  return TRUE;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Undefined condition. */
int R_Wxx (struct sARM7 *cpu)
  {
  // behaviour undefined
  return FALSE;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Calculates barrel shifter output. */
UINT32 WyliczPrzes (struct sARM7 *cpu)
  {
  int Rm, Rs, i;
  UINT32 w;

  // Rm is source for the shift operation
  Rm = cpu->kod & 15;

  if (cpu->kod & (1 << 4))
    {
    s_cykle++;
    // shift count in Rs (8 lowest bits)
    if (Rm != ARM7_PC)
      w = cpu->Rx [Rm];
    else
      w = (cpu->Rx [ARM7_PC] & ~3) + 12 + PC_ADJUSTMENT;
    // Rs can't be PC
    Rs = (cpu->kod >> 8) & 15;
    i = (UINT8)cpu->Rx [Rs];
    if (i == 0)
      {
      // special case
      cpu->carry = (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ? 1 : 0;
      return w;
      }

    switch ((cpu->kod >> 5) & 3)
      {
      case 0:
        w = LSL_x (cpu, w, i);
        break;
      case 1:
        w = LSR_x (cpu, w, i);
        break;
      case 2:
        w = ASR_x (cpu, w, i);
        break;
      case 3:
        w = ROR_x (cpu, w, i);
        break;
      }
    }
  else
    {
    // shift count as immediate in opcode
    if (Rm != ARM7_PC)
      w = cpu->Rx [Rm];
    else
      w = (cpu->Rx [ARM7_PC] & ~3) + 8 + PC_ADJUSTMENT;
    i = (cpu->kod >> 7) & 31;

    switch ((cpu->kod >> 5) & 3)
      {
      case 0:
        w = LSL_x (cpu, w, i);
        break;
      case 1:
        if (i > 0)
          w = LSR_x (cpu, w, i);
        else
          w = LSR_x (cpu, w, 32);
        break;
      case 2:
        if (i > 0)
          w = ASR_x (cpu, w, i);
        else
          w = ASR_x (cpu, w, 32);
        break;
      case 3:
        if (i > 0)
          w = ROR_x (cpu, w, i);
        else
          w = RRX_1 (cpu, w);
        break;
      }
    }
  return w;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Logical shift left. */
UINT32 LSL_x (struct sARM7 *cpu, UINT32 w, int i)
{
	// LSL #0 copies C into carry out and returns unmodified value
	if (i == 0)
	{
		cpu->carry = (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ? 1 : 0;
		return w;
	}
	// LSL #32 copies LSB to carry out and returns zero
	if (i == 32)
	{
		cpu->carry = w & 1;
		return 0;
	}
	// LSL > #32 returns zero for both carry and output
	if (i > 32)
	{
		cpu->carry = 0;
		return 0;
	}
        // normal shift
	cpu->carry = (w & (1 << (32 - i))) ? 1 : 0;
	w = SHL (w, i);
	return w;
}
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Logical shift right. */
UINT32 LSR_x (struct sARM7 *cpu, UINT32 w, int i)
{
	// LSR #32 copies MSB to carry out and returns zero
	if (i == 32)
	{
		cpu->carry = (w & (1 << 31)) ? 1 : 0;
		return 0;
	}
	// LSR > #32 returns zero for both carry and output
	if (i > 32)
	{
		cpu->carry = 0;
		return 0;
	}
        // normal shift
	cpu->carry = (w & (1 << (i - 1))) ? 1 : 0;
	w = SHR (w, i);
	return w;
}
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Arithmetic shift right. */
UINT32 ASR_x (struct sARM7 *cpu, UINT32 w, int i)
{
	// ASR >= #32 carry out and output value depends on the minus sign
	if (i >= 32)
	{
		if (w & (1 << 31))
		{
			cpu->carry = 1;
			return ~0;
		}

		cpu->carry = 0;
		return 0;
	}
	// normal shift
	cpu->carry = (w & (1 << (i - 1))) ? 1 : 0;
	w = SAR (w, i);
	return w;
}
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Rotate right. */
UINT32 ROR_x (struct sARM7 *cpu, UINT32 w, int i)
{
	// mask count to [0; 31]
	i &= 0x1f;
	// ROR #32,#64,etc. copies MSB into carry out and returns unmodified value
	if (i == 0)
	{
		cpu->carry = (w & (1 << 31)) ? 1 : 0;
		return w;
	}
	// normal shift
	cpu->carry = (w & (1 << (i-1))) ? 1 : 0;
	w = ROR (w, i);
	return w;
}
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Rotate right extended. */
UINT32 RRX_1 (struct sARM7 *cpu, UINT32 w)
  {
  // same as RCR by 1 in IA32
  cpu->carry = w & 1;
  return (w >> 1) | ((cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) << 2);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Group 00x opcodes. */
void R_G00x (struct sARM7 *cpu)
  {
#ifdef ARM7_THUMB
  // 24 constant bits
  if ((cpu->kod & 0x0ffffff0) == 0x012fff10)	// BX - branch with possible mode transfer
  {
  #ifdef ARM7_THUMB
  	int Rn = cpu->Rx[cpu->kod & 0xf];

	// switching to Thumb mode?
	if (Rn & 1)
	{
		ARM7_SetCPSR(cpu->Rx[ARM7_CPSR] | ARM7_CPSR_T);
	}
       
	cpu->Rx[ARM7_PC] = Rn & ~1;
  #endif
  }
  // 15 constant bits
  else if ((cpu->kod & 0x0fb00ff0) == 0x01000090)
    R_SWP (cpu);
  // 10 constant bits
  else if ((cpu->kod & 0x0fc000f0) == 0x00000090)
    R_MUL_MLA (cpu);
  // 10 constant bits
  else if ((cpu->kod & 0x0e400f90) == 0x00000090)
    R_HSDT (cpu);
  // 9 constant bits
  else if ((cpu->kod & 0x0f8000f0) == 0x00800090)
  {
//    logerror("G00x / Multiply long\n");
  }
  // 6 constant bits	 
  else if ((cpu->kod & 0x0e400090) == 0x00400090)
    R_HSDT (cpu);
  // 2 constant bits
  else
    {
    if ((cpu->kod & 0x01900000) == 0x01000000)
      // TST, TEQ, CMP & CMN without S bit are "PSR Transfer"
      R_PSR (cpu);
    else
      // the rest is "Data processing"
      R_DP (cpu);
    }
#else
  if ((cpu->kod & 0x03b00090) == 0x01000090)
    R_SWP (cpu);
  else if ((cpu->kod & 0x03c00090) == 0x00000090)
    R_MUL_MLA (cpu);
  else
    {
    if ((cpu->kod & 0x01900000) == 0x01000000)
      // TST, TEQ, CMP & CMN without S bit are "PSR Transfer"
      R_PSR (cpu);
    else
      // the rest is "Data processing"
      R_DP (cpu);
    }
#endif
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Single data swap. */
void R_SWP (struct sARM7 *cpu)
  {
  int Rn, Rd, Rm;
  UINT32 adres, w;

#define BIT_B (cpu->kod & (1 << 21))

  s_cykle += 4;
  // none of these can be PC
  Rn = (cpu->kod >> 16) & 15;
  Rd = (cpu->kod >> 12) & 15;
  Rm = cpu->kod & 15;
  adres = cpu->Rx [Rn];

  if (BIT_B)
    {
    // "byte"
    w = arm7_read_8 (cpu, adres);
    arm7_write_8 (cpu, adres, (UINT8)cpu->Rx [Rm]);
    }
  else
    {
    // "word"
    w = RBOD (arm7_read_32 (cpu, adres & ~3), adres & 3);
    arm7_write_32 (cpu, adres & ~3, cpu->Rx [Rm]);
    }
  cpu->Rx [Rd] = w;

#undef BIT_B
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Multiply instructions. */
void R_MUL_MLA (struct sARM7 *cpu)
  {
  int Rm, Rs, Rn, Rd;
  UINT32 wynik;

#define BIT_A (cpu->kod & (1 << 21))
#define BIT_S (cpu->kod & (1 << 20))

  s_cykle += 2;
  // none of these can be PC, also Rd != Rm
  Rd = (cpu->kod >> 16) & 15,
  Rs = (cpu->kod >> 8) & 15,
  Rm = cpu->kod & 15;

  // MUL
  wynik = cpu->Rx [Rm] * cpu->Rx [Rs];
  if (BIT_A)
    {
    // MLA
    Rn = (cpu->kod >> 12) & 15;
    wynik += cpu->Rx [Rn];
    }
  cpu->Rx [Rd] = wynik;

  if (BIT_S)
    {
    // V remains unchanged, C is undefined
    cpu->Rx [ARM7_CPSR] &= ~(ARM7_CPSR_N | ARM7_CPSR_Z);
    if (wynik == 0)
      cpu->Rx [ARM7_CPSR] |= ARM7_CPSR_Z;
    cpu->Rx [ARM7_CPSR] |= wynik & 0x80000000;
    }

#undef BIT_S
#undef BIT_A
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** PSR Transfer. */
void R_PSR (struct sARM7 *cpu)
  {
  int Rd, Rm;
  UINT32 w, arg;

#define BIT_I (cpu->kod & (1 << 25))
#define BIT_P (cpu->kod & (1 << 22))

  // none of the registers involved can be PC

  if (cpu->kod & (1 << 21))
    {
    // MSR
    Rm = cpu->kod & 15;
    if (BIT_I)
      // immediate (lower 12 bits)
      arg = ROR (cpu->kod & 0xff, ((cpu->kod >> 8) & 0xf) * 2);
    else
      // register
      arg = cpu->Rx [Rm];

    // decode mask bits
    if (BIT_P)
      {
      w = cpu->Rx [ARM7_SPSR];
      if (ARM7_CPSR_M (cpu->Rx [ARM7_CPSR]) > ARM7_CPSR_M_usr &&\
 ARM7_CPSR_M (cpu->Rx [ARM7_CPSR]) < ARM7_CPSR_M_sys)
        {
        if (cpu->kod & (1 << 16))
          w = (w & 0xffffff00) | (arg & 0x000000ff);
        if (cpu->kod & (1 << 17))
          w = (w & 0xffff00ff) | (arg & 0x0000ff00);
        if (cpu->kod & (1 << 18))
          w = (w & 0xff00ffff) | (arg & 0x00ff0000);
        if (cpu->kod & (1 << 19))
          // ARMv5E should have 0xf8000000 argument mask
          w = (w & 0x00ffffff) | (arg & 0xf0000000);
        }
      // force valid mode
      w |= 0x10;
      cpu->Rx [ARM7_SPSR] = w;
      }
    else
      {
      w = cpu->Rx [ARM7_CPSR];
      // only flags can be changed in User mode
      if (ARM7_CPSR_M (cpu->Rx [ARM7_CPSR]) != ARM7_CPSR_M_usr)
        {
        if (cpu->kod & (1 << 16))
          w = (w & 0xffffff00) | (arg & 0x000000ff);
        if (cpu->kod & (1 << 17))
          w = (w & 0xffff00ff) | (arg & 0x0000ff00);
        if (cpu->kod & (1 << 18))
          w = (w & 0xff00ffff) | (arg & 0x00ff0000);
        }
      if (cpu->kod & (1 << 19))
        // ARMv5E should have 0xf8000000 argument mask
        w = (w & 0x00ffffff) | (arg & 0xf0000000);
      // force valid mode
      w |= 0x10;
      ARM7_SetCPSR (cpu, w);
      }
    }
  else
    {
    // MRS
    Rd = (cpu->kod >> 12) & 15;
    if (BIT_P)
      cpu->Rx [Rd] = cpu->Rx [ARM7_SPSR];
    else
      cpu->Rx [Rd] = cpu->Rx [ARM7_CPSR];
    }

#undef BIT_P
#undef BIT_I
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Data processing instructions. */
void R_DP (struct sARM7 *cpu)
  {
  int Rn;
  ARM7_REG arg1, arg2, w;

#define BIT_I (cpu->kod & (1 << 25))

  // Rn can be PC, so we need to account for that
  Rn = (cpu->kod >> 16) & 15;

  if (BIT_I)
    {
    if (Rn != ARM7_PC)
      arg1 = cpu->Rx [Rn];
    else
      arg1 = (cpu->Rx [ARM7_PC] & ~3) + 8 + PC_ADJUSTMENT;
    // immediate in lowest 12 bits
    arg2 = ROR (cpu->kod & 0xff, ((cpu->kod >> 8) & 0xf) * 2);
    // preload carry out from C
    cpu->carry = (cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ? 1 : 0;
    }
  else
    {
    if (Rn != ARM7_PC)
      arg1 = cpu->Rx [Rn];
    else
      // register or immediate shift?
      if (cpu->kod & (1 << 4))
        arg1 = (cpu->Rx [ARM7_PC] & ~3) + 12 + PC_ADJUSTMENT;
      else
        arg1 = (cpu->Rx [ARM7_PC] & ~3) + 8 + PC_ADJUSTMENT;
    // calculate in barrel shifter
    arg2 = WyliczPrzes (cpu);
    }

  // decode instruction type
  switch ((cpu->kod >> 21) & 15)
    {
    case 0:
      // AND
      R_WynikDP (cpu, arg1 & arg2);
      break;

    case 1:
      // EOR
      R_WynikDP (cpu, arg1 ^ arg2);
      break;

    case 2:
      // SUB
      w = arg1 - arg2;
      cpu->carry = SUBCARRY (arg1, arg2, w);
      cpu->overflow = SUBOVERFLOW (arg1, arg2, w);
      R_WynikDP (cpu, w);
      break;

    case 3:
      // RSB
      w = arg2 - arg1;
      cpu->carry = SUBCARRY (arg2, arg1, w);
      cpu->overflow = SUBOVERFLOW (arg2, arg1, w);
      R_WynikDP (cpu, w);
      break;

    case 4:
      // ADD
      w = arg1 + arg2;
      cpu->carry = ADDCARRY (arg1, arg2, w);
      cpu->overflow = ADDOVERFLOW (arg1, arg2, w);
      R_WynikDP (cpu, w);
      break;

    case 5:
      // ADC
      w = arg1 + arg2 + ((cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ? 1 : 0);
      cpu->carry = ADDCARRY (arg1, arg2, w);
      cpu->overflow = ADDOVERFLOW (arg1, arg2, w);
      R_WynikDP (cpu, w);
      break;

    case 6:
      // SBC
      w = arg1 - arg2 - ((cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ? 0 : 1);
      cpu->carry = SUBCARRY (arg1, arg2, w);
      cpu->overflow = SUBOVERFLOW (arg1, arg2, w);
      R_WynikDP (cpu, w);
      break;

    case 7:
      // RSC
      w = arg2 - arg1 - ((cpu->Rx [ARM7_CPSR] & ARM7_CPSR_C) ? 0 : 1);
      cpu->carry = SUBCARRY (arg2, arg1, w);
      cpu->overflow = SUBOVERFLOW (arg2, arg1, w);
      R_WynikDP (cpu, w);
      break;

    case 8:
      // TST
      R_FlagiDP (cpu, arg1 & arg2);
      break;

    case 9:
      // TEQ
      R_FlagiDP (cpu, arg1 ^ arg2);
      break;

    case 10:
      // CMP
      w = arg1 - arg2;
      cpu->carry = SUBCARRY (arg1, arg2, w);
      cpu->overflow = SUBOVERFLOW (arg1, arg2, w);
      R_FlagiDP (cpu, w);
      break;

    case 11:
      // CMN
      w = arg1 + arg2;
      cpu->carry = ADDCARRY (arg1, arg2, w);
      cpu->overflow = ADDOVERFLOW (arg1, arg2, w);
      R_FlagiDP (cpu, w);
      break;

    case 12:
      // ORR
      R_WynikDP (cpu, arg1 | arg2);
      break;

    case 13:
      // MOV
      R_WynikDP (cpu, arg2);
      break;

    case 14:
      // BIC
      R_WynikDP (cpu, arg1 & ~arg2);
      break;

    case 15:
      // MVN
      R_WynikDP (cpu, ~arg2);
      break;
    }

#undef BIT_I
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Data processing result writeback. */
void R_WynikDP (struct sARM7 *cpu, ARM7_REG w)
  {
  int Rd;

#define BIT_S (cpu->kod & (1 << 20))

  Rd = (cpu->kod >> 12) & 15;
  cpu->Rx [Rd] = w;
  if (BIT_S)
    {
    if (Rd == ARM7_PC)
      {
      s_cykle += 4;
      // copy current SPSR to CPSR
      ARM7_SetCPSR (cpu, cpu->Rx [ARM7_SPSR]);
      }
    else
      // save new flags
      R_FlagiDP (cpu, w);
    }

#undef BIT_S
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Data processing flags writeback. */
void R_FlagiDP (struct sARM7 *cpu, ARM7_REG w)
  {
  // arithmetic or logical instruction?
  if (s_tabAL [(cpu->kod >> 21) & 15])
    {
    cpu->Rx [ARM7_CPSR] &= ~(ARM7_CPSR_N | ARM7_CPSR_Z | ARM7_CPSR_C |\
 ARM7_CPSR_V);
    cpu->Rx [ARM7_CPSR] |= cpu->overflow << 28;
    }
  else
    cpu->Rx [ARM7_CPSR] &= ~(ARM7_CPSR_N | ARM7_CPSR_Z | ARM7_CPSR_C);
  cpu->Rx [ARM7_CPSR] |= cpu->carry << 29;
  if (w == 0)
    cpu->Rx [ARM7_CPSR] |= ARM7_CPSR_Z;
  cpu->Rx [ARM7_CPSR] |= w & 0x80000000;
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Single data transfer. */
void R_SDT (struct sARM7 *cpu)
  {
  int Rn, Rd, offset;
  UINT32 adres, w = 0;

#define BIT_I (cpu->kod & (1 << 25))
#define BIT_P (cpu->kod & (1 << 24))
#define BIT_U (cpu->kod & (1 << 23))
#define BIT_B (cpu->kod & (1 << 22))
#define BIT_W (cpu->kod & (1 << 21))
#define BIT_L (cpu->kod & (1 << 20))

  if (BIT_I && (cpu->kod & (1 << 4)))
    {
    R_Und (cpu);
    return;
    }

  Rn = (cpu->kod >> 16) & 15,
  Rd = (cpu->kod >> 12) & 15;
  if (Rn != ARM7_PC)
    adres = cpu->Rx [Rn];
  else
    adres = cpu->Rx [ARM7_PC] & ~3;
  if (!BIT_L)
    if (Rd != ARM7_PC)
      w = cpu->Rx [Rd];
    else
      w = (cpu->Rx [ARM7_PC] & ~3) + 12 + PC_ADJUSTMENT;

  if (BIT_I)
    // calculate value in barrel shifter
    offset = WyliczPrzes (cpu);
  else
    // immediate in lowest 12 bits
    offset = cpu->kod & 0xfff;

  if (!BIT_U)
    offset = -offset;
  if (BIT_P)
    {
    // "pre-index"
    adres += offset;
    if (BIT_W)
      // "write-back"
      cpu->Rx [Rn] = adres;
    }
  else
    // "post-index"
    cpu->Rx [Rn] += offset;
  if (Rn == ARM7_PC)
    adres += 8 + PC_ADJUSTMENT;

  if (BIT_L)
    {
    s_cykle += 3;
    // "load"
    if (BIT_B)
      // "byte"
      cpu->Rx [Rd] = arm7_read_8 (cpu, adres);
    else
      // "word"
      cpu->Rx [Rd] = RBOD (arm7_read_32 (cpu, adres & ~3), adres & 3);
    }
  else
    {
    s_cykle += 2;
    // "store"
    if (BIT_B)
      // "byte"
      arm7_write_8 (cpu, adres, (UINT8)w);
    else
      // "word"
      arm7_write_32 (cpu, adres & ~3, w);
    }

#undef BIT_L
#undef BIT_W
#undef BIT_B
#undef BIT_U
#undef BIT_P
#undef BIT_I
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Undefined. */
void R_Und (struct sARM7 *cpu)
  {
  UINT32 sr = cpu->Rx [ARM7_CPSR];
  ARM7_SetCPSR (cpu, ARM7_CPSR_MX (sr, ARM7_CPSR_M_und) | ARM7_CPSR_I);
  cpu->Rx [ARM7_SPSR] = sr;
  cpu->Rx [ARM7_LR] = cpu->Rx [ARM7_PC] + 4;
  cpu->Rx [ARM7_PC] = 0x00000004;
  }
  //--------------------------------------------------------------------------

#define BIT_U (cpu->kod & (1 << 23))
#define BIT_S (cpu->kod & (1 << 22))
  //--------------------------------------------------------------------------
  /** Block Data Transfer. */
void R_BDT (struct sARM7 *cpu)
  {
  int Rn, usr = FALSE;
  UINT32 adres;
  ARM7_REG cpsr = 0;

#define BIT_L (cpu->kod & (1 << 20))

  // Rn can't be PC
  Rn = (cpu->kod >> 16) & 15;
  adres = cpu->Rx [Rn];

  // transfer in User mode
  if (BIT_S)
    if (!BIT_L || !(cpu->kod & (1 << ARM7_PC)))
      usr = TRUE;

  if (usr)
    {
//EMU_BLAD (BLAD_WEWNETRZNY, "BDT: user transfer");
    cpsr = cpu->Rx [ARM7_CPSR];
    ARM7_SetCPSR (cpu, ARM7_CPSR_MX (cpsr, ARM7_CPSR_M_usr));
    }

  if (BIT_L)
    // "load"
    R_LDM (cpu, Rn, adres);
  else
    // "store"
    R_STM (cpu, Rn, adres);

  if (usr)
    ARM7_SetCPSR (cpu, cpsr);

#undef BIT_L
  }
  //--------------------------------------------------------------------------

#define BIT_P (cpu->kod & (1 << 24))
#define BIT_W (cpu->kod & (1 << 21))
  //--------------------------------------------------------------------------
  /** Block load instructions. */
void R_LDM (struct sARM7 *cpu, int Rn, UINT32 adres)
  {
  int i, n, sp;

  // count registers on the list
  for (i = 0, n = 0; i < 16; i++)
    if (cpu->kod & (1 << i))
      n++;
  s_cykle += n * 2 + 1;

  n <<= 2;
  // transfer type
  sp = BIT_P;
  if (!BIT_U)
    {
    // "down"
    n = -n;
    adres += n;
    sp = !sp;
    }
  if (BIT_W)
    // "write-back"
    cpu->Rx [Rn] += n;

  // for all registers in mask
  if (sp)
    for (i = 0; i < 16; i++)
      {
      if (!(cpu->kod & (1 << i)))
        continue;
      adres += 4;
      cpu->Rx [i] = arm7_read_32 (cpu, adres);
      }
  else
    for (i = 0; i < 16; i++)
      {
      if (!(cpu->kod & (1 << i)))
        continue;
      cpu->Rx [i] = arm7_read_32 (cpu, adres);
      adres += 4;
      }

  // special case - mode change when PC is written
  if ((cpu->kod & (1 << ARM7_PC)) && BIT_S)
    ARM7_SetCPSR (cpu, cpu->Rx [ARM7_SPSR]);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Block store instructions. */
void R_STM (struct sARM7 *cpu, int Rn, UINT32 adres)
  {
  int i, n, p, sp;

  // count registers on the list and remember the first one
  for (i = 0, n = 0, p = -1; i < 16; i++)
    if (cpu->kod & (1 << i))
      {
      n++;
      if (p < 0)
        p = i;
      }
  s_cykle += n * 2;

  n <<= 2;
  // transfer type
  sp = BIT_P;
  if (!BIT_U)
    {
    // "down"
    n = -n;
    adres += n;
    sp = !sp;
    }
  // if base register is not the first one to transfer, writeback happens here
  if (BIT_W && Rn != p)
    // "write-back"
    cpu->Rx [Rn] += n;

  // registers R0-R14
  if (sp)
    for (i = 0; i < 15; i++)
      {
      if (!(cpu->kod & (1 << i)))
        continue;
      adres += 4;
      arm7_write_32 (cpu, adres, cpu->Rx [i]);
      }
  else
    for (i = 0; i < 15; i++)
      {
      if (!(cpu->kod & (1 << i)))
        continue;
      arm7_write_32 (cpu, adres, cpu->Rx [i]);
      adres += 4;
      }

  // PC is a special case
  if (cpu->kod & (1 << ARM7_PC))
  {
    if (sp)
      {
      adres += 4;
      arm7_write_32 (cpu, adres, (cpu->Rx [ARM7_PC] & ~3) + 12 + PC_ADJUSTMENT);
      }
    else
      {
      arm7_write_32 (cpu, adres, (cpu->Rx [ARM7_PC] & ~3) + 12 + PC_ADJUSTMENT);
      adres += 4;
      }
   }

  // if base register is the first one to transfer, writeback happens here
  if (BIT_W && Rn == p)
    // "write-back"
    cpu->Rx [Rn] += n;
  }
  //--------------------------------------------------------------------------
#undef BIT_W
#undef BIT_P
#undef BIT_S
#undef BIT_U

  //--------------------------------------------------------------------------
  /** Branch/Branch with link. */
void R_B_BL (struct sARM7 *cpu)
  {
  INT32 offset;

#define BIT_L (cpu->kod & (1 << 24))

  s_cykle += 4;
  offset = (cpu->kod & 0x00ffffff) << 2;
  if (offset & 0x02000000)
    offset |= 0xfc000000;
  offset += 8 + PC_ADJUSTMENT;
  if (BIT_L)
    // "Branch with link"
    cpu->Rx [ARM7_LR] = (cpu->Rx [ARM7_PC] & ~3) + 4 + PC_ADJUSTMENT;
  // "Branch"
  cpu->Rx [ARM7_PC] += offset;

#undef BIT_L
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Group 110 opcodes. */
void R_G110 (struct sARM7 *cpu)
  {
//	logerror("ARM7: G110 / Coprocessor data transfer\n");
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Group 111 opcodes. */
void R_G111 (struct sARM7 *cpu)
  {
  if ((cpu->kod & 0xf0000000) == 0xe0000000)
    {
/*    if (cpu->kod & (1 << 4))
	logerror("ARM7: G111 / Coprocessor register transfer\n");
    else
	logerror("ARM7: G111 / Coprocessor data operation\n"); */
    }
  else
    {
    UINT32 sr = cpu->Rx [ARM7_CPSR];
    ARM7_SetCPSR (cpu, ARM7_CPSR_MX (sr, ARM7_CPSR_M_svc) | ARM7_CPSR_I);
    cpu->Rx [ARM7_SPSR] = sr;
    cpu->Rx [ARM7_LR] = cpu->Rx [ARM7_PC];
    cpu->Rx [ARM7_PC] = 0x00000008;
    }
  }
  //--------------------------------------------------------------------------

#ifdef ARM7_THUMB
  //--------------------------------------------------------------------------
  /** Halfword and Signed Data Transfer. */
void R_HSDT ()
  {
  int Rm, Rd, Rn, offset;
  uint32_t adres, w;

#define BIT_P (cpu->kod & (1 << 24))
#define BIT_U (cpu->kod & (1 << 23))
#define BIT_W (cpu->kod & (1 << 21))
#define BIT_L (cpu->kod & (1 << 20))
#define BIT_S (cpu->kod & (1 << 6))
#define BIT_H (cpu->kod & (1 << 5))

  // Rm can't be PC
  Rn = (cpu->kod >> 16) & 15;
  Rd = (cpu->kod >> 12) & 15;
  if (Rn != ARM7_PC)
    adres = cpu->Rx [Rn];
  else
    adres = cpu->Rx [ARM7_PC] & ~3;
  if (!BIT_L)
    if (Rd != ARM7_PC)
      w = cpu->Rx [Rd];
    else
      w = (cpu->Rx [ARM7_PC] & ~3) + 12 + POPRAWKA_PC;

  if (1 << 22)
    // immediate
    offset = ((cpu->kod >> 4) & 0xf0) | (cpu->kod & 15);
  else
    {
    // register
    Rm = cpu->kod & 15;
    offset = cpu->Rx [Rm];
    }

  if (!BIT_U)
    offset = -offset;
  if (BIT_P)
    {
    // "pre-index"
    adres += offset;
    if (BIT_W)
      // "write-back"
      cpu->Rx [Rn] = adres;
    }
  else
    // "post-index"
    cpu->Rx [Rn] += offset;
  if (Rn == ARM7_PC)
    adres += 8 + POPRAWKA_PC;

  if (BIT_L)
    {
    // "load"
    s_cykle += 3;
    if (BIT_S)
      {
      if (BIT_H)
        // "signed halfword"
        cpu->Rx [Rd] = (INT32)(INT16)arm7_read_16 (adres);
      else
        // "signed byte"
        cpu->Rx [Rd] = (INT32)(INT8)arm7_read_8 (cpu, adres);
      }
    else
      // "unsigned halfword"
      cpu->Rx [Rd] = arm7_read_16 (adres);
    }
  else
    {
    // store
    s_cykle += 2;
    if (BIT_H)
      // "halfword"
      arm7_write_16 (adres, (UINT16)w);
    else
      // "byte"
      arm7_write_8 (cpu, adres, (UINT8)w);
    }

#undef BIT_H
#undef BIT_S
#undef BIT_L
#undef BIT_W
#undef BIT_U
#undef BIT_P
  }
  //--------------------------------------------------------------------------
#endif
