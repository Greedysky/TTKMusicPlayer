#ifndef _ARM7_THUMB_
#define _ARM7_THUMB_

#define THUMB_INSN_TYPE		((UINT16) 0xf000)
#define THUMB_COND_TYPE		((UINT16) 0x0f00)
#define THUMB_GROUP4_TYPE	((UINT16) 0x0c00)
#define THUMB_GROUP5_TYPE	((UINT16) 0x0e00)
#define THUMB_GROUP5_RM		((UINT16) 0x01c0)
#define THUMB_GROUP5_RN		((UINT16) 0x0038)
#define THUMB_GROUP5_RD		((UINT16) 0x0007)
#define THUMB_ADDSUB_RNIMM	((UINT16) 0x01c0)
#define THUMB_ADDSUB_RS		((UINT16) 0x0038)
#define THUMB_ADDSUB_RD		((UINT16) 0x0007)
#define THUMB_INSN_ADDSUB	((UINT16) 0x0800)
#define THUMB_INSN_CMP		((UINT16) 0x0800)
#define THUMB_INSN_SUB		((UINT16) 0x0800)
#define THUMB_INSN_IMM_RD	((UINT16) 0x0700)
#define THUMB_INSN_IMM_S	((UINT16) 0x0080)
#define THUMB_INSN_IMM		((UINT16) 0x00ff)
#define THUMB_ADDSUB_TYPE	((UINT16) 0x0600)
#define THUMB_HIREG_OP		((UINT16) 0x0300)
#define THUMB_HIREG_H		((UINT16) 0x00c0)
#define THUMB_HIREG_RS		((UINT16) 0x0038)
#define THUMB_HIREG_RD		((UINT16) 0x0007)
#define THUMB_STACKOP_TYPE	((UINT16) 0x0f00)
#define THUMB_STACKOP_L		((UINT16) 0x0800)
#define THUMB_STACKOP_RD	((UINT16) 0x0700)
#define THUMB_ALUOP_TYPE	((UINT16) 0x03c0)
#define THUMB_BLOP_LO		((UINT16) 0x0800)
#define THUMB_BLOP_OFFS		((UINT16) 0x07ff)
#define THUMB_SHIFT_R		((UINT16) 0x0800)
#define THUMB_SHIFT_AMT		((UINT16) 0x07c0)
#define THUMB_HALFOP_L		((UINT16) 0x0800)
#define THUMB_HALFOP_OFFS	((UINT16) 0x07c0)
#define THUMB_BRANCH_OFFS	((UINT16) 0x07ff)
#define THUMB_LSOP_L		((UINT16) 0x0800)
#define THUMB_LSOP_OFFS		((UINT16) 0x07c0)
#define THUMB_MULTLS		((UINT16) 0x0800)
#define THUMB_MULTLS_BASE	((UINT16) 0x0700)
#define THUMB_RELADDR_SP	((UINT16) 0x0800)
#define THUMB_RELADDR_RD	((UINT16) 0x0700)
#define THUMB_INSN_TYPE_SHIFT		12
#define THUMB_COND_TYPE_SHIFT		8
#define THUMB_GROUP4_TYPE_SHIFT		10
#define THUMB_GROUP5_TYPE_SHIFT		9
#define THUMB_ADDSUB_TYPE_SHIFT		9
#define THUMB_INSN_IMM_RD_SHIFT		8
#define THUMB_STACKOP_TYPE_SHIFT	8
#define THUMB_HIREG_OP_SHIFT		8
#define THUMB_STACKOP_RD_SHIFT		8
#define THUMB_MULTLS_BASE_SHIFT		8
#define THUMB_RELADDR_RD_SHIFT		8
#define THUMB_HIREG_H_SHIFT			6
#define THUMB_HIREG_RS_SHIFT		3
#define THUMB_ALUOP_TYPE_SHIFT		6
#define THUMB_SHIFT_AMT_SHIFT		6
#define THUMB_HALFOP_OFFS_SHIFT		6
#define THUMB_LSOP_OFFS_SHIFT		6
#define THUMB_GROUP5_RM_SHIFT		6
#define THUMB_GROUP5_RN_SHIFT		3
#define THUMB_GROUP5_RD_SHIFT		0
#define THUMB_ADDSUB_RNIMM_SHIFT	6
#define THUMB_ADDSUB_RS_SHIFT		3
#define THUMB_ADDSUB_RD_SHIFT		0

#define THUMB_SIGN_BIT				((UINT32)(1<<31))
#define THUMB_SIGN_BITS_DIFFER(a,b)	(((a)^(b)) >> 31)

#define N_IS_SET(pc)    ((pc) & ARM7_CPSR_N)
#define Z_IS_SET(pc)    ((pc) & ARM7_CPSR_Z)
#define C_IS_SET(pc)    ((pc) & ARM7_CPSR_C)
#define V_IS_SET(pc)    ((pc) & ARM7_CPSR_V)
#define I_IS_SET(pc)    ((pc) & ARM7_CPSR_I)
#define F_IS_SET(pc)    ((pc) & ARM7_CPSR_F)
#define T_IS_SET(pc)    ((pc) & ARM7_CPSR_T)

#define N_IS_CLEAR(pc)  (!N_IS_SET(pc))
#define Z_IS_CLEAR(pc)  (!Z_IS_SET(pc))
#define C_IS_CLEAR(pc)  (!C_IS_SET(pc))
#define V_IS_CLEAR(pc)  (!V_IS_SET(pc))
#define I_IS_CLEAR(pc)  (!I_IS_SET(pc))
#define F_IS_CLEAR(pc)  (!F_IS_SET(pc))
#define T_IS_CLEAR(pc)  (!T_IS_SET(pc))

enum
{
    COND_EQ = 0,    /* Z: equal */
    COND_NE,        /* ~Z: not equal */
    COND_CS, COND_HS = 2,   /* C: unsigned higher or same */
    COND_CC, COND_LO = 3,   /* ~C: unsigned lower */
    COND_MI,        /* N: negative */
    COND_PL,        /* ~N: positive or zero */
    COND_VS,        /* V: overflow */
    COND_VC,        /* ~V: no overflow */
    COND_HI,        /* C && ~Z: unsigned higher */
    COND_LS,        /* ~C || Z: unsigned lower or same */
    COND_GE,        /* N == V: greater or equal */
    COND_LT,        /* N != V: less than */
    COND_GT,        /* ~Z && (N == V): greater than */
    COND_LE,        /* Z || (N != V): less than or equal */
    COND_AL,        /* always */
    COND_NV         /* never */
};

#define GET_CPSR	ARM7.Rx [ARM7_CPSR]

#define GET_REGISTER(r) ARM7.Rx[(r)]
#define SET_REGISTER(r, v) ARM7.Rx[(r)] = (v)

#define R15 ARM7.Rx[ARM7_PC]


// public function
int ARM7i_Thumb_Step(void);

#endif

