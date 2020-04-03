#ifndef _MEMORY_H
#define _MEMORY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNU__
#define UNUSEDARG __attribute__((__unused__))
#else
#define UNUSEDARG
#endif

#define MAX_BANKS		20


/* obsolete, to be removed */
#define READ_WORD(a)		  (*(UINT16 *)(a))
#define WRITE_WORD(a,d) 	  (*(UINT16 *)(a) = (d))
#define COMBINE_WORD(w,d)	  (((w) & ((d) >> 16)) | ((d) & 0xffff))
#define COMBINE_WORD_MEM(a,d) (WRITE_WORD((a), (READ_WORD(a) & ((d) >> 16)) | (d)))

#define ASSERT_LINE (1)
#define CLEAR_LINE  (0)
#define TIME_NEVER (0)

#define TIME_IN_HZ(hz)        (1.0 / (double)(hz))
#define TIME_IN_MSEC(ms)      ((double)(ms) * (1.0 / 1000.0))
#define TIME_IN_USEC(us)      ((double)(us) * (1.0 / 1000000.0))

/***************************************************************************

Note that the memory hooks are not passed the actual memory address where
the operation takes place, but the offset from the beginning of the block
they are assigned to. This makes handling of mirror addresses easier, and
makes the handlers a bit more "object oriented". If you handler needs to
read/write the main memory area, provide a "base" pointer: it will be
initialized by the main engine to point to the beginning of the memory block
assigned to the handler. You may also provided a pointer to "size": it
will be set to the length of the memory area processed by the handler.

***************************************************************************/

#define MEMORY_WIDTH_MASK		0x00000003
#define MEMORY_WIDTH_8			0x00000001
#define MEMORY_WIDTH_16 		0x00000002
#define MEMORY_WIDTH_32 		0x00000003

#define MEMORY_TYPE_MASK		0x30000000
#define MEMORY_TYPE_MEM 		0x10000000
#define MEMORY_TYPE_IO			0x20000000

#define MEMORY_DIRECTION_MASK   0xc0000000
#define MEMORY_DIRECTION_READ   0x40000000
#define MEMORY_DIRECTION_WRITE  0x80000000

typedef unsigned int offs_t;
typedef offs_t (*opbase_handler)(UNUSEDARG offs_t address);

/***************************************************************************
	8-BIT	Core memory read/write/opbase handler types
***************************************************************************/

typedef unsigned char data8_t;
typedef unsigned short data16_t;

typedef data8_t (*mem_read_handler)(UNUSEDARG offs_t offset);
typedef void (*mem_write_handler)(UNUSEDARG offs_t offset, UNUSEDARG data8_t data);

#define READ_HANDLER(name) data8_t name(UNUSEDARG offs_t offset)
#define WRITE_HANDLER(name) void name(UNUSEDARG offs_t offset, UNUSEDARG data8_t data)
#define OPBASE_HANDLER(name) offs_t name(UNUSEDARG offs_t address)

#define READ16_HANDLER(name) data16_t name(UNUSEDARG offs_t offset, UNUSEDARG UINT32 mem_mask)
#define WRITE16_HANDLER(name) void name(UNUSEDARG offs_t offset, UNUSEDARG data16_t data, UNUSEDARG UINT32 mem_mask)
#define OPBASE16_HANDLER(name) offs_t name(UNUSEDARG offs_t address)

#define MRA_NOP 0                           /* don't care, return 0 */
#define MWA_NOP 0							/* do nothing */
#define MRA_RAM ((mem_read_handler)-1)		/* plain RAM location (return its contents) */
#define MWA_RAM ((mem_write_handler)-1) 	/* plain RAM location (store the value) */
#define MRA_ROM ((mem_read_handler)-2)		/* plain ROM location (return its contents) */
#define MWA_ROM ((mem_write_handler)-2) 	/* plain ROM location (do nothing) */
/**************************************************************************
 * If the CPU opcodes are encrypted, they are fetched from a different
 * memory space. In such a case, if the program dynamically creates code
 * in RAM and executes it, it won't work unless you use MWA_RAMROM
 * to affect both memory spaces.
 **************************************************************************/
#define MWA_RAMROM ((mem_write_handler)-3)

/* bank memory */
#define MRA_BANK1 ((mem_read_handler)-10)
#define MWA_BANK1 ((mem_write_handler)-10)
#define MRA_BANK2 ((mem_read_handler)-11)
#define MWA_BANK2 ((mem_write_handler)-11)
#define MRA_BANK3 ((mem_read_handler)-12)
#define MWA_BANK3 ((mem_write_handler)-12)
#define MRA_BANK4 ((mem_read_handler)-13)
#define MWA_BANK4 ((mem_write_handler)-13)
#define MRA_BANK5 ((mem_read_handler)-14)
#define MWA_BANK5 ((mem_write_handler)-14)
#define MRA_BANK6 ((mem_read_handler)-15)
#define MWA_BANK6 ((mem_write_handler)-15)
#define MRA_BANK7 ((mem_read_handler)-16)
#define MWA_BANK7 ((mem_write_handler)-16)
#define MRA_BANK8 ((mem_read_handler)-17)
#define MWA_BANK8 ((mem_write_handler)-17)
#define MRA_BANK9 ((mem_read_handler)-18)
#define MWA_BANK9 ((mem_write_handler)-18)
#define MRA_BANK10 ((mem_read_handler)-19)
#define MWA_BANK10 ((mem_write_handler)-19)
#define MRA_BANK11 ((mem_read_handler)-20)
#define MWA_BANK11 ((mem_write_handler)-20)
#define MRA_BANK12 ((mem_read_handler)-21)
#define MWA_BANK12 ((mem_write_handler)-21)
#define MRA_BANK13 ((mem_read_handler)-22)
#define MWA_BANK13 ((mem_write_handler)-22)
#define MRA_BANK14 ((mem_read_handler)-23)
#define MWA_BANK14 ((mem_write_handler)-23)
#define MRA_BANK15 ((mem_read_handler)-24)
#define MWA_BANK15 ((mem_write_handler)-24)
#define MRA_BANK16 ((mem_read_handler)-25)
#define MWA_BANK16 ((mem_write_handler)-25)
#define MRA_BANK17 ((mem_read_handler)-26)
#define MWA_BANK17 ((mem_write_handler)-26)
#define MRA_BANK18 ((mem_read_handler)-27)
#define MWA_BANK18 ((mem_write_handler)-27)
#define MRA_BANK19 ((mem_read_handler)-28)
#define MWA_BANK19 ((mem_write_handler)-28)
#define MRA_BANK20 ((mem_read_handler)-29)
#define MWA_BANK20 ((mem_write_handler)-29)

struct Memory_ReadAddress
{
	offs_t start, end;
	mem_read_handler handler;				/* see special values above */
};

struct Memory_WriteAddress
{
    offs_t start, end;
	mem_write_handler handler;				/* see special values above */
	data8_t **base;							/* optional (see explanation above) */
    size_t *size;                           /* optional (see explanation above) */
};

#define MEMORY_MARKER ((offs_t)~0)

#define MEMORY_END { MEMORY_MARKER, 0 } };

#define IS_MEMORY_MARKER( ma ) ((ma)->start == MEMORY_MARKER && (ma)->end < MEMORY_MARKER)
#define IS_MEMORY_END( ma ) ((ma)->start == MEMORY_MARKER && (ma)->end == 0)

#define MEMORY_READ_START(name) const struct Memory_ReadAddress name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_READ | MEMORY_TYPE_MEM | MEMORY_WIDTH_8 },
#define MEMORY_WRITE_START(name) const struct Memory_WriteAddress name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_WRITE | MEMORY_TYPE_MEM | MEMORY_WIDTH_8 },

/***************************************************************************
	16-BIT	Core memory read/write/opbase handler types
***************************************************************************/

typedef data16_t (*mem_read16_handler)(UNUSEDARG offs_t offset);
typedef void (*mem_write16_handler)(UNUSEDARG offs_t offset, UNUSEDARG data16_t data, UNUSEDARG UINT32 mem_mask);

#define READ16_HANDLER(name) data16_t name(UNUSEDARG offs_t offset, UNUSEDARG UINT32 mem_mask)
#define WRITE16_HANDLER(name) void name(UNUSEDARG offs_t offset, UNUSEDARG data16_t data, UNUSEDARG UINT32 mem_mask)
#define OPBASE16_HANDLER(name) offs_t name(UNUSEDARG offs_t address)

#define MRA16_NOP 0 						/* don't care, return 0 */
#define MWA16_NOP 0 						/* do nothing */
#define MRA16_RAM ((mem_read16_handler)-1)  /* plain RAM location (return its contents) */
#define MWA16_RAM ((mem_write16_handler)-1) /* plqain RAM location (store the value) */
#define MRA16_ROM ((mem_read16_handler)-2)	/* plain ROM location (return its contents) */
#define MWA16_ROM ((mem_write16_handler)-2) /* plain ROM location (do nothing) */
/**************************************************************************
 * If the CPU opcodes are encrypted, they are fetched from a different
 * memory space. In such a case, if the program dynamically creates code
 * in RAM and executes it, it won't work unless you use MWA_RAMROM
 * to affect both memory spaces.
 **************************************************************************/
#define MWA16_RAMROM ((mem_write16_handler)-3)

/* bank memory */
#define MRA16_BANK1 ((mem_read16_handler)-10)
#define MWA16_BANK1 ((mem_write16_handler)-10)
#define MRA16_BANK2 ((mem_read16_handler)-11)
#define MWA16_BANK2 ((mem_write16_handler)-11)
#define MRA16_BANK3 ((mem_read16_handler)-12)
#define MWA16_BANK3 ((mem_write16_handler)-12)
#define MRA16_BANK4 ((mem_read16_handler)-13)
#define MWA16_BANK4 ((mem_write16_handler)-13)
#define MRA16_BANK5 ((mem_read16_handler)-14)
#define MWA16_BANK5 ((mem_write16_handler)-14)
#define MRA16_BANK6 ((mem_read16_handler)-15)
#define MWA16_BANK6 ((mem_write16_handler)-15)
#define MRA16_BANK7 ((mem_read16_handler)-16)
#define MWA16_BANK7 ((mem_write16_handler)-16)
#define MRA16_BANK8 ((mem_read16_handler)-17)
#define MWA16_BANK8 ((mem_write16_handler)-17)
#define MRA16_BANK9 ((mem_read16_handler)-18)
#define MWA16_BANK9 ((mem_write16_handler)-18)
#define MRA16_BANK10 ((mem_read16_handler)-19)
#define MWA16_BANK10 ((mem_write16_handler)-19)
#define MRA16_BANK11 ((mem_read16_handler)-20)
#define MWA16_BANK11 ((mem_write16_handler)-20)
#define MRA16_BANK12 ((mem_read16_handler)-21)
#define MWA16_BANK12 ((mem_write16_handler)-21)
#define MRA16_BANK13 ((mem_read16_handler)-22)
#define MWA16_BANK13 ((mem_write16_handler)-22)
#define MRA16_BANK14 ((mem_read16_handler)-23)
#define MWA16_BANK14 ((mem_write16_handler)-23)
#define MRA16_BANK15 ((mem_read16_handler)-24)
#define MWA16_BANK15 ((mem_write16_handler)-24)
#define MRA16_BANK16 ((mem_read16_handler)-25)
#define MWA16_BANK16 ((mem_write16_handler)-25)
#define MRA16_BANK17 ((mem_read16_handler)-26)
#define MWA16_BANK17 ((mem_write16_handler)-26)
#define MRA16_BANK18 ((mem_read16_handler)-27)
#define MWA16_BANK18 ((mem_write16_handler)-27)
#define MRA16_BANK19 ((mem_read16_handler)-28)
#define MWA16_BANK19 ((mem_write16_handler)-28)
#define MRA16_BANK20 ((mem_read16_handler)-29)
#define MWA16_BANK20 ((mem_write16_handler)-29)

struct Memory_ReadAddress16
{
	offs_t start, end;
	mem_read16_handler handler; 			/* see special values above */
};

struct Memory_WriteAddress16
{
	offs_t start, end;
	mem_write16_handler handler;			/* see special values above */
	data16_t **base;						/* optional (see explanation above) */
    size_t *size;                           /* optional (see explanation above) */
};

#define MEMORY_READ16_START(name) const struct Memory_ReadAddress16 name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_READ | MEMORY_TYPE_MEM | MEMORY_WIDTH_16 },
#define MEMORY_WRITE16_START(name) const struct Memory_WriteAddress16 name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_WRITE | MEMORY_TYPE_MEM | MEMORY_WIDTH_16 },

/***************************************************************************
	32-BIT	Core memory read/write/opbase handler types
***************************************************************************/

typedef UINT32 data32_t;

typedef data32_t (*mem_read32_handler)(UNUSEDARG offs_t offset);
typedef void (*mem_write32_handler)(UNUSEDARG offs_t offset, UNUSEDARG data32_t data, UNUSEDARG UINT32 mem_mask);

#define READ32_HANDLER(name) data32_t name(UNUSEDARG offs_t offset, UNUSEDARG UINT32 mem_mask)
#define WRITE32_HANDLER(name) void name(UNUSEDARG offs_t offset, UNUSEDARG data32_t data, UNUSEDARG UINT32 mem_mask)
#define OPBASE32_HANDLER(name) offs_t name(UNUSEDARG offs_t address)

#define MRA32_NOP 0 						/* don't care, return 0 */
#define MWA32_NOP 0 						/* do nothing */
#define MRA32_RAM ((mem_read32_handler)-1)  /* plain RAM location (return its contents) */
#define MWA32_RAM ((mem_write32_handler)-1) /* plain RAM location (store the value) */
#define MRA32_ROM ((mem_read32_handler)-2)	/* plain ROM location (return its contents) */
#define MWA32_ROM ((mem_write32_handler)-2) /* plain ROM location (do nothing) */
/**************************************************************************
 * If the CPU opcodes are encrypted, they are fetched from a different
 * memory space. In such a case, if the program dynamically creates code
 * in RAM and executes it, it won't work unless you use MWA_RAMROM
 * to affect both memory spaces.
 **************************************************************************/
#define MWA32_RAMROM ((mem_write32_handler)-3)

/* bank memory */
#define MRA32_BANK1 ((mem_read32_handler)-10)
#define MWA32_BANK1 ((mem_write32_handler)-10)
#define MRA32_BANK2 ((mem_read32_handler)-11)
#define MWA32_BANK2 ((mem_write32_handler)-11)
#define MRA32_BANK3 ((mem_read32_handler)-12)
#define MWA32_BANK3 ((mem_write32_handler)-12)
#define MRA32_BANK4 ((mem_read32_handler)-13)
#define MWA32_BANK4 ((mem_write32_handler)-13)
#define MRA32_BANK5 ((mem_read32_handler)-14)
#define MWA32_BANK5 ((mem_write32_handler)-14)
#define MRA32_BANK6 ((mem_read32_handler)-15)
#define MWA32_BANK6 ((mem_write32_handler)-15)
#define MRA32_BANK7 ((mem_read32_handler)-16)
#define MWA32_BANK7 ((mem_write32_handler)-16)
#define MRA32_BANK8 ((mem_read32_handler)-17)
#define MWA32_BANK8 ((mem_write32_handler)-17)
#define MRA32_BANK9 ((mem_read32_handler)-18)
#define MWA32_BANK9 ((mem_write32_handler)-18)
#define MRA32_BANK10 ((mem_read32_handler)-19)
#define MWA32_BANK10 ((mem_write32_handler)-19)
#define MRA32_BANK11 ((mem_read32_handler)-20)
#define MWA32_BANK11 ((mem_write32_handler)-20)
#define MRA32_BANK12 ((mem_read32_handler)-21)
#define MWA32_BANK12 ((mem_write32_handler)-21)
#define MRA32_BANK13 ((mem_read32_handler)-22)
#define MWA32_BANK13 ((mem_write32_handler)-22)
#define MRA32_BANK14 ((mem_read32_handler)-23)
#define MWA32_BANK14 ((mem_write32_handler)-23)
#define MRA32_BANK15 ((mem_read32_handler)-24)
#define MWA32_BANK15 ((mem_write32_handler)-24)
#define MRA32_BANK32 ((mem_read32_handler)-25)
#define MWA32_BANK32 ((mem_write32_handler)-25)
#define MRA32_BANK17 ((mem_read32_handler)-26)
#define MWA32_BANK17 ((mem_write32_handler)-26)
#define MRA32_BANK18 ((mem_read32_handler)-27)
#define MWA32_BANK18 ((mem_write32_handler)-27)
#define MRA32_BANK19 ((mem_read32_handler)-28)
#define MWA32_BANK19 ((mem_write32_handler)-28)
#define MRA32_BANK20 ((mem_read32_handler)-29)
#define MWA32_BANK20 ((mem_write32_handler)-29)

struct Memory_ReadAddress32
{
	offs_t start, end;
	mem_read32_handler handler; 			/* see special values above */
};

struct Memory_WriteAddress32
{
	offs_t start, end;
	mem_write32_handler handler;			/* see special values above */
	data32_t **base;						/* optional (see explanation above) */
	size_t *size;							/* optional (see explanation above) */
};

#define MEMORY_READ32_START(name) const struct Memory_ReadAddress32 name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_READ | MEMORY_TYPE_MEM | MEMORY_WIDTH_32 },
#define MEMORY_WRITE32_START(name) const struct Memory_WriteAddress32 name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_WRITE | MEMORY_TYPE_MEM | MEMORY_WIDTH_32 },

/***************************************************************************

IN and OUT ports are handled like memory accesses, the hook template is the
same so you can interchange them. Of course there is no 'base' pointer for
IO ports.

***************************************************************************/

struct IO_ReadPort
{
	offs_t start,end;
	mem_read_handler handler;				/* see special values below */
};

#define IORP_NOP 0	/* don't care, return 0 */


struct IO_WritePort
{
	offs_t start,end;
	mem_write_handler handler;				/* see special values below */
};

#define IOWP_NOP 0	/* do nothing */

#define PORT_READ_START(name) const struct IO_ReadPort name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_READ | MEMORY_TYPE_IO | MEMORY_WIDTH_8 },
#define PORT_WRITE_START(name) const struct IO_WritePort name[] = { \
	{ MEMORY_MARKER, MEMORY_DIRECTION_WRITE | MEMORY_TYPE_IO | MEMORY_WIDTH_8 },
#define PORT_END	MEMORY_END

/***************************************************************************

If a memory region contains areas that are outside of the ROM region for
an address space, the memory system will allocate an array of structures
to track the external areas.

***************************************************************************/

#define MAX_EXT_MEMORY 64

struct ExtMemory
{
	offs_t start,end,region;
    UINT8 *data;
};

extern struct ExtMemory ext_memory[MAX_EXT_MEMORY];



/***************************************************************************

For a given number of address bits, we need to determine how many elements
there are in the first and second-order lookup tables. We also need to know
how many low-order bits to ignore. The ABITS* values represent these
constants for each address space type we support.

***************************************************************************/

/* memory element block size */
#define MH_SBITS		8			/* sub element bank size */
#define MH_PBITS		8			/* port current element size */
#define MH_ELEMAX		64			/* sub elements limit */
#define MH_HARDMAX		64			/* hardware functions limit */

/* 16 bits address */
#define ABITS1_16		12
#define ABITS2_16		4
#define ABITS_MIN_16	0			/* minimum memory block is 1 byte */
/* 16 bits address (word access) */
#define ABITS1_16W	12
#define ABITS2_16W	3
#define ABITS_MIN_16W 1			/* minimum memory block is 2 bytes */
/* 20 bits address */
#define ABITS1_20		12
#define ABITS2_20		8
#define ABITS_MIN_20	0			/* minimum memory block is 1 byte */
/* 21 bits address */
#define ABITS1_21		13
#define ABITS2_21		8
#define ABITS_MIN_21	0			/* minimum memory block is 1 byte */
/* 24 bits address */
#define ABITS1_24		16
#define ABITS2_24		8
#define ABITS_MIN_24	0			/* minimum memory block is 1 byte */
/* 24 bits address (word access) */
#define ABITS1_24W	15
#define ABITS2_24W	8
#define ABITS_MIN_24W 1			/* minimum memory block is 2 bytes */
/* 24 bits address (dword access) */
#define ABITS1_24DW	14
#define ABITS2_24DW	8
#define ABITS_MIN_24DW 2			/* minimum memory block is 4 bytes */
/* 26 bits address (dword access) */
#define ABITS1_26DW	16
#define ABITS2_26DW	8
#define ABITS_MIN_26DW 2			/* minimum memory block is 4 bytes */
/* 29 bits address (word access) */
#define ABITS1_29W	20
#define ABITS2_29W	8
#define ABITS_MIN_29W	1			/* minimum memory block is 2 bytes */
/* 32 bits address (word access) */
#define ABITS1_32W	23
#define ABITS2_32W	8
#define ABITS_MIN_32W	1			/* minimum memory block is 2 bytes */
/* 32 bits address (dword access) */
#define ABITS1_32DW	22
#define ABITS2_32DW	8
#define ABITS_MIN_32DW	2			/* minimum memory block is 4 bytes */
/* mask bits */
#define MHMASK(abits)	 (0xffffffff >> (32 - abits))


/***************************************************************************

	Global variables

***************************************************************************/

typedef unsigned char MHELE;

extern MHELE ophw;				/* opcode handler */
extern MHELE *cur_mrhard;		/* current set of read handlers */
extern MHELE *cur_mwhard;		/* current set of write handlers */

extern UINT8 *OP_RAM;			/* opcode RAM base */
extern UINT8 *OP_ROM;			/* opcode ROM base */
extern UINT8 *cpu_bankbase[];	/* array of bank bases */


/* global memory access width and mask (16-bit and 32-bit under-size accesses) */
//extern UINT32 mem_width;
//extern UINT32 mem_mask;
//extern UINT32 mem_offs;

/***************************************************************************

	Macros

***************************************************************************/

/* ----- 16-bit memory accessing ----- */
#define COMBINE_DATA(varptr)	(*(varptr) = (*(varptr) & mem_mask) | (data & ~mem_mask))
#define ACCESSING_LSB ((mem_mask & 0x00ff) == 0)
#define ACCESSING_MSB ((mem_mask & 0xff00) == 0)

//extern unsigned char prgrom[128*1024];

/* ----- opcode reading ----- */
#define cpu_readop cpu_readmem16
//#define cpu_readop16(A) 	READ_WORD(&prgrom[A&0x3fff])
//#define cpu_readop32(A) 	READ_DWORD(&prgrom[A&0x3fff])

/* ----- opcode argument reading ----- */
#define cpu_readop_arg cpu_readmem16
//#define cpu_readop_arg16(A) READ_WORD(&prgrom[A&0x3fff])
//#define cpu_readop_arg32(A) READ_DWORD(&prgrom[A&0x3fff])

/* ----- bank switching for CPU cores ----- */
#define change_pc_generic(pc,abits2,abitsmin,shift,setop)	\
{															\
	if (cur_mrhard[(pc)>>(abits2+abitsmin+shift)] != ophw)	\
		setop(pc);											\
}
#define change_pc16(pc) 	 
//change_pc_generic(pc, ABITS2_16, ABITS_MIN_16, 0, cpu_setOPbase16)
#define change_pc20(pc) 	 change_pc_generic(pc, ABITS2_20, ABITS_MIN_20, 0, cpu_setOPbase20)
#define change_pc21(pc) 	 change_pc_generic(pc, ABITS2_21, ABITS_MIN_21, 0, cpu_setOPbase21)
#define change_pc24(pc) 	 change_pc_generic(pc, ABITS2_24, ABITS_MIN_24, 0, cpu_setOPbase24)
#define change_pc16bew(pc)	 change_pc_generic(pc, ABITS2_16W,   ABITS_MIN_16W,   0, cpu_setOPbase16bew)
#define change_pc16lew(pc)	 change_pc_generic(pc, ABITS2_16W,   ABITS_MIN_16W,   0, cpu_setOPbase16lew)
#define change_pc24bew(pc)	 change_pc_generic(pc, ABITS2_24W,   ABITS_MIN_24W,   0, cpu_setOPbase24bew)
#define change_pc29lew(pc)   change_pc_generic(pc, ABITS2_29W,   ABITS_MIN_29W,   3, cpu_setOPbase29lew)
#define change_pc32bew(pc) 	 change_pc_generic(pc, ABITS2_32W,   ABITS_MIN_32W,   0, cpu_setOPbase32bew)
#define change_pc32lew(pc)	 change_pc_generic(pc, ABITS2_32W,   ABITS_MIN_32W,   0, cpu_setOPbase32lew)
#define change_pc24bedw(pc)	 change_pc_generic(pc, ABITS2_24DW,  ABITS_MIN_24DW,  0, cpu_setOPbase24bedw)
#define change_pc26ledw(pc)	 change_pc_generic(pc, ABITS2_26DW,  ABITS_MIN_26DW,  0, cpu_setOPbase26ledw)
#define change_pc32bedw(pc)  change_pc_generic(pc, ABITS2_32DW,  ABITS_MIN_32DW,  0, cpu_setOPbase32bedw)

/* backward compatibility */
#define change_pc(pc)
// change_pc16(pc)

/* ----- for use OPbaseOverride driver, request override callback to next cpu_setOPbase ----- */
#define catch_nextBranch()	(ophw = 0xff)

/* -----  bank switching macro ----- */
#define cpu_setbank(bank, base) 						\
{														\
	if (bank >= 1 && bank <= MAX_BANKS) 				\
	{													\
		cpu_bankbase[bank] = (UINT8 *)(base);			\
		if (ophw == bank)								\
		{												\
			ophw = 0xff;								\
			cpu_set_op_base(cpu_get_pc());				\
		}												\
	}													\
}


/***************************************************************************

	Function prototypes

***************************************************************************/

/* ----- memory setup function ----- */

/* ----- memory read functions ----- */
data8_t cpu_readmem16(offs_t address);
data8_t cpu_readmem20(offs_t address);
data8_t cpu_readmem21(offs_t address);
data8_t cpu_readmem24(offs_t address);

data16_t cpu_readmem16bew(offs_t address);
data16_t cpu_readmem16bew_word(offs_t address);
data16_t cpu_readmem16lew(offs_t address);
data16_t cpu_readmem16lew_word(offs_t address);
data16_t cpu_readmem24bew(offs_t address);
data16_t cpu_readmem24bew_word(offs_t address);
data16_t cpu_readmem29lew(offs_t address);
data16_t cpu_readmem29lew_word(offs_t address);
data16_t cpu_readmem32bew(offs_t address);
data16_t cpu_readmem32bew_word(offs_t address);
data16_t cpu_readmem32lew(offs_t address);
data16_t cpu_readmem32lew_word(offs_t address);

data32_t cpu_readmem24bedw(offs_t address);
data32_t cpu_readmem24bedw_word(offs_t address);
data32_t cpu_readmem24bedw_dword(offs_t address);
data32_t cpu_readmem26ledw(offs_t address);
data32_t cpu_readmem26ledw_word(offs_t address);
data32_t cpu_readmem26ledw_dword(offs_t address);
data32_t cpu_readmem27bedw(offs_t address);
data32_t cpu_readmem27bedw_word(offs_t address);
data32_t cpu_readmem27bedw_dword(offs_t address);
data32_t cpu_readmem32bedw(offs_t address);
data32_t cpu_readmem32bedw_word(offs_t address);
data32_t cpu_readmem32bedw_dword(offs_t address);

/* ----- memory write functions ----- */
void cpu_writemem16(offs_t address,data8_t data);
void cpu_writemem20(offs_t address,data8_t data);
void cpu_writemem21(offs_t address,data8_t data);
void cpu_writemem24(offs_t address,data8_t data);

void cpu_writemem16bew(offs_t address,data16_t data);
void cpu_writemem16bew_word(offs_t address,data16_t data);
void cpu_writemem16lew(offs_t address,data16_t data);
void cpu_writemem16lew_word(offs_t address,data16_t data);
void cpu_writemem24bew(offs_t address,data16_t data);
void cpu_writemem24bew_word(offs_t address,data16_t data);
void cpu_writemem29lew(offs_t address,data16_t data);
void cpu_writemem29lew_word(offs_t address,data16_t data);
void cpu_writemem32bew(offs_t address,data16_t data);
void cpu_writemem32bew_word(offs_t address,data16_t data);
void cpu_writemem32lew(offs_t address,data16_t data);
void cpu_writemem32lew_word(offs_t address,data16_t data);

void cpu_writemem24bedw(offs_t address,data32_t data);
void cpu_writemem24bedw_word(offs_t address,data32_t data);
void cpu_writemem24bedw_dword(offs_t address,data32_t data);
void cpu_writemem26ledw(offs_t address,data32_t data);
void cpu_writemem26ledw_word(offs_t address,data32_t data);
void cpu_writemem26ledw_dword(offs_t address,data32_t data);
void cpu_writemem27bedw(offs_t address,data32_t data);
void cpu_writemem27bedw_word(offs_t address,data32_t data);
void cpu_writemem27bedw_dword(offs_t address,data32_t data);
void cpu_writemem32bedw(offs_t address,data32_t data);
void cpu_writemem32bedw_word(offs_t address,data32_t data);
void cpu_writemem32bedw_dword(offs_t address,data32_t data);

/* ----- port I/O functions ----- */
int cpu_readport(int port);
void cpu_writeport(int port, int value);

/* ----- dynamic memory/port mapping ----- */
void *install_mem_read_handler(int cpu, int start, int end, mem_read_handler handler);
void *install_mem_read16_handler(int cpu, int start, int end, mem_read16_handler handler);
void *install_mem_read32_handler(int cpu, int start, int end, mem_read32_handler handler);
void *install_mem_write_handler(int cpu, int start, int end, mem_write_handler handler);
void *install_mem_write16_handler(int cpu, int start, int end, mem_write16_handler handler);
void *install_mem_write32_handler(int cpu, int start, int end, mem_write32_handler handler);
void *install_port_read_handler(int cpu, int start, int end, mem_read_handler handler);
void *install_port_write_handler(int cpu, int start, int end, mem_write_handler handler);

/* ----- dynamic bank handlers ----- */
void cpu_setbankhandler_r(int bank, mem_read_handler handler);
void cpu_setbankhandler_w(int bank, mem_write_handler handler);

/* ----- opcode base control ---- */
void cpu_setOPbase16(offs_t pc);
void cpu_setOPbase20(offs_t pc);
void cpu_setOPbase21(offs_t pc);
void cpu_setOPbase24(offs_t pc);
void cpu_setOPbase16bew(offs_t pc);
void cpu_setOPbase16lew(offs_t pc);
void cpu_setOPbase24bew(offs_t pc);
void cpu_setOPbase24bedw(offs_t pc);
void cpu_setOPbase26ledw(offs_t pc);
void cpu_setOPbase29lew(offs_t pc);
void cpu_setOPbase32bew(offs_t pc);
void cpu_setOPbase32lew(offs_t pc);
void cpu_setOPbaseoverride(int cpu, opbase_handler function);

/* ----- harder-to-explain functions ---- */

/* use this to set the a different opcode base address when using a CPU with
   opcodes and data encrypted separately */
void memory_set_opcode_base(int cpu, void *base);

/* look up a chunk of memory and get its start/end addresses, and its base.
Pass in the cpu number and the offset. It will find the chunk containing
that offset and return the start and end addresses, along with a pointer to
the base of the memory.
This can be used (carefully!) by drivers that wish to access memory directly
without going through the readmem/writemem accessors (e.g., blitters). */
void *findmemorychunk(int cpu, int offset, int *chunkstart, int *chunkend);

#ifdef __cplusplus
}
#endif

#endif	/* !_MEMORY_H */

