#ifndef _MIPS_H
#define _MIPS_H

#include "ao.h"
#include "osd_cpu.h"
//#include "driver.h"

typedef void genf(void);
typedef int offs_t;

#define cpu_readop32(pc) program_read_dword_32le(cpu, pc)
#define change_pc(pc)																	\


#ifdef __GNUC__
#if (__GNUC__ < 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ <= 7))
#define UNUSEDARG
#else
#define UNUSEDARG __attribute__((__unused__))
#endif
#else
#define UNUSEDARG
#endif

typedef int8		(*read8_handler)  (UNUSEDARG offs_t offset);
typedef void		(*write8_handler) (UNUSEDARG offs_t offset, UNUSEDARG int8 data);
typedef int16		(*read16_handler) (UNUSEDARG offs_t offset, UNUSEDARG int16 mem_mask);
typedef void		(*write16_handler)(UNUSEDARG offs_t offset, UNUSEDARG int16 data, UNUSEDARG int16 mem_mask);
typedef int32		(*read32_handler) (UNUSEDARG offs_t offset, UNUSEDARG int32 mem_mask);
typedef void		(*write32_handler)(UNUSEDARG offs_t offset, UNUSEDARG int32 data, UNUSEDARG int32 mem_mask);
typedef int64		(*read64_handler) (UNUSEDARG offs_t offset, UNUSEDARG int64 mem_mask);
typedef void		(*write64_handler)(UNUSEDARG offs_t offset, UNUSEDARG int64 data, UNUSEDARG int64 mem_mask);

union read_handlers_t
{
	genf *				handler;
	read8_handler		handler8;
	read16_handler		handler16;
	read32_handler		handler32;
	read64_handler		handler64;
};

union write_handlers_t
{
	genf *				handler;
	write8_handler		handler8;
	write16_handler		handler16;
	write32_handler		handler32;
	write64_handler		handler64;
};

struct address_map_t
{
	uint32				flags;				/* flags and additional info about this entry */
	offs_t				start, end;			/* start/end (or mask/match) values */
	offs_t				mirror;				/* mirror bits */
	offs_t				mask;				/* mask bits */
	union read_handlers_t read;				/* read handler callback */
	union write_handlers_t write;			/* write handler callback */
	void *				memory;				/* pointer to memory backing this entry */
	uint32				share;				/* index of a shared memory block */
	void **				base;				/* receives pointer to memory (optional) */
	size_t *			size;				/* receives size of area in bytes (optional) */
};
typedef struct address_map_t *(*construct_map_t)(struct address_map_t *map);

#define MAX_FILE_SLOTS	(32)

typedef struct
{
	char name[10];
	uint32 dispatch;
} ExternLibEntries;

typedef struct
{
	int32  iState;		// state of thread

	uint32 flags;		// flags
	uint32 routine;		// start of code for the thread
	uint32 stackloc;	// stack location in IOP RAM
	uint32 stacksize;	// stack size
	uint32 refCon;		// user value passed in at CreateThread time

	uint32 waitparm;	// what we're waiting on if in one the TS_WAIT* states
	
	uint32 save_regs[37];	// CPU registers belonging to this thread
} Thread;

typedef struct
{
	uint32 attr;
	uint32 option;
	int32 init;
	int32 current;
	int32 max;
	int32 threadsWaiting;
	int32 inuse;
} Semaphore;

#define SEMA_MAX	(64)

typedef struct
{
	uint32 type;
	uint32 value;
	uint32 param;
	int    inUse;
} EventFlag;

typedef struct
{
	uint32 count;
	uint32 mode;
	uint32 target;
	uint32 sysclock;
} Counter;


typedef struct
{
	uint32 desc;
	int32 status;
	int32 mode;
	uint32 fhandler;
} EvtCtrlBlk[32];

typedef struct
{
	int32  iActive;
	uint32 count;
	uint32 target;
	uint32 source;
	uint32 prescale;
	uint32 handler;
	uint32 hparam;
	uint32 mode;
} IOPTimer;

typedef struct mips_cpu_context_s
{
    int psf_refresh;
	UINT32 op;
	UINT32 pc;
	UINT32 prevpc;
	UINT32 delayv;
	UINT32 delayr;
	UINT32 hi;
	UINT32 lo;
	UINT32 r[ 32 ];
	UINT32 cp0r[ 32 ];
	PAIR cp2cr[ 32 ];
	PAIR cp2dr[ 32 ];
	int (*irq_callback)(struct mips_cpu_context_s *cpu, int irqline);
    int mips_ICount;
    // PSX main RAM
    uint32 psx_ram[(2*1024*1024)/4];
    uint32 psx_scratch[0x400];
    // backup image to restart songs
    uint32 initial_ram[(2*1024*1024)/4];
    uint32 initial_scratch[0x400];

    // spu
    struct spu_state_s *spu;
    struct spu2_state_s *spu2;
    void (*spu_callback)(unsigned char *, long, void *);
    void *spu_callback_data;

    // state
    Counter root_cnts[3];	// 3 of the bastards

    EvtCtrlBlk *Event;
    EvtCtrlBlk *CounterEvent;

    uint32 spu_delay, dma_icr, irq_data, irq_mask, dma_timer, WAI;
    uint32 dma4_madr, dma4_bcr, dma4_chcr, dma4_delay;
    uint32 dma7_madr, dma7_bcr, dma7_chcr, dma7_delay;
    uint32 dma4_cb, dma7_cb, dma4_fval, dma4_flag, dma7_fval, dma7_flag;
    uint32 irq9_cb, irq9_fval, irq9_flag;

    volatile int softcall_target;
    int filestat[MAX_FILE_SLOTS];
    uint8 *filedata[MAX_FILE_SLOTS];
    uint32 filesize[MAX_FILE_SLOTS], filepos[MAX_FILE_SLOTS];
    int intr_susp;

    uint64 sys_time;
    int timerexp;

    int32 iNumLibs;
    ExternLibEntries	reglibs[32];

    int32 iNumFlags;
    EventFlag evflags[32];

    int32 iNumSema;
    Semaphore semaphores[SEMA_MAX];

    int32 iNumThreads, iCurThread;
    Thread threads[32];
    IOPTimer iop_timers[8];
    int32 iNumTimers;
    int fcnt;
    uint32 heap_addr, entry_int;
    uint32 irq_regs[37];
    int irq_mutex;

} mips_cpu_context;

union cpuinfo
{
	int64	i;											/* generic integers */
	void *	p;											/* generic pointers */
	genf *  f;											/* generic function pointers */
	char *	s;											/* generic strings */
	mips_cpu_context *cpu;

	void	(*setinfo)(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info);/* CPUINFO_PTR_SET_INFO */
//	void	(*getcontext)(void *context);				/* CPUINFO_PTR_GET_CONTEXT */
//	void	(*setcontext)(void *context);				/* CPUINFO_PTR_SET_CONTEXT */
	void	(*init)(mips_cpu_context *cpu);								/* CPUINFO_PTR_INIT */
	void	(*reset)(mips_cpu_context *cpu, void *param);						/* CPUINFO_PTR_RESET */
	void	(*exit)(mips_cpu_context *cpu);								/* CPUINFO_PTR_EXIT */
	int		(*execute)(mips_cpu_context *cpu, int cycles);						/* CPUINFO_PTR_EXECUTE */
	void	(*burn)(mips_cpu_context *cpu, int cycles);						/* CPUINFO_PTR_BURN */
	offs_t	(*disassemble)(mips_cpu_context *cpu, char *buffer, offs_t pc);	/* CPUINFO_PTR_DISASSEMBLE */
	int		(*irqcallback)(mips_cpu_context *cpu, int state);					/* CPUINFO_PTR_IRQCALLBACK */
	int *	icount;										/* CPUINFO_PTR_INSTRUCTION_COUNTER */
	construct_map_t internal_map;						/* CPUINFO_PTR_INTERNAL_MEMORY_MAP */
};

enum
{
	MIPS_PC = 1,
	MIPS_DELAYV, MIPS_DELAYR,
	MIPS_HI, MIPS_LO,
	MIPS_R0, MIPS_R1,
	MIPS_R2, MIPS_R3,
	MIPS_R4, MIPS_R5,
	MIPS_R6, MIPS_R7,
	MIPS_R8, MIPS_R9,
	MIPS_R10, MIPS_R11,
	MIPS_R12, MIPS_R13,
	MIPS_R14, MIPS_R15,
	MIPS_R16, MIPS_R17,
	MIPS_R18, MIPS_R19,
	MIPS_R20, MIPS_R21,
	MIPS_R22, MIPS_R23,
	MIPS_R24, MIPS_R25,
	MIPS_R26, MIPS_R27,
	MIPS_R28, MIPS_R29,
	MIPS_R30, MIPS_R31,
	MIPS_CP0R0, MIPS_CP0R1,
	MIPS_CP0R2, MIPS_CP0R3,
	MIPS_CP0R4, MIPS_CP0R5,
	MIPS_CP0R6, MIPS_CP0R7,
	MIPS_CP0R8, MIPS_CP0R9,
	MIPS_CP0R10, MIPS_CP0R11,
	MIPS_CP0R12, MIPS_CP0R13,
	MIPS_CP0R14, MIPS_CP0R15,
	MIPS_CP0R16, MIPS_CP0R17,
	MIPS_CP0R18, MIPS_CP0R19,
	MIPS_CP0R20, MIPS_CP0R21,
	MIPS_CP0R22, MIPS_CP0R23,
	MIPS_CP0R24, MIPS_CP0R25,
	MIPS_CP0R26, MIPS_CP0R27,
	MIPS_CP0R28, MIPS_CP0R29,
	MIPS_CP0R30, MIPS_CP0R31,
	MIPS_CP2DR0, MIPS_CP2DR1,
	MIPS_CP2DR2, MIPS_CP2DR3,
	MIPS_CP2DR4, MIPS_CP2DR5,
	MIPS_CP2DR6, MIPS_CP2DR7,
	MIPS_CP2DR8, MIPS_CP2DR9,
	MIPS_CP2DR10, MIPS_CP2DR11,
	MIPS_CP2DR12, MIPS_CP2DR13,
	MIPS_CP2DR14, MIPS_CP2DR15,
	MIPS_CP2DR16, MIPS_CP2DR17,
	MIPS_CP2DR18, MIPS_CP2DR19,
	MIPS_CP2DR20, MIPS_CP2DR21,
	MIPS_CP2DR22, MIPS_CP2DR23,
	MIPS_CP2DR24, MIPS_CP2DR25,
	MIPS_CP2DR26, MIPS_CP2DR27,
	MIPS_CP2DR28, MIPS_CP2DR29,
	MIPS_CP2DR30, MIPS_CP2DR31,
	MIPS_CP2CR0, MIPS_CP2CR1,
	MIPS_CP2CR2, MIPS_CP2CR3,
	MIPS_CP2CR4, MIPS_CP2CR5,
	MIPS_CP2CR6, MIPS_CP2CR7,
	MIPS_CP2CR8, MIPS_CP2CR9,
	MIPS_CP2CR10, MIPS_CP2CR11,
	MIPS_CP2CR12, MIPS_CP2CR13,
	MIPS_CP2CR14, MIPS_CP2CR15,
	MIPS_CP2CR16, MIPS_CP2CR17,
	MIPS_CP2CR18, MIPS_CP2CR19,
	MIPS_CP2CR20, MIPS_CP2CR21,
	MIPS_CP2CR22, MIPS_CP2CR23,
	MIPS_CP2CR24, MIPS_CP2CR25,
	MIPS_CP2CR26, MIPS_CP2CR27,
	MIPS_CP2CR28, MIPS_CP2CR29,
	MIPS_CP2CR30, MIPS_CP2CR31
};

#define MIPS_INT_NONE	( -1 )

#define MIPS_IRQ0	( 0 )
#define MIPS_IRQ1	( 1 )
#define MIPS_IRQ2	( 2 )
#define MIPS_IRQ3	( 3 )
#define MIPS_IRQ4	( 4 )
#define MIPS_IRQ5	( 5 )

#define MIPS_BYTE_EXTEND( a ) ( (INT32)(INT8)a )
#define MIPS_WORD_EXTEND( a ) ( (INT32)(INT16)a )

#define INS_OP( op ) ( ( op >> 26 ) & 63 )
#define INS_RS( op ) ( ( op >> 21 ) & 31 )
#define INS_RT( op ) ( ( op >> 16 ) & 31 )
#define INS_IMMEDIATE( op ) ( op & 0xffff )
#define INS_TARGET( op ) ( op & 0x3ffffff )
#define INS_RD( op ) ( ( op >> 11 ) & 31 )
#define INS_SHAMT( op ) ( ( op >> 6 ) & 31 )
#define INS_FUNCT( op ) ( op & 63 )
#define INS_CODE( op ) ( ( op >> 6 ) & 0xfffff )
#define INS_CO( op ) ( ( op >> 25 ) & 1 )
#define INS_COFUN( op ) ( op & 0x1ffffff )
#define INS_CF( op ) ( op & 63 )

#define GTE_OP( op ) ( ( op >> 20 ) & 31 )
#define GTE_SF( op ) ( ( op >> 19 ) & 1 )
#define GTE_MX( op ) ( ( op >> 17 ) & 3 )
#define GTE_V( op ) ( ( op >> 15 ) & 3 )
#define GTE_CV( op ) ( ( op >> 13 ) & 3 )
#define GTE_CD( op ) ( ( op >> 11 ) & 3 ) /* not used */
#define GTE_LM( op ) ( ( op >> 10 ) & 1 )
#define GTE_CT( op ) ( ( op >> 6 ) & 15 ) /* not used */
#define GTE_FUNCT( op ) ( op & 63 )

#define OP_SPECIAL ( 0 )
#define OP_REGIMM ( 1 )
#define OP_J ( 2 )
#define OP_JAL ( 3 )
#define OP_BEQ ( 4 )
#define OP_BNE ( 5 )
#define OP_BLEZ ( 6 )
#define OP_BGTZ ( 7 )
#define OP_ADDI ( 8 )
#define OP_ADDIU ( 9 )
#define OP_SLTI ( 10 )
#define OP_SLTIU ( 11 )
#define OP_ANDI ( 12 )
#define OP_ORI ( 13 )
#define OP_XORI ( 14 )
#define OP_LUI ( 15 )
#define OP_COP0 ( 16 )
#define OP_COP1 ( 17 )
#define OP_COP2 ( 18 )
#define OP_LB ( 32 )
#define OP_LH ( 33 )
#define OP_LWL ( 34 )
#define OP_LW ( 35 )
#define OP_LBU ( 36 )
#define OP_LHU ( 37 )
#define OP_LWR ( 38 )
#define OP_SB ( 40 )
#define OP_SH ( 41 )
#define OP_SWL ( 42 )
#define OP_SW ( 43 )
#define OP_SWR ( 46 )
#define OP_LWC1 ( 49 )
#define OP_LWC2 ( 50 )
#define OP_SWC1 ( 57 )
#define OP_SWC2 ( 58 )

/* OP_SPECIAL */
#define FUNCT_SLL ( 0 )
#define FUNCT_SRL ( 2 )
#define FUNCT_SRA ( 3 )
#define FUNCT_SLLV ( 4 )
#define FUNCT_SRLV ( 6 )
#define FUNCT_SRAV ( 7 )
#define FUNCT_JR ( 8 )
#define FUNCT_JALR ( 9 )	   
#define FUNCT_HLECALL ( 11 )
#define FUNCT_SYSCALL ( 12 )
#define FUNCT_BREAK ( 13 )
#define FUNCT_MFHI ( 16 )
#define FUNCT_MTHI ( 17 )
#define FUNCT_MFLO ( 18 )
#define FUNCT_MTLO ( 19 )
#define FUNCT_MULT ( 24 )
#define FUNCT_MULTU ( 25 )
#define FUNCT_DIV ( 26 )
#define FUNCT_DIVU ( 27 )
#define FUNCT_ADD ( 32 )
#define FUNCT_ADDU ( 33 )
#define FUNCT_SUB ( 34 )
#define FUNCT_SUBU ( 35 )
#define FUNCT_AND ( 36 )
#define FUNCT_OR ( 37 )
#define FUNCT_XOR ( 38 )
#define FUNCT_NOR ( 39 )
#define FUNCT_SLT ( 42 )
#define FUNCT_SLTU ( 43 )

/* OP_REGIMM */
#define RT_BLTZ ( 0 )
#define RT_BGEZ ( 1 )
#define RT_BLTZAL ( 16 )
#define RT_BGEZAL ( 17 )

/* OP_COP0/OP_COP1/OP_COP2 */
#define RS_MFC ( 0 )
#define RS_CFC ( 2 )
#define RS_MTC ( 4 )
#define RS_CTC ( 6 )
#define RS_BC ( 8 )

/* RS_BC */
#define RT_BCF ( 0 )
#define RT_BCT ( 1 )

/* OP_COP0 */
#define CF_RFE ( 16 )

#ifdef MAME_DEBUG
unsigned DasmMIPS(char *buff, unsigned _pc);
#endif

#if (HAS_PSXCPU)
void psxcpu_get_info(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info);
#endif

mips_cpu_context *mips_alloc(void);
void mips_free (mips_cpu_context *cpu);

void mips_init(mips_cpu_context *cpu);
void mips_exit(mips_cpu_context *cpu);
void mips_reset(mips_cpu_context *cpu, void *param );
int mips_execute(mips_cpu_context *cpu, int cycles );
void mips_set_info(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info);
void mips_get_info(mips_cpu_context *cpu, UINT32 state, union cpuinfo *info);
int mips_execute( mips_cpu_context *cpu, int cycles );
int mips_get_icount(mips_cpu_context *cpu);
void mips_set_icount(mips_cpu_context *cpu, int count);

uint32 mips_get_cause(mips_cpu_context *cpu);
uint32 mips_get_status(mips_cpu_context *cpu);
void mips_set_status(mips_cpu_context *cpu, uint32 status);
uint32 mips_get_ePC(mips_cpu_context *cpu);


void psx_hw_init(mips_cpu_context *cpu);
void psx_hw_slice(mips_cpu_context *cpu);
void psx_hw_frame(mips_cpu_context *cpu);
void ps2_hw_slice(mips_cpu_context *cpu);
void ps2_hw_frame(mips_cpu_context *cpu);

void mips_shorten_frame(mips_cpu_context *cpu);
uint32 psf2_load_file(mips_cpu_context *cpu, char *file, uint8 *buf, uint32 buflen);
uint32 psf2_load_elf(mips_cpu_context *cpu, uint8 *start, uint32 len);
void psx_hw_runcounters(mips_cpu_context *cpu);


void psx_bios_hle(mips_cpu_context *cpu, uint32 pc);
void psx_iop_call(mips_cpu_context *cpu, uint32 pc, uint32 callnum);
uint8 program_read_byte_32le(mips_cpu_context *cpu, offs_t address);
uint16 program_read_word_32le(mips_cpu_context *cpu, offs_t address);
uint32 program_read_dword_32le(mips_cpu_context *cpu, offs_t address);
void program_write_byte_32le(mips_cpu_context *cpu, offs_t address, uint8 data);
void program_write_word_32le(mips_cpu_context *cpu, offs_t address, uint16 data);
void program_write_dword_32le(mips_cpu_context *cpu, offs_t address, uint32 data);

// SPU1
void setlength(struct spu_state_s *spu, s32 stop, s32 fade);

// SPU2
void SPU2write(mips_cpu_context *cpu, unsigned long reg, unsigned short val);
unsigned short SPU2read(mips_cpu_context *cpu, unsigned long reg);
void SPU2readDMA4Mem(mips_cpu_context *cpu, uint32 usPSXMem,int iSize);
void SPU2writeDMA4Mem(mips_cpu_context *cpu, uint32 usPSXMem,int iSize);
void SPU2readDMA7Mem(mips_cpu_context *cpu, uint32 usPSXMem,int iSize);
void SPU2writeDMA7Mem(mips_cpu_context *cpu, uint32 usPSXMem,int iSize);
void SPU2interruptDMA4(mips_cpu_context *cpu);
void SPU2interruptDMA7(mips_cpu_context *cpu);

#endif
