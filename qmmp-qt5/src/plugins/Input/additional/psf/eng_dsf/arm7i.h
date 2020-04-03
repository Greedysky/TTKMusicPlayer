//
// ARM7 processor emulator - interpreter core
// version 1.6 / 2008-02-16
// (c) Radoslaw Balcewicz
//

#ifndef _ARM7i_h_
#define _ARM7i_h_

#include "arm7.h"

  //--------------------------------------------------------------------------
  // public functions

  /** Single step, returns number of burned cycles. */
int ARM7i_Step(struct sARM7 *cpu);
  //--------------------------------------------------------------------------

#endif
