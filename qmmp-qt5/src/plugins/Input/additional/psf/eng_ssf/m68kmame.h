#ifndef M68KMAME__HEADER
#define M68KMAME__HEADER

/* ======================================================================== */
/* ============================== MAME STUFF ============================== */
/* ======================================================================== */

#include "cpuintrf.h"
#include "memory.h"
#include "mamedbg.h"
#include "m68000.h"

extern int m68ki_remaining_cycles;

/* Configuration switches (see m68kconf.h for explanation) */
#define M68K_SEPARATE_READS         OPT_ON

#define M68K_SIMULATE_PD_WRITES     OPT_ON

#define M68K_EMULATE_INT_ACK        OPT_ON
#define M68K_INT_ACK_CALLBACK(A)

#define M68K_EMULATE_BKPT_ACK       OPT_OFF
#define M68K_BKPT_ACK_CALLBACK()

#define M68K_EMULATE_TRACE          OPT_OFF

#define M68K_EMULATE_RESET          OPT_ON
#define M68K_RESET_CALLBACK()

#define M68K_EMULATE_FC             OPT_OFF
#define M68K_SET_FC_CALLBACK(A)

#define M68K_MONITOR_PC             OPT_SPECIFY_HANDLER
#define M68K_SET_PC_CALLBACK(A)     (*m68k_memory_intf.changepc)(A)

#define M68K_INSTRUCTION_HOOK       OPT_SPECIFY_HANDLER
#define M68K_INSTRUCTION_CALLBACK() CALL_MAME_DEBUG

#define M68K_EMULATE_PREFETCH       OPT_ON

#define M68K_LOG_ENABLE             OPT_OFF
#define M68K_LOG_1010_1111          OPT_OFF
#define M68K_LOG_FILEHANDLE         errorlog

#define M68K_EMULATE_ADDRESS_ERROR  OPT_OFF

#define M68K_USE_64_BIT             OPT_OFF

extern struct m68k_memory_interface m68k_memory_intf;

#define m68k_read_memory_8(address)          (*m68k_memory_intf.read8)(address)
#define m68k_read_memory_16(address)         (*m68k_memory_intf.read16)(address)
#define m68k_read_memory_32(address)         (*m68k_memory_intf.read32)(address)

INLINE unsigned int m68k_read_immediate_16(unsigned int address)
{
	return cpu_readop16((address) ^ m68k_memory_intf.opcode_xor);
}

INLINE unsigned int m68k_read_immediate_32(unsigned int address)
{
	return ((m68k_read_immediate_16(address) << 16) | m68k_read_immediate_16((address)+2));
}

INLINE unsigned int m68k_read_pcrelative_8(unsigned int address)
{
	if (address >= encrypted_opcode_start[cpu_getactivecpu()] &&
			address < encrypted_opcode_end[cpu_getactivecpu()])
		return ((m68k_read_immediate_16(address&~1)>>(8*(1-(address & 1))))&0xff);
	else
		return m68k_read_memory_8(address);
}

INLINE unsigned int m68k_read_pcrelative_16(unsigned int address)
{
	if (address >= encrypted_opcode_start[cpu_getactivecpu()] &&
			address < encrypted_opcode_end[cpu_getactivecpu()])
		return m68k_read_immediate_16(address);
	else
		return m68k_read_memory_16(address);
}

INLINE unsigned int m68k_read_pcrelative_32(unsigned int address)
{
	if (address >= encrypted_opcode_start[cpu_getactivecpu()] &&
			address < encrypted_opcode_end[cpu_getactivecpu()])
		return m68k_read_immediate_32(address);
	else
		return m68k_read_memory_32(address);
}

#define m68k_read_disassembler_16(address)   m68k_read_immediate_16(address)
#define m68k_read_disassembler_32(address)   m68k_read_immediate_32(address)


#define m68k_write_memory_8(address, value)  (*m68k_memory_intf.write8)(address, value)
#define m68k_write_memory_16(address, value) (*m68k_memory_intf.write16)(address, value)
#define m68k_write_memory_32(address, value) (*m68k_memory_intf.write32)(address, value)

/* Special call to simulate undocumented 68k behavior when move.l with a
 * predecrement destination mode is executed.
 * A real 68k first writes the high word to [address+2], and then writes the
 * low word to [address].
 */
INLINE void m68k_write_memory_32_pd(unsigned int address, unsigned int value)
{
	(*m68k_memory_intf.write16)(address+2, value>>16);
	(*m68k_memory_intf.write16)(address, value&0xffff);
}


#ifdef A68K0
#define M68K_EMULATE_010            OPT_OFF
#else

// The PPC 68k core defines A68K0 internal to itself to avoid recompiling
// all of MAME when you want to disable it. The downside is that the C and x86
// cores can't agree on the same name for the icount variable, so we force the
// issue with a Mac-specific hack.
#ifdef macintosh
extern int m68k_ICount;
#else
#define m68ki_remaining_cycles m68k_ICount
#endif

/* M68K Variants */
#if HAS_M68010
#define M68K_EMULATE_010            OPT_ON
#else
#define M68K_EMULATE_010            OPT_OFF
#endif

#endif	// A68K0

#ifdef A68K2
#define M68K_EMULATE_EC020          OPT_OFF
#define M68K_EMULATE_020            OPT_OFF
#else

// The PPC 68k core defines A68K2 internal to itself to avoid recompiling
// all of MAME when you want to disable it. The downside is that the C and x86
// cores can't agree on the same name for the icount variable, so we force the
// issue with a Mac-specific hack.
#ifdef macintosh
extern int M68020_ICount;
#else
#define m68ki_remaining_cycles m68k_ICount
#endif

#undef  M68K_EMULATE_010
#define M68K_EMULATE_010            OPT_ON

#if HAS_M68EC020
#define M68K_EMULATE_EC020          OPT_ON
#else
#define M68K_EMULATE_EC020          OPT_OFF
#endif

#if HAS_M68020
#define M68K_EMULATE_020            OPT_ON
#else
#define M68K_EMULATE_020            OPT_OFF
#endif

#endif // A68K2

/* ======================================================================== */
/* ============================== END OF FILE ============================= */
/* ======================================================================== */

#endif /* M68KMAME__HEADER */
