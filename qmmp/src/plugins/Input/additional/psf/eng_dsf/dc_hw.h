#ifndef _DC_HW_H_
#define _DC_HW_H_

#include "arm7i.h"

void dc_hw_init(struct sARM7 *cpu);
void dc_hw_free (struct sARM7 *cpu);
void dc_write8(struct sARM7 *cpu, int addr, uint8 data);
void dc_write16(struct sARM7 *cpu, int addr, uint16 data);
void dc_write32(struct sARM7 *cpu, int addr, uint32 data);
uint8 dc_read8(struct sARM7 *cpu, int addr);
uint16 dc_read16(struct sARM7 *cpu, int addr);
uint32 dc_read32(struct sARM7 *cpu, int addr);

#endif

