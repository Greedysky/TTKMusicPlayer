//
// ARM7 processor emulator
// version 1.6 / 2008-02-16
// (c) Radoslaw Balcewicz
//

#ifndef _ARM7_h_
#define _ARM7_h_

#include "cpuintrf.h"
#include "aica.h"

  //--------------------------------------------------------------------------
  // definitions and macros

  /** If defined, will turn on specific behavior emulation, as well as some
 optimizations that are valid only for Dreamcast AICA. */
#define ARM7_DREAMCAST

  /** Define to enable Thumb support for ARM7. */
//#define ARM7_THUMB

  // sanity tests
#ifdef ARM7_DREAMCAST
 #ifdef ARM7_THUMB
  #warning "Dreamcast ARM7 is a -DI type, it doesn't support Thumb mode."
 #endif
#else
// #warning "Instructions cycle counts might not be correct."
#endif

  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // CPU definitions

  /** Status flags in CPSR register. */
#define ARM7_CPSR_N (1 << 31)
#define ARM7_CPSR_Z (1 << 30)
#define ARM7_CPSR_C (1 << 29)
#define ARM7_CPSR_V (1 << 28)
#define ARM7_CPSR_I (1 << 7)
#define ARM7_CPSR_F (1 << 6)
#define ARM7_CPSR_T (1 << 5)
  /** CPSR bit mask for current operating mode. */
#define ARM7_CPSR_M(x) ((x) & 0x1f)
#define ARM7_CPSR_MX(sr,x) (((sr) & ~0x1f) | ((x) & 0x1f))
  /** Bit combinations for each operating mode. */
#define ARM7_CPSR_M_usr 0x10
#define ARM7_CPSR_M_fiq 0x11
#define ARM7_CPSR_M_irq 0x12
#define ARM7_CPSR_M_svc 0x13
#define ARM7_CPSR_M_abt 0x17
#define ARM7_CPSR_M_und 0x11
#define ARM7_CPSR_M_sys 0x1f

  /** Control flags for ARM7 core. */
#define ARM7_FL_FIQ (1 << 0)
#define ARM7_FL_IRQ (1 << 1)

  /** Operating modes. */
#define ARM7_MODE_usr 0
#define ARM7_MODE_fiq 1
#define ARM7_MODE_irq 2
#define ARM7_MODE_svc 3
#define ARM7_MODE_abt 4
#define ARM7_MODE_und 5
#define ARM7_MODE_sys 0
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // register definitions

  /** ARM7 register type (all are 32-bit). */
typedef INT32 ARM7_REG;

enum
{
    ARM7_R0 = 0, ARM7_R1, ARM7_R2, ARM7_R3, ARM7_R4, ARM7_R5, ARM7_R6, ARM7_R7,
    ARM7_R8, ARM7_R9, ARM7_R10, ARM7_R11, ARM7_R12, ARM7_R13, ARM7_R14, ARM7_R15
};

  /** R13 is stack pointer. */
#define ARM7_SP 13
  /** R14 is link/return address. */
#define ARM7_LR 14
  /** R15 is program counter. */
#define ARM7_PC 15
  /** CPSR control register. */
#define ARM7_CPSR 16
  /** SPSR control register. */
#define ARM7_SPSR 17
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** ARM7 CPU state structure. */
struct sARM7
  {
  /** All-purpose and control registers (for current mode). */
  ARM7_REG Rx [18];
  /** Banked registers for all operating modes. */
  ARM7_REG Rx_bank [6][10];

  /** FIQ and IRQ interrupt requests. */
  int fiq, irq;

  /** Carry flag for barrel shifter and ALU operations. */
  int carry;
  /** Overflow flag for arithmetic instructions. */
  int overflow;

  /** Emulation control flags. */
  int flagi;

  /** Instruction code. */
  UINT32 kod;
  /** Cycle counter. */
  int cykle;

  uint8 dc_ram[8*1024*1024];

  struct AICAinterface aica_interface;
  struct _AICA *AICA;
  };
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // public procedures

  /** ARM7 allocate spu state. */
struct sARM7* ARM7_Alloc (void);

void ARM7_Free (struct sARM7 *cpu);

  /** ARM7 emulator init. */
void ARM7_Init (struct sARM7 *cpu);

  /** Power-ON reset. */
void ARM7_HardReset (struct sARM7 *cpu);
  /** Hardware reset via /RESET line. */
void ARM7_SoftReset (struct sARM7 *cpu);

  /** CPSR update, possibly changing operating mode. */
void ARM7_SetCPSR (struct sARM7 *cpu, ARM7_REG sr);

  /** Sets FIQ line state. */
void ARM7_SetFIQ (struct sARM7 *cpu, int stan);
  /** Sets IRQ line state. */
void ARM7_SetIRQ (struct sARM7 *cpu, int stan);

  /** Tests for pending interrupts, switches to one if possible. */
void ARM7_CheckIRQ (struct sARM7 *cpu);

  /** Single step. */
void ARM7_Step (struct sARM7 *cpu);
  /** Runs emulation for at least n cycles, returns actual amount of cycles
 burned - normal interpreter. */
int ARM7_Execute (struct sARM7 *cpu, int n);
  //--------------------------------------------------------------------------

enum
{
    ARM7_IRQ_LINE=0, ARM7_FIRQ_LINE,
    ARM7_NUM_LINES
};

#ifdef ENABLE_DEBUGGER
extern UINT32 arm7_disasm( struct sARM7 *cpu, char *pBuf, UINT32 pc, UINT32 opcode );
extern UINT32 thumb_disasm( struct sARM7 *cpu, char *pBuf, UINT32 pc, UINT16 opcode );
#endif
#endif
