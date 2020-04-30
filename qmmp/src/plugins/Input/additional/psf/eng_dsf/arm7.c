//
// ARM7 processor emulator
// version 1.6 / 2008-02-16
// (c) Radoslaw Balcewicz
//

#include "arm7.h"
#include "arm7i.h"

#ifdef ARM7_THUMB
#include "arm7thumb.h"
#endif

  //--------------------------------------------------------------------------
  // definitions and macros

  /** Macro for accessing banked registers. */
#define RX_BANK(t,r) (cpu->Rx_bank [t][r - 8])
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // private functions

  /** CPU Reset. */
static void Reset (struct sARM7 *cpu);
  //--------------------------------------------------------------------------

  // private variables

  /** Table for decoding bit-coded mode to zero based index. */
static const int s_tabTryb [32] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, ARM7_MODE_usr, ARM7_MODE_fiq, ARM7_MODE_irq,
 ARM7_MODE_svc, -1, -1, -1, ARM7_MODE_abt, -1, -1, -1, ARM7_MODE_und,
 -1, -1, -1, ARM7_MODE_sys};
  //--------------------------------------------------------------------------


  // public functions

struct sARM7* ARM7_Alloc ()
{
    struct sARM7 *cpu = malloc (sizeof (struct sARM7));
    memset (cpu, 0, sizeof (struct sARM7));
    return cpu;
}

void ARM7_Free (struct sARM7 *cpu) {
    free (cpu);
}

  //--------------------------------------------------------------------------
  /** ARM7 emulator init. */
void ARM7_Init (struct sARM7 *cpu)
  {
  // sane startup values
  cpu->fiq = 0;
  cpu->irq = 0;
  cpu->carry = 0;
  cpu->overflow = 0;
  cpu->flagi = FALSE;
  cpu->cykle = 0;

  // reset will do the rest
  ARM7_HardReset (cpu);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Power-ON reset. */
void ARM7_HardReset (struct sARM7 *cpu)
  {
  // CPSR that makes sense
  cpu->Rx [ARM7_CPSR] = ARM7_CPSR_I | ARM7_CPSR_F | ARM7_CPSR_M_svc;
  Reset (cpu);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Hardware reset via /RESET line. */
void ARM7_SoftReset (struct sARM7 *cpu)
  {
  Reset (cpu);
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** CPSR update, possibly changing operating mode. */
void ARM7_SetCPSR (struct sARM7 *cpu, ARM7_REG sr)
  {
  int stary, nowy;

  stary = s_tabTryb [ARM7_CPSR_M (cpu->Rx [ARM7_CPSR])];
  nowy = s_tabTryb [ARM7_CPSR_M (sr)];
  // do we have to change modes?
  if (nowy != stary)
    {
    // save this mode registers
    RX_BANK (stary, ARM7_SP) = cpu->Rx [ARM7_SP],
    RX_BANK (stary, ARM7_LR) = cpu->Rx [ARM7_LR],
    RX_BANK (stary, ARM7_SPSR) = cpu->Rx [ARM7_SPSR];
    if (stary == ARM7_MODE_fiq)
      {
      // copy R8-R12
      RX_BANK (ARM7_MODE_fiq, 8) = cpu->Rx [8],
      RX_BANK (ARM7_MODE_fiq, 9) = cpu->Rx [9],
      RX_BANK (ARM7_MODE_fiq, 10) = cpu->Rx [10],
      RX_BANK (ARM7_MODE_fiq, 11) = cpu->Rx [11],
      RX_BANK (ARM7_MODE_fiq, 12) = cpu->Rx [12];
      cpu->Rx [8] = RX_BANK (ARM7_MODE_usr, 8),
      cpu->Rx [9] = RX_BANK (ARM7_MODE_usr, 9),
      cpu->Rx [10] = RX_BANK (ARM7_MODE_usr, 10),
      cpu->Rx [11] = RX_BANK (ARM7_MODE_usr, 11),
      cpu->Rx [12] = RX_BANK (ARM7_MODE_usr, 12);
      }

    // fetch new mode registers
    cpu->Rx [ARM7_SP] = RX_BANK (nowy, ARM7_SP),
    cpu->Rx [ARM7_LR] = RX_BANK (nowy, ARM7_LR),
    cpu->Rx [ARM7_SPSR] = RX_BANK (nowy, ARM7_SPSR);
    if (nowy == ARM7_MODE_fiq)
      {
      // copy R8-R12
      RX_BANK (ARM7_MODE_usr, 8) = cpu->Rx [8],
      RX_BANK (ARM7_MODE_usr, 9) = cpu->Rx [9],
      RX_BANK (ARM7_MODE_usr, 10) = cpu->Rx [10],
      RX_BANK (ARM7_MODE_usr, 11) = cpu->Rx [11],
      RX_BANK (ARM7_MODE_usr, 12) = cpu->Rx [12];
      cpu->Rx [8] = RX_BANK (ARM7_MODE_fiq, 8),
      cpu->Rx [9] = RX_BANK (ARM7_MODE_fiq, 9),
      cpu->Rx [10] = RX_BANK (ARM7_MODE_fiq, 10),
      cpu->Rx [11] = RX_BANK (ARM7_MODE_fiq, 11),
      cpu->Rx [12] = RX_BANK (ARM7_MODE_fiq, 12);
      }
    }

  // new CPSR value
  cpu->Rx [ARM7_CPSR] = sr;

  // mode change could've enabled interrups, so we test for those and set
  // appropriate flag for the instruction loop to catch
  if (cpu->fiq)
    cpu->flagi |= ARM7_FL_FIQ;
#ifndef ARM7_DREAMCAST
  if (cpu->irq)
    cpu->flagi |= ARM7_FL_IRQ;
#endif
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Sets FIQ line state. */
void ARM7_SetFIQ (struct sARM7 *cpu, int stan)
  {
  stan = stan ? TRUE : FALSE;
  // we catch changes only
  if (stan ^ cpu->fiq)
    {
    cpu->fiq = stan;
    if (cpu->fiq)
      cpu->flagi |= ARM7_FL_FIQ;
    }
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Sets IRQ line state. */
void ARM7_SetIRQ (struct sARM7 *cpu, int stan)
  {
  stan = stan ? TRUE : FALSE;
  // we catch changes only
  if (stan ^ cpu->irq)
    {
    cpu->irq = stan;
    if (cpu->irq)
      cpu->flagi |= ARM7_FL_IRQ;
    }
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Tests for pending interrupts, switches to one if possible. */
void ARM7_CheckIRQ (struct sARM7 *cpu)
  {
  UINT32 sr = cpu->Rx [ARM7_CPSR];

  // clear all interrupt flags
  cpu->flagi &= ~(ARM7_FL_FIQ | ARM7_FL_IRQ);
  
  // check for pending interrupts we can switch to
  // (FIQ can interrupt IRQ, but not the other way around)
  if (cpu->fiq)
    {
    if (!(sr & ARM7_CPSR_F))
      {
      // FIQ
      ARM7_SetCPSR (cpu, ARM7_CPSR_MX (sr, ARM7_CPSR_M_fiq) | ARM7_CPSR_F | ARM7_CPSR_I);
      cpu->Rx [ARM7_SPSR] = sr;
      // set new PC (return from interrupt will subtract 4)
      cpu->Rx [ARM7_LR] = cpu->Rx [ARM7_PC] + 4;
      cpu->Rx [ARM7_PC] = 0x0000001c;
      }
    }
#ifndef ARM7_DREAMCAST
  if (cpu->irq)
    {
    if (!(sr & ARM7_CPSR_I))
      {
      // IRQ
      ARM7_SetCPSR (cpu, ARM7_CPSR_MX (sr, ARM7_CPSR_M_irq) | ARM7_CPSR_I);
      cpu->Rx [ARM7_SPSR] = sr;
      // set new PC (return from interrupt will subtract 4)
      cpu->Rx [ARM7_LR] = cpu->Rx [ARM7_PC] + 4;
      cpu->Rx [ARM7_PC] = 0x00000018;
      cpu->irq = 0;
      }
    }
#endif
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Single step. */
void ARM7_Step (struct sARM7 *cpu)
{
  // make a step
#ifdef ARM7_THUMB
  if (cpu->Rx[ARM7_CPSR] & ARM7_CPSR_T)
  {
	ARM7i_Thumb_Step(cpu);
  }
  else
#endif
  {
        ARM7i_Step (cpu);
  }
  // and test interrupts
  ARM7_CheckIRQ (cpu);
}
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  /** Runs emulation for at least n cycles, returns actual amount of cycles
 burned - normal interpreter. */
int ARM7_Execute (struct sARM7 *cpu, int n)
  {
  cpu->cykle = 0;
  while (cpu->cykle < n)
    {
    ARM7_CheckIRQ (cpu);
    while (!cpu->flagi && cpu->cykle < n)
      // make one step, sum up cycles
      cpu->cykle += ARM7i_Step (cpu);
    }
  return cpu->cykle;
  }
  //--------------------------------------------------------------------------


  // private functions


  //--------------------------------------------------------------------------
  /** CPU Reset. */
void Reset (struct sARM7 *cpu)
  {
  // clear ALU flags
  cpu->carry = 0;
  cpu->overflow = 0;
  // test CPSR mode and pick a valid one if necessary
  if (s_tabTryb [ARM7_CPSR_M (cpu->Rx [ARM7_CPSR])] < 0)
    cpu->Rx [ARM7_CPSR] = ARM7_CPSR_I | ARM7_CPSR_F | ARM7_CPSR_M_svc;
  // set up registers according to manual
  RX_BANK (ARM7_MODE_svc, ARM7_LR) = cpu->Rx [ARM7_PC];
  RX_BANK (ARM7_MODE_svc, ARM7_SPSR) = cpu->Rx [ARM7_CPSR];
  ARM7_SetCPSR (cpu, ARM7_CPSR_I | ARM7_CPSR_F | ARM7_CPSR_M_svc);
  cpu->Rx [ARM7_PC] = 0x00000000;
  }
  //--------------------------------------------------------------------------
