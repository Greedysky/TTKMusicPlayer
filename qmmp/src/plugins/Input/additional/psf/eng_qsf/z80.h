#ifndef Z80_H
#define Z80_H

#include "cpuintrf.h"
#include "osd_cpu.h"

#undef VERBOSE
#define VERBOSE 0

#if VERBOSE
#define LOG(x)	logerror x
#else
#define LOG(x)
#endif

/* execute main opcodes inside a big switch statement */
#ifndef BIG_SWITCH
#define BIG_SWITCH			1
#endif

/* big flags array for ADD/ADC/SUB/SBC/CP results */
#define BIG_FLAGS_ARRAY		1

/* Set to 1 for a more exact (but somewhat slower) Z80 emulation */
#define Z80_EXACT			1

/* on JP and JR opcodes check for tight loops */
#define BUSY_LOOP_HACKS		1

/* check for delay loops counting down BC */
#define TIME_LOOP_HACKS		1

#ifdef X86_ASM
#undef	BIG_FLAGS_ARRAY
#define BIG_FLAGS_ARRAY		0
#endif

enum {
	Z80_PC=1, Z80_SP, Z80_AF, Z80_BC, Z80_DE, Z80_HL,
	Z80_IX, Z80_IY,	Z80_AF2, Z80_BC2, Z80_DE2, Z80_HL2,
	Z80_R, Z80_I, Z80_IM, Z80_IFF1, Z80_IFF2, Z80_HALT,
	Z80_NMI_STATE, Z80_IRQ_STATE, Z80_DC0, Z80_DC1, Z80_DC2, Z80_DC3
};

enum {
	Z80_TABLE_op,
	Z80_TABLE_cb,
	Z80_TABLE_ed,
	Z80_TABLE_xy,
	Z80_TABLE_xycb,
	Z80_TABLE_ex	/* cycles counts for taken jr/jp/call and interrupt latency (rst opcodes) */
};

/****************************************************************************/
/* The Z80 registers. HALT is set to 1 when the CPU is halted, the refresh	*/
/* register is calculated as follows: refresh=(Regs.R&127)|(Regs.R2&128)	*/
/****************************************************************************/
typedef struct {
/* 00 */	PAIR	PREPC,PC,SP,AF,BC,DE,HL,IX,IY;
/* 24 */	PAIR	AF2,BC2,DE2,HL2;
/* 34 */	UINT8	R,R2,IFF1,IFF2,HALT,IM,I;
/* 3B */	UINT8	irq_max;			/* number of daisy chain devices		*/
/* 3C */	INT8	request_irq;		/* daisy chain next request device		*/
/* 3D */	INT8	service_irq;		/* daisy chain next reti handling device */
/* 3E */	UINT8	nmi_state;			/* nmi line state */
/* 3F */	UINT8	irq_state;			/* irq line state */
/* 40 */	UINT8	int_state[Z80_MAXDAISY];
/* 44 */	Z80_DaisyChain irq[Z80_MAXDAISY];
/* 84 */	int		(*irq_callback)(int irqline);
/* 88 */	int		extra_cycles;		/* extra cycles for interrupts */
}	Z80_Regs;


typedef struct {
    int z80_ICount;              /* T-state count                        */
    Z80_Regs Z80;
    UINT32 EA;
    int after_EI;

    UINT8 SZ[256];		/* zero and sign flags */
    UINT8 SZ_BIT[256];	/* zero, sign and parity/overflow (=zero) flags for BIT opcode */
    UINT8 SZP[256];		/* zero, sign and parity flags */
    UINT8 SZHV_inc[256]; /* zero, sign, half carry and overflow flags INC r8 */
    UINT8 SZHV_dec[256]; /* zero, sign, half carry and overflow flags DEC r8 */

#if BIG_FLAGS_ARRAY
    UINT8 *SZHVC_add;
    UINT8 *SZHVC_sub;
#endif

    void *userdata;
} z80_state_t;

z80_state_t *z80_init(void);
void z80_free (z80_state_t *z80);
void z80_reset (z80_state_t *z80, void *param);
void z80_exit (z80_state_t *z80);
int z80_execute(z80_state_t *z80, int cycles);
void z80_burn(z80_state_t *z80, int cycles);
unsigned z80_get_context (z80_state_t *z80, void *dst);
void z80_set_context (z80_state_t *z80, void *src);
const void *z80_get_cycle_table (z80_state_t *z80, int which);
void z80_set_cycle_table (z80_state_t *z80, int which, void *new_tbl);
unsigned z80_get_reg (z80_state_t *z80, int regnum);
void z80_set_reg (z80_state_t *z80, int regnum, unsigned val);
void z80_set_irq_line(z80_state_t *z80, int irqline, int state);
void z80_set_irq_callback(z80_state_t *z80, int (*irq_callback)(int));
const char *z80_info(z80_state_t *z80, void *context, int regnum);
unsigned z80_dasm(z80_state_t *z80, char *buffer, unsigned pc);

#ifdef MAME_DEBUG
unsigned DasmZ80(z80_state_t *z80, char *buffer, unsigned pc);
#endif

void cps1_decode(unsigned char *rom, int swap_key1,int swap_key2,int addr_key,int xor_key);
#endif

