//
// ARM7 processor emulator - THUMB support
// version 2.0 / 2008-02-08
// By R. Belmont and SGINut
//

#include "arm7.h"
#include "arm7i.h"
#include "arm7thumb.h"

// base cycle counts for Thumb instructions
static const int thumbCycles[256] =
{
//  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 1
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 3
    1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // 4
    2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // 5
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,  // 6
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,  // 7
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,  // 8
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,  // 9
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // a
    1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 4, 1, 1,  // b
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // c
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,  // d
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // e
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2   // f
};

// utility macros

#define IsNeg(i) ((i) >> 31)
#define IsPos(i) ((~(i)) >> 31)

#define N_BIT   31
#define Z_BIT   30
#define C_BIT   29
#define V_BIT   28
#define SIGN_BIT                ((UINT32)(1<<31))

#define HandleThumbALUAddFlags(rd, rn, op2) \
    ARM7_SetCPSR( \
      ((GET_CPSR &~ (ARM7_CPSR_N | ARM7_CPSR_Z | ARM7_CPSR_V | ARM7_CPSR_C)) \
      | (((!THUMB_SIGN_BITS_DIFFER(rn, op2)) && THUMB_SIGN_BITS_DIFFER(rn, rd)) \
          << V_BIT) \
      | (((~(rn)) < (op2)) << C_BIT) \
      | HandleALUNZFlags(rd))); \
  	R15 += 2;

#define HandleThumbALUSubFlags(rd, rn, op2) \
    ARM7_SetCPSR( \
      ((GET_CPSR &~ (ARM7_CPSR_N | ARM7_CPSR_Z | ARM7_CPSR_V | ARM7_CPSR_C)) \
      | ((THUMB_SIGN_BITS_DIFFER(rn, op2) && THUMB_SIGN_BITS_DIFFER(rn, rd)) \
          << V_BIT) \
      | (((IsNeg(rn) & IsPos(op2)) | (IsNeg(rn) & IsPos(rd)) | (IsPos(op2) & IsPos(rd))) ? ARM7_CPSR_C : 0) \
      | HandleALUNZFlags(rd))); \
	R15 += 2;

#define HandleALUNZFlags(rd) \
  (((rd) & SIGN_BIT) | ((!(rd)) << Z_BIT))

// memory accessors
#include "arm7memil.c"

// public functions
int ARM7i_Thumb_Step()
{
	UINT32 readword;
	UINT32 addr, insn;
	UINT32 rm, rn, rs, rd, op2, imm, rrs, rrd;
	INT32 offs;
	UINT32 pc = ARM7.Rx[ARM7_PC];
	int cycles;

	insn = arm7_read_16(pc & (~1));

	cycles = (3 - thumbCycles[insn >> 8]);

	switch( ( insn & THUMB_INSN_TYPE ) >> THUMB_INSN_TYPE_SHIFT )
	{
		case 0x0: /* Logical shifting */
			ARM7_SetCPSR(GET_CPSR &~ (ARM7_CPSR_N | ARM7_CPSR_Z));
			if( insn & THUMB_SHIFT_R ) /* Shift right */
			{
				rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
				rrs = GET_REGISTER(rs);
				offs = ( insn & THUMB_SHIFT_AMT ) >> THUMB_SHIFT_AMT_SHIFT;
				if( offs != 0 )
				{
					SET_REGISTER( rd, rrs >> offs );
				if( rrs & ( 1 << (offs-1) ) )
				{
					ARM7_SetCPSR(GET_CPSR | ARM7_CPSR_C);
				}
				else
				{
					ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_C);
				}
				}
				else
				{
					SET_REGISTER( rd, 0 );
					if( rrs & 0x80000000 )
					{
					        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
					}
					else
					{
					        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
					}
				}
				ARM7_SetCPSR(GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
				ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
			        R15 += 2;
			}
			else /* Shift left */
			{
				rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
				rrs = GET_REGISTER(rs);
				offs = ( insn & THUMB_SHIFT_AMT ) >> THUMB_SHIFT_AMT_SHIFT;
				if( offs != 0 )
				{
					SET_REGISTER( rd, rrs << offs );
					if( rrs & ( 1 << ( 31 - ( offs - 1 ) ) ) )
					{
						ARM7_SetCPSR(GET_CPSR | ARM7_CPSR_C);
					}
					else
					{
						ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_C);
					}
				}
				else
				{
					SET_REGISTER( rd, rrs );
				}
				ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
				ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
				R15 += 2;
			}
			break;
		case 0x1: /* Arithmetic */
			if( insn & THUMB_INSN_ADDSUB )
			{
				switch( ( insn & THUMB_ADDSUB_TYPE ) >> THUMB_ADDSUB_TYPE_SHIFT )
				{
					case 0x0: /* ADD Rd, Rs, Rn */
						rn = GET_REGISTER( ( insn & THUMB_ADDSUB_RNIMM ) >> THUMB_ADDSUB_RNIMM_SHIFT );
						rs = GET_REGISTER( ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT );
						rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
						SET_REGISTER( rd, rs + rn );
						HandleThumbALUAddFlags( GET_REGISTER(rd), rs, rn );
						break;
					case 0x1: /* SUB Rd, Rs, Rn */
						rn = GET_REGISTER( ( insn & THUMB_ADDSUB_RNIMM ) >> THUMB_ADDSUB_RNIMM_SHIFT );
						rs = GET_REGISTER( ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT );
						rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
						SET_REGISTER( rd, rs - rn );
						HandleThumbALUSubFlags( GET_REGISTER(rd), rs, rn );
						break;
					case 0x2: /* ADD Rd, Rs, #imm */
						imm = ( insn & THUMB_ADDSUB_RNIMM ) >> THUMB_ADDSUB_RNIMM_SHIFT;
						rs = GET_REGISTER( ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT );
						rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
						SET_REGISTER( rd, rs + imm );
						HandleThumbALUAddFlags( GET_REGISTER(rd), rs, imm );
						break;
					case 0x3: /* SUB Rd, Rs, #imm */
						imm = ( insn & THUMB_ADDSUB_RNIMM ) >> THUMB_ADDSUB_RNIMM_SHIFT;
						rs = GET_REGISTER( ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT );
						rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
						SET_REGISTER( rd, rs - imm );
						HandleThumbALUSubFlags( GET_REGISTER(rd), rs,imm );
						break;
					default:
						printf("%08x: G1 Undefined Thumb instruction: %04x\n", pc, insn);
						R15 += 2;
						break;
				}
			}
			else
			{
				/* ASR.. */
				//if( insn & THUMB_SHIFT_R ) /* Shift right */
				{
					rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
					rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
					rrs = GET_REGISTER(rs);
					offs = ( insn & THUMB_SHIFT_AMT ) >> THUMB_SHIFT_AMT_SHIFT;
					if( offs == 0 )
					{
						offs = 32;
					}

					if( offs >= 32 )
					{
					        if( rrs >> 31 )
					{
						ARM7_SetCPSR(GET_CPSR | ARM7_CPSR_C);
					}
					else
					{
						ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_C);
					}
					        SET_REGISTER( rd, ( rrs & 0x80000000 ) ? 0xFFFFFFFF : 0x00000000 );
					}
					else
					{
					        if( ( rrs >> ( offs - 1 ) ) & 1 )
					        {
					        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
					        }
					        else
					        {
					        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
					        }
					        SET_REGISTER( rd, ( rrs & 0x80000000 ) ? ( ( 0xFFFFFFFF << ( 32 - offs ) ) | ( rrs >> offs ) ) : ( rrs >> offs ) );
					}
					ARM7_SetCPSR(GET_CPSR &~ (ARM7_CPSR_N | ARM7_CPSR_Z));
					ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
					R15 += 2;
				}

			}
			break;
		case 0x2: /* CMP / MOV */
			if( insn & THUMB_INSN_CMP )
			{
				rn = GET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT );
				op2 = ( insn & THUMB_INSN_IMM );
				rd = rn - op2;
				HandleThumbALUSubFlags( rd, rn, op2 );
				//mame_printf_debug("%08x: xxx Thumb instruction: CMP R%d (%08x), %02x (N=%d, Z=%d, C=%d, V=%d)\n", pc, ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT, GET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT ), op2, N_IS_SET(GET_CPSR) ? 1 : 0, Z_IS_SET(GET_CPSR) ? 1 : 0, C_IS_SET(GET_CPSR) ? 1 : 0, V_IS_SET(GET_CPSR) ? 1 : 0);
			}
			else
			{
				rd = ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT;
				op2 = ( insn & THUMB_INSN_IMM );
				SET_REGISTER( rd, op2 );
				ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
				ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
				R15 += 2;
			}
			break;
		case 0x3: /* ADD/SUB immediate */
			if( insn & THUMB_INSN_SUB ) /* SUB Rd, #Offset8 */
			{
				rn = GET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT );
				op2 = ( insn & THUMB_INSN_IMM );
				//mame_printf_debug("%08x:  Thumb instruction: SUB R%d, %02x\n", pc, ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT, op2);
				rd = rn - op2;
				SET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT, rd );
				HandleThumbALUSubFlags( rd, rn, op2 );
			}
			else /* ADD Rd, #Offset8 */
			{
				rn = GET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT );
				op2 = insn & THUMB_INSN_IMM;
				rd = rn + op2;
				//mame_printf_debug("%08x:  Thumb instruction: ADD R%d, %02x\n", pc, ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT, op2);
				SET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT, rd );
				HandleThumbALUAddFlags( rd, rn, op2 );
			}
			break;
		case 0x4: /* Rd & Rm instructions */
			switch( ( insn & THUMB_GROUP4_TYPE ) >> THUMB_GROUP4_TYPE_SHIFT )
			{
				case 0x0:
					switch( ( insn & THUMB_ALUOP_TYPE ) >> THUMB_ALUOP_TYPE_SHIFT )
					{
						case 0x0: /* AND Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							SET_REGISTER( rd, GET_REGISTER(rd) & GET_REGISTER(rs) );
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0x1: /* EOR Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							SET_REGISTER( rd, GET_REGISTER(rd) ^ GET_REGISTER(rs) );
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0x2: /* LSL Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							rrd = GET_REGISTER(rd);
							offs = GET_REGISTER(rs) & 0x000000ff;
							if (offs > 0)
							{
								if ( offs < 32 )
								{
								        SET_REGISTER( rd, rrd << offs );
								        if( rrd & ( 1 << ( 31 - ( offs - 1 ) ) ) )
								        {
									        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
								        }
								        else
								        {
									        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
								        }
								}
								else if( offs == 32 )
								{
								        SET_REGISTER( rd, 0 );
								        if( rrd & 1 )
								        {
									        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
								        }
								        else
								        {
									        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
								        }
								}
								else
								{
								        SET_REGISTER( rd, 0 );
								        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
								}
							}
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0x3: /* LSR Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							rrd = GET_REGISTER(rd);
							offs = GET_REGISTER(rs) & 0x000000ff;
							if (offs >  0)
							{
								if( offs < 32 )
								{
								        SET_REGISTER( rd, rrd >> offs );
								        if( rrd & ( 1 << ( offs - 1 ) ) )
								        {
									        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
								        }
								        else
								        {
									        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
								        }
								}
								else if( offs == 32 )
								{
								        SET_REGISTER( rd, 0 );
								        if( rrd & 0x80000000 )
								        {
									        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
								        }
								        else
								        {
									        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
								        }
								}
								else
								{
								        SET_REGISTER( rd, 0 );
								        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
								}
							}
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0x4: /* ASR Rd, Rs */
							{
								rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
								rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
								rrs = GET_REGISTER(rs)&0xff;
								rrd = GET_REGISTER(rd);
								if (rrs != 0)
								{
									if (rrs >= 32)
									{
										if (rrd>>31)
										{
											ARM7_SetCPSR(GET_CPSR | ARM7_CPSR_C);
										}
										else
										{
											ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_C);
										}
										SET_REGISTER( rd, (GET_REGISTER(rd) & 0x80000000) ? 0xFFFFFFFF : 0x00000000 );
									}
									else
									{
										if ((rrd>>(rs-1))&1)
										{
											ARM7_SetCPSR(GET_CPSR | ARM7_CPSR_C);
										}
										else
										{
											ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_C);
										}
										SET_REGISTER( rd, (rrd & 0x80000000) ? ((0xFFFFFFFF<<(32-rrs)) | (rrd>>rrs)) : (rrd>>rrs));
									}
								}
								ARM7_SetCPSR(GET_CPSR &~ (ARM7_CPSR_N | ARM7_CPSR_Z));
								ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
								R15 += 2;
							}
							break;
						case 0x5: /* ADC Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							op2=(GET_CPSR & ARM7_CPSR_C) ? 1 : 0;
							rn=GET_REGISTER(rd) + GET_REGISTER(rs) + op2;
							HandleThumbALUAddFlags( rn, GET_REGISTER(rd), ( GET_REGISTER(rs) ) ); //?
							SET_REGISTER( rd, rn);
							break;
						case 0x6: /* SBC Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							op2=(GET_CPSR & ARM7_CPSR_C) ? 0 : 1;
							rn=GET_REGISTER(rd) - GET_REGISTER(rs) - op2;
							HandleThumbALUSubFlags( rn, GET_REGISTER(rd), ( GET_REGISTER(rs) ) ); //?
							SET_REGISTER( rd, rn);
							break;
						case 0x7: /* ROR Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
                     					rrd = GET_REGISTER(rd);
							imm = GET_REGISTER(rs) & 0x0000001f;
							SET_REGISTER( rd, ( rrd >> imm ) | ( rrd << ( 32 - imm ) ) );
							if( rrd & ( 1 << ( imm - 1 ) ) )
							{
							        ARM7_SetCPSR( GET_CPSR | ARM7_CPSR_C );
							}
							else
							{
							        ARM7_SetCPSR( GET_CPSR &~ ARM7_CPSR_C );
							}
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0x8: /* TST Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) & GET_REGISTER(rs) ) );
							R15 += 2;
							break;
						case 0x9: /* NEG Rd, Rs - todo: check me */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
				                        rrs = GET_REGISTER(rs);
							rn = 0 - rrs;
							SET_REGISTER( rd, rn );
							HandleThumbALUSubFlags( GET_REGISTER(rd), 0, rrs );
							break;
						case 0xa: /* CMP Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							rn = GET_REGISTER(rd) - GET_REGISTER(rs);
							HandleThumbALUSubFlags( rn, GET_REGISTER(rd), GET_REGISTER(rs) );
							break;
						case 0xb: /* CMN Rd, Rs - check flags, add dasm */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							rn = GET_REGISTER(rd) + GET_REGISTER(rs);
							HandleThumbALUAddFlags( rn, GET_REGISTER(rd), GET_REGISTER(rs) );
							break;
						case 0xc: /* ORR Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							SET_REGISTER( rd, GET_REGISTER(rd) | GET_REGISTER(rs) );
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0xd: /* MUL Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							rn = GET_REGISTER(rd) * GET_REGISTER(rs);
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							SET_REGISTER( rd, rn );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( rn ) );
							R15 += 2;
							break;
						case 0xe: /* BIC Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							SET_REGISTER( rd, GET_REGISTER(rd) & (~GET_REGISTER(rs)) );
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						case 0xf: /* MVN Rd, Rs */
							rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
							rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
							op2 = GET_REGISTER(rs);
							SET_REGISTER( rd, ~op2 );
							ARM7_SetCPSR( GET_CPSR &~ ( ARM7_CPSR_Z | ARM7_CPSR_N ) );
							ARM7_SetCPSR( GET_CPSR | HandleALUNZFlags( GET_REGISTER(rd) ) );
							R15 += 2;
							break;
						default:
							printf("%08x: G4-0 Undefined Thumb instruction: %04x %x\n", pc, insn, ( insn & THUMB_ALUOP_TYPE ) >> THUMB_ALUOP_TYPE_SHIFT);
							R15 += 2;
							break;
					}
					break;
				case 0x1:
					switch( ( insn & THUMB_HIREG_OP ) >> THUMB_HIREG_OP_SHIFT )
					{
						case 0x0: /* ADD Rd, Rs */
							rs = ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT;
							rd = insn & THUMB_HIREG_RD;
							switch( ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT )
							{
								case 0x1: /* ADD Rd, HRs */
					      				SET_REGISTER( rd, GET_REGISTER(rd) + GET_REGISTER(rs+8) );
									// emulate the effects of pre-fetch
									if (rs == 7)
									{
										SET_REGISTER(rd, GET_REGISTER(rd) + 4);
									}
									break;
								case 0x2: /* ADD HRd, Rs */
									SET_REGISTER( rd+8, GET_REGISTER(rd+8) + GET_REGISTER(rs) );
									if (rd == 7)
									{
										R15 += 2;
										change_pc(R15);
									}
									break;
								case 0x3: /* Add HRd, HRs */
									SET_REGISTER( rd+8, GET_REGISTER(rd+8) + GET_REGISTER(rs+8) );
									// emulate the effects of pre-fetch
									if (rs == 7)
									{
										SET_REGISTER(rd+8, GET_REGISTER(rd+8) + 4);
									}
									if (rd == 7)
									{
										R15 += 2;
										change_pc(R15);
									}
									break;
								default:
									printf("%08x: G4-1-0 Undefined Thumb instruction: %04x %x\n", pc, insn, ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT );
									break;
							}
							R15 += 2;
							break;
						case 0x1: /* CMP */
							switch( ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT )
							{
								case 0x0: /* CMP Rd, Rs */
									rs = GET_REGISTER( ( ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT ) );
									rd = GET_REGISTER( insn & THUMB_HIREG_RD );
									rn = rd - rs;
									HandleThumbALUSubFlags( rn, rd, rs );
									break;
								case 0x1: /* CMP Rd, Hs */
									rs = GET_REGISTER( ( ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT ) + 8 );
									rd = GET_REGISTER( insn & THUMB_HIREG_RD );
									rn = rd - rs;
									HandleThumbALUSubFlags( rn, rd, rs );
									break;
								case 0x2: /* CMP Hd, Rs */
									rs = GET_REGISTER( ( ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT ) );
									rd = GET_REGISTER( (insn & THUMB_HIREG_RD) + 8 );
									rn = rd - rs;
									HandleThumbALUSubFlags( rn, rd, rs );
									break;
								case 0x3: /* CMP Hd, Hs */
									rs = GET_REGISTER( ( ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT ) + 8 );
									rd = GET_REGISTER( (insn & THUMB_HIREG_RD) + 8 );
									rn = rd - rs;
									HandleThumbALUSubFlags( rn, rd, rs );
									break;
								default:
									printf("%08x: G4-1 Undefined Thumb instruction: %04x %x\n", pc, insn, ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT);
									R15 += 2;
									break;
							}
							break;
						case 0x2: /* MOV */
							switch( ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT )
							{
								case 0x1:	// MOV Rd, Hs
									rs = ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT;
									rd = insn & THUMB_HIREG_RD;
									if( rs == 7 )
									{
										SET_REGISTER( rd, GET_REGISTER(rs + 8) + 4 );
									}
									else
									{
										SET_REGISTER( rd, GET_REGISTER(rs + 8) );
									}
									R15 += 2;
									break;
								case 0x2:	// MOV Hd, Rs
									rs = ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT;
									rd = insn & THUMB_HIREG_RD;
									SET_REGISTER( rd + 8, GET_REGISTER(rs) );
									if( rd != 7 )
									{
										R15 += 2;
									}
									else
				                                        {
                                            					R15 &= ~1;
										change_pc(R15);
                                        				}
									break;
								case 0x3:	// MOV Hd, Hs
									rs = ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT;
									rd = insn & THUMB_HIREG_RD;
									if (rs == 7)
									{
										SET_REGISTER( rd + 8, GET_REGISTER(rs+8)+4 );
									}
									else
									{
									SET_REGISTER( rd + 8, GET_REGISTER(rs+8) );
									}
									if( rd != 7 )
									{
										R15 += 2;
									}

								        if( rd == 7 )
				                                        {
                                            					R15 &= ~1;
										change_pc(R15);
                                        				}
									break;
								default:
									printf("%08x: G4-2 Undefined Thumb instruction: %04x (%x)\n", pc, insn, ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT);
									R15 += 2;
									break;
							}
							break;
						case 0x3:
							switch( ( insn & THUMB_HIREG_H ) >> THUMB_HIREG_H_SHIFT )
							{
								case 0x0:
									rd = ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT;
									addr = GET_REGISTER(rd);
									if( addr & 1 )
									{
										addr &= ~1;
									}
									else
									{
										ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_T);
										if( addr & 2 )
										{
											addr += 2;
										}
									}
									R15 = addr;
									break;
								case 0x1:
									addr = GET_REGISTER( ( ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT ) + 8 );
									if( ( ( ( insn & THUMB_HIREG_RS ) >> THUMB_HIREG_RS_SHIFT ) + 8 ) == 15 )
									{
										addr += 2;
									}
									if( addr & 1 )
									{
										addr &= ~1;
									}
									else
									{
										ARM7_SetCPSR(GET_CPSR &~ ARM7_CPSR_T);
										if( addr & 2 )
										{
											addr += 2;
										}
									}
									R15 = addr;
									break;
								default:
									printf("%08x: G4-3 Undefined Thumb instruction: %04x\n", pc, insn);
									R15 += 2;
									break;
							}
							break;
						default:
							printf("%08x: G4-x Undefined Thumb instruction: %04x\n", pc, insn);
							R15 += 2;
							break;
					}
					break;
				case 0x2:
				case 0x3:
					readword = arm7_read_32( ( R15 & ~2 ) + 4 + ( ( insn & THUMB_INSN_IMM ) << 2 ) );
					SET_REGISTER( ( insn & THUMB_INSN_IMM_RD ) >> THUMB_INSN_IMM_RD_SHIFT, readword );
					R15 += 2;
					break;
				default:
					printf("%08x: G4-y Undefined Thumb instruction: %04x\n", pc, insn);
					R15 += 2;
					break;
			}
			break;
		case 0x5: /* LDR* STR* */
			switch( ( insn & THUMB_GROUP5_TYPE ) >> THUMB_GROUP5_TYPE_SHIFT )
			{
				case 0x0: /* STR Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					arm7_write_32( addr, GET_REGISTER(rd) );
					R15 += 2;
					break;
				case 0x1: /* STRH Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					arm7_write_16( addr, GET_REGISTER(rd) );
					R15 += 2;
					break;
				case 0x2: /* STRB Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
 					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
 					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
 					arm7_write_8( addr, GET_REGISTER(rd) );
					R15 += 2;
					break;
				case 0x3: /* LDSB Rd, [Rn, Rm] todo, add dasm */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					op2 = arm7_read_8( addr );
					if( op2 & 0x00000080 )
					{
						op2 |= 0xffffff00;
					}
					SET_REGISTER( rd, op2 );
					R15 += 2;
					break;
				case 0x4: /* LDR Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					op2 = arm7_read_32( addr );
					SET_REGISTER( rd, op2 );
					R15 += 2;
					break;
				case 0x5: /* LDRH Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					op2 = arm7_read_16( addr );
					SET_REGISTER( rd, op2 );
					R15 += 2;
					break;

				case 0x6: /* LDRB Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					op2 = arm7_read_8( addr );
					SET_REGISTER( rd, op2 );
					R15 += 2;
					break;
				case 0x7: /* LDSH Rd, [Rn, Rm] */
					rm = ( insn & THUMB_GROUP5_RM ) >> THUMB_GROUP5_RM_SHIFT;
					rn = ( insn & THUMB_GROUP5_RN ) >> THUMB_GROUP5_RN_SHIFT;
					rd = ( insn & THUMB_GROUP5_RD ) >> THUMB_GROUP5_RD_SHIFT;
					addr = GET_REGISTER(rn) + GET_REGISTER(rm);
					op2 = arm7_read_16( addr );
					if( op2 & 0x00008000 )
					{
						op2 |= 0xffff0000;
					}
					SET_REGISTER( rd, op2 );
					R15 += 2;
					break;
				default:
					printf("%08x: G5 Undefined Thumb instruction: %04x\n", pc, insn);
					R15 += 2;
					break;
			}
			break;
		case 0x6: /* Word Store w/ Immediate Offset */
			if( insn & THUMB_LSOP_L ) /* Load */
			{
				rn = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = insn & THUMB_ADDSUB_RD;
				offs = ( ( insn & THUMB_LSOP_OFFS ) >> THUMB_LSOP_OFFS_SHIFT ) << 2;
				SET_REGISTER( rd, arm7_read_32(GET_REGISTER(rn) + offs) ); // fix
				R15 += 2;
			}
			else /* Store */
			{
				rn = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = insn & THUMB_ADDSUB_RD;
				offs = ( ( insn & THUMB_LSOP_OFFS ) >> THUMB_LSOP_OFFS_SHIFT ) << 2;
				arm7_write_32( GET_REGISTER(rn) + offs, GET_REGISTER(rd) );
				R15 += 2;
			}
			break;
		case 0x7: /* Byte Store w/ Immeidate Offset */
			if( insn & THUMB_LSOP_L ) /* Load */
			{
				rn = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = insn & THUMB_ADDSUB_RD;
				offs = ( insn & THUMB_LSOP_OFFS ) >> THUMB_LSOP_OFFS_SHIFT;
				SET_REGISTER( rd, arm7_read_8( GET_REGISTER(rn) + offs ) );
				R15 += 2;
			}
			else /* Store */
			{
				rn = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = insn & THUMB_ADDSUB_RD;
				offs = ( insn & THUMB_LSOP_OFFS ) >> THUMB_LSOP_OFFS_SHIFT;
				arm7_write_8( GET_REGISTER(rn) + offs, GET_REGISTER(rd) );
				R15 += 2;
			}
			break;
		case 0x8: /* Load/Store Halfword */
			if( insn & THUMB_HALFOP_L ) /* Load */
			{
				imm = ( insn & THUMB_HALFOP_OFFS ) >> THUMB_HALFOP_OFFS_SHIFT;
				rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
				SET_REGISTER( rd, arm7_read_16( GET_REGISTER(rs) + ( imm << 1 ) ) );
				R15 += 2;
			}
			else /* Store */
			{
				imm = ( insn & THUMB_HALFOP_OFFS ) >> THUMB_HALFOP_OFFS_SHIFT;
				rs = ( insn & THUMB_ADDSUB_RS ) >> THUMB_ADDSUB_RS_SHIFT;
				rd = ( insn & THUMB_ADDSUB_RD ) >> THUMB_ADDSUB_RD_SHIFT;
				arm7_write_16( GET_REGISTER(rs) + ( imm << 1 ), GET_REGISTER(rd) );
				R15 += 2;
			}
			break;
		case 0x9: /* Stack-Relative Load/Store */
			if( insn & THUMB_STACKOP_L )
			{
				rd = ( insn & THUMB_STACKOP_RD ) >> THUMB_STACKOP_RD_SHIFT;
				offs = (UINT8)( insn & THUMB_INSN_IMM );
				readword = arm7_read_32( GET_REGISTER(13) + ( (UINT32)offs << 2 ) );
				SET_REGISTER( rd, readword );
				R15 += 2;
			}
			else
			{
				rd = ( insn & THUMB_STACKOP_RD ) >> THUMB_STACKOP_RD_SHIFT;
				offs = (UINT8)( insn & THUMB_INSN_IMM );
				arm7_write_32( GET_REGISTER(13) + ( (UINT32)offs << 2 ), GET_REGISTER(rd) );
				R15 += 2;
			}
			break;
		case 0xa: /* Get relative address */
			if( insn & THUMB_RELADDR_SP ) /* ADD Rd, SP, #nn */
			{
				rd = ( insn & THUMB_RELADDR_RD ) >> THUMB_RELADDR_RD_SHIFT;
				offs = (UINT8)( insn & THUMB_INSN_IMM ) << 2;
				SET_REGISTER( rd, GET_REGISTER(13) + offs );
				R15 += 2;
			}
			else /* ADD Rd, PC, #nn */
			{
				rd = ( insn & THUMB_RELADDR_RD ) >> THUMB_RELADDR_RD_SHIFT;
				offs = (UINT8)( insn & THUMB_INSN_IMM ) << 2;
				SET_REGISTER( rd, ( ( R15 + 4 ) & ~2 ) + offs );
				R15 += 2;
			}
			break;
		case 0xb: /* Stack-Related Opcodes */
			switch( ( insn & THUMB_STACKOP_TYPE ) >> THUMB_STACKOP_TYPE_SHIFT )
			{
				case 0x0: /* ADD SP, #imm */
					addr = ( insn & THUMB_INSN_IMM );
					addr &= ~THUMB_INSN_IMM_S;
					SET_REGISTER( 13, GET_REGISTER(13) + ( ( insn & THUMB_INSN_IMM_S ) ? -( addr << 2 ) : ( addr << 2 ) ) );
					R15 += 2;
					break;
				case 0x4: /* PUSH {Rlist} */
					for( offs = 7; offs >= 0; offs-- )
					{
						if( insn & ( 1 << offs ) )
						{
							SET_REGISTER( 13, GET_REGISTER(13) - 4 );
							arm7_write_32( GET_REGISTER(13), GET_REGISTER(offs) );
						}
					}
					R15 += 2;
					break;
				case 0x5: /* PUSH {Rlist}{LR} */
					SET_REGISTER( 13, GET_REGISTER(13) - 4 );
					arm7_write_32( GET_REGISTER(13), GET_REGISTER(14) );
					for( offs = 7; offs >= 0; offs-- )
					{
						if( insn & ( 1 << offs ) )
						{
							SET_REGISTER( 13, GET_REGISTER(13) - 4 );
							arm7_write_32( GET_REGISTER(13), GET_REGISTER(offs) );
						}
					}
					R15 += 2;
					break;
				case 0xc: /* POP {Rlist} */
					for( offs = 0; offs < 8; offs++ )
					{
						if( insn & ( 1 << offs ) )
						{
							SET_REGISTER( offs, arm7_read_32( GET_REGISTER(13) ) );
							SET_REGISTER( 13, GET_REGISTER(13) + 4 );
						}
					}
					R15 += 2;
					break;
				case 0xd: /* POP {Rlist}{PC} */
					for( offs = 0; offs < 8; offs++ )
					{
						if( insn & ( 1 << offs ) )
						{
							SET_REGISTER( offs, arm7_read_32( GET_REGISTER(13) ) );
							SET_REGISTER( 13, GET_REGISTER(13) + 4 );
						}
					}
					R15 = arm7_read_32( GET_REGISTER(13) ) & ~1;
					SET_REGISTER( 13, GET_REGISTER(13) + 4 );
					break;
				default:
					printf("%08x: Gb Undefined Thumb instruction: %04x\n", pc, insn);
					R15 += 2;
					break;
			}
			break;
		case 0xc: /* Multiple Load/Store */
			if( insn & THUMB_MULTLS ) /* Load */
			{
				rd = ( insn & THUMB_MULTLS_BASE ) >> THUMB_MULTLS_BASE_SHIFT;
				for( offs = 0; offs < 8; offs++ )
				{
					if( insn & ( 1 << offs ) )
					{
						SET_REGISTER( offs, arm7_read_32( (GET_REGISTER(rd)&0xfffffffc) ) );
						SET_REGISTER( rd, GET_REGISTER(rd) + 4 );
					}
				}
				R15 += 2;
			}
			else /* Store */
			{
				rd = ( insn & THUMB_MULTLS_BASE ) >> THUMB_MULTLS_BASE_SHIFT;
				for( offs = 0; offs < 8; offs++ )
				{
					if( insn & ( 1 << offs ) )
					{
						arm7_write_32( (GET_REGISTER(rd)&0xfffffffc), GET_REGISTER(offs) );
						SET_REGISTER( rd, GET_REGISTER(rd) + 4 );
					}
				}
				R15 += 2;
			}
			break;
		case 0xd: /* Conditional Branch */
			offs = (INT8)( insn & THUMB_INSN_IMM );
			switch( ( insn & THUMB_COND_TYPE ) >> THUMB_COND_TYPE_SHIFT )
			{
				case COND_EQ:
		        	    	if( Z_IS_SET(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_NE:
		        	    	if( Z_IS_CLEAR(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_CS:
		        	    	if( C_IS_SET(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_CC:
		        	    	if( C_IS_CLEAR(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_MI:
		        	    	if( N_IS_SET(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_PL:
		        	    	if( N_IS_CLEAR(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_VS:
		        	    	if( V_IS_SET(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_VC:
		        	    	if( V_IS_CLEAR(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_HI:
		        	    	if( C_IS_SET(GET_CPSR) && Z_IS_CLEAR(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_LS:
		        	    	if( C_IS_CLEAR(GET_CPSR) || Z_IS_SET(GET_CPSR) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_GE:
		        	    	if( !(GET_CPSR & ARM7_CPSR_N) == !(GET_CPSR & ARM7_CPSR_V) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_LT:
		        	    	if( !(GET_CPSR & ARM7_CPSR_N) != !(GET_CPSR & ARM7_CPSR_V) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_GT:
		        	    	if( Z_IS_CLEAR(GET_CPSR) && ( !(GET_CPSR & ARM7_CPSR_N) == !(GET_CPSR & ARM7_CPSR_V) ) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_LE:
		        	    	if( Z_IS_SET(GET_CPSR) || ( !(GET_CPSR & ARM7_CPSR_N) != !(GET_CPSR & ARM7_CPSR_V) ) )
		        	    	{
						R15 += 4 + (offs << 1);
					}
					else
					{
						R15 += 2;
					}
					break;
				case COND_AL:
					printf("%08x: Undefined Thumb instruction: %04x (ARM9 reserved)\n", pc, insn);
					R15 += 2;
					break;

				case COND_NV:	// SWI (this is sort of a "hole" in the opcode encoding)
					ARM7_SetSWI();
//					R15 -= 4;
					break;
			}
			break;
		case 0xe: /* B #offs */
        if( insn & THUMB_BLOP_LO )
        {
                addr = GET_REGISTER(14);
                addr += ( insn & THUMB_BLOP_OFFS ) << 1;
                addr &= 0xfffffffc;
                SET_REGISTER( 14, ( R15 + 4 ) | 1 );
                R15 = addr;
        }
        else
        {
                offs = ( insn & THUMB_BRANCH_OFFS ) << 1;
                if( offs & 0x00000800 )
                {
                offs |= 0xfffff800;
                }
                R15 += 4 + offs;
        }
			break;
		case 0xf: /* BL */
			if( insn & THUMB_BLOP_LO )
			{
				addr = GET_REGISTER(14);
				addr += ( insn & THUMB_BLOP_OFFS ) << 1;
				SET_REGISTER( 14, ( R15 + 2 ) | 1 );
				R15 = addr;
			}
			else
			{
				addr = ( insn & THUMB_BLOP_OFFS ) << 12;
				if( addr & ( 1 << 22 ) )
				{
					addr |= 0xff800000;
				}
				addr += R15 + 4;
				SET_REGISTER( 14, addr );
				R15 += 2;
			}
			break;
		default:
			printf("%08x: Undefined Thumb instruction: %04x\n", pc, insn);
			R15 += 2;
			break;
	}

	return cycles;
}
