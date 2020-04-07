/*
 * Z80~Ex, ZILoG Z80 CPU emulator.
 *
 * by Boo-boo [boo_boo(^at^)inbox.ru]
 * contains code from the FUSE project (http://fuse-emulator.sourceforge.net)
 * released under GNU GPL v2
 *
 */

#ifndef _Z80EX_MACROS_H_INCLUDED
#define _Z80EX_MACROS_H_INCLUDED

/* Macros used for accessing the registers */
#define A   cpu->af.b.h
#define F   cpu->af.b.l
#define AF  cpu->af.w

#define B   cpu->bc.b.h
#define C   cpu->bc.b.l
#define BC  cpu->bc.w

#define D   cpu->de.b.h
#define E   cpu->de.b.l
#define DE  cpu->de.w

#define H   cpu->hl.b.h
#define L   cpu->hl.b.l
#define HL  cpu->hl.w

#define A_  cpu->af_.b.h
#define F_  cpu->af_.b.l
#define AF_ cpu->af_.w

#define B_  cpu->bc_.b.h
#define C_  cpu->bc_.b.l
#define BC_ cpu->bc_.w

#define D_  cpu->de_.b.h
#define E_  cpu->de_.b.l
#define DE_ cpu->de_.w

#define H_  cpu->hl_.b.h
#define L_  cpu->hl_.b.l
#define HL_ cpu->hl_.w

#define IXH cpu->ix.b.h
#define IXL cpu->ix.b.l
#define IX  cpu->ix.w

#define IYH cpu->iy.b.h
#define IYL cpu->iy.b.l
#define IY  cpu->iy.w

#define SPH cpu->sp.b.h
#define SPL cpu->sp.b.l
#define SP  cpu->sp.w

#define PCH cpu->pc.b.h
#define PCL cpu->pc.b.l
#define PC  cpu->pc.w

#define I  cpu->i
#define R  cpu->r
#define R7 cpu->r7

#define IFF1 cpu->iff1
#define IFF2 cpu->iff2
#define IM   cpu->im

#define MEMPTRh cpu->memptr.b.h
#define MEMPTRl cpu->memptr.b.l
#define MEMPTR cpu->memptr.w


/* The flags */

#define FLAG_C	0x01
#define FLAG_N	0x02
#define FLAG_P	0x04
#define FLAG_V	FLAG_P
#define FLAG_3	0x08
#define FLAG_H	0x10
#define FLAG_5	0x20
#define FLAG_Z	0x40
#define FLAG_S	0x80

/*read opcode*/
#define READ_OP_M1() (cpu->int_vector_req? cpu->intread_cb(cpu, cpu->intread_cb_user_data) : cpu->mread_cb(cpu, PC++, 1, cpu->mread_cb_user_data))

/*read opcode argument*/
#define READ_OP() (cpu->int_vector_req? cpu->intread_cb(cpu, cpu->intread_cb_user_data) : cpu->mread_cb(cpu, PC++, 0, cpu->mread_cb_user_data))


#ifndef Z80EX_OPSTEP_FAST_AND_ROUGH

/*wait until end of opcode-tstate given (to be used on opcode execution)*/
#define T_WAIT_UNTIL(t_state) \
{ \
	int nn;\
	for(nn=cpu->op_tstate;nn < t_state;nn++) { \
		cpu->op_tstate++; \
		cpu->tstate++; \
		if(cpu->tstate_cb != NULL) cpu->tstate_cb(cpu, cpu->tstate_cb_user_data); \
	}\
}

/*spend <amount> t-states (not affecting opcode-tstate counter,
for using outside of certain opcode execution)*/
#define TSTATES(amount) \
{\
	int nn;\
	for(nn=0; nn < amount; nn++) \
	{ \
		cpu->tstate++; \
		if(cpu->tstate_cb != NULL) cpu->tstate_cb(cpu, cpu->tstate_cb_user_data); \
	}\
}

/*read byte from memory*/
#define READ_MEM(result, addr, t_state) \
{ \
	T_WAIT_UNTIL(t_state); \
	result=(cpu->mread_cb(cpu, (addr), 0, cpu->mread_cb_user_data)); \
}

/*read byte from port*/
#define READ_PORT(result, port, t_state) \
{ \
	T_WAIT_UNTIL(t_state); \
	result=(cpu->pread_cb(cpu, (port), cpu->pread_cb_user_data)); \
}

/*write byte to memory*/
#define WRITE_MEM(addr, vbyte, t_state) \
{ \
	T_WAIT_UNTIL(t_state); \
	cpu->mwrite_cb(cpu, addr, vbyte, cpu->mwrite_cb_user_data); \
}

/*write byte to port*/
#define WRITE_PORT(port, vbyte, t_state) \
{ \
	T_WAIT_UNTIL(t_state); \
	cpu->pwrite_cb(cpu, (port), vbyte, cpu->pwrite_cb_user_data); \
}

#else
/*Z80EX_OPSTEP_FAST_AND_ROUGH*/

#define T_WAIT_UNTIL(t_state) {cpu->tstate = t_state; cpu->op_tstate = t_state;} 

#define TSTATES(amount) {cpu->tstate += amount;}

/*read byte from memory*/
#define READ_MEM(result, addr, t_state) \
{ \
	result=(cpu->mread_cb(cpu, (addr), 0, cpu->mread_cb_user_data)); \
}

/*read byte from port*/
#define READ_PORT(result, port, t_state) \
{ \
	result=(cpu->pread_cb(cpu, (port), cpu->pread_cb_user_data)); \
}

/*write byte to memory*/
#define WRITE_MEM(addr, vbyte, t_state) \
{ \
	cpu->mwrite_cb(cpu, addr, vbyte, cpu->mwrite_cb_user_data); \
}

/*write byte to port*/
#define WRITE_PORT(port, vbyte, t_state) \
{ \
	cpu->pwrite_cb(cpu, (port), vbyte, cpu->pwrite_cb_user_data); \
}

#endif
/*<< #ifndef Z80EX_OPSTEP_FAST_AND_ROUGH*/


/* instructions */

#define AND(value)\
{\
	A &= (value);\
	F = FLAG_H | sz53p_table[A];\
}

#define ADC(a, value)\
{\
	Z80EX_WORD adctemp = A + (value) + ( F & FLAG_C ); \
	Z80EX_BYTE lookup = ( (       A & 0x88 ) >> 3 ) | \
			    ( ( (value) & 0x88 ) >> 2 ) | \
			    ( ( adctemp & 0x88 ) >> 1 );  \
	A=adctemp;\
	F = ( adctemp & 0x100 ? FLAG_C : 0 ) |\
		halfcarry_add_table[lookup & 0x07] | overflow_add_table[lookup >> 4] |\
		sz53_table[A];\
}

#define ADC16(hl, value)\
{\
	Z80EX_DWORD add16temp= HL + (value) + ( F & FLAG_C ); \
	Z80EX_BYTE lookup = ( (        HL & 0x8800 ) >> 11 ) | \
			    ( (   (value) & 0x8800 ) >> 10 ) | \
			    ( ( add16temp & 0x8800 ) >>  9 );  \
	MEMPTR=hl+1;\
	HL = add16temp;\
	F = ( add16temp & 0x10000 ? FLAG_C : 0 )|\
		overflow_add_table[lookup >> 4] |\
		( H & ( FLAG_3 | FLAG_5 | FLAG_S ) ) |\
		halfcarry_add_table[lookup&0x07]|\
		( HL ? 0 : FLAG_Z );\
}

#define ADD(a, value)\
{\
	Z80EX_WORD addtemp = A + (value); \
	Z80EX_BYTE lookup = ( (       A & 0x88 ) >> 3 ) | \
			    ( ( (value) & 0x88 ) >> 2 ) | \
			    ( ( addtemp & 0x88 ) >> 1 );  \
	A=addtemp;\
	F = ( addtemp & 0x100 ? FLAG_C : 0 ) |\
		halfcarry_add_table[lookup & 0x07] | overflow_add_table[lookup >> 4] |\
		sz53_table[A];\
}

#define ADD16(value1,value2)\
{\
	Z80EX_DWORD add16temp = (value1) + (value2); \
	Z80EX_BYTE lookup = ( (  (value1) & 0x0800 ) >> 11 ) | \
			    ( (  (value2) & 0x0800 ) >> 10 ) | \
			    ( ( add16temp & 0x0800 ) >>  9 );  \
	MEMPTR=value1+1;\
	(value1) = add16temp;\
	F = ( F & ( FLAG_V | FLAG_Z | FLAG_S ) ) |\
		( add16temp & 0x10000 ? FLAG_C : 0 )|\
		( ( add16temp >> 8 ) & ( FLAG_3 | FLAG_5 ) ) |\
		halfcarry_add_table[lookup];\
}

#define BIT( bit, value ) \
{ \
	F = ( F & FLAG_C ) | FLAG_H | sz53p_table[(value) & (0x01 << (bit))] | ((value) & 0x28); \
}

/*BIT n,(IX+d/IY+d) and BIT n,(HL)*/
#define BIT_MPTR( bit, value) \
{ \
	F = ( F & FLAG_C ) | FLAG_H | (sz53p_table[(value) & (0x01 << (bit))] & 0xD7) | ((MEMPTRh) & 0x28); \
}

#define CALL(addr, wr1, wr2) \
{\
	PUSH(PC,wr1,wr2); \
	PC=addr; \
	MEMPTR=addr;\
}

#define CP(value)\
{\
	Z80EX_WORD cptemp = A - value; \
	Z80EX_BYTE lookup = ( (       A & 0x88 ) >> 3 ) | \
			    ( ( (value) & 0x88 ) >> 2 ) | \
			    ( (  cptemp & 0x88 ) >> 1 );  \
	F = ( cptemp & 0x100 ? FLAG_C : ( cptemp ? 0 : FLAG_Z ) ) | FLAG_N |\
		halfcarry_sub_table[lookup & 0x07] |\
			overflow_sub_table[lookup >> 4] |\
			( value & ( FLAG_3 | FLAG_5 ) ) |\
			( cptemp & FLAG_S );\
}

#define DEC(value)\
{\
	F = ( F & FLAG_C ) | ( (value)&0x0f ? 0 : FLAG_H ) | FLAG_N;\
	(value)--;\
	F |= ( (value)==0x7f ? FLAG_V : 0 ) | sz53_table[value];\
}

#define DEC16(value)\
{\
	(value)--;\
}

#define INC(value)\
{\
	(value)++;\
	F = ( F & FLAG_C ) | ( (value)==0x80 ? FLAG_V : 0 ) |\
		( (value)&0x0f ? 0 : FLAG_H ) | sz53_table[(value)];\
}

#define INC16(value)\
{\
	(value)++;\
}

#define LD(dst, src) \
{\
	dst=src; \
}

/*ld (nnnn|BC|DE), A*/
#define LD_A_TO_ADDR_MPTR(dst, src, addr) \
{\
	dst=src; \
	MEMPTRh=A;\
	MEMPTRl=((addr+1) & 0xFF);\
}

/*ld a,(BC|DE|nnnn)*/
#define LD_A_FROM_ADDR_MPTR(dst, src, addr) \
{\
	dst=src; \
	MEMPTR=addr+1;\
}

#define LD16(dst, src) \
{\
	dst=src; \
}

/*ld (nnnn),BC|DE|SP|HL|IX|IY*/
#define LD_RP_TO_ADDR_MPTR_16(dst, src, addr) \
{\
	dst=src; \
	MEMPTR=addr+1;\
}

/*ld BC|DE|SP|HL|IX|IY,(nnnn)*/
#define LD_RP_FROM_ADDR_MPTR_16(dst, src, addr) \
{\
	dst=src; \
	MEMPTR=addr+1;\
}

#define JP_NO_MPTR(addr) \
{ \
	PC=addr; \
}

#define JP(addr) \
{ \
	PC=addr; \
	MEMPTR=addr;\
}

#define JR(offset) \
{\
	PC+=offset; \
	MEMPTR=PC;\
}

#define OR(value)\
{\
	A |= (value);\
	F = sz53p_table[A];\
}

#define OUT(port,reg, wr) \
{\
	WRITE_PORT(port,reg,wr); \
	MEMPTR=port+1;\
}

/*OUT (nn),A*/
#define OUT_A(port,reg, wr) \
{\
	WRITE_PORT(port,reg,wr); \
	MEMPTRl=((port+1) & 0xFF);\
	MEMPTRh=A;\
}

#define IN(reg,port,rd) \
{\
	READ_PORT(reg,port,rd); \
	F = ( F & FLAG_C) | sz53p_table[(reg)];\
	MEMPTR=port+1;\
}

/*IN A,(nn)*/
#define IN_A(reg,port,rd) \
{\
	READ_PORT(reg,port,rd); \
	MEMPTR=port+1;\
}

#define IN_F(port, rd) \
{\
	Z80EX_BYTE val; \
	READ_PORT(val, port, rd); \
	F = ( F & FLAG_C) | sz53p_table[(val)];\
	MEMPTR=port+1;\
}

#define POP(rp, rd1, rd2) \
{\
	regpair tmp; \
	READ_MEM(tmp.b.l,SP++,rd1);\
	READ_MEM(tmp.b.h,SP++,rd2);\
	rp=tmp.w;\
}

/*wr1=t-states before first byte, wr2=t-states before second*/
#define PUSH(rp, wr1, wr2) \
{\
	regpair tmp; \
	tmp.w=rp; \
	WRITE_MEM(--SP, tmp.b.h, wr1); \
	WRITE_MEM(--SP, tmp.b.l, wr2); \
}

#define RET(rd1, rd2) \
{\
	POP(PC, rd1, rd2);\
	MEMPTR=PC;\
}

#define RL(value)\
{\
	Z80EX_BYTE rltemp = (value); \
	(value) = ( (value)<<1 ) | ( F & FLAG_C );\
	F = ( rltemp >> 7 ) | sz53p_table[(value)];\
}

#define RLC(value)\
{\
	(value) = ( (value)<<1 ) | ( (value)>>7 );\
	F = ( (value) & FLAG_C ) | sz53p_table[(value)];\
}

#define RR(value)\
{\
	Z80EX_BYTE rrtemp = (value); \
	(value) = ( (value)>>1 ) | ( F << 7 );\
	F = ( rrtemp & FLAG_C ) | sz53p_table[(value)];\
}

#define RRC(value)\
{\
	F = (value) & FLAG_C;\
	(value) = ( (value)>>1 ) | ( (value)<<7 );\
	F |= sz53p_table[(value)];\
}

#define RST(value, w1, w2)\
{\
	PUSH(PC, w1, w2);\
	PC=(value);\
	MEMPTR=PC;\
}

#define SBC(a, value)\
{\
	Z80EX_WORD sbctemp = A - (value) - ( F & FLAG_C ); \
	Z80EX_BYTE lookup = ( (       A & 0x88 ) >> 3 ) | \
			    ( ( (value) & 0x88 ) >> 2 ) | \
			    ( ( sbctemp & 0x88 ) >> 1 );  \
	A=sbctemp;\
	F = ( sbctemp & 0x100 ? FLAG_C : 0 ) | FLAG_N |\
	halfcarry_sub_table[lookup & 0x07] | overflow_sub_table[lookup >> 4] |\
	sz53_table[A];\
}

#define SBC16(hl, value)\
{\
	Z80EX_DWORD sub16temp = HL - (value) - (F & FLAG_C); \
	Z80EX_BYTE lookup = ( (        HL & 0x8800 ) >> 11 ) | \
			    ( (   (value) & 0x8800 ) >> 10 ) | \
			    ( ( sub16temp & 0x8800 ) >>  9 );  \
	MEMPTR=hl+1;\
	HL = sub16temp;\
	F = ( sub16temp & 0x10000 ? FLAG_C : 0 ) |\
	FLAG_N | overflow_sub_table[lookup >> 4] |\
	( H & ( FLAG_3 | FLAG_5 | FLAG_S ) ) |\
	halfcarry_sub_table[lookup&0x07] |\
	( HL ? 0 : FLAG_Z) ;\
}

#define SLA(value)\
{\
	F = (value) >> 7;\
	(value) <<= 1;\
	F |= sz53p_table[(value)];\
}

#define SLL(value)\
{\
	F = (value) >> 7;\
	(value) = ( (value) << 1 ) | 0x01;\
	F |= sz53p_table[(value)];\
}

#define SRA(value)\
{\
	F = (value) & FLAG_C;\
	(value) = ( (value) & 0x80 ) | ( (value) >> 1 );\
	F |= sz53p_table[(value)];\
}

#define SRL(value)\
{\
	F = (value) & FLAG_C;\
	(value) >>= 1;\
	F |= sz53p_table[(value)];\
}

#define SUB(value)\
{\
	Z80EX_WORD subtemp = A - (value); \
	Z80EX_BYTE lookup = ( (       A & 0x88 ) >> 3 ) | \
			    ( ( (value) & 0x88 ) >> 2 ) | \
			    (  (subtemp & 0x88 ) >> 1 );  \
	A=subtemp;\
	F = ( subtemp & 0x100 ? FLAG_C : 0 ) | FLAG_N |\
	halfcarry_sub_table[lookup & 0x07] | overflow_sub_table[lookup >> 4] |\
	sz53_table[A];\
}

#define XOR(value)\
{\
	A ^= (value);\
	F = sz53p_table[A];\
}

#define RRD(rd, wr) \
{\
	Z80EX_BYTE bytetemp;\
	READ_MEM(bytetemp, HL, rd);\
	WRITE_MEM(HL,  ( A << 4 ) | ( bytetemp >> 4 ) ,wr);\
	A = ( A & 0xf0 ) | ( bytetemp & 0x0f );\
	F = ( F & FLAG_C ) | sz53p_table[A];\
	MEMPTR=HL+1;\
}

#define RLD(rd, wr) \
{\
	Z80EX_BYTE bytetemp;\
	READ_MEM(bytetemp, HL, rd);\
	WRITE_MEM(HL, (bytetemp << 4 ) | ( A & 0x0f ) ,wr);\
	A = ( A & 0xf0 ) | ( bytetemp >> 4 );\
	F = ( F & FLAG_C ) | sz53p_table[A];\
	MEMPTR=HL+1;\
}


#define IM_(mode)\
{\
	IM=mode;\
}

#define LD_A_R() \
{\
	A=(R&0x7f) | (R7&0x80);\
	F = ( F & FLAG_C ) | sz53_table[A] | ( IFF2 ? FLAG_V : 0 );\
	cpu->reset_PV_on_int=1;\
}

#define LD_R_A() \
{\
	R=R7=A;\
}

#define LD_A_I() \
{\
	A=I;\
	F = ( F & FLAG_C ) | sz53_table[A] | ( IFF2 ? FLAG_V : 0 );\
	cpu->reset_PV_on_int=1;\
}

#define NEG() \
{\
	Z80EX_BYTE bytetemp=A;\
	A=0;\
	SUB(bytetemp);\
}

#define RETI(rd1, rd2) \
{\
	IFF1=IFF2;\
	RET(rd1, rd2);\
}

/*same as RETI, only opcode is different*/
#define RETN(rd1, rd2) \
{\
	IFF1=IFF2;\
	RET(rd1, rd2);\
}

#define LDI(rd, wr) \
{\
	Z80EX_BYTE bytetemp;\
	READ_MEM(bytetemp, HL, rd);\
	BC--;\
	WRITE_MEM(DE,bytetemp,wr);\
	DE++; HL++;\
	bytetemp += A;\
	F = ( F & ( FLAG_C | FLAG_Z | FLAG_S ) ) | ( BC ? FLAG_V : 0 ) |\
	  ( bytetemp & FLAG_3 ) | ( (bytetemp & 0x02) ? FLAG_5 : 0 );\
}

#define CPI(rd) \
{\
	Z80EX_BYTE value,bytetemp,lookup;\
	READ_MEM(value, HL, rd);\
	bytetemp = A - value;\
	  lookup = ( (        A & 0x08 ) >> 3 ) |\
	           ( (  (value) & 0x08 ) >> 2 ) |\
	           ( ( bytetemp & 0x08 ) >> 1 );\
	HL++; BC--;\
	F = ( F & FLAG_C ) | ( BC ? ( FLAG_V | FLAG_N ) : FLAG_N ) |\
	  halfcarry_sub_table[lookup] | ( bytetemp ? 0 : FLAG_Z ) |\
	  ( bytetemp & FLAG_S );\
	if(F & FLAG_H) bytetemp--;\
	F |= ( bytetemp & FLAG_3 ) | ( (bytetemp&0x02) ? FLAG_5 : 0 );\
	MEMPTR=MEMPTR+1;\
}

/*undocumented flag effects for block output operations*/
#define OUT_BL(pbyte) \
{\
	Z80EX_BYTE kval;\
	kval=pbyte+L;\
	if((pbyte+L) > 255) F |= (FLAG_C | FLAG_H);\
	F |= parity_table[((kval & 7) ^ B)];\
}

/*undocumented flag effects for block input operations*/
#define IN_BL(pbyte, c_add) \
{\
	Z80EX_BYTE kval;\
	kval=pbyte+((C+(c_add)) & 0xff);\
	if((pbyte+((C+(c_add)) & 0xff)) > 255) F |= (FLAG_C | FLAG_H);\
	F |= parity_table[((kval & 7) ^ B)];\
}

#define INI(rd, wr) \
{\
	Z80EX_BYTE initemp;\
	MEMPTR=BC+1;\
	READ_PORT(initemp, BC, rd);\
	WRITE_MEM( HL, initemp, wr );\
	B--; HL++;\
	F = ( initemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	IN_BL(initemp,1);\
}

#define OUTI(rd, wr) \
{\
	Z80EX_BYTE outitemp;\
	READ_MEM(outitemp, HL, rd);\
	B--;	\
	MEMPTR=BC+1;\
	WRITE_PORT(BC,outitemp,wr);\
	HL++;\
	F = (outitemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	OUT_BL(outitemp);\
}

#define LDD(rd, wr) \
{\
	Z80EX_BYTE bytetemp;\
	READ_MEM(bytetemp, HL, rd);\
	BC--;\
	WRITE_MEM(DE,bytetemp,wr);\
	DE--; HL--;\
	bytetemp += A;\
	F = ( F & ( FLAG_C | FLAG_Z | FLAG_S ) ) | ( BC ? FLAG_V : 0 ) |\
	  ( bytetemp & FLAG_3 ) | ( (bytetemp & 0x02) ? FLAG_5 : 0 );\
}

#define CPD(rd) \
{\
	Z80EX_BYTE value,bytetemp,lookup;\
	READ_MEM(value, HL, rd);\
	bytetemp = A - value;\
	  lookup = ( (        A & 0x08 ) >> 3 ) |\
	           ( (  (value) & 0x08 ) >> 2 ) |\
	           ( ( bytetemp & 0x08 ) >> 1 );\
	HL--; BC--;\
	F = ( F & FLAG_C ) | ( BC ? ( FLAG_V | FLAG_N ) : FLAG_N ) |\
	  halfcarry_sub_table[lookup] | ( bytetemp ? 0 : FLAG_Z ) |\
	  ( bytetemp & FLAG_S );\
	if(F & FLAG_H) bytetemp--;\
	F |= ( bytetemp & FLAG_3 ) | ( (bytetemp&0x02) ? FLAG_5 : 0 );\
	MEMPTR=MEMPTR-1;\
}

#define IND(rd,wr) \
{\
	Z80EX_BYTE initemp;\
	MEMPTR=BC-1;\
	READ_PORT(initemp, BC, rd);\
	WRITE_MEM( HL, initemp, wr );\
	B--; HL--;\
	F = ( initemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	IN_BL(initemp,-1)\
}

#define OUTD(rd, wr) \
{\
	Z80EX_BYTE outitemp;\
	READ_MEM(outitemp, HL, rd);\
	B--;\
	MEMPTR=BC-1;\
	WRITE_PORT(BC,outitemp,wr);\
	HL--;\
	F = (outitemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	OUT_BL(outitemp);\
}

#define LDIR(t1,t2,rd,wr) \
{\
	Z80EX_BYTE bytetemp;\
	READ_MEM(bytetemp, HL, rd);\
	WRITE_MEM(DE,bytetemp,wr);\
	HL++; DE++; BC--;\
	bytetemp += A;\
	F = ( F & ( FLAG_C | FLAG_Z | FLAG_S ) ) | ( BC ? FLAG_V : 0 ) |\
	  ( bytetemp & FLAG_3 ) | ( (bytetemp & 0x02) ? FLAG_5 : 0 );\
	if(BC) {\
		PC-=2;\
		T_WAIT_UNTIL(t2);\
		MEMPTR=PC+1;\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
}

#define CPIR(t1, t2, rd) \
{\
	Z80EX_BYTE value,bytetemp,lookup;\
	READ_MEM(value, HL, rd);\
	bytetemp = A - value;\
	  lookup = ( (        A & 0x08 ) >> 3 ) |\
		   ( (  (value) & 0x08 ) >> 2 ) |\
		   ( ( bytetemp & 0x08 ) >> 1 );\
	HL++; BC--;\
	F = ( F & FLAG_C ) | ( BC ? ( FLAG_V | FLAG_N ) : FLAG_N ) |\
	  halfcarry_sub_table[lookup] | ( bytetemp ? 0 : FLAG_Z ) |\
	  ( bytetemp & FLAG_S );\
	if(F & FLAG_H) bytetemp--;\
	F |= ( bytetemp & FLAG_3 ) | ( (bytetemp&0x02) ? FLAG_5 : 0 );\
	if( ( F & ( FLAG_V | FLAG_Z ) ) == FLAG_V ) {\
		PC-=2;\
		MEMPTR=PC+1;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		MEMPTR=MEMPTR+1;\
		T_WAIT_UNTIL(t1);\
	}\
}

#define INIR(t1,t2,rd,wr) \
{\
	Z80EX_BYTE initemp;\
	READ_PORT(initemp, BC, rd);\
	WRITE_MEM( HL, initemp, wr);\
	MEMPTR=BC+1;\
	B--; HL++;\
	F = ( initemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	if( B ) {\
		PC -= 2;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
	IN_BL(initemp,1)\
}

#define OTIR(t1,t2,rd,wr) \
{\
	Z80EX_BYTE outitemp;\
	READ_MEM(outitemp, HL, rd);\
	B--;\
	MEMPTR=BC+1;\
	WRITE_PORT(BC, outitemp, wr);\
	HL++;\
	F = (outitemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	if( B ) {\
		PC -= 2;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
	OUT_BL(outitemp);\
}

#define LDDR(t1,t2,rd,wr) \
{\
	Z80EX_BYTE bytetemp;\
	READ_MEM(bytetemp, HL, rd);\
	WRITE_MEM(DE,bytetemp,wr);\
	HL--; DE--; BC--;\
	bytetemp += A;\
	F = ( F & ( FLAG_C | FLAG_Z | FLAG_S ) ) | ( BC ? FLAG_V : 0 ) |\
	  ( bytetemp & FLAG_3 ) | ( (bytetemp & 0x02) ? FLAG_5 : 0 );\
	if(BC) {\
		PC-=2;\
		T_WAIT_UNTIL(t2);\
		MEMPTR=PC+1;\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
}

#define CPDR(t1,t2,rd) \
{\
	Z80EX_BYTE value,bytetemp,lookup;\
	READ_MEM(value, HL, rd);\
	bytetemp = A - value;\
	  lookup = ( (        A & 0x08 ) >> 3 ) |\
		   ( (  (value) & 0x08 ) >> 2 ) |\
		   ( ( bytetemp & 0x08 ) >> 1 );\
	HL--; BC--;\
	F = ( F & FLAG_C ) | ( BC ? ( FLAG_V | FLAG_N ) : FLAG_N ) |\
	  halfcarry_sub_table[lookup] | ( bytetemp ? 0 : FLAG_Z ) |\
	  ( bytetemp & FLAG_S );\
	if(F & FLAG_H) bytetemp--;\
	F |= ( bytetemp & FLAG_3 ) | ( (bytetemp&0x02) ? FLAG_5 : 0 );\
	if( ( F & ( FLAG_V | FLAG_Z ) ) == FLAG_V ) {\
		PC-=2;\
		MEMPTR=PC+1;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		MEMPTR=MEMPTR-1;\
		T_WAIT_UNTIL(t1);\
	}\
}

#define INDR(t1,t2,rd,wr) \
{\
	Z80EX_BYTE initemp;\
	READ_PORT(initemp, BC, rd);\
	WRITE_MEM( HL, initemp, wr );\
	MEMPTR=BC-1;\
	B--; HL--;\
	F = ( initemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	if( B ) {\
		PC -= 2;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
	IN_BL(initemp,-1)\
}

#define OTDR(t1,t2,rd,wr) \
{\
	Z80EX_BYTE outitemp;\
	READ_MEM(outitemp, HL, rd);\
	B--;\
	MEMPTR=BC-1;\
	WRITE_PORT(BC,outitemp,wr);\
	HL--;\
	F = (outitemp & 0x80 ? FLAG_N : 0 ) | sz53_table[B];\
	if( B ) {\
		PC -= 2;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
	OUT_BL(outitemp);\
}

#define RLCA() \
{\
	A = ( A << 1 ) | ( A >> 7 );\
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |\
		( A & ( FLAG_C | FLAG_3 | FLAG_5 ) );\
}

#define RRCA() \
{\
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) | ( A & FLAG_C );\
	A = ( A >> 1) | ( A << 7 );\
	F |= ( A & ( FLAG_3 | FLAG_5 ) );\
}

#define DJNZ(offset, t1, t2) \
{\
	B--;\
	if(B) {\
		PC += offset;\
		MEMPTR=PC;\
		T_WAIT_UNTIL(t2);\
	}\
	else\
	{\
		T_WAIT_UNTIL(t1);\
	}\
}

#define RLA() \
{\
	Z80EX_BYTE bytetemp = A;\
	A = ( A << 1 ) | ( F & FLAG_C );\
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |\
	  ( A & ( FLAG_3 | FLAG_5 ) ) | ( bytetemp >> 7 );\
}

#define RRA() \
{\
	Z80EX_BYTE bytetemp = A;\
	A = ( A >> 1 ) | ( F << 7 );\
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |\
	  ( A & ( FLAG_3 | FLAG_5 ) ) | ( bytetemp & FLAG_C ) ;\
}

#define DAA() \
{\
	const Z80EX_BYTE *tdaa = (daatab+(A+0x100*((F & 3) + ((F >> 2) & 4)))*2);\
	F = *tdaa; A = *(tdaa + 1);\
}

/* old, non-exact version, from FUSE
#define DAA() \
{\
	Z80EX_BYTE add = 0, carry = ( F & FLAG_C );\
	if( ( F & FLAG_H ) || ( (A & 0x0f)>9 ) ) add=6;\
	if( carry || (A > 0x9f ) ) add|=0x60;\
	if( A > 0x99 ) carry=1;\
	if ( F & FLAG_N ) {\
	  SUB(add);\
	} else {\
	  if( (A>0x90) && ( (A & 0x0f)>9) ) add|=0x60;\
	  ADD(A, add);\
	}\
	F = ( F & ~( FLAG_C | FLAG_P) ) | carry | parity_table[A];\
}
*/

#define EX(rp1,rp2) \
{\
	Z80EX_WORD wordtemp=rp1; rp1=rp2; rp2=wordtemp;\
}

#define EX_MPTR(rp1,rp2) \
{\
	Z80EX_WORD wordtemp=rp1; rp1=rp2; rp2=wordtemp;\
	MEMPTR=wordtemp;\
}

#define CPL() \
{\
	A ^= 0xff;\
	F = ( F & ( FLAG_C | FLAG_P | FLAG_Z | FLAG_S ) ) |\
		( A & ( FLAG_3 | FLAG_5 ) ) | ( FLAG_N | FLAG_H );\
}

#define SCF() \
{\
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |\
		( A & ( FLAG_3 | FLAG_5          ) ) |\
		FLAG_C;\
}

#define CCF() \
{\
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |\
		( ( F & FLAG_C ) ? FLAG_H : FLAG_C ) | ( A & ( FLAG_3 | FLAG_5 ) );\
}

#define HALT() \
{\
	cpu->halted=1;\
	PC--;\
}

#define EXX() \
{\
	Z80EX_WORD wordtemp;\
	wordtemp = BC; BC = BC_; BC_ = wordtemp;\
	wordtemp = DE; DE = DE_; DE_ = wordtemp;\
	wordtemp = HL; HL = HL_; HL_ = wordtemp;\
}

#define DI() \
{\
	IFF1=IFF2=0;\
}

#define EI() \
{\
	IFF1 = IFF2 = 1;\
	cpu->noint_once=1;\
}

#define SET(bit, val) \
{\
	val |= (1 << bit);\
}

#define RES(bit, val) \
{\
	val &= ~(1 << bit);\
}


#endif	
