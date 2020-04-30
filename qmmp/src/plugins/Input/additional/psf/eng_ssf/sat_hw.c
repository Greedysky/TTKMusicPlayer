/*
    sat_hw.c - Saturn sound hardware glue/emulation/whatever

    supported: main RAM (512 KB)
    	       SCSP + timers
	       MC68000 CPU

    Copyright (c) 2007, R. Belmont and Richard Bannister.
    
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the names of R. Belmont and Richard Bannister nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>

#include "ao.h"
#include "scsp.h"
#include "sat_hw.h"
#include "m68k.h"
#include "m68kcpu.h"

static void scsp_irq(m68ki_cpu_core *cpu, int irq)
{
	if (irq > 0)
	{
		m68k_set_irq(cpu, irq);
	}
}

#define MIXER_PAN_LEFT    1
#define MIXER_PAN_RIGHT   2
#define MIXER(level,pan) ((level & 0xff) | ((pan & 0x03) << 8))
#define YM3012_VOL(LVol,LPan,RVol,RPan) (MIXER(LVol,LPan)|(MIXER(RVol,RPan) << 16))

#if 0
static struct SCSPinterface scsp_interface =
{
	1,
	{ sat_ram, },
	{ YM3012_VOL(100, MIXER_PAN_LEFT, 100, MIXER_PAN_RIGHT) },
	{ scsp_irq, },
};
#endif

void sat_hw_init(m68ki_cpu_core *cpu)
{
//	m68k_init();
	m68k_set_cpu_type(cpu, M68K_CPU_TYPE_68000);
	m68k_pulse_reset(cpu);
    struct SCSPinterface scsp_interface;
	scsp_interface.num = 1;
	scsp_interface.region[0] = cpu->sat_ram;
	scsp_interface.mixing_level[0] = YM3012_VOL(100, MIXER_PAN_LEFT, 100, MIXER_PAN_RIGHT);
	scsp_interface.irq_callback[0] = scsp_irq;
	scsp_interface.cpu = cpu;

	cpu->SCSP = SCSP_Start(&scsp_interface);
}

void sat_hw_free(struct m68ki_cpu_core_s *cpu) {
    SCSP_Exit (cpu->SCSP);
}

/* M68k memory handlers */

unsigned int m68k_read_memory_8(m68ki_cpu_core *cpu, unsigned int address)
{
	if (address < (512*1024))
		return cpu->sat_ram[address^1];

	if (address >= 0x100000 && address < 0x100c00)
	{
		int foo = SCSP_0_r(cpu->SCSP, (address - 0x100000)/2, 0);

		if (address & 1)
			return foo & 0xff;
		else
			return foo>>8;
	}

	printf("R8 @ %x\n", address);
	return 0;
}

unsigned int m68k_read_memory_16(m68ki_cpu_core *cpu, unsigned int address)
{
	if (address < (512*1024))
	{
		return mem_readword_swap((unsigned short *)(cpu->sat_ram+address));
	}

	if (address >= 0x100000 && address < 0x100c00)
		return SCSP_0_r(cpu->SCSP, (address-0x100000)/2, 0);

	printf("R16 @ %x\n", address);
	return 0;
}

unsigned int m68k_read_memory_32(m68ki_cpu_core *cpu, unsigned int address)
{
	if (address < 0x80000)
	{
		return cpu->sat_ram[address+2] | cpu->sat_ram[address+3]<<8 | cpu->sat_ram[address]<<16 | cpu->sat_ram[address+1]<<24;
	}

	printf("R32 @ %x\n", address);
	return 0;
}

void m68k_write_memory_8(m68ki_cpu_core *cpu, unsigned int address, unsigned int data)
{
	if (address < 0x80000)
	{
		cpu->sat_ram[address^1] = data;
		return;
	}

	if (address >= 0x100000 && address < 0x100c00)
	{
		address -= 0x100000;
		if (address & 1)
			SCSP_0_w(cpu->SCSP, address>>1, data, 0xff00);
		else
			SCSP_0_w(cpu->SCSP, address>>1, data<<8, 0x00ff);
		return;
	}
}

void m68k_write_memory_16(m68ki_cpu_core *cpu, unsigned int address, unsigned int data)
{
	if (address < 0x80000)
	{
		cpu->sat_ram[address+1] = (data>>8)&0xff;
		cpu->sat_ram[address] = data&0xff;
		return;
	}

	if (address >= 0x100000 && address < 0x100c00)
	{
		SCSP_0_w(cpu->SCSP, (address-0x100000)>>1, data, 0x0000);
		return;
	}
}

void m68k_write_memory_32(m68ki_cpu_core *cpu, unsigned int address, unsigned int data)
{
	if (address < 0x80000)
	{
		cpu->sat_ram[address+1] = (data>>24)&0xff;
		cpu->sat_ram[address] = (data>>16)&0xff;
		cpu->sat_ram[address+3] = (data>>8)&0xff;
		cpu->sat_ram[address+2] = data&0xff;
		return;
	}

	if (address >= 0x100000 && address < 0x100c00)
	{
		address -= 0x100000;
		SCSP_0_w(cpu->SCSP, address>>1, data>>16, 0x0000);
		SCSP_0_w(cpu->SCSP, (address>>1)+1, data&0xffff, 0x0000);
		return;
	}
}
