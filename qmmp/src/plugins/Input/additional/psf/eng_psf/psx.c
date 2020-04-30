/*
 * Sony CXD8530AQ/CXD8530BQ/CXD8530CQ/CXD8661R
 *
 * PSX CPU emulator for the MAME project written by smf
 * Thanks to Farfetch'd for information on the delay slot bug
 *
 * The PSX CPU is a custom r3000a with a built in
 * geometry transform engine, no mmu & no data cache.
 *
 * There is a stall circuit for load delays, but
 * it doesn't work if the load occurs in a branch
 * delay slot.
 *
 */

#include <stdio.h>
#include "ao.h"
#include "cpuintrf.h"
#include "psx.h"

#define EXC_INT ( 0 )
#define EXC_ADEL ( 4 )
#define EXC_ADES ( 5 )
#define EXC_SYS ( 8 )
#define EXC_BP ( 9 )
#define EXC_RI ( 10 )
#define EXC_CPU ( 11 )
#define EXC_OVF ( 12 )

#define CP0_RANDOM ( 1 )
#define CP0_BADVADDR ( 8 )
#define CP0_SR ( 12 )
#define CP0_CAUSE ( 13 )
#define CP0_EPC ( 14 )
#define CP0_PRID ( 15 )

#define SR_IEC ( 1L << 0 )
#define SR_KUC ( 1L << 1 )
#define SR_ISC ( 1L << 16 )
#define SR_SWC ( 1L << 17 )
#define SR_TS  ( 1L << 21 )
#define SR_BEV ( 1L << 22 )
#define SR_RE ( 1L << 25 )
#define SR_CU0 ( 1L << 28 )
#define SR_CU1 ( 1L << 29 )
#define SR_CU2 ( 1L << 30 )
#define SR_CU3 ( 1L << 31 )

#define CAUSE_EXC ( 31L << 2 )
#define CAUSE_IP ( 255L << 8 )
#define CAUSE_IP2 ( 1L << 10 )
#define CAUSE_IP3 ( 1L << 11 )
#define CAUSE_IP4 ( 1L << 12 )
#define CAUSE_IP5 ( 1L << 13 )
#define CAUSE_IP6 ( 1L << 14 )
#define CAUSE_IP7 ( 1L << 15 )
#define CAUSE_CE ( 3L << 28 )
#define CAUSE_CE0 ( 0L << 28 )
#define CAUSE_CE1 ( 1L << 28 )
#define CAUSE_CE2 ( 2L << 28 )
#define CAUSE_BD ( 1L << 31 )

static UINT8 mips_reg_layout[] =
{
	MIPS_PC, -1,
	MIPS_DELAYV, MIPS_DELAYR, -1,
	MIPS_HI, MIPS_LO, -1,
	-1,
	MIPS_R0, MIPS_R1, -1,
	MIPS_R2, MIPS_R3, -1,
	MIPS_R4, MIPS_R5, -1,
	MIPS_R6, MIPS_R7, -1,
	MIPS_R8, MIPS_R9, -1,
	MIPS_R10, MIPS_R11, -1,
	MIPS_R12, MIPS_R13, -1,
	MIPS_R14, MIPS_R15, -1,
	MIPS_R16, MIPS_R17, -1,
	MIPS_R18, MIPS_R19, -1,
	MIPS_R20, MIPS_R21, -1,
	MIPS_R22, MIPS_R23, -1,
	MIPS_R24, MIPS_R25, -1,
	MIPS_R26, MIPS_R27, -1,
	MIPS_R28, MIPS_R29, -1,
	MIPS_R30, MIPS_R31, -1,
	-1,
	MIPS_CP0R0, MIPS_CP0R1, -1,
	MIPS_CP0R2, MIPS_CP0R3, -1,
	MIPS_CP0R4, MIPS_CP0R5, -1,
	MIPS_CP0R6, MIPS_CP0R7, -1,
	MIPS_CP0R8, MIPS_CP0R9, -1,
	MIPS_CP0R10, MIPS_CP0R11, -1,
	MIPS_CP0R12, MIPS_CP0R13, -1,
	MIPS_CP0R14, MIPS_CP0R15, -1,
	MIPS_CP0R16, MIPS_CP0R17, -1,
	MIPS_CP0R18, MIPS_CP0R19, -1,
	MIPS_CP0R20, MIPS_CP0R21, -1,
	MIPS_CP0R22, MIPS_CP0R23, -1,
	MIPS_CP0R24, MIPS_CP0R25, -1,
	MIPS_CP0R26, MIPS_CP0R27, -1,
	MIPS_CP0R28, MIPS_CP0R29, -1,
	MIPS_CP0R30, MIPS_CP0R31, -1,
	-1,
	MIPS_CP2DR0, MIPS_CP2DR1, -1,
	MIPS_CP2DR2, MIPS_CP2DR3, -1,
	MIPS_CP2DR4, MIPS_CP2DR5, -1,
	MIPS_CP2DR6, MIPS_CP2DR7, -1,
	MIPS_CP2DR8, MIPS_CP2DR9, -1,
	MIPS_CP2DR10, MIPS_CP2DR11, -1,
	MIPS_CP2DR12, MIPS_CP2DR13, -1,
	MIPS_CP2DR14, MIPS_CP2DR15, -1,
	MIPS_CP2DR16, MIPS_CP2DR17, -1,
	MIPS_CP2DR18, MIPS_CP2DR19, -1,
	MIPS_CP2DR20, MIPS_CP2DR21, -1,
	MIPS_CP2DR22, MIPS_CP2DR23, -1,
	MIPS_CP2DR24, MIPS_CP2DR25, -1,
	MIPS_CP2DR26, MIPS_CP2DR27, -1,
	MIPS_CP2DR28, MIPS_CP2DR29, -1,
	MIPS_CP2DR30, MIPS_CP2DR31, -1,
	-1,
	MIPS_CP2CR0, MIPS_CP2CR1, -1,
	MIPS_CP2CR2, MIPS_CP2CR3, -1,
	MIPS_CP2CR4, MIPS_CP2CR5, -1,
	MIPS_CP2CR6, MIPS_CP2CR7, -1,
	MIPS_CP2CR8, MIPS_CP2CR9, -1,
	MIPS_CP2CR10, MIPS_CP2CR11, -1,
	MIPS_CP2CR12, MIPS_CP2CR13, -1,
	MIPS_CP2CR14, MIPS_CP2CR15, -1,
	MIPS_CP2CR16, MIPS_CP2CR17, -1,
	MIPS_CP2CR18, MIPS_CP2CR19, -1,
	MIPS_CP2CR20, MIPS_CP2CR21, -1,
	MIPS_CP2CR22, MIPS_CP2CR23, -1,
	MIPS_CP2CR24, MIPS_CP2CR25, -1,
	MIPS_CP2CR26, MIPS_CP2CR27, -1,
	MIPS_CP2CR28, MIPS_CP2CR29, -1,
	MIPS_CP2CR30, MIPS_CP2CR31,
	0
};

static UINT8 mips_win_layout[] = {
	45, 0,35,13,	/* register window (top right) */
	 0, 0,44,13,	/* disassembler window (left, upper) */
	 0,14,44, 8,	/* memory #1 window (left, middle) */
	45,14,35, 8,	/* memory #2 window (lower) */
	 0,23,80, 1 	/* command line window (bottom rows) */
};

static const char *delayn[] =
{
	"pc", "at", "v0", "v1", "a0", "a1", "a2", "a3",
	"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
	"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
	"t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
	"pc"
};

#define REGPC ( 32 )

//static mips_cpu_context mipscpu;

static UINT32 mips_mtc0_writemask[]=
{
	0xffffffff, /* INDEX */
	0x00000000, /* RANDOM */
	0xffffff00, /* ENTRYLO */
	0x00000000,
	0xffe00000, /* CONTEXT */
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000, /* BADVADDR */
	0x00000000,
	0xffffffc0, /* ENTRYHI */
	0x00000000,
	0xf27fff3f, /* SR */
	0x00000300, /* CAUSE */
	0x00000000, /* EPC */
	0x00000000, /* PRID */
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000
};

#if 0
void GTELOG(const char *a,...)
{
	va_list va;
	char s_text[ 1024 ];
	va_start( va, a );
	vsprintf( s_text, a, va );
	va_end( va );
	logerror( "%08x: GTE: %08x %s\n", cpu->pc, INS_COFUN( cpu->op ), s_text );
}
#else
static INLINE void GTELOG(const char *a, ...) {}
#endif

static UINT32 getcp2dr( mips_cpu_context *cpu, int n_reg );
static void setcp2dr( mips_cpu_context *cpu, int n_reg, UINT32 n_value );
static UINT32 getcp2cr( mips_cpu_context *cpu, int n_reg );
static void setcp2cr( mips_cpu_context *cpu, int n_reg, UINT32 n_value );
static void docop2( mips_cpu_context *cpu, int gteop );
static void mips_exception(mips_cpu_context *cpu,  int exception );

void mips_stop( void )
{
#ifdef MAME_DEBUG
	extern int debug_key_pressed;
	debug_key_pressed = 1;
	CALL_MAME_DEBUG;
#endif
}

static INLINE void mips_set_cp0r(mips_cpu_context *cpu, int reg, UINT32 value )
{
	cpu->cp0r[ reg ] = value;
	if( reg == CP0_SR || reg == CP0_CAUSE )
	{
		if( ( cpu->cp0r[ CP0_SR ] & SR_IEC ) != 0 && ( cpu->cp0r[ CP0_SR ] & cpu->cp0r[ CP0_CAUSE ] & CAUSE_IP ) != 0 )
		{
			mips_exception(cpu, EXC_INT );
		}
		else if( cpu->delayr != REGPC && ( cpu->pc & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 3 ) ) != 0 )
		{
			mips_exception( cpu, EXC_ADEL );
			mips_set_cp0r( cpu, CP0_BADVADDR, cpu->pc );
		}
	}
}

static INLINE void mips_commit_delayed_load(mips_cpu_context *cpu)
{
	if( cpu->delayr != 0 )
	{
		cpu->r[ cpu->delayr ] = cpu->delayv;
		cpu->delayr = 0;
		cpu->delayv = 0;
	}
}

static INLINE void mips_delayed_branch(mips_cpu_context *cpu, UINT32 n_adr )
{
	if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 3 ) ) != 0 )
	{
		mips_exception( cpu, EXC_ADEL );
		mips_set_cp0r( cpu, CP0_BADVADDR, n_adr );
	}
	else
	{
		mips_commit_delayed_load(cpu);
		cpu->delayr = REGPC;
		cpu->delayv = n_adr;
		cpu->pc += 4;
	}
}

static INLINE void mips_set_pc(mips_cpu_context *cpu, unsigned val )
{
	cpu->pc = val;
	change_pc( val );
	cpu->delayr = 0;
	cpu->delayv = 0;
}

static INLINE void mips_advance_pc(mips_cpu_context *cpu)
{
	if( cpu->delayr == REGPC )
	{
		mips_set_pc( cpu, cpu->delayv );
	}
	else
	{
		mips_commit_delayed_load(cpu);
		cpu->pc += 4;
	}
}

static INLINE void mips_load(mips_cpu_context *cpu, UINT32 n_r, UINT32 n_v )
{
	mips_advance_pc(cpu);
	if( n_r != 0 )
	{
		cpu->r[ n_r ] = n_v;
	}
}

static INLINE void mips_delayed_load( mips_cpu_context *cpu, UINT32 n_r, UINT32 n_v )
{
	if( cpu->delayr == REGPC )
	{
		mips_set_pc( cpu, cpu->delayv );
		cpu->delayr = n_r;
		cpu->delayv = n_v;
	}
	else
	{
		mips_commit_delayed_load(cpu);
		cpu->pc += 4;
		if( n_r != 0 )
		{
			cpu->r[ n_r ] = n_v;
		}
	}
}

static void mips_exception( mips_cpu_context *cpu, int exception )
{
	mips_set_cp0r( cpu, CP0_SR, ( cpu->cp0r[ CP0_SR ] & ~0x3f ) | ( ( cpu->cp0r[ CP0_SR ] << 2 ) & 0x3f ) );
	if( cpu->delayr == REGPC )
	{
		mips_set_cp0r( cpu, CP0_EPC, cpu->pc - 4 );
		mips_set_cp0r( cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~CAUSE_EXC ) | CAUSE_BD | ( exception << 2 ) );
	}
	else
	{
		mips_commit_delayed_load(cpu);
		mips_set_cp0r( cpu, CP0_EPC, cpu->pc );
		mips_set_cp0r( cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~( CAUSE_EXC | CAUSE_BD ) ) | ( exception << 2 ) );
	}
	if( cpu->cp0r[ CP0_SR ] & SR_BEV )
	{
		mips_set_pc (cpu,  0xbfc00180 );
	}
	else
	{
		mips_set_pc (cpu,  0x80000080 );
	}
}

mips_cpu_context *mips_alloc( void ) {
    mips_cpu_context *ctx = malloc (sizeof (mips_cpu_context));
    memset (ctx, 0, sizeof (mips_cpu_context));
    return ctx;
}

void mips_init(mips_cpu_context *cpu)
{
#if 0
	int cpu = cpu_getactivecpu();

	state_save_register_UINT32( "psxcpu", cpu, "op", &cpu->op, 1 );
	state_save_register_UINT32( "psxcpu", cpu, "pc", &cpu->pc, 1 );
	state_save_register_UINT32( "psxcpu", cpu, "delayv", &cpu->delayv, 1 );
	state_save_register_UINT32( "psxcpu", cpu, "delayr", &cpu->delayr, 1 );
	state_save_register_UINT32( "psxcpu", cpu, "hi", &cpu->hi, 1 );
	state_save_register_UINT32( "psxcpu", cpu, "lo", &cpu->lo, 1 );
	state_save_register_UINT32( "psxcpu", cpu, "r", &cpu->r[ 0 ], 32 );
	state_save_register_UINT32( "psxcpu", cpu, "cp0r", &cpu->cp0r[ 0 ], 32 );
	state_save_register_UINT32( "psxcpu", cpu, "cp2cr", &cpu->cp2cr[ 0 ].d, 32 );
	state_save_register_UINT32( "psxcpu", cpu, "cp2dr", &cpu->cp2dr[ 0 ].d, 32 );
#endif
}

void mips_reset( mips_cpu_context *cpu, void *param )
{
	mips_set_cp0r( cpu, CP0_SR, ( cpu->cp0r[ CP0_SR ] & ~( SR_TS | SR_SWC | SR_KUC | SR_IEC ) ) | SR_BEV );
	mips_set_cp0r( cpu, CP0_RANDOM, 63 ); /* todo: */
	mips_set_cp0r( cpu, CP0_PRID, 0x00000200 ); /* todo: */
	mips_set_pc (cpu,  0xbfc00000 );
	cpu->prevpc = 0xffffffff;
}

void mips_exit(mips_cpu_context *cpu) {
    free (cpu);
}

void mips_shorten_frame(mips_cpu_context *cpu)
{
	cpu->mips_ICount = 0;
}

void psx_hw_runcounters(mips_cpu_context *cpu);

int psxcpu_verbose = 0;

int mips_execute( mips_cpu_context *cpu, int cycles )
{
	UINT32 n_res;

	cpu->mips_ICount = cycles;
	do
	{
//		CALL_MAME_DEBUG;

//		psx_hw_runcounters();
	
		cpu->op = cpu_readop32( cpu->pc );

#if 0
		while (cpu->prevpc == cpu->pc)
		{
			psx_hw_runcounters();
			cpu->mips_ICount--;

			if (cpu->mips_ICount == 0) return cycles;
		}

		// if we're not in a delay slot, update
		// if we're in a delay slot and the delay instruction is not NOP, update
		if (( cpu->delayr == 0 ) || ((cpu->delayr != 0) && (cpu->op != 0)))
		{
			cpu->prevpc = cpu->pc;
		}	
#endif
#if 0
		if (1) //psxcpu_verbose)
		{
			printf("[%08x: %08x] [SP %08x RA %08x V0 %08x V1 %08x A0 %08x S0 %08x S1 %08x]\n", cpu->pc, cpu->op, cpu->r[29], cpu->r[31], cpu->r[2], cpu->r[3], cpu->r[4], cpu->r[ 16 ], cpu->r[ 17 ]);
//			psxcpu_verbose--;
		}
#endif
		switch( INS_OP( cpu->op ) )
		{
		case OP_SPECIAL:
			switch( INS_FUNCT( cpu->op ) )
			{
			case FUNCT_HLECALL:
//				printf("HLECALL, PC = %08x\n", cpu->pc);
				psx_bios_hle(cpu, cpu->pc);
				break;
			case FUNCT_SLL:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RT( cpu->op ) ] << INS_SHAMT( cpu->op ) );
				break;
			case FUNCT_SRL:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RT( cpu->op ) ] >> INS_SHAMT( cpu->op ) );
				break;
			case FUNCT_SRA:
				mips_load(cpu,  INS_RD( cpu->op ), (INT32)cpu->r[ INS_RT( cpu->op ) ] >> INS_SHAMT( cpu->op ) );
				break;
			case FUNCT_SLLV:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RT( cpu->op ) ] << ( cpu->r[ INS_RS( cpu->op ) ] & 31 ) );
				break;
			case FUNCT_SRLV:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RT( cpu->op ) ] >> ( cpu->r[ INS_RS( cpu->op ) ] & 31 ) );
				break;
			case FUNCT_SRAV:
				mips_load(cpu,  INS_RD( cpu->op ), (INT32)cpu->r[ INS_RT( cpu->op ) ] >> ( cpu->r[ INS_RS( cpu->op ) ] & 31 ) );
				break;
			case FUNCT_JR:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception( cpu, EXC_RI );
				}
				else
				{
					mips_delayed_branch( cpu, cpu->r[ INS_RS( cpu->op ) ] );
				}
				break;
			case FUNCT_JALR:
				n_res = cpu->pc + 8;
				mips_delayed_branch( cpu, cpu->r[ INS_RS( cpu->op ) ] );
				if( INS_RD( cpu->op ) != 0 )
				{
					cpu->r[ INS_RD( cpu->op ) ] = n_res;
				}
				break;
			case FUNCT_SYSCALL:
				mips_exception( cpu, EXC_SYS );
				break;
			case FUNCT_BREAK:
				printf("BREAK!\n");
				exit(-1);
//				mips_exception( cpu, EXC_BP );
				mips_advance_pc(cpu);
				break;
			case FUNCT_MFHI:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->hi );
				break;
			case FUNCT_MTHI:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception(cpu, EXC_RI );
				}
				else
				{
					mips_advance_pc(cpu);
					cpu->hi = cpu->r[ INS_RS( cpu->op ) ];
				}
				break;
			case FUNCT_MFLO:
				mips_load(cpu,  INS_RD( cpu->op ),  cpu->lo );
				break;
			case FUNCT_MTLO:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception(cpu, EXC_RI );
				}
				else
				{
					mips_advance_pc(cpu);
					cpu->lo = cpu->r[ INS_RS( cpu->op ) ];
				}
				break;
			case FUNCT_MULT:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception(cpu, EXC_RI );
				}
				else
				{
					INT64 n_res64;
					n_res64 = MUL_64_32_32( (INT32)cpu->r[ INS_RS( cpu->op ) ], (INT32)cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
					cpu->lo = LO32_32_64( n_res64 );
					cpu->hi = HI32_32_64( n_res64 );
				}
				break;
			case FUNCT_MULTU:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception(cpu, EXC_RI );
				}
				else
				{
					UINT64 n_res64;
					n_res64 = MUL_U64_U32_U32( cpu->r[ INS_RS( cpu->op ) ], cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
					cpu->lo = LO32_U32_U64( n_res64 );
					cpu->hi = HI32_U32_U64( n_res64 );
				}
				break;
			case FUNCT_DIV:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception(cpu, EXC_RI );
				}
				else
				{
					UINT32 n_div;
					UINT32 n_mod;
					if( cpu->r[ INS_RT( cpu->op ) ] != 0 )
					{
						n_div = (INT32)cpu->r[ INS_RS( cpu->op ) ] / (INT32)cpu->r[ INS_RT( cpu->op ) ];
						n_mod = (INT32)cpu->r[ INS_RS( cpu->op ) ] % (INT32)cpu->r[ INS_RT( cpu->op ) ];
						mips_advance_pc(cpu);
						cpu->lo = n_div;
						cpu->hi = n_mod;
					}
					else
					{
						mips_advance_pc(cpu);
					}
				}
				break;
			case FUNCT_DIVU:
				if( INS_RD( cpu->op ) != 0 )
				{
					mips_exception(cpu, EXC_RI );
				}
				else
				{
					UINT32 n_div;
					UINT32 n_mod;
					if( cpu->r[ INS_RT( cpu->op ) ] != 0 )
					{
						n_div = cpu->r[ INS_RS( cpu->op ) ] / cpu->r[ INS_RT( cpu->op ) ];
						n_mod = cpu->r[ INS_RS( cpu->op ) ] % cpu->r[ INS_RT( cpu->op ) ];
						mips_advance_pc(cpu);
						cpu->lo = n_div;
						cpu->hi = n_mod;
					}
					else
					{
						mips_advance_pc(cpu);
					}
				}
				break;
			case FUNCT_ADD:
				{
					n_res = cpu->r[ INS_RS( cpu->op ) ] + cpu->r[ INS_RT( cpu->op ) ];
					if( (INT32)( ~( cpu->r[ INS_RS( cpu->op ) ] ^ cpu->r[ INS_RT( cpu->op ) ] ) & ( cpu->r[ INS_RS( cpu->op ) ] ^ n_res ) ) < 0 )
					{
						mips_exception(cpu, EXC_OVF );
					}
					else
					{
						mips_load(cpu,  INS_RD( cpu->op ), n_res );
					}
				}
				break;
			case FUNCT_ADDU:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] + cpu->r[ INS_RT( cpu->op ) ] );
				break;
			case FUNCT_SUB:
				n_res = cpu->r[ INS_RS( cpu->op ) ] - cpu->r[ INS_RT( cpu->op ) ];
				if( (INT32)( ( cpu->r[ INS_RS( cpu->op ) ] ^ cpu->r[ INS_RT( cpu->op ) ] ) & ( cpu->r[ INS_RS( cpu->op ) ] ^ n_res ) ) < 0 )
				{
					mips_exception(cpu, EXC_OVF );
				}
				else
				{
					mips_load(cpu,  INS_RD( cpu->op ), n_res );
				}
				break;
			case FUNCT_SUBU:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] - cpu->r[ INS_RT( cpu->op ) ] );
				break;
			case FUNCT_AND:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] & cpu->r[ INS_RT( cpu->op ) ] );
				break;
			case FUNCT_OR:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] | cpu->r[ INS_RT( cpu->op ) ] );
				break;
			case FUNCT_XOR:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] ^ cpu->r[ INS_RT( cpu->op ) ] );
				break;
			case FUNCT_NOR:
				mips_load(cpu,  INS_RD( cpu->op ), ~( cpu->r[ INS_RS( cpu->op ) ] | cpu->r[ INS_RT( cpu->op ) ] ) );
				break;
			case FUNCT_SLT:
				mips_load(cpu,  INS_RD( cpu->op ), (INT32)cpu->r[ INS_RS( cpu->op ) ] < (INT32)cpu->r[ INS_RT( cpu->op ) ] );
				break;
			case FUNCT_SLTU:
				mips_load(cpu,  INS_RD( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] < cpu->r[ INS_RT( cpu->op ) ] );
				break;
			default:
				mips_exception(cpu, EXC_RI );
				break;
			}
			break;
		case OP_REGIMM:
			switch( INS_RT( cpu->op ) )
			{
			case RT_BLTZ:
				if( (INT32)cpu->r[ INS_RS( cpu->op ) ] < 0 )
				{
					mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
				}
				else
				{
					mips_advance_pc(cpu);
				}
				break;
			case RT_BGEZ:
				if( (INT32)cpu->r[ INS_RS( cpu->op ) ] >= 0 )
				{
					mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
				}
				else
				{
					mips_advance_pc(cpu);
				}
				break;
			case RT_BLTZAL:
				n_res = cpu->pc + 8;
				if( (INT32)cpu->r[ INS_RS( cpu->op ) ] < 0 )
				{
					mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
				}
				else
				{
					mips_advance_pc(cpu);
				}
				cpu->r[ 31 ] = n_res;
				break;
			case RT_BGEZAL:
				n_res = cpu->pc + 8;
				if( (INT32)cpu->r[ INS_RS( cpu->op ) ] >= 0 )
				{
					mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
				}
				else
				{
					mips_advance_pc(cpu);
				}
				cpu->r[ 31 ] = n_res;
				break;
			}
			break;
		case OP_J:
			mips_delayed_branch( cpu, ( ( cpu->pc + 4 ) & 0xf0000000 ) + ( INS_TARGET( cpu->op ) << 2 ) );
			break;
		case OP_JAL:
			n_res = cpu->pc + 8;
			mips_delayed_branch( cpu, ( ( cpu->pc + 4 ) & 0xf0000000 ) + ( INS_TARGET( cpu->op ) << 2 ) );
			cpu->r[ 31 ] = n_res;
			break;
		case OP_BEQ:
			if( cpu->r[ INS_RS( cpu->op ) ] == cpu->r[ INS_RT( cpu->op ) ] )
			{
				mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
			}
			else
			{
				mips_advance_pc(cpu);
			}
			break;
		case OP_BNE:
			if( cpu->r[ INS_RS( cpu->op ) ] != cpu->r[ INS_RT( cpu->op ) ] )
			{
				mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
			}
			else
			{
				mips_advance_pc(cpu);
			}
			break;
		case OP_BLEZ:
			if( INS_RT( cpu->op ) != 0 )
			{
				mips_exception(cpu, EXC_RI );
			}
			else if( (INT32)cpu->r[ INS_RS( cpu->op ) ] <= 0 )
			{
				mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
			}
			else
			{
				mips_advance_pc(cpu);
			}
			break;
		case OP_BGTZ:
			if( INS_RT( cpu->op ) != 0 )
			{
				mips_exception(cpu, EXC_RI );
			}
			else if( (INT32)cpu->r[ INS_RS( cpu->op ) ] > 0 )
			{
				mips_delayed_branch( cpu, cpu->pc + 4 + ( MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) << 2 ) );
			}
			else
			{
				mips_advance_pc(cpu);
			}
			break;
		case OP_ADDI:
			{
				UINT32 n_imm;
				n_imm = MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				n_res = cpu->r[ INS_RS( cpu->op ) ] + n_imm;
				if( (INT32)( ~( cpu->r[ INS_RS( cpu->op ) ] ^ n_imm ) & ( cpu->r[ INS_RS( cpu->op ) ] ^ n_res ) ) < 0 )
				{
					mips_exception(cpu, EXC_OVF );
				}
				else
				{
					mips_load(cpu,  INS_RT( cpu->op ), n_res );
				}
			}
			break;
		case OP_ADDIU:
			if (INS_RT( cpu->op ) == 0)
			{
				psx_iop_call(cpu, cpu->pc, INS_IMMEDIATE(cpu->op));
				mips_advance_pc(cpu);
			}
			else
			{
				mips_load(cpu,  INS_RT( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) );
			}
			break;
		case OP_SLTI:
			mips_load(cpu,  INS_RT( cpu->op ), (INT32)cpu->r[ INS_RS( cpu->op ) ] < MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) );
			break;
		case OP_SLTIU:
			mips_load(cpu,  INS_RT( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] < (UINT32)MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) ) );
			break;
		case OP_ANDI:
			mips_load(cpu,  INS_RT( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] & INS_IMMEDIATE( cpu->op ) );
			break;
		case OP_ORI:
			mips_load(cpu,  INS_RT( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] | INS_IMMEDIATE( cpu->op ) );
			break;
		case OP_XORI:
			mips_load(cpu,  INS_RT( cpu->op ), cpu->r[ INS_RS( cpu->op ) ] ^ INS_IMMEDIATE( cpu->op ) );
			break;
		case OP_LUI:
			mips_load(cpu,  INS_RT( cpu->op ), INS_IMMEDIATE( cpu->op ) << 16 );
			break;
		case OP_COP0:
			if( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) != 0 && ( cpu->cp0r[ CP0_SR ] & SR_CU0 ) == 0 )
			{
				mips_exception(cpu, EXC_CPU );
				mips_set_cp0r( cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~CAUSE_CE ) | CAUSE_CE0 );
			}
			else
			{
				switch( INS_RS( cpu->op ) )
				{
				case RS_MFC:
					mips_delayed_load( cpu, INS_RT( cpu->op ), cpu->cp0r[ INS_RD( cpu->op ) ] );
					break;
				case RS_CFC:
					/* todo: */
					logerror( "%08x: COP0 CFC not supported\n", cpu->pc );
					mips_stop();
					mips_advance_pc(cpu);
					break;
				case RS_MTC:
					n_res = ( cpu->cp0r[ INS_RD( cpu->op ) ] & ~mips_mtc0_writemask[ INS_RD( cpu->op ) ] ) |
						( cpu->r[ INS_RT( cpu->op ) ] & mips_mtc0_writemask[ INS_RD( cpu->op ) ] );
					mips_advance_pc(cpu);
					mips_set_cp0r(cpu, INS_RD( cpu->op ), n_res );
					break;
				case RS_CTC:
					/* todo: */
					logerror( "%08x: COP0 CTC not supported\n", cpu->pc );
					mips_stop();
					mips_advance_pc(cpu);
					break;
				case RS_BC:
					switch( INS_RT( cpu->op ) )
					{
					case RT_BCF:
						/* todo: */
						logerror( "%08x: COP0 BCF not supported\n", cpu->pc );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					case RT_BCT:
						/* todo: */
						logerror( "%08x: COP0 BCT not supported\n", cpu->pc );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					default:
						/* todo: */
						logerror( "%08x: COP0 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					}
					break;
				default:
					switch( INS_CO( cpu->op ) )
					{
					case 1:
						switch( INS_CF( cpu->op ) )
						{
						case CF_RFE:
							mips_advance_pc(cpu);
							mips_set_cp0r(cpu, CP0_SR, ( cpu->cp0r[ CP0_SR ] & ~0xf ) | ( ( cpu->cp0r[ CP0_SR ] >> 2 ) & 0xf ) );
							break;
						default:
							/* todo: */
							logerror( "%08x: COP0 unknown command %08x\n", cpu->pc, cpu->op );
							mips_stop();
							mips_advance_pc(cpu);
							break;
						}
						break;
					default:
						/* todo: */
						logerror( "%08x: COP0 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					}
					break;
				}
			}
			break;
		case OP_COP1:
			if( ( cpu->cp0r[ CP0_SR ] & SR_CU1 ) == 0 )
			{
				mips_exception(cpu, EXC_CPU );
				mips_set_cp0r(cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~CAUSE_CE ) | CAUSE_CE1 );
			}
			else
			{
				switch( INS_RS( cpu->op ) )
				{
				case RS_MFC:
					/* todo: */
					logerror( "%08x: COP1 BCT not supported\n", cpu->pc );
					mips_stop();
					mips_advance_pc(cpu);
					break;
				case RS_CFC:
					/* todo: */
					logerror( "%08x: COP1 CFC not supported\n", cpu->pc );
					mips_stop();
					mips_advance_pc(cpu);
					break;
				case RS_MTC:
					/* todo: */
					logerror( "%08x: COP1 MTC not supported\n", cpu->pc );
					mips_stop();
					mips_advance_pc(cpu);
					break;
				case RS_CTC:
					/* todo: */
					logerror( "%08x: COP1 CTC not supported\n", cpu->pc );
					mips_stop();
					mips_advance_pc(cpu);
					break;
				case RS_BC:
					switch( INS_RT( cpu->op ) )
					{
					case RT_BCF:
						/* todo: */
						logerror( "%08x: COP1 BCF not supported\n", cpu->pc );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					case RT_BCT:
						/* todo: */
						logerror( "%08x: COP1 BCT not supported\n", cpu->pc );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					default:
						/* todo: */
						logerror( "%08x: COP1 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					}
					break;
				default:
					switch( INS_CO( cpu->op ) )
					{
					case 1:
						/* todo: */
						logerror( "%08x: COP1 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					default:
						/* todo: */
						logerror( "%08x: COP1 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					}
					break;
				}
			}
			break;
		case OP_COP2:
			if( ( cpu->cp0r[ CP0_SR ] & SR_CU2 ) == 0 )
			{
				mips_exception(cpu, EXC_CPU );
				mips_set_cp0r(cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~CAUSE_CE ) | CAUSE_CE2 );
			}
			else
			{
				switch( INS_RS( cpu->op ) )
				{
				case RS_MFC:
					mips_delayed_load(cpu, INS_RT( cpu->op ), getcp2dr(cpu, INS_RD( cpu->op ) ) );
					break;
				case RS_CFC:
					mips_delayed_load(cpu, INS_RT( cpu->op ), getcp2cr(cpu, INS_RD( cpu->op ) ) );
					break;
				case RS_MTC:
					setcp2dr(cpu, INS_RD( cpu->op ), cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
					break;
				case RS_CTC:
					setcp2cr(cpu, INS_RD( cpu->op ), cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
					break;
				case RS_BC:
					switch( INS_RT( cpu->op ) )
					{
					case RT_BCF:
						/* todo: */
						logerror( "%08x: COP2 BCF not supported\n", cpu->pc );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					case RT_BCT:
						/* todo: */
						logerror( "%08x: COP2 BCT not supported\n", cpu->pc );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					default:
						/* todo: */
						logerror( "%08x: COP2 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					}
					break;
				default:
					switch( INS_CO( cpu->op ) )
					{
					case 1:
						docop2(cpu, INS_COFUN( cpu->op ) );
						mips_advance_pc(cpu);
						break;
					default:
						/* todo: */
						logerror( "%08x: COP2 unknown command %08x\n", cpu->pc, cpu->op );
						mips_stop();
						mips_advance_pc(cpu);
						break;
					}
					break;
				}
			}
			break;
		case OP_LB:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LB SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), MIPS_BYTE_EXTEND( program_read_byte_32le( cpu, n_adr ^ 3 ) ) );
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), MIPS_BYTE_EXTEND( program_read_byte_32le( cpu, n_adr ) ) );
				}
			}
			break;
		case OP_LH:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LH SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 1 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), MIPS_WORD_EXTEND( program_read_word_32le( cpu, n_adr ^ 2 ) ) );
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 1 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), MIPS_WORD_EXTEND( program_read_word_32le( cpu, n_adr ) ) );
				}
			}
			break;
		case OP_LWL:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LWL SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 0:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0x00ffffff ) | ( (UINT32)program_read_byte_32le( cpu, n_adr + 3 ) << 24 );
						break;
					case 1:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0x0000ffff ) | ( (UINT32)program_read_word_32le( cpu, n_adr + 1 ) << 16 );
						break;
					case 2:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0x000000ff ) | ( (UINT32)program_read_byte_32le( cpu, n_adr - 1 ) << 8 ) | ( (UINT32)program_read_word_32le( cpu, n_adr ) << 16 );
						break;
					default:
						n_res = program_read_dword_32le( cpu, n_adr - 3 );
						break;
					}
					mips_delayed_load(cpu, INS_RT( cpu->op ), n_res );
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 0:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0x00ffffff ) | ( (UINT32)program_read_byte_32le( cpu, n_adr ) << 24 );
						break;
					case 1:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0x0000ffff ) | ( (UINT32)program_read_word_32le( cpu, n_adr - 1 ) << 16 );
						break;
					case 2:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0x000000ff ) | ( (UINT32)program_read_word_32le( cpu, n_adr - 2 ) << 8 ) | ( (UINT32)program_read_byte_32le( cpu, n_adr ) << 24 );
						break;
					default:
						n_res = program_read_dword_32le( cpu, n_adr - 3 );
						break;
					}
					mips_delayed_load(cpu, INS_RT( cpu->op ), n_res );
				}
			}
			break;
		case OP_LW:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LW SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
#if 0
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 3 ) ) != 0 )
				{
					printf("ADEL\n");
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
#endif
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), program_read_dword_32le( cpu, n_adr ) );
				}
			}
			break;
		case OP_LBU:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LBU SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), program_read_byte_32le( cpu, n_adr ^ 3 ) );
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), program_read_byte_32le( cpu, n_adr ) );
				}
			}
			break;
		case OP_LHU:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LHU SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 1 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), program_read_word_32le( cpu, n_adr ^ 2 ) );
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 1 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					mips_delayed_load(cpu, INS_RT( cpu->op ), program_read_word_32le( cpu, n_adr ) );
				}
			}
			break;
		case OP_LWR:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LWR SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 3:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0xffffff00 ) | program_read_byte_32le( cpu, n_adr - 3 );
						break;
					case 2:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0xffff0000 ) | program_read_word_32le( cpu, n_adr - 2 );
						break;
					case 1:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0xff000000 ) | program_read_word_32le( cpu, n_adr - 1 ) | ( (UINT32)program_read_byte_32le( cpu, n_adr + 1 ) << 16 );
						break;
					default:
						n_res = program_read_dword_32le( cpu, n_adr );
						break;
					}
					mips_delayed_load(cpu, INS_RT( cpu->op ), n_res );
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 3:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0xffffff00 ) | program_read_byte_32le( cpu, n_adr );
						break;
					case 2:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0xffff0000 ) | program_read_word_32le( cpu, n_adr );
						break;
					case 1:
						n_res = ( cpu->r[ INS_RT( cpu->op ) ] & 0xff000000 ) | program_read_byte_32le( cpu, n_adr ) | ( (UINT32)program_read_word_32le( cpu, n_adr + 1 ) << 8 );
						break;
					default:
						n_res = program_read_dword_32le( cpu, n_adr );
						break;
					}
					mips_delayed_load(cpu, INS_RT( cpu->op ), n_res );
				}
			}
			break;
		case OP_SB:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: SB SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					program_write_byte_32le( cpu, n_adr ^ 3, cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					program_write_byte_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
				}
			}
			break;
		case OP_SH:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: SH SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 1 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					program_write_word_32le( cpu, n_adr ^ 2, cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 1 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					program_write_word_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
				}
			}
			break;
		case OP_SWL:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				printf("SR_ISC not supported\n");
				logerror( "%08x: SWL SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					printf("permission violation?\n");
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 0:
						program_write_byte_32le( cpu, n_adr + 3, cpu->r[ INS_RT( cpu->op ) ] >> 24 );
						break;
					case 1:
						program_write_word_32le( cpu, n_adr + 1, cpu->r[ INS_RT( cpu->op ) ] >> 16 );
						break;
					case 2:
						program_write_byte_32le( cpu, n_adr - 1, cpu->r[ INS_RT( cpu->op ) ] >> 8 );
						program_write_word_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] >> 16 );
						break;
					case 3:
						program_write_dword_32le( cpu, n_adr - 3, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					}
					mips_advance_pc(cpu);
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					printf("permission violation 2\n");
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 0:
						program_write_byte_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] >> 24 );
						break;
					case 1:
						program_write_word_32le( cpu, n_adr - 1, cpu->r[ INS_RT( cpu->op ) ] >> 16 );
						break;
					case 2:
						program_write_word_32le( cpu, n_adr - 2, cpu->r[ INS_RT( cpu->op ) ] >> 8 );
						program_write_byte_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] >> 24 );
						break;
					case 3:
						program_write_dword_32le( cpu, n_adr - 3, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					}
					mips_advance_pc(cpu);
				}
			}
			break;
		case OP_SW:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
/* used by bootstrap
				logerror( "%08x: SW SR_ISC not supported\n", cpu->pc );
				mips_stop();
*/
				mips_advance_pc(cpu);
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if(0) // ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 3 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					program_write_dword_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
					mips_advance_pc(cpu);
				}
			}
			break;
		case OP_SWR:
			if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: SWR SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else if( ( cpu->cp0r[ CP0_SR ] & ( SR_RE | SR_KUC ) ) == ( SR_RE | SR_KUC ) )
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 0:
						program_write_dword_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					case 1:
						program_write_word_32le( cpu, n_adr - 1, cpu->r[ INS_RT( cpu->op ) ] );
						program_write_byte_32le( cpu, n_adr + 1, cpu->r[ INS_RT( cpu->op ) ] >> 16 );
						break;
					case 2:
						program_write_word_32le( cpu, n_adr - 2, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					case 3:
						program_write_byte_32le( cpu, n_adr - 3, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					}
					mips_advance_pc(cpu);
				}
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					switch( n_adr & 3 )
					{
					case 0:
						program_write_dword_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					case 1:
						program_write_byte_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
						program_write_word_32le( cpu, n_adr + 1, cpu->r[ INS_RT( cpu->op ) ] >> 8 );
						break;
					case 2:
						program_write_word_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					case 3:
						program_write_byte_32le( cpu, n_adr, cpu->r[ INS_RT( cpu->op ) ] );
						break;
					}
					mips_advance_pc(cpu);
				}
			}
			break;
		case OP_LWC1:
			/* todo: */
			logerror( "%08x: COP1 LWC not supported\n", cpu->pc );
			mips_stop();
			mips_advance_pc(cpu);
			break;
		case OP_LWC2:
			if( ( cpu->cp0r[ CP0_SR ] & SR_CU2 ) == 0 )
			{
				mips_exception(cpu, EXC_CPU );
				mips_set_cp0r(cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~CAUSE_CE ) | CAUSE_CE2 );
			}
			else if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: LWC2 SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 3 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADEL );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					/* todo: delay? */
					setcp2dr(cpu, INS_RT( cpu->op ), program_read_dword_32le( cpu, n_adr ) );
					mips_advance_pc(cpu);
				}
			}
			break;
		case OP_SWC1:
			/* todo: */
			logerror( "%08x: COP1 SWC not supported\n", cpu->pc );
			mips_stop();
			mips_advance_pc(cpu);
			break;
		case OP_SWC2:
			if( ( cpu->cp0r[ CP0_SR ] & SR_CU2 ) == 0 )
			{
				mips_exception(cpu, EXC_CPU );
				mips_set_cp0r(cpu, CP0_CAUSE, ( cpu->cp0r[ CP0_CAUSE ] & ~CAUSE_CE ) | CAUSE_CE2 );
			}
			else if( ( cpu->cp0r[ CP0_SR ] & SR_ISC ) != 0 )
			{
				/* todo: */
				logerror( "%08x: SWC2 SR_ISC not supported\n", cpu->pc );
				mips_stop();
				mips_advance_pc(cpu);
			}
			else
			{
				UINT32 n_adr;
				n_adr = cpu->r[ INS_RS( cpu->op ) ] + MIPS_WORD_EXTEND( INS_IMMEDIATE( cpu->op ) );
				if( ( n_adr & ( ( ( cpu->cp0r[ CP0_SR ] & SR_KUC ) << 30 ) | 3 ) ) != 0 )
				{
					mips_exception(cpu, EXC_ADES );
					mips_set_cp0r(cpu, CP0_BADVADDR, n_adr );
				}
				else
				{
					program_write_dword_32le( cpu, n_adr, getcp2dr(cpu, INS_RT( cpu->op ) ) );
					mips_advance_pc(cpu);
				}
			}
			break;
		default:
//			printf( "%08x: unknown opcode %08x (prev %08x, RA %08x)\n", cpu->pc, cpu->op, cpu->prevpc,  cpu->r[31] );
//			mips_stop();
//			mips_exception(cpu, EXC_RI );
  			break;
		}
//skipinterp: // unused label
		cpu->mips_ICount--;
	} while( cpu->mips_ICount > 0 );

	return cycles - cpu->mips_ICount;
}

#if 0
static void mips_get_context( void *dst )
{
	if( dst )
	{
		*(mips_cpu_context *)dst = mipscpu;
	}
}

static void mips_set_context( void *src )
{
	if( src )
	{
		mipscpu = *(mips_cpu_context *)src;
		change_pc( cpu->pc );
	}
}
#endif

static void set_irq_line( mips_cpu_context *cpu, int irqline, int state )
{
	UINT32 ip;

	switch( irqline )
	{
	case MIPS_IRQ0:
		ip = CAUSE_IP2;
		break;
	case MIPS_IRQ1:
		ip = CAUSE_IP3;
		break;
	case MIPS_IRQ2:
		ip = CAUSE_IP4;
		break;
	case MIPS_IRQ3:
		ip = CAUSE_IP5;
		break;
	case MIPS_IRQ4:
		ip = CAUSE_IP6;
		break;
	case MIPS_IRQ5:
		ip = CAUSE_IP7;
		break;
	default:
		return;
	}

	switch( state )
	{
	case CLEAR_LINE:
		mips_set_cp0r(cpu, CP0_CAUSE, cpu->cp0r[ CP0_CAUSE ] & ~ip );
		break;
	case ASSERT_LINE:
		mips_set_cp0r(cpu, CP0_CAUSE, cpu->cp0r[ CP0_CAUSE ] |= ip );
		if( cpu->irq_callback )
		{
			/* HOLD_LINE interrupts are not supported by the architecture.
			By acknowledging the interupt here they are treated like PULSE_LINE
			interrupts, so if the interrupt isn't enabled it will be ignored.
			There is also a problem with PULSE_LINE interrupts as the interrupt
			pending bits aren't latched the emulated code won't know what caused
			the interrupt. */
			(*cpu->irq_callback)( cpu, irqline );
		}
		break;
	}
}

/****************************************************************************
 * Return a formatted string for a register
 ****************************************************************************/

offs_t mips_dasm( mips_cpu_context *cpu, char *buffer, offs_t pc )
{
	offs_t ret;
	change_pc( pc );
#ifdef MAME_DEBUG
	ret = DasmMIPS( buffer, pc );
#else
	sprintf( buffer, "$%08x", cpu_readop32( pc ) );
	ret = 4;
#endif
	change_pc( cpu->pc );
	return ret;
}

/* preliminary gte code */

#define VXY0 ( cpu->cp2dr[ 0 ].d )
#define VX0  ( cpu->cp2dr[ 0 ].w.l )
#define VY0  ( cpu->cp2dr[ 0 ].w.h )
#define VZ0  ( cpu->cp2dr[ 1 ].w.l )
#define VXY1 ( cpu->cp2dr[ 2 ].d )
#define VX1  ( cpu->cp2dr[ 2 ].w.l )
#define VY1  ( cpu->cp2dr[ 2 ].w.h )
#define VZ1  ( cpu->cp2dr[ 3 ].w.l )
#define VXY2 ( cpu->cp2dr[ 4 ].d )
#define VX2  ( cpu->cp2dr[ 4 ].w.l )
#define VY2  ( cpu->cp2dr[ 4 ].w.h )
#define VZ2  ( cpu->cp2dr[ 5 ].w.l )
#define RGB  ( cpu->cp2dr[ 6 ].d )
#define R    ( cpu->cp2dr[ 6 ].b.l )
#define G    ( cpu->cp2dr[ 6 ].b.h )
#define B    ( cpu->cp2dr[ 6 ].b.h2 )
#define CODE ( cpu->cp2dr[ 6 ].b.h3 )
#define OTZ  ( cpu->cp2dr[ 7 ].w.l )
#define IR0  ( cpu->cp2dr[ 8 ].d )
#define IR1  ( cpu->cp2dr[ 9 ].d )
#define IR2  ( cpu->cp2dr[ 10 ].d )
#define IR3  ( cpu->cp2dr[ 11 ].d )
#define SXY0 ( cpu->cp2dr[ 12 ].d )
#define SX0  ( cpu->cp2dr[ 12 ].w.l )
#define SY0  ( cpu->cp2dr[ 12 ].w.h )
#define SXY1 ( cpu->cp2dr[ 13 ].d )
#define SX1  ( cpu->cp2dr[ 13 ].w.l )
#define SY1  ( cpu->cp2dr[ 13 ].w.h )
#define SXY2 ( cpu->cp2dr[ 14 ].d )
#define SX2  ( cpu->cp2dr[ 14 ].w.l )
#define SY2  ( cpu->cp2dr[ 14 ].w.h )
#define SXYP ( cpu->cp2dr[ 15 ].d )
#define SXP  ( cpu->cp2dr[ 15 ].w.l )
#define SYP  ( cpu->cp2dr[ 15 ].w.h )
#define SZ0  ( cpu->cp2dr[ 16 ].w.l )
#define SZ1  ( cpu->cp2dr[ 17 ].w.l )
#define SZ2  ( cpu->cp2dr[ 18 ].w.l )
#define SZ3  ( cpu->cp2dr[ 19 ].w.l )
#define RGB0 ( cpu->cp2dr[ 20 ].d )
#define R0   ( cpu->cp2dr[ 20 ].b.l )
#define G0   ( cpu->cp2dr[ 20 ].b.h )
#define B0   ( cpu->cp2dr[ 20 ].b.h2 )
#define CD0  ( cpu->cp2dr[ 20 ].b.h3 )
#define RGB1 ( cpu->cp2dr[ 21 ].d )
#define R1   ( cpu->cp2dr[ 21 ].b.l )
#define G1   ( cpu->cp2dr[ 21 ].b.h )
#define B1   ( cpu->cp2dr[ 21 ].b.h2 )
#define CD1  ( cpu->cp2dr[ 21 ].b.h3 )
#define RGB2 ( cpu->cp2dr[ 22 ].d )
#define R2   ( cpu->cp2dr[ 22 ].b.l )
#define G2   ( cpu->cp2dr[ 22 ].b.h )
#define B2   ( cpu->cp2dr[ 22 ].b.h2 )
#define CD2  ( cpu->cp2dr[ 22 ].b.h3 )
#define RES1 ( cpu->cp2dr[ 23 ].d )
#define MAC0 ( cpu->cp2dr[ 24 ].d )
#define MAC1 ( cpu->cp2dr[ 25 ].d )
#define MAC2 ( cpu->cp2dr[ 26 ].d )
#define MAC3 ( cpu->cp2dr[ 27 ].d )
#define IRGB ( cpu->cp2dr[ 28 ].d )
#define ORGB ( cpu->cp2dr[ 29 ].d )
#define LZCS ( cpu->cp2dr[ 30 ].d )
#define LZCR ( cpu->cp2dr[ 31 ].d )

#define D1  ( cpu->cp2cr[ 0 ].d )
#define R11 ( cpu->cp2cr[ 0 ].w.l )
#define R12 ( cpu->cp2cr[ 0 ].w.h )
#define R13 ( cpu->cp2cr[ 1 ].w.l )
#define R21 ( cpu->cp2cr[ 1 ].w.h )
#define D2  ( cpu->cp2cr[ 2 ].d )
#define R22 ( cpu->cp2cr[ 2 ].w.l )
#define R23 ( cpu->cp2cr[ 2 ].w.h )
#define R31 ( cpu->cp2cr[ 3 ].w.l )
#define R32 ( cpu->cp2cr[ 3 ].w.h )
#define D3  ( cpu->cp2cr[ 4 ].d )
#define R33 ( cpu->cp2cr[ 4 ].w.l )
#define TRX ( cpu->cp2cr[ 5 ].d )
#define TRY ( cpu->cp2cr[ 6 ].d )
#define TRZ ( cpu->cp2cr[ 7 ].d )
#define L11 ( cpu->cp2cr[ 8 ].w.l )
#define L12 ( cpu->cp2cr[ 8 ].w.h )
#define L13 ( cpu->cp2cr[ 9 ].w.l )
#define L21 ( cpu->cp2cr[ 9 ].w.h )
#define L22 ( cpu->cp2cr[ 10 ].w.l )
#define L23 ( cpu->cp2cr[ 10 ].w.h )
#define L31 ( cpu->cp2cr[ 11 ].w.l )
#define L32 ( cpu->cp2cr[ 11 ].w.h )
#define L33 ( cpu->cp2cr[ 12 ].w.l )
#define RBK ( cpu->cp2cr[ 13 ].d )
#define GBK ( cpu->cp2cr[ 14 ].d )
#define BBK ( cpu->cp2cr[ 15 ].d )
#define LR1 ( cpu->cp2cr[ 16 ].w.l )
#define LR2 ( cpu->cp2cr[ 16 ].w.h )
#define LR3 ( cpu->cp2cr[ 17 ].w.l )
#define LG1 ( cpu->cp2cr[ 17 ].w.h )
#define LG2 ( cpu->cp2cr[ 18 ].w.l )
#define LG3 ( cpu->cp2cr[ 18 ].w.h )
#define LB1 ( cpu->cp2cr[ 19 ].w.l )
#define LB2 ( cpu->cp2cr[ 19 ].w.h )
#define LB3 ( cpu->cp2cr[ 20 ].w.l )
#define RFC ( cpu->cp2cr[ 21 ].d )
#define GFC ( cpu->cp2cr[ 22 ].d )
#define BFC ( cpu->cp2cr[ 23 ].d )
#define OFX ( cpu->cp2cr[ 24 ].d )
#define OFY ( cpu->cp2cr[ 25 ].d )
#define H   ( cpu->cp2cr[ 26 ].w.l )
#define DQA ( cpu->cp2cr[ 27 ].w.l )
#define DQB ( cpu->cp2cr[ 28 ].d )
#define ZSF3 ( cpu->cp2cr[ 29 ].w.l )
#define ZSF4 ( cpu->cp2cr[ 30 ].w.l )
#define FLAG ( cpu->cp2cr[ 31 ].d )

static UINT32 getcp2dr(mips_cpu_context *cpu, int n_reg )
{
	if( n_reg == 1 || n_reg == 3 || n_reg == 5 || n_reg == 8 || n_reg == 9 || n_reg == 10 || n_reg == 11 )
	{
		cpu->cp2dr[ n_reg ].d = (INT32)(INT16)cpu->cp2dr[ n_reg ].d;
	}
	else if( n_reg == 17 || n_reg == 18 || n_reg == 19 )
	{
		cpu->cp2dr[ n_reg ].d = (UINT32)(UINT16)cpu->cp2dr[ n_reg ].d;
	}
	else if( n_reg == 29 )
	{
		ORGB = ( ( IR1 >> 7 ) & 0x1f ) | ( ( IR2 >> 2 ) & 0x3e0 ) | ( ( IR3 << 3 ) & 0x7c00 );
	}
	GTELOG( "get CP2DR%u=%08x", n_reg, cpu->cp2dr[ n_reg ].d );
	return cpu->cp2dr[ n_reg ].d;
}

static void setcp2dr(mips_cpu_context *cpu, int n_reg, UINT32 n_value )
{
	GTELOG( "set CP2DR%u=%08x", n_reg, n_value );
	cpu->cp2dr[ n_reg ].d = n_value;

	if( n_reg == 15 )
	{
		SXY0 = SXY1;
		SXY1 = SXY2;
		SXY2 = SXYP;
	}
	else if( n_reg == 28 )
	{
		IR1 = ( IRGB & 0x1f ) << 4;
		IR2 = ( IRGB & 0x3e0 ) >> 1;
		IR3 = ( IRGB & 0x7c00 ) >> 6;
	}
	else if( n_reg == 30 )
	{
		UINT32 n_lzcs = LZCS;
		UINT32 n_lzcr = 0;

		if( ( n_lzcs & 0x80000000 ) == 0 )
		{
			n_lzcs = ~n_lzcs;
		}
		while( ( n_lzcs & 0x80000000 ) != 0 )
		{
			n_lzcr++;
			n_lzcs <<= 1;
		}
		LZCR = n_lzcr;
	}
}

static UINT32 getcp2cr(mips_cpu_context *cpu, int n_reg )
{
	GTELOG( "get CP2CR%u=%08x", n_reg, cpu->cp2cr[ n_reg ].d );
	return cpu->cp2cr[ n_reg ].d;
}

static void setcp2cr(mips_cpu_context *cpu, int n_reg, UINT32 n_value )
{
	GTELOG( "set CP2CR%u=%08x", n_reg, n_value );
	cpu->cp2cr[ n_reg ].d = n_value;
}

static INLINE INT32 LIM( mips_cpu_context *cpu, INT32 n_value, INT32 n_max, INT32 n_min, UINT32 n_flag )
{
	if( n_value > n_max )
	{
		FLAG |= n_flag;
		return n_max;
	}
	else if( n_value < n_min )
	{
		FLAG |= n_flag;
		return n_min;
	}
	return n_value;
}

static INLINE INT64 BOUNDS( mips_cpu_context *cpu, INT64 n_value, INT64 n_max, int n_maxflag, INT64 n_min, int n_minflag )
{
	if( n_value > n_max )
	{
		FLAG |= n_maxflag;
	}
	else if( n_value < n_min )
	{
		FLAG |= n_minflag;
	}
	return n_value;
}

#define A1( a ) BOUNDS( cpu, ( a ), 0x7fffffff, 30, -(INT64)0x80000000, ( 1 << 27 ) )
#define A2( a ) BOUNDS( cpu, ( a ), 0x7fffffff, 29, -(INT64)0x80000000, ( 1 << 26 ) )
#define A3( a ) BOUNDS( cpu, ( a ), 0x7fffffff, 28, -(INT64)0x80000000, ( 1 << 25 ) )
#define Lm_B1( a, l ) LIM( cpu, ( a ), 0x7fff, -0x8000 * !l, ( 1 << 31 ) | ( 1 << 24 ) )
#define Lm_B2( a, l ) LIM( cpu, ( a ), 0x7fff, -0x8000 * !l, ( 1 << 31 ) | ( 1 << 23 ) )
#define Lm_B3( a, l ) LIM( cpu, ( a ), 0x7fff, -0x8000 * !l, ( 1 << 22 ) )
#define Lm_C1( a ) LIM( cpu, ( a ), 0x00ff, 0x0000, ( 1 << 21 ) )
#define Lm_C2( a ) LIM( cpu, ( a ), 0x00ff, 0x0000, ( 1 << 20 ) )
#define Lm_C3( a ) LIM( cpu, ( a ), 0x00ff, 0x0000, ( 1 << 19 ) )
#define Lm_D( a ) LIM( cpu, ( a ), 0xffff, 0x0000, ( 1 << 31 ) | ( 1 << 18 ) )

static INLINE UINT32 Lm_E( mips_cpu_context *cpu, UINT32 n_z )
{
	if( n_z <= H / 2 )
	{
		n_z = H / 2;
		FLAG |= ( 1 << 31 ) | ( 1 << 17 );
	}
	if( n_z == 0 )
	{
		n_z = 1;
	}
	return n_z;
}

#define F( a ) BOUNDS( cpu, ( a ), 0x7fffffff, ( 1 << 31 ) | ( 1 << 16 ), -(INT64)0x80000000, ( 1 << 31 ) | ( 1 << 15 ) )
#define Lm_G1( a ) LIM( cpu, ( a ), 0x3ff, -0x400, ( 1 << 31 ) | ( 1 << 14 ) )
#define Lm_G2( a ) LIM( cpu, ( a ), 0x3ff, -0x400, ( 1 << 31 ) | ( 1 << 13 ) )
#define Lm_H( a ) LIM( cpu, ( a ), 0xfff, 0x000, ( 1 << 12 ) )

static void docop2(mips_cpu_context *cpu, int gteop )
{
	int n_sf;
	int n_v;
	int n_lm;
	int n_pass;
	UINT16 n_v1;
	UINT16 n_v2;
	UINT16 n_v3;
	UINT16 **p_n_mx;
	UINT32 **p_n_cv;
	UINT16 n_zm = 0;
	UINT32 n_zc = 0;
	UINT16 *p_n_vx[] = { &VX0, &VX1, &VX2 };
	UINT16 *p_n_vy[] = { &VY0, &VY1, &VY2 };
	UINT16 *p_n_vz[] = { &VZ0, &VZ1, &VZ2 };
	UINT16 *p_n_rm[] = { &R11, &R12, &R13, &R21, &R22, &R23, &R31, &R32, &R33 };
	UINT16 *p_n_lm[] = { &L11, &L12, &L13, &L21, &L22, &L23, &L31, &L32, &L33 };
	UINT16 *p_n_cm[] = { &LR1, &LR2, &LR3, &LG1, &LG2, &LG3, &LB1, &LB2, &LB3 };
	UINT16 *p_n_zm[] = { &n_zm, &n_zm, &n_zm, &n_zm, &n_zm, &n_zm, &n_zm, &n_zm, &n_zm };
	UINT16 **p_p_n_mx[] = { p_n_rm, p_n_lm, p_n_cm, p_n_zm };
	UINT32 *p_n_tr[] = { &TRX, &TRY, &TRZ };
	UINT32 *p_n_bk[] = { &RBK, &GBK, &BBK };
	UINT32 *p_n_fc[] = { &RFC, &GFC, &BFC };
	UINT32 *p_n_zc[] = { &n_zc, &n_zc, &n_zc };
	UINT32 **p_p_n_cv[] = { p_n_tr, p_n_bk, p_n_fc, p_n_zc };

	switch( GTE_FUNCT( gteop ) )
	{
	case 0x01:
		if( gteop == 0x0180001 )
		{
			GTELOG( "RTPS" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT32)TRX << 12 ) + ( (INT16)R11 * (INT16)VX0 ) + ( (INT16)R12 * (INT16)VY0 ) + ( (INT16)R13 * (INT16)VZ0 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)(INT32)TRY << 12 ) + ( (INT16)R21 * (INT16)VX0 ) + ( (INT16)R22 * (INT16)VY0 ) + ( (INT16)R23 * (INT16)VZ0 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)(INT32)TRZ << 12 ) + ( (INT16)R31 * (INT16)VX0 ) + ( (INT16)R32 * (INT16)VY0 ) + ( (INT16)R33 * (INT16)VZ0 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 0 );
			IR2 = Lm_B2( (INT32)MAC2, 0 );
			IR3 = Lm_B3( (INT32)MAC3, 0 );
			SZ0 = SZ1;
			SZ1 = SZ2;
			SZ2 = SZ3;
			SZ3 = Lm_D( (INT32)MAC3 );
			SXY0 = SXY1;
			SXY1 = SXY2;
			SX2 = Lm_G1( F( (INT64)(INT32)OFX + ( (INT64)(INT16)IR1 * ( ( (UINT32)H << 16 ) / Lm_E( cpu, SZ3 ) ) ) ) >> 16 );
			SY2 = Lm_G2( F( (INT64)(INT32)OFY + ( (INT64)(INT16)IR2 * ( ( (UINT32)H << 16 ) / Lm_E( cpu, SZ3 ) ) ) ) >> 16 );
			MAC0 = F( (INT64)(INT32)DQB + ( (INT64)(INT16)DQA * ( ( (UINT32)H << 16 ) / Lm_E( cpu, SZ3 ) ) ) );
			IR0 = Lm_H( (INT32)MAC0 >> 12 );
			return;
		}
		break;
	case 0x06:
		if( gteop == 0x0400006 ||
			gteop == 0x1400006 ||
			gteop == 0x0155cc6 )
		{
			GTELOG( "NCLIP" );
			FLAG = 0;

			MAC0 = F( ( (INT64)(INT16)SX0 * (INT16)SY1 ) + ( (INT16)SX1 * (INT16)SY2 ) + ( (INT16)SX2 * (INT16)SY0 ) - ( (INT16)SX0 * (INT16)SY2 ) - ( (INT16)SX1 * (INT16)SY0 ) - ( (INT16)SX2 * (INT16)SY1 ) );
			return;
		}
		break;
	case 0x0c:
		if( GTE_OP( gteop ) == 0x17 )
		{
			GTELOG( "OP" );
			n_sf = 12 * GTE_SF( gteop );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT32)D2 * (INT16)IR3 ) - ( (INT64)(INT32)D3 * (INT16)IR2 ) ) >> n_sf );
			MAC2 = A2( ( ( (INT64)(INT32)D3 * (INT16)IR1 ) - ( (INT64)(INT32)D1 * (INT16)IR3 ) ) >> n_sf );
			MAC3 = A3( ( ( (INT64)(INT32)D1 * (INT16)IR2 ) - ( (INT64)(INT32)D2 * (INT16)IR1 ) ) >> n_sf );
			IR1 = Lm_B1( (INT32)MAC1, 0 );
			IR2 = Lm_B2( (INT32)MAC2, 0 );
			IR3 = Lm_B3( (INT32)MAC3, 0 );
			return;
		}
		break;
	case 0x10:
		if( gteop == 0x0780010 )
		{
			GTELOG( "DPCS" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)R << 16 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)RFC - ( R << 4 ), 0 ) ) ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)G << 16 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)GFC - ( G << 4 ), 0 ) ) ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)B << 16 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)BFC - ( B << 4 ), 0 ) ) ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 0 );
			IR2 = Lm_B2( (INT32)MAC2, 0 );
			IR3 = Lm_B3( (INT32)MAC3, 0 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x11:
		if( gteop == 0x0980011 )
		{
			GTELOG( "INTPL" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT16)IR1 << 12 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)RFC - (INT16)IR1, 0 ) ) ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)(INT16)IR2 << 12 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)GFC - (INT16)IR2, 0 ) ) ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)(INT16)IR3 << 12 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)BFC - (INT16)IR3, 0 ) ) ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 0 );
			IR2 = Lm_B2( (INT32)MAC2, 0 );
			IR3 = Lm_B3( (INT32)MAC3, 0 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 );
			return;
		}
		break;
	case 0x12:
		if( GTE_OP( gteop ) == 0x04 )
		{
			GTELOG( "MVMVA" );
			n_sf = 12 * GTE_SF( gteop );
			p_n_mx = p_p_n_mx[ GTE_MX( gteop ) ];
			n_v = GTE_V( gteop );
			if( n_v < 3 )
			{
				n_v1 = *p_n_vx[ n_v ];
				n_v2 = *p_n_vy[ n_v ];
				n_v3 = *p_n_vz[ n_v ];
			}
			else
			{
				n_v1 = IR1;
				n_v2 = IR2;
				n_v3 = IR3;
			}
			p_n_cv = p_p_n_cv[ GTE_CV( gteop ) ];
			n_lm = GTE_LM( gteop );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT32)*p_n_cv[ 0 ] << 12 ) + ( (INT16)*p_n_mx[ 0 ] * (INT16)n_v1 ) + ( (INT16)*p_n_mx[ 1 ] * (INT16)n_v2 ) + ( (INT16)*p_n_mx[ 2 ] * (INT16)n_v3 ) ) >> n_sf );
			MAC2 = A2( ( ( (INT64)(INT32)*p_n_cv[ 1 ] << 12 ) + ( (INT16)*p_n_mx[ 3 ] * (INT16)n_v1 ) + ( (INT16)*p_n_mx[ 4 ] * (INT16)n_v2 ) + ( (INT16)*p_n_mx[ 5 ] * (INT16)n_v3 ) ) >> n_sf );
			MAC3 = A3( ( ( (INT64)(INT32)*p_n_cv[ 2 ] << 12 ) + ( (INT16)*p_n_mx[ 6 ] * (INT16)n_v1 ) + ( (INT16)*p_n_mx[ 7 ] * (INT16)n_v2 ) + ( (INT16)*p_n_mx[ 8 ] * (INT16)n_v3 ) ) >> n_sf );

			IR1 = Lm_B1( (INT32)MAC1, n_lm );
			IR2 = Lm_B2( (INT32)MAC2, n_lm );
			IR3 = Lm_B3( (INT32)MAC3, n_lm );
			return;
		}
		break;
	case 0x13:
		if( gteop == 0x0e80413 )
		{
			GTELOG( "NCDS" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT16)L11 * (INT16)VX0 ) + ( (INT16)L12 * (INT16)VY0 ) + ( (INT16)L13 * (INT16)VZ0 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)(INT16)L21 * (INT16)VX0 ) + ( (INT16)L22 * (INT16)VY0 ) + ( (INT16)L23 * (INT16)VZ0 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)(INT16)L31 * (INT16)VX0 ) + ( (INT16)L32 * (INT16)VY0 ) + ( (INT16)L33 * (INT16)VZ0 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			MAC1 = A1( ( ( ( (INT64)R << 4 ) * (INT16)IR1 ) + ( (INT16)IR0 * Lm_B1( (INT32)RFC - ( ( R * (INT16)IR1 ) >> 8 ), 0 ) ) ) >> 12 );
			MAC2 = A2( ( ( ( (INT64)G << 4 ) * (INT16)IR2 ) + ( (INT16)IR0 * Lm_B2( (INT32)GFC - ( ( G * (INT16)IR2 ) >> 8 ), 0 ) ) ) >> 12 );
			MAC3 = A3( ( ( ( (INT64)B << 4 ) * (INT16)IR3 ) + ( (INT16)IR0 * Lm_B3( (INT32)BFC - ( ( B * (INT16)IR3 ) >> 8 ), 0 ) ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x14:
		if( gteop == 0x1280414 )
		{
			GTELOG( "CDP" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
			IR1 = Lm_B1( MAC1, 1 );
			IR2 = Lm_B2( MAC2, 1 );
			IR3 = Lm_B3( MAC3, 1 );
			MAC1 = A1( ( ( ( (INT64)R << 4 ) * (INT16)IR1 ) + ( (INT16)IR0 * Lm_B1( (INT32)RFC - ( ( R * (INT16)IR1 ) >> 8 ), 0 ) ) ) >> 12 );
			MAC2 = A2( ( ( ( (INT64)G << 4 ) * (INT16)IR2 ) + ( (INT16)IR0 * Lm_B2( (INT32)GFC - ( ( G * (INT16)IR2 ) >> 8 ), 0 ) ) ) >> 12 );
			MAC3 = A3( ( ( ( (INT64)B << 4 ) * (INT16)IR3 ) + ( (INT16)IR0 * Lm_B3( (INT32)BFC - ( ( B * (INT16)IR3 ) >> 8 ), 0 ) ) ) >> 12 );
			IR1 = Lm_B1( MAC1, 1 );
			IR2 = Lm_B2( MAC2, 1 );
			IR3 = Lm_B3( MAC3, 1 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x16:
		if( gteop == 0x0f80416 )
		{
			GTELOG( "NCDT" );
			FLAG = 0;

			for( n_v = 0; n_v < 3; n_v++ )
			{
				MAC1 = A1( ( ( (INT64)(INT16)L11 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L12 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L13 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)(INT16)L21 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L22 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L23 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)(INT16)L31 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L32 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L33 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				MAC1 = A1( ( ( ( (INT64)R << 4 ) * (INT16)IR1 ) + ( (INT16)IR0 * Lm_B1( (INT32)RFC - ( ( R * (INT16)IR1 ) >> 8 ), 0 ) ) ) >> 12 );
				MAC2 = A2( ( ( ( (INT64)G << 4 ) * (INT16)IR2 ) + ( (INT16)IR0 * Lm_B2( (INT32)GFC - ( ( G * (INT16)IR2 ) >> 8 ), 0 ) ) ) >> 12 );
				MAC3 = A3( ( ( ( (INT64)B << 4 ) * (INT16)IR3 ) + ( (INT16)IR0 * Lm_B3( (INT32)BFC - ( ( B * (INT16)IR3 ) >> 8 ), 0 ) ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				CD0 = CD1;
				CD1 = CD2;
				CD2 = CODE;
				R0 = R1;
				R1 = R2;
				R2 = Lm_C1( (INT32)MAC1 >> 4 );
				G0 = G1;
				G1 = G2;
				G2 = Lm_C2( (INT32)MAC2 >> 4 );
				B0 = B1;
				B1 = B2;
				B2 = Lm_C3( (INT32)MAC3 >> 4 );
			}
			return;
		}
		break;
	case 0x1b:
		if( gteop == 0x108041b )
		{
			GTELOG( "NCCS" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT16)L11 * (INT16)VX0 ) + ( (INT16)L12 * (INT16)VY0 ) + ( (INT16)L13 * (INT16)VZ0 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)(INT16)L21 * (INT16)VX0 ) + ( (INT16)L22 * (INT16)VY0 ) + ( (INT16)L23 * (INT16)VZ0 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)(INT16)L31 * (INT16)VX0 ) + ( (INT16)L32 * (INT16)VY0 ) + ( (INT16)L33 * (INT16)VZ0 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			MAC1 = A1( ( (INT64)R * (INT16)IR1 ) >> 8 );
			MAC2 = A2( ( (INT64)G * (INT16)IR2 ) >> 8 );
			MAC3 = A3( ( (INT64)B * (INT16)IR3 ) >> 8 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x1c:
		if( gteop == 0x138041c )
		{
			GTELOG( "CC" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
			IR1 = Lm_B1( MAC1, 1 );
			IR2 = Lm_B2( MAC2, 1 );
			IR3 = Lm_B3( MAC3, 1 );
			MAC1 = A1( ( (INT64)R * (INT16)IR1 ) >> 8 );
			MAC2 = A2( ( (INT64)G * (INT16)IR2 ) >> 8 );
			MAC3 = A3( ( (INT64)B * (INT16)IR3 ) >> 8 );
			IR1 = Lm_B1( MAC1, 1 );
			IR2 = Lm_B2( MAC2, 1 );
			IR3 = Lm_B3( MAC3, 1 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x1e:
		if( gteop == 0x0c8041e )
		{
			GTELOG( "NCS" );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT16)L11 * (INT16)VX0 ) + ( (INT16)L12 * (INT16)VY0 ) + ( (INT16)L13 * (INT16)VZ0 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)(INT16)L21 * (INT16)VX0 ) + ( (INT16)L22 * (INT16)VY0 ) + ( (INT16)L23 * (INT16)VZ0 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)(INT16)L31 * (INT16)VX0 ) + ( (INT16)L32 * (INT16)VY0 ) + ( (INT16)L33 * (INT16)VZ0 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
			MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
			MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
			IR1 = Lm_B1( (INT32)MAC1, 1 );
			IR2 = Lm_B2( (INT32)MAC2, 1 );
			IR3 = Lm_B3( (INT32)MAC3, 1 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x20:
		if( gteop == 0x0d80420 )
		{
			GTELOG( "NCT" );
			FLAG = 0;

			for( n_v = 0; n_v < 3; n_v++ )
			{
				MAC1 = A1( ( ( (INT64)(INT16)L11 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L12 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L13 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)(INT16)L21 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L22 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L23 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)(INT16)L31 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L32 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L33 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				CD0 = CD1;
				CD1 = CD2;
				CD2 = CODE;
				R0 = R1;
				R1 = R2;
				R2 = Lm_C1( (INT32)MAC1 >> 4 );
				G0 = G1;
				G1 = G2;
				G2 = Lm_C2( (INT32)MAC2 >> 4 );
				B0 = B1;
				B1 = B2;
				B2 = Lm_C3( (INT32)MAC3 >> 4 );
			}
			return;
		}
		break;
	case 0x28:
		if( GTE_OP( gteop ) == 0x0a && GTE_LM( gteop ) == 1 )
		{
			GTELOG( "SQR" );
			n_sf = 12 * GTE_SF( gteop );
			FLAG = 0;

			MAC1 = A1( ( (INT64)(INT16)IR1 * (INT16)IR1 ) >> n_sf );
			MAC2 = A2( ( (INT64)(INT16)IR2 * (INT16)IR2 ) >> n_sf );
			MAC3 = A3( ( (INT64)(INT16)IR3 * (INT16)IR3 ) >> n_sf );
			IR1 = Lm_B1( MAC1, 1 );
			IR2 = Lm_B2( MAC2, 1 );
			IR3 = Lm_B3( MAC3, 1 );
			return;
		}
		break;
	// DCPL 0x29
	case 0x2a:
		if( gteop == 0x0f8002a )
		{
			GTELOG( "DPCT" );
			FLAG = 0;

			for( n_pass = 0; n_pass < 3; n_pass++ )
			{
				MAC1 = A1( ( ( (INT64)R0 << 16 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)RFC - ( R0 << 4 ), 0 ) ) ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)G0 << 16 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)GFC - ( G0 << 4 ), 0 ) ) ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)B0 << 16 ) + ( (INT64)(INT16)IR0 * ( Lm_B1( (INT32)BFC - ( B0 << 4 ), 0 ) ) ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 0 );
				IR2 = Lm_B2( (INT32)MAC2, 0 );
				IR3 = Lm_B3( (INT32)MAC3, 0 );
				CD0 = CD1;
				CD1 = CD2;
				CD2 = CODE;
				R0 = R1;
				R1 = R2;
				R2 = Lm_C1( (INT32)MAC1 >> 4 );
				G0 = G1;
				G1 = G2;
				G2 = Lm_C2( (INT32)MAC2 >> 4 );
				B0 = B1;
				B1 = B2;
				B2 = Lm_C3( (INT32)MAC3 >> 4 );
			}
			return;
		}
		break;
	case 0x2d:
		if( gteop == 0x158002d )
		{
			GTELOG( "AVSZ3" );
			FLAG = 0;

			MAC0 = F( ( (INT64)(INT16)ZSF3 * SZ1 ) + ( (INT16)ZSF3 * SZ2 ) + ( (INT16)ZSF3 * SZ3 ) );
			OTZ = Lm_D( (INT32)MAC0 >> 12 );
			return;
		}
		break;
	case 0x2e:
		if( gteop == 0x168002e )
		{
			GTELOG( "AVSZ4" );
			FLAG = 0;

			MAC0 = F( ( (INT64)(INT16)ZSF4 * SZ0 ) + ( (INT16)ZSF4 * SZ1 ) + ( (INT16)ZSF4 * SZ2 ) + ( (INT16)ZSF4 * SZ3 ) );
			OTZ = Lm_D( (INT32)MAC0 >> 12 );
			return;
		}
		break;
	case 0x30:
		if( gteop == 0x0280030 )
		{
			GTELOG( "RTPT" );
			FLAG = 0;

			for( n_v = 0; n_v < 3; n_v++ )
			{
				MAC1 = A1( ( ( (INT64)(INT32)TRX << 12 ) + ( (INT16)R11 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)R12 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)R13 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)(INT32)TRY << 12 ) + ( (INT16)R21 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)R22 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)R23 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)(INT32)TRZ << 12 ) + ( (INT16)R31 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)R32 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)R33 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 0 );
				IR2 = Lm_B2( (INT32)MAC2, 0 );
				IR3 = Lm_B3( (INT32)MAC3, 0 );
				SZ0 = SZ1;
				SZ1 = SZ2;
				SZ2 = SZ3;
				SZ3 = Lm_D( (INT32)MAC3 );
				SXY0 = SXY1;
				SXY1 = SXY2;
				SX2 = Lm_G1( F( ( (INT64)(INT32)OFX + ( (INT64)(INT16)IR1 * ( ( (UINT32)H << 16 ) / Lm_E( cpu, SZ3 ) ) ) ) >> 16 ) );
				SY2 = Lm_G2( F( ( (INT64)(INT32)OFY + ( (INT64)(INT16)IR2 * ( ( (UINT32)H << 16 ) / Lm_E( cpu, SZ3 ) ) ) ) >> 16 ) );
				MAC0 = F( (INT64)(INT32)DQB + ( (INT64)(INT16)DQA * ( ( (UINT32)H << 16 ) / Lm_E( cpu, SZ3 ) ) ) );
				IR0 = Lm_H( (INT32)MAC0 >> 12 );
			}
			return;
		}
		break;
	case 0x3d:
		if( GTE_OP( gteop ) == 0x09 ||
			GTE_OP( gteop ) == 0x19 )
		{
			GTELOG( "GPF" );
			n_sf = 12 * GTE_SF( gteop );
			FLAG = 0;

			MAC1 = A1( ( (INT64)(INT16)IR0 * (INT16)IR1 ) >> n_sf );
			MAC2 = A2( ( (INT64)(INT16)IR0 * (INT16)IR2 ) >> n_sf );
			MAC3 = A3( ( (INT64)(INT16)IR0 * (INT16)IR3 ) >> n_sf );
			IR1 = Lm_B1( (INT32)MAC1, 0 );
			IR2 = Lm_B2( (INT32)MAC2, 0 );
			IR3 = Lm_B3( (INT32)MAC3, 0 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x3e:
		if( GTE_OP( gteop ) == 0x1a )
		{
			GTELOG( "GPL" );
			n_sf = 12 * GTE_SF( gteop );
			FLAG = 0;

			MAC1 = A1( ( ( (INT64)(INT32)MAC1 << n_sf ) + ( (INT16)IR0 * (INT16)IR1 ) ) >> n_sf );
			MAC2 = A2( ( ( (INT64)(INT32)MAC2 << n_sf ) + ( (INT16)IR0 * (INT16)IR2 ) ) >> n_sf );
			MAC3 = A3( ( ( (INT64)(INT32)MAC3 << n_sf ) + ( (INT16)IR0 * (INT16)IR3 ) ) >> n_sf );
			IR1 = Lm_B1( (INT32)MAC1, 0 );
			IR2 = Lm_B2( (INT32)MAC2, 0 );
			IR3 = Lm_B3( (INT32)MAC3, 0 );
			CD0 = CD1;
			CD1 = CD2;
			CD2 = CODE;
			R0 = R1;
			R1 = R2;
			R2 = Lm_C1( (INT32)MAC1 >> 4 );
			G0 = G1;
			G1 = G2;
			G2 = Lm_C2( (INT32)MAC2 >> 4 );
			B0 = B1;
			B1 = B2;
			B2 = Lm_C3( (INT32)MAC3 >> 4 );
			return;
		}
		break;
	case 0x3f:
		if( gteop == 0x108043f ||
			gteop == 0x118043f )
		{
			GTELOG( "NCCT" );
			FLAG = 0;

			for( n_v = 0; n_v < 3; n_v++ )
			{
				MAC1 = A1( ( ( (INT64)(INT16)L11 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L12 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L13 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)(INT16)L21 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L22 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L23 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)(INT16)L31 * (INT16)*p_n_vx[ n_v ] ) + ( (INT16)L32 * (INT16)*p_n_vy[ n_v ] ) + ( (INT16)L33 * (INT16)*p_n_vz[ n_v ] ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				MAC1 = A1( ( ( (INT64)RBK << 12 ) + ( (INT16)LR1 * (INT16)IR1 ) + ( (INT16)LR2 * (INT16)IR2 ) + ( (INT16)LR3 * (INT16)IR3 ) ) >> 12 );
				MAC2 = A2( ( ( (INT64)GBK << 12 ) + ( (INT16)LG1 * (INT16)IR1 ) + ( (INT16)LG2 * (INT16)IR2 ) + ( (INT16)LG3 * (INT16)IR3 ) ) >> 12 );
				MAC3 = A3( ( ( (INT64)BBK << 12 ) + ( (INT16)LB1 * (INT16)IR1 ) + ( (INT16)LB2 * (INT16)IR2 ) + ( (INT16)LB3 * (INT16)IR3 ) ) >> 12 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				MAC1 = A1( ( (INT64)R * (INT16)IR1 ) >> 8 );
				MAC2 = A2( ( (INT64)G * (INT16)IR2 ) >> 8 );
				MAC3 = A3( ( (INT64)B * (INT16)IR3 ) >> 8 );
				IR1 = Lm_B1( (INT32)MAC1, 1 );
				IR2 = Lm_B2( (INT32)MAC2, 1 );
				IR3 = Lm_B3( (INT32)MAC3, 1 );
				CD0 = CD1;
				CD1 = CD2;
				CD2 = CODE;
				R0 = R1;
				R1 = R2;
				R2 = Lm_C1( (INT32)MAC1 >> 4 );
				G0 = G1;
				G1 = G2;
				G2 = Lm_C2( (INT32)MAC2 >> 4 );
				B0 = B1;
				B1 = B2;
				B2 = Lm_C3( (INT32)MAC3 >> 4 );
			}
			return;
		}
		break;
	}
//	usrintf_showmessage_secs( 1, "unknown GTE op %08x", gteop );
	logerror( "%08x: unknown GTE op %08x\n", cpu->pc, gteop );
	mips_stop();
}

/**************************************************************************
 * Generic set_info
 **************************************************************************/

void mips_set_info(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info)
{
	switch (state)
	{
		/* --- the following bits of info are set as 64-bit signed integers --- */
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ0:		set_irq_line(cpu, MIPS_IRQ0, info->i);		break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ1:		set_irq_line(cpu, MIPS_IRQ1, info->i);		break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ2:		set_irq_line(cpu, MIPS_IRQ2, info->i);		break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ3:		set_irq_line(cpu, MIPS_IRQ3, info->i);		break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ4:		set_irq_line(cpu, MIPS_IRQ4, info->i);		break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ5:		set_irq_line(cpu, MIPS_IRQ5, info->i);		break;

		case CPUINFO_INT_PC:							mips_set_pc (cpu,  info->i );					break;
		case CPUINFO_INT_REGISTER + MIPS_PC:			mips_set_pc (cpu,  info->i );					break;
		case CPUINFO_INT_SP:							/* no stack */							break;
		case CPUINFO_INT_REGISTER + MIPS_DELAYV:		cpu->delayv = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_DELAYR:		if( info->i <= REGPC ) cpu->delayr = info->i; break;
		case CPUINFO_INT_REGISTER + MIPS_HI:			cpu->hi = info->i;					break;
		case CPUINFO_INT_REGISTER + MIPS_LO:			cpu->lo = info->i;					break;
		case CPUINFO_INT_REGISTER + MIPS_R0:			cpu->r[ 0 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R1:			cpu->r[ 1 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R2:			cpu->r[ 2 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R3:			cpu->r[ 3 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R4:			cpu->r[ 4 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R5:			cpu->r[ 5 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R6:			cpu->r[ 6 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R7:			cpu->r[ 7 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R8:			cpu->r[ 8 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R9:			cpu->r[ 9 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R10:			cpu->r[ 10 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R11:			cpu->r[ 11 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R12:			cpu->r[ 12 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R13:			cpu->r[ 13 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R14:			cpu->r[ 14 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R15:			cpu->r[ 15 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R16:			cpu->r[ 16 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R17:			cpu->r[ 17 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R18:			cpu->r[ 18 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R19:			cpu->r[ 19 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R20:			cpu->r[ 20 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R21:			cpu->r[ 21 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R22:			cpu->r[ 22 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R23:			cpu->r[ 23 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R24:			cpu->r[ 24 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R25:			cpu->r[ 25 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R26:			cpu->r[ 26 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R27:			cpu->r[ 27 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R28:			cpu->r[ 28 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R29:			cpu->r[ 29 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R30:			cpu->r[ 30 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_R31:			cpu->r[ 31 ] = info->i;				break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R0:			mips_set_cp0r(cpu, 0, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R1:			mips_set_cp0r(cpu, 1, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R2:			mips_set_cp0r(cpu, 2, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R3:			mips_set_cp0r(cpu, 3, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R4:			mips_set_cp0r(cpu, 4, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R5:			mips_set_cp0r(cpu, 5, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R6:			mips_set_cp0r(cpu, 6, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R7:			mips_set_cp0r(cpu, 7, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R8:			mips_set_cp0r(cpu, 8, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R9:			mips_set_cp0r(cpu, 9, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R10:		mips_set_cp0r(cpu, 10, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R11:		mips_set_cp0r(cpu, 11, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R12:		mips_set_cp0r(cpu, 12, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R13:		mips_set_cp0r(cpu, 13, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R14:		mips_set_cp0r(cpu, 14, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R15:		mips_set_cp0r(cpu, 15, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R16:		mips_set_cp0r(cpu, 16, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R17:		mips_set_cp0r(cpu, 17, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R18:		mips_set_cp0r(cpu, 18, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R19:		mips_set_cp0r(cpu, 19, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R20:		mips_set_cp0r(cpu, 20, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R21:		mips_set_cp0r(cpu, 21, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R22:		mips_set_cp0r(cpu, 22, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R23:		mips_set_cp0r(cpu, 23, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R24:		mips_set_cp0r(cpu, 24, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R25:		mips_set_cp0r(cpu, 25, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R26:		mips_set_cp0r(cpu, 26, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R27:		mips_set_cp0r(cpu, 27, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R28:		mips_set_cp0r(cpu, 28, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R29:		mips_set_cp0r(cpu, 29, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R30:		mips_set_cp0r(cpu, 30, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R31:		mips_set_cp0r(cpu, 31, info->i );			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR0:		cpu->cp2dr[ 0 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR1:		cpu->cp2dr[ 1 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR2:		cpu->cp2dr[ 2 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR3:		cpu->cp2dr[ 3 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR4:		cpu->cp2dr[ 4 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR5:		cpu->cp2dr[ 5 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR6:		cpu->cp2dr[ 6 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR7:		cpu->cp2dr[ 7 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR8:		cpu->cp2dr[ 8 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR9:		cpu->cp2dr[ 9 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR10:		cpu->cp2dr[ 10 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR11:		cpu->cp2dr[ 11 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR12:		cpu->cp2dr[ 12 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR13:		cpu->cp2dr[ 13 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR14:		cpu->cp2dr[ 14 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR15:		cpu->cp2dr[ 15 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR16:		cpu->cp2dr[ 16 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR17:		cpu->cp2dr[ 17 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR18:		cpu->cp2dr[ 18 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR19:		cpu->cp2dr[ 19 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR20:		cpu->cp2dr[ 20 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR21:		cpu->cp2dr[ 21 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR22:		cpu->cp2dr[ 22 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR23:		cpu->cp2dr[ 23 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR24:		cpu->cp2dr[ 24 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR25:		cpu->cp2dr[ 25 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR26:		cpu->cp2dr[ 26 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR27:		cpu->cp2dr[ 27 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR28:		cpu->cp2dr[ 28 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR29:		cpu->cp2dr[ 29 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR30:		cpu->cp2dr[ 30 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR31:		cpu->cp2dr[ 31 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR0:		cpu->cp2cr[ 0 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR1:		cpu->cp2cr[ 1 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR2:		cpu->cp2cr[ 2 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR3:		cpu->cp2cr[ 3 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR4:		cpu->cp2cr[ 4 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR5:		cpu->cp2cr[ 5 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR6:		cpu->cp2cr[ 6 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR7:		cpu->cp2cr[ 7 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR8:		cpu->cp2cr[ 8 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR9:		cpu->cp2cr[ 9 ].d = info->i;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR10:		cpu->cp2cr[ 10 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR11:		cpu->cp2cr[ 11 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR12:		cpu->cp2cr[ 12 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR13:		cpu->cp2cr[ 13 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR14:		cpu->cp2cr[ 14 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR15:		cpu->cp2cr[ 15 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR16:		cpu->cp2cr[ 16 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR17:		cpu->cp2cr[ 17 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR18:		cpu->cp2cr[ 18 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR19:		cpu->cp2cr[ 19 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR20:		cpu->cp2cr[ 20 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR21:		cpu->cp2cr[ 21 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR22:		cpu->cp2cr[ 22 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR23:		cpu->cp2cr[ 23 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR24:		cpu->cp2cr[ 24 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR25:		cpu->cp2cr[ 25 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR26:		cpu->cp2cr[ 26 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR27:		cpu->cp2cr[ 27 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR28:		cpu->cp2cr[ 28 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR29:		cpu->cp2cr[ 29 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR30:		cpu->cp2cr[ 30 ].d = info->i;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR31:		cpu->cp2cr[ 31 ].d = info->i;		break;
		
		/* --- the following bits of info are set as pointers to data or functions --- */
		case CPUINFO_PTR_IRQ_CALLBACK:					cpu->irq_callback = info->irqcallback;			break;
	}
}



/**************************************************************************
 * Generic get_info
 **************************************************************************/

void mips_get_info(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info)
{
	switch (state)
	{
		/* --- the following bits of info are returned as 64-bit signed integers --- */
		case CPUINFO_INT_CONTEXT_SIZE:					info->i = sizeof(mips_cpu_context);				break;
		case CPUINFO_INT_INPUT_LINES:					info->i = 6;							break;
		case CPUINFO_INT_DEFAULT_IRQ_VECTOR:			info->i = 0;							break;
		case CPUINFO_INT_ENDIANNESS:					info->i = CPU_IS_LE;					break;
		case CPUINFO_INT_CLOCK_DIVIDER:					info->i = 1;							break;
		case CPUINFO_INT_MIN_INSTRUCTION_BYTES:			info->i = 4;							break;
		case CPUINFO_INT_MAX_INSTRUCTION_BYTES:			info->i = 4;							break;
		case CPUINFO_INT_MIN_CYCLES:					info->i = 1;							break;
		case CPUINFO_INT_MAX_CYCLES:					info->i = 40;							break;
		
		case CPUINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_PROGRAM:	info->i = 32;					break;
		case CPUINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_PROGRAM: info->i = 32;					break;
		case CPUINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_PROGRAM: info->i = 0;					break;
		case CPUINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_DATA:	info->i = 0;					break;
		case CPUINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_DATA: 	info->i = 0;					break;
		case CPUINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_DATA: 	info->i = 0;					break;
		case CPUINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_IO:		info->i = 0;					break;
		case CPUINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_IO: 		info->i = 0;					break;
		case CPUINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_IO: 		info->i = 0;					break;

		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ0:		info->i = (cpu->cp0r[ CP0_CAUSE ] & 0x400) ? ASSERT_LINE : CLEAR_LINE; break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ1:		info->i = (cpu->cp0r[ CP0_CAUSE ] & 0x800) ? ASSERT_LINE : CLEAR_LINE; break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ2:		info->i = (cpu->cp0r[ CP0_CAUSE ] & 0x1000) ? ASSERT_LINE : CLEAR_LINE; break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ3:		info->i = (cpu->cp0r[ CP0_CAUSE ] & 0x2000) ? ASSERT_LINE : CLEAR_LINE; break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ4:		info->i = (cpu->cp0r[ CP0_CAUSE ] & 0x4000) ? ASSERT_LINE : CLEAR_LINE; break;
		case CPUINFO_INT_INPUT_STATE + MIPS_IRQ5:		info->i = (cpu->cp0r[ CP0_CAUSE ] & 0x8000) ? ASSERT_LINE : CLEAR_LINE; break;

		case CPUINFO_INT_PREVIOUSPC:					/* not implemented */					break;

		case CPUINFO_INT_PC:							info->i = cpu->pc;					break;
		case CPUINFO_INT_REGISTER + MIPS_PC:			info->i = cpu->pc;					break;
		case CPUINFO_INT_SP:
			/* because there is no hardware stack and the pipeline causes the cpu to execute the
			instruction after a subroutine call before the subroutine is executed there is little
			chance of cmd_step_over() in mamedbg.c working. */
								info->i = 0;													break;
		case CPUINFO_INT_REGISTER + MIPS_DELAYV:		info->i = cpu->delayv;				break;
		case CPUINFO_INT_REGISTER + MIPS_DELAYR:		info->i = cpu->delayr;				break;
		case CPUINFO_INT_REGISTER + MIPS_HI:			info->i = cpu->hi;					break;
		case CPUINFO_INT_REGISTER + MIPS_LO:			info->i = cpu->lo;					break;
		case CPUINFO_INT_REGISTER + MIPS_R0:			info->i = cpu->r[ 0 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R1:			info->i = cpu->r[ 1 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R2:			info->i = cpu->r[ 2 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R3:			info->i = cpu->r[ 3 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R4:			info->i = cpu->r[ 4 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R5:			info->i = cpu->r[ 5 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R6:			info->i = cpu->r[ 6 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R7:			info->i = cpu->r[ 7 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R8:			info->i = cpu->r[ 8 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R9:			info->i = cpu->r[ 9 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R10:			info->i = cpu->r[ 10 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R11:			info->i = cpu->r[ 11 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R12:			info->i = cpu->r[ 12 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R13:			info->i = cpu->r[ 13 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R14:			info->i = cpu->r[ 14 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R15:			info->i = cpu->r[ 15 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R16:			info->i = cpu->r[ 16 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R17:			info->i = cpu->r[ 17 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R18:			info->i = cpu->r[ 18 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R19:			info->i = cpu->r[ 19 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R20:			info->i = cpu->r[ 20 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R21:			info->i = cpu->r[ 21 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R22:			info->i = cpu->r[ 22 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R23:			info->i = cpu->r[ 23 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R24:			info->i = cpu->r[ 24 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R25:			info->i = cpu->r[ 25 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R26:			info->i = cpu->r[ 26 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R27:			info->i = cpu->r[ 27 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R28:			info->i = cpu->r[ 28 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R29:			info->i = cpu->r[ 29 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R30:			info->i = cpu->r[ 30 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_R31:			info->i = cpu->r[ 31 ];				break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R0:			info->i = cpu->cp0r[ 0 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R1:			info->i = cpu->cp0r[ 1 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R2:			info->i = cpu->cp0r[ 2 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R3:			info->i = cpu->cp0r[ 3 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R4:			info->i = cpu->cp0r[ 4 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R5:			info->i = cpu->cp0r[ 5 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R6:			info->i = cpu->cp0r[ 6 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R7:			info->i = cpu->cp0r[ 7 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R8:			info->i = cpu->cp0r[ 8 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R9:			info->i = cpu->cp0r[ 9 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R10:		info->i = cpu->cp0r[ 10 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R11:		info->i = cpu->cp0r[ 11 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R12:		info->i = cpu->cp0r[ 12 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R13:		info->i = cpu->cp0r[ 13 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R14:		info->i = cpu->cp0r[ 14 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R15:		info->i = cpu->cp0r[ 15 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R16:		info->i = cpu->cp0r[ 16 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R17:		info->i = cpu->cp0r[ 17 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R18:		info->i = cpu->cp0r[ 18 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R19:		info->i = cpu->cp0r[ 19 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R20:		info->i = cpu->cp0r[ 20 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R21:		info->i = cpu->cp0r[ 21 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R22:		info->i = cpu->cp0r[ 22 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R23:		info->i = cpu->cp0r[ 23 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R24:		info->i = cpu->cp0r[ 24 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R25:		info->i = cpu->cp0r[ 25 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R26:		info->i = cpu->cp0r[ 26 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R27:		info->i = cpu->cp0r[ 27 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R28:		info->i = cpu->cp0r[ 28 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R29:		info->i = cpu->cp0r[ 29 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R30:		info->i = cpu->cp0r[ 30 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP0R31:		info->i = cpu->cp0r[ 31 ];			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR0:		info->i = cpu->cp2dr[ 0 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR1:		info->i = cpu->cp2dr[ 1 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR2:		info->i = cpu->cp2dr[ 2 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR3:		info->i = cpu->cp2dr[ 3 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR4:		info->i = cpu->cp2dr[ 4 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR5:		info->i = cpu->cp2dr[ 5 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR6:		info->i = cpu->cp2dr[ 6 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR7:		info->i = cpu->cp2dr[ 7 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR8:		info->i = cpu->cp2dr[ 8 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR9:		info->i = cpu->cp2dr[ 9 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR10:		info->i = cpu->cp2dr[ 10 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR11:		info->i = cpu->cp2dr[ 11 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR12:		info->i = cpu->cp2dr[ 12 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR13:		info->i = cpu->cp2dr[ 13 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR14:		info->i = cpu->cp2dr[ 14 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR15:		info->i = cpu->cp2dr[ 15 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR16:		info->i = cpu->cp2dr[ 16 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR17:		info->i = cpu->cp2dr[ 17 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR18:		info->i = cpu->cp2dr[ 18 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR19:		info->i = cpu->cp2dr[ 19 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR20:		info->i = cpu->cp2dr[ 20 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR21:		info->i = cpu->cp2dr[ 21 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR22:		info->i = cpu->cp2dr[ 22 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR23:		info->i = cpu->cp2dr[ 23 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR24:		info->i = cpu->cp2dr[ 24 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR25:		info->i = cpu->cp2dr[ 25 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR26:		info->i = cpu->cp2dr[ 26 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR27:		info->i = cpu->cp2dr[ 27 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR28:		info->i = cpu->cp2dr[ 28 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR29:		info->i = cpu->cp2dr[ 29 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR30:		info->i = cpu->cp2dr[ 30 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2DR31:		info->i = cpu->cp2dr[ 31 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR0:		info->i = cpu->cp2cr[ 0 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR1:		info->i = cpu->cp2cr[ 1 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR2:		info->i = cpu->cp2cr[ 2 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR3:		info->i = cpu->cp2cr[ 3 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR4:		info->i = cpu->cp2cr[ 4 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR5:		info->i = cpu->cp2cr[ 5 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR6:		info->i = cpu->cp2cr[ 6 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR7:		info->i = cpu->cp2cr[ 7 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR8:		info->i = cpu->cp2cr[ 8 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR9:		info->i = cpu->cp2cr[ 9 ].d;			break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR10:		info->i = cpu->cp2cr[ 10 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR11:		info->i = cpu->cp2cr[ 11 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR12:		info->i = cpu->cp2cr[ 12 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR13:		info->i = cpu->cp2cr[ 13 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR14:		info->i = cpu->cp2cr[ 14 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR15:		info->i = cpu->cp2cr[ 15 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR16:		info->i = cpu->cp2cr[ 16 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR17:		info->i = cpu->cp2cr[ 17 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR18:		info->i = cpu->cp2cr[ 18 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR19:		info->i = cpu->cp2cr[ 19 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR20:		info->i = cpu->cp2cr[ 20 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR21:		info->i = cpu->cp2cr[ 21 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR22:		info->i = cpu->cp2cr[ 22 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR23:		info->i = cpu->cp2cr[ 23 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR24:		info->i = cpu->cp2cr[ 24 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR25:		info->i = cpu->cp2cr[ 25 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR26:		info->i = cpu->cp2cr[ 26 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR27:		info->i = cpu->cp2cr[ 27 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR28:		info->i = cpu->cp2cr[ 28 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR29:		info->i = cpu->cp2cr[ 29 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR30:		info->i = cpu->cp2cr[ 30 ].d;		break;
		case CPUINFO_INT_REGISTER + MIPS_CP2CR31:		info->i = cpu->cp2cr[ 31 ].d;		break;

		/* --- the following bits of info are returned as pointers to data or functions --- */
		case CPUINFO_PTR_SET_INFO:						info->setinfo = mips_set_info;			break;
//		case CPUINFO_PTR_GET_CONTEXT:					info->getcontext = mips_get_context;	break;
//		case CPUINFO_PTR_SET_CONTEXT:					info->setcontext = mips_set_context;	break;
		case CPUINFO_PTR_INIT:							info->init = mips_init;					break;
		case CPUINFO_PTR_RESET:							info->reset = mips_reset;				break;
		case CPUINFO_PTR_EXIT:							info->exit = mips_exit;					break;
		case CPUINFO_PTR_EXECUTE:						info->execute = mips_execute;			break;
		case CPUINFO_PTR_BURN:							info->burn = NULL;						break;
		case CPUINFO_PTR_DISASSEMBLE:					info->disassemble = mips_dasm;			break;
		case CPUINFO_PTR_IRQ_CALLBACK:					info->irqcallback = cpu->irq_callback; break;
		case CPUINFO_PTR_INSTRUCTION_COUNTER:			info->icount = &cpu->mips_ICount;			break;
		case CPUINFO_PTR_REGISTER_LAYOUT:				info->p = mips_reg_layout;				break;
		case CPUINFO_PTR_WINDOW_LAYOUT:					info->p = mips_win_layout;				break;

		/* --- the following bits of info are returned as NULL-terminated strings --- */
#if 0
		case CPUINFO_STR_NAME:							strcpy(info->s = cpuintrf_temp_str(), "PSX CPU"); break;
		case CPUINFO_STR_CORE_FAMILY:					strcpy(info->s = cpuintrf_temp_str(), "mipscpu"); break;
		case CPUINFO_STR_CORE_VERSION:					strcpy(info->s = cpuintrf_temp_str(), "1.4"); break;
		case CPUINFO_STR_CORE_FILE:						strcpy(info->s = cpuintrf_temp_str(), __FILE__); break;
		case CPUINFO_STR_CORE_CREDITS:					strcpy(info->s = cpuintrf_temp_str(), "Copyright 2003 smf"); break;

		case CPUINFO_STR_FLAGS:							strcpy(info->s = cpuintrf_temp_str(), " "); break;

		case CPUINFO_STR_REGISTER + MIPS_PC:			sprintf( info->s = cpuintrf_temp_str(), "pc      :%08x", cpu->pc ); break;
		case CPUINFO_STR_REGISTER + MIPS_DELAYV:		sprintf( info->s = cpuintrf_temp_str(), "delay   :%08x", cpu->delayv ); break;
		case CPUINFO_STR_REGISTER + MIPS_DELAYR:		sprintf( info->s = cpuintrf_temp_str(), "delay %s:%02x", delayn[ cpu->delayr ], cpu->delayr ); break;
		case CPUINFO_STR_REGISTER + MIPS_HI:			sprintf( info->s = cpuintrf_temp_str(), "hi      :%08x", cpu->hi ); break;
		case CPUINFO_STR_REGISTER + MIPS_LO:			sprintf( info->s = cpuintrf_temp_str(), "lo      :%08x", cpu->lo ); break;
		case CPUINFO_STR_REGISTER + MIPS_R0:			sprintf( info->s = cpuintrf_temp_str(), "zero    :%08x", cpu->r[ 0 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R1:			sprintf( info->s = cpuintrf_temp_str(), "at      :%08x", cpu->r[ 1 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R2:			sprintf( info->s = cpuintrf_temp_str(), "v0      :%08x", cpu->r[ 2 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R3:			sprintf( info->s = cpuintrf_temp_str(), "v1      :%08x", cpu->r[ 3 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R4:			sprintf( info->s = cpuintrf_temp_str(), "a0      :%08x", cpu->r[ 4 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R5:			sprintf( info->s = cpuintrf_temp_str(), "a1      :%08x", cpu->r[ 5 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R6:			sprintf( info->s = cpuintrf_temp_str(), "a2      :%08x", cpu->r[ 6 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R7:			sprintf( info->s = cpuintrf_temp_str(), "a3      :%08x", cpu->r[ 7 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R8:			sprintf( info->s = cpuintrf_temp_str(), "t0      :%08x", cpu->r[ 8 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R9:			sprintf( info->s = cpuintrf_temp_str(), "t1      :%08x", cpu->r[ 9 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R10:			sprintf( info->s = cpuintrf_temp_str(), "t2      :%08x", cpu->r[ 10 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R11:			sprintf( info->s = cpuintrf_temp_str(), "t3      :%08x", cpu->r[ 11 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R12:			sprintf( info->s = cpuintrf_temp_str(), "t4      :%08x", cpu->r[ 12 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R13:			sprintf( info->s = cpuintrf_temp_str(), "t5      :%08x", cpu->r[ 13 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R14:			sprintf( info->s = cpuintrf_temp_str(), "t6      :%08x", cpu->r[ 14 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R15:			sprintf( info->s = cpuintrf_temp_str(), "t7      :%08x", cpu->r[ 15 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R16:			sprintf( info->s = cpuintrf_temp_str(), "s0      :%08x", cpu->r[ 16 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R17:			sprintf( info->s = cpuintrf_temp_str(), "s1      :%08x", cpu->r[ 17 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R18:			sprintf( info->s = cpuintrf_temp_str(), "s2      :%08x", cpu->r[ 18 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R19:			sprintf( info->s = cpuintrf_temp_str(), "s3      :%08x", cpu->r[ 19 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R20:			sprintf( info->s = cpuintrf_temp_str(), "s4      :%08x", cpu->r[ 20 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R21:			sprintf( info->s = cpuintrf_temp_str(), "s5      :%08x", cpu->r[ 21 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R22:			sprintf( info->s = cpuintrf_temp_str(), "s6      :%08x", cpu->r[ 22 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R23:			sprintf( info->s = cpuintrf_temp_str(), "s7      :%08x", cpu->r[ 23 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R24:			sprintf( info->s = cpuintrf_temp_str(), "t8      :%08x", cpu->r[ 24 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R25:			sprintf( info->s = cpuintrf_temp_str(), "t9      :%08x", cpu->r[ 25 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R26:			sprintf( info->s = cpuintrf_temp_str(), "k0      :%08x", cpu->r[ 26 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R27:			sprintf( info->s = cpuintrf_temp_str(), "k1      :%08x", cpu->r[ 27 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R28:			sprintf( info->s = cpuintrf_temp_str(), "gp      :%08x", cpu->r[ 28 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R29:			sprintf( info->s = cpuintrf_temp_str(), "sp      :%08x", cpu->r[ 29 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R30:			sprintf( info->s = cpuintrf_temp_str(), "fp      :%08x", cpu->r[ 30 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_R31:			sprintf( info->s = cpuintrf_temp_str(), "ra      :%08x", cpu->r[ 31 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R0:			sprintf( info->s = cpuintrf_temp_str(), "Index   :%08x", cpu->cp0r[ 0 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R1:			sprintf( info->s = cpuintrf_temp_str(), "Random  :%08x", cpu->cp0r[ 1 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R2:			sprintf( info->s = cpuintrf_temp_str(), "EntryLo :%08x", cpu->cp0r[ 2 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R3:			sprintf( info->s = cpuintrf_temp_str(), "cp0r3   :%08x", cpu->cp0r[ 3 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R4:			sprintf( info->s = cpuintrf_temp_str(), "Context :%08x", cpu->cp0r[ 4 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R5:			sprintf( info->s = cpuintrf_temp_str(), "cp0r5   :%08x", cpu->cp0r[ 5 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R6:			sprintf( info->s = cpuintrf_temp_str(), "cp0r6   :%08x", cpu->cp0r[ 6 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R7:			sprintf( info->s = cpuintrf_temp_str(), "cp0r7   :%08x", cpu->cp0r[ 7 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R8:			sprintf( info->s = cpuintrf_temp_str(), "BadVAddr:%08x", cpu->cp0r[ 8 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R9:			sprintf( info->s = cpuintrf_temp_str(), "cp0r9   :%08x", cpu->cp0r[ 9 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R10:		sprintf( info->s = cpuintrf_temp_str(), "EntryHi :%08x", cpu->cp0r[ 10 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R11:		sprintf( info->s = cpuintrf_temp_str(), "cp0r11  :%08x", cpu->cp0r[ 11 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R12:		sprintf( info->s = cpuintrf_temp_str(), "SR      :%08x", cpu->cp0r[ 12 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R13:		sprintf( info->s = cpuintrf_temp_str(), "Cause   :%08x", cpu->cp0r[ 13 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R14:		sprintf( info->s = cpuintrf_temp_str(), "EPC     :%08x", cpu->cp0r[ 14 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R15:		sprintf( info->s = cpuintrf_temp_str(), "PRId    :%08x", cpu->cp0r[ 15 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R16:		sprintf( info->s = cpuintrf_temp_str(), "cp0r16  :%08x", cpu->cp0r[ 16 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R17:		sprintf( info->s = cpuintrf_temp_str(), "cp0r17  :%08x", cpu->cp0r[ 17 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R18:		sprintf( info->s = cpuintrf_temp_str(), "cp0r18  :%08x", cpu->cp0r[ 18 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R19:		sprintf( info->s = cpuintrf_temp_str(), "cp0r19  :%08x", cpu->cp0r[ 19 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R20:		sprintf( info->s = cpuintrf_temp_str(), "cp0r20  :%08x", cpu->cp0r[ 20 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R21:		sprintf( info->s = cpuintrf_temp_str(), "cp0r21  :%08x", cpu->cp0r[ 21 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R22:		sprintf( info->s = cpuintrf_temp_str(), "cp0r22  :%08x", cpu->cp0r[ 22 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R23:		sprintf( info->s = cpuintrf_temp_str(), "cp0r23  :%08x", cpu->cp0r[ 23 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R24:		sprintf( info->s = cpuintrf_temp_str(), "cp0r24  :%08x", cpu->cp0r[ 24 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R25:		sprintf( info->s = cpuintrf_temp_str(), "cp0r25  :%08x", cpu->cp0r[ 25 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R26:		sprintf( info->s = cpuintrf_temp_str(), "cp0r26  :%08x", cpu->cp0r[ 26 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R27:		sprintf( info->s = cpuintrf_temp_str(), "cp0r27  :%08x", cpu->cp0r[ 27 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R28:		sprintf( info->s = cpuintrf_temp_str(), "cp0r28  :%08x", cpu->cp0r[ 28 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R29:		sprintf( info->s = cpuintrf_temp_str(), "cp0r29  :%08x", cpu->cp0r[ 29 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R30:		sprintf( info->s = cpuintrf_temp_str(), "cp0r30  :%08x", cpu->cp0r[ 30 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP0R31:		sprintf( info->s = cpuintrf_temp_str(), "cp0r31  :%08x", cpu->cp0r[ 31 ] ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR0:		sprintf( info->s = cpuintrf_temp_str(), "vxy0    :%08x", cpu->cp2dr[ 0 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR1:		sprintf( info->s = cpuintrf_temp_str(), "vz0     :%08x", cpu->cp2dr[ 1 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR2:		sprintf( info->s = cpuintrf_temp_str(), "vxy1    :%08x", cpu->cp2dr[ 2 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR3:		sprintf( info->s = cpuintrf_temp_str(), "vz1     :%08x", cpu->cp2dr[ 3 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR4:		sprintf( info->s = cpuintrf_temp_str(), "vxy2    :%08x", cpu->cp2dr[ 4 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR5:		sprintf( info->s = cpuintrf_temp_str(), "vz2     :%08x", cpu->cp2dr[ 5 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR6:		sprintf( info->s = cpuintrf_temp_str(), "rgb     :%08x", cpu->cp2dr[ 6 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR7:		sprintf( info->s = cpuintrf_temp_str(), "otz     :%08x", cpu->cp2dr[ 7 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR8:		sprintf( info->s = cpuintrf_temp_str(), "ir0     :%08x", cpu->cp2dr[ 8 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR9:		sprintf( info->s = cpuintrf_temp_str(), "ir1     :%08x", cpu->cp2dr[ 9 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR10:		sprintf( info->s = cpuintrf_temp_str(), "ir2     :%08x", cpu->cp2dr[ 10 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR11:		sprintf( info->s = cpuintrf_temp_str(), "ir3     :%08x", cpu->cp2dr[ 11 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR12:		sprintf( info->s = cpuintrf_temp_str(), "sxy0    :%08x", cpu->cp2dr[ 12 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR13:		sprintf( info->s = cpuintrf_temp_str(), "sxy1    :%08x", cpu->cp2dr[ 13 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR14:		sprintf( info->s = cpuintrf_temp_str(), "sxy2    :%08x", cpu->cp2dr[ 14 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR15:		sprintf( info->s = cpuintrf_temp_str(), "sxyp    :%08x", cpu->cp2dr[ 15 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR16:		sprintf( info->s = cpuintrf_temp_str(), "sz0     :%08x", cpu->cp2dr[ 16 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR17:		sprintf( info->s = cpuintrf_temp_str(), "sz1     :%08x", cpu->cp2dr[ 17 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR18:		sprintf( info->s = cpuintrf_temp_str(), "sz2     :%08x", cpu->cp2dr[ 18 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR19:		sprintf( info->s = cpuintrf_temp_str(), "sz3     :%08x", cpu->cp2dr[ 19 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR20:		sprintf( info->s = cpuintrf_temp_str(), "rgb0    :%08x", cpu->cp2dr[ 20 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR21:		sprintf( info->s = cpuintrf_temp_str(), "rgb1    :%08x", cpu->cp2dr[ 21 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR22:		sprintf( info->s = cpuintrf_temp_str(), "rgb2    :%08x", cpu->cp2dr[ 22 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR23:		sprintf( info->s = cpuintrf_temp_str(), "res1    :%08x", cpu->cp2dr[ 23 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR24:		sprintf( info->s = cpuintrf_temp_str(), "mac0    :%08x", cpu->cp2dr[ 24 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR25:		sprintf( info->s = cpuintrf_temp_str(), "mac1    :%08x", cpu->cp2dr[ 25 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR26:		sprintf( info->s = cpuintrf_temp_str(), "mac2    :%08x", cpu->cp2dr[ 26 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR27:		sprintf( info->s = cpuintrf_temp_str(), "mac3    :%08x", cpu->cp2dr[ 27 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR28:		sprintf( info->s = cpuintrf_temp_str(), "irgb    :%08x", cpu->cp2dr[ 28 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR29:		sprintf( info->s = cpuintrf_temp_str(), "orgb    :%08x", cpu->cp2dr[ 29 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR30:		sprintf( info->s = cpuintrf_temp_str(), "lzcs    :%08x", cpu->cp2dr[ 30 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2DR31:		sprintf( info->s = cpuintrf_temp_str(), "lzcr    :%08x", cpu->cp2dr[ 31 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR0:		sprintf( info->s = cpuintrf_temp_str(), "r11r12  :%08x", cpu->cp2cr[ 0 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR1:		sprintf( info->s = cpuintrf_temp_str(), "r13r21  :%08x", cpu->cp2cr[ 1 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR2:		sprintf( info->s = cpuintrf_temp_str(), "r22r23  :%08x", cpu->cp2cr[ 2 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR3:		sprintf( info->s = cpuintrf_temp_str(), "r31r32  :%08x", cpu->cp2cr[ 3 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR4:		sprintf( info->s = cpuintrf_temp_str(), "r33     :%08x", cpu->cp2cr[ 4 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR5:		sprintf( info->s = cpuintrf_temp_str(), "trx     :%08x", cpu->cp2cr[ 5 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR6:		sprintf( info->s = cpuintrf_temp_str(), "try     :%08x", cpu->cp2cr[ 6 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR7:		sprintf( info->s = cpuintrf_temp_str(), "trz     :%08x", cpu->cp2cr[ 7 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR8:		sprintf( info->s = cpuintrf_temp_str(), "l11l12  :%08x", cpu->cp2cr[ 8 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR9:		sprintf( info->s = cpuintrf_temp_str(), "l13l21  :%08x", cpu->cp2cr[ 9 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR10:		sprintf( info->s = cpuintrf_temp_str(), "l22l23  :%08x", cpu->cp2cr[ 10 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR11:		sprintf( info->s = cpuintrf_temp_str(), "l31l32  :%08x", cpu->cp2cr[ 11 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR12:		sprintf( info->s = cpuintrf_temp_str(), "l33     :%08x", cpu->cp2cr[ 12 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR13:		sprintf( info->s = cpuintrf_temp_str(), "rbk     :%08x", cpu->cp2cr[ 13 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR14:		sprintf( info->s = cpuintrf_temp_str(), "gbk     :%08x", cpu->cp2cr[ 14 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR15:		sprintf( info->s = cpuintrf_temp_str(), "bbk     :%08x", cpu->cp2cr[ 15 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR16:		sprintf( info->s = cpuintrf_temp_str(), "lr1lr2  :%08x", cpu->cp2cr[ 16 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR17:		sprintf( info->s = cpuintrf_temp_str(), "lr31g1  :%08x", cpu->cp2cr[ 17 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR18:		sprintf( info->s = cpuintrf_temp_str(), "lg2lg3  :%08x", cpu->cp2cr[ 18 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR19:		sprintf( info->s = cpuintrf_temp_str(), "lb1lb2  :%08x", cpu->cp2cr[ 19 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR20:		sprintf( info->s = cpuintrf_temp_str(), "lb3     :%08x", cpu->cp2cr[ 20 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR21:		sprintf( info->s = cpuintrf_temp_str(), "rfc     :%08x", cpu->cp2cr[ 21 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR22:		sprintf( info->s = cpuintrf_temp_str(), "gfc     :%08x", cpu->cp2cr[ 22 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR23:		sprintf( info->s = cpuintrf_temp_str(), "bfc     :%08x", cpu->cp2cr[ 23 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR24:		sprintf( info->s = cpuintrf_temp_str(), "ofx     :%08x", cpu->cp2cr[ 24 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR25:		sprintf( info->s = cpuintrf_temp_str(), "ofy     :%08x", cpu->cp2cr[ 25 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR26:		sprintf( info->s = cpuintrf_temp_str(), "h       :%08x", cpu->cp2cr[ 26 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR27:		sprintf( info->s = cpuintrf_temp_str(), "dqa     :%08x", cpu->cp2cr[ 27 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR28:		sprintf( info->s = cpuintrf_temp_str(), "dqb     :%08x", cpu->cp2cr[ 28 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR29:		sprintf( info->s = cpuintrf_temp_str(), "zsf3    :%08x", cpu->cp2cr[ 29 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR30:		sprintf( info->s = cpuintrf_temp_str(), "zsf4    :%08x", cpu->cp2cr[ 30 ].d ); break;
		case CPUINFO_STR_REGISTER + MIPS_CP2CR31:		sprintf( info->s = cpuintrf_temp_str(), "flag    :%08x", cpu->cp2cr[ 31 ].d ); break;
#endif
	}
}

uint32 mips_get_cause(mips_cpu_context *cpu)
{
	return cpu->cp0r[ CP0_CAUSE ];
}

uint32 mips_get_status(mips_cpu_context *cpu)
{
	return cpu->cp0r[ CP0_SR ];
}

void mips_set_status(mips_cpu_context *cpu, uint32 status)
{
	cpu->cp0r[ CP0_SR ] = status;
}

uint32 mips_get_ePC(mips_cpu_context *cpu)
{
	return cpu->cp0r[ CP0_EPC ];
}

int mips_get_icount(mips_cpu_context *cpu)
{
	return cpu->mips_ICount;
}

void mips_set_icount(mips_cpu_context *cpu, int count)
{
	cpu->mips_ICount = count;
}


#if (HAS_PSXCPU)
/**************************************************************************
 * CPU-specific set_info
 **************************************************************************/

void psxcpu_get_info(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info)
{
	switch (state)
	{
		/* --- the following bits of info are returned as NULL-terminated strings --- */
//		case CPUINFO_STR_NAME:							strcpy(info->s = cpuintrf_temp_str(), "PSX CPU"); break;

		default:
			mips_get_info(cpu, state, info);
			break;
	}
}
#endif
