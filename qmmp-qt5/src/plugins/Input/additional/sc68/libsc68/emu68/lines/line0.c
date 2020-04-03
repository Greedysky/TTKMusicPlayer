/* line0.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line0.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 0: Bit Manipulation/MOVEP/Immediate */

static inline
void BTST_reg(emu68_t * const emu68, const int bit, int reg0)
{
  /* BTST.L #b,Dn */
  int68_t a = REG68.d[reg0];
  BTSTL(a,a,bit);
  ADDCYCLE(2);
}

static inline
void BTST_mem(emu68_t * const emu68, const int bit, int mode, int reg0)
{
  /* BTST.B #b,<Ae> */
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  BTSTB(a,a,bit);
  ADDCYCLE(0);
}

static inline
void BCHG_reg(emu68_t * const emu68, const int bit, int reg0)
{
  /* BCHG.L #b,Dn */
  int68_t a = REG68.d[reg0];
  BCHGL(a,a,bit);
  REG68.d[reg0] = (u32) a;
  ADDCYCLE(4);
}

static inline
void BCHG_mem(emu68_t * const emu68, const int bit, int mode, int reg0)
{
  /* BCHG.B #b,<Ae> */
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  BCHGB(a,a,bit);
  write_B(addr,a);
  ADDCYCLE(4);
}

static inline
void BCLR_reg(emu68_t * const emu68, const int bit, int reg0)
{
  /* BCLR.L #b,Dn */
  int68_t a = REG68.d[reg0];
  BCLRL(a,a,bit);
  REG68.d[reg0] = (u32) a;
  ADDCYCLE(6);
}

static inline
void BCLR_mem(emu68_t * const emu68, const int bit, int mode, int reg0)
{
  /* BCLR.B #b,<Ae> */
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  BCLRB(a,a,bit);
  write_B(addr,a);
  ADDCYCLE(4);
}

static inline
void BSET_reg(emu68_t * const emu68, const int bit, int reg0)
{
  /* BSET.L #b,Dn */
  int68_t a = REG68.d[reg0];
  BSETL(a,a,bit);
  REG68.d[reg0] = (u32) a;
  ADDCYCLE(4);
}

static inline
void BSET_mem(emu68_t * const emu68, const int bit, int mode, int reg0)
{
  /* BSET.B #b,<Ae> */
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  BSETB(a,a,bit);
  write_B(addr,a);
  ADDCYCLE(4);
}

static void l0_ill(emu68_t * const emu68, int reg0)
{
  reg0 = reg0;
  ILLEGAL;
}

static void l0_ORRb0(emu68_t * const emu68, int reg0)
{
  /* ORRI.B #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

static void l0_ANDb0(emu68_t * const emu68, int reg0)
{
  /* ANDI.B #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

static void l0_EORb0(emu68_t * const emu68, int reg0)
{
  /* EORI.B #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  EORB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

static void l0_ADDb0(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

static void l0_SUBb0(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

static void l0_CMPb0(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw0(emu68_t * const emu68, int reg0)
{
  /* ORRI.W #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

static void l0_ANDw0(emu68_t * const emu68, int reg0)
{
  /* ANDI.W #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

static void l0_EORw0(emu68_t * const emu68, int reg0)
{
  /* EORI.W #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  EORW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

static void l0_ADDw0(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

static void l0_SUBw0(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

static void l0_CMPw0(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl0(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

static void l0_ANDl0(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

static void l0_EORl0(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  EORL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

static void l0_ADDl0(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

static void l0_SUBl0(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

static void l0_CMPl0(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,Dn */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  CMPL(s,d);
}

static void l0_ORRb2(emu68_t * const emu68, int reg0)
{
  /* ORRI.B #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ANDb2(emu68_t * const emu68, int reg0)
{
  /* ANDI.B #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_EORb2(emu68_t * const emu68, int reg0)
{
  /* EORI.B #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ADDb2(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_SUBb2(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_CMPb2(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw2(emu68_t * const emu68, int reg0)
{
  /* ORRI.W #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ANDw2(emu68_t * const emu68, int reg0)
{
  /* ANDI.W #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_EORw2(emu68_t * const emu68, int reg0)
{
  /* EORI.W #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ADDw2(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_SUBw2(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_CMPw2(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl2(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ANDl2(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_EORl2(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ADDl2(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_SUBl2(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_CMPl2(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  CMPL(s,d);
}

static void l0_ORRb3(emu68_t * const emu68, int reg0)
{
  /* ORRI.B #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ANDb3(emu68_t * const emu68, int reg0)
{
  /* ANDI.B #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_EORb3(emu68_t * const emu68, int reg0)
{
  /* EORI.B #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ADDb3(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_SUBb3(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_CMPb3(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw3(emu68_t * const emu68, int reg0)
{
  /* ORRI.W #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ANDw3(emu68_t * const emu68, int reg0)
{
  /* ANDI.W #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_EORw3(emu68_t * const emu68, int reg0)
{
  /* EORI.W #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ADDw3(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_SUBw3(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_CMPw3(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl3(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ANDl3(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_EORl3(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ADDl3(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_SUBl3(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_CMPl3(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,(An)+ */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  CMPL(s,d);
}

static void l0_ORRb4(emu68_t * const emu68, int reg0)
{
  /* ORRI.B #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ANDb4(emu68_t * const emu68, int reg0)
{
  /* ANDI.B #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_EORb4(emu68_t * const emu68, int reg0)
{
  /* EORI.B #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ADDb4(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_SUBb4(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_CMPb4(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw4(emu68_t * const emu68, int reg0)
{
  /* ORRI.W #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ANDw4(emu68_t * const emu68, int reg0)
{
  /* ANDI.W #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_EORw4(emu68_t * const emu68, int reg0)
{
  /* EORI.W #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ADDw4(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_SUBw4(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_CMPw4(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl4(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ANDl4(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_EORl4(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ADDl4(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_SUBl4(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_CMPl4(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,-(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  CMPL(s,d);
}

static void l0_ORRb5(emu68_t * const emu68, int reg0)
{
  /* ORRI.B #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ANDb5(emu68_t * const emu68, int reg0)
{
  /* ANDI.B #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_EORb5(emu68_t * const emu68, int reg0)
{
  /* EORI.B #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ADDb5(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_SUBb5(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_CMPb5(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw5(emu68_t * const emu68, int reg0)
{
  /* ORRI.W #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ANDw5(emu68_t * const emu68, int reg0)
{
  /* ANDI.W #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_EORw5(emu68_t * const emu68, int reg0)
{
  /* EORI.W #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ADDw5(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_SUBw5(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_CMPw5(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl5(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ANDl5(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_EORl5(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ADDl5(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_SUBl5(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_CMPl5(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,d(An) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  CMPL(s,d);
}

static void l0_ORRb6(emu68_t * const emu68, int reg0)
{
  /* ORRI.B #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ANDb6(emu68_t * const emu68, int reg0)
{
  /* ANDI.B #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_EORb6(emu68_t * const emu68, int reg0)
{
  /* EORI.B #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_ADDb6(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_SUBb6(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_CMPb6(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw6(emu68_t * const emu68, int reg0)
{
  /* ORRI.W #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ANDw6(emu68_t * const emu68, int reg0)
{
  /* ANDI.W #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_EORw6(emu68_t * const emu68, int reg0)
{
  /* EORI.W #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_ADDw6(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_SUBw6(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_CMPw6(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl6(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ANDl6(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_EORl6(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ADDl6(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_SUBl6(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_CMPl6(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,d(An,Xi) */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  CMPL(s,d);
}

static void l0_ORRb7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) { /* ORR TO CCR */
    uint68_t a;
    a = get_nextw()&255;
    REG68.sr |= a;
  } else {
  /* ORRI.B #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
  }
}

static void l0_ANDb7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) { /* AND TO CCR */
    uint68_t a;
    a = get_nextw()|0xFF00;
    REG68.sr &= a;
  } else {
  /* ANDI.B #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
  }
}

static void l0_EORb7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) { /* EOR TO CCR */
    uint68_t a;
    a = get_nextw()&255;
    REG68.sr ^= a;
  } else {
  /* EORI.B #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
  }
}

static void l0_ADDb7(emu68_t * const emu68, int reg0)
{
  /* ADDI.B #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_SUBb7(emu68_t * const emu68, int reg0)
{
  /* SUBI.B #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

static void l0_CMPb7(emu68_t * const emu68, int reg0)
{
  /* CMPI.B #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  CMPB(s,d);
}

static void l0_ORRw7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) { /* ORR TO SR */
    uint68_t a;
    a = get_nextw();
    REG68.sr |= a;
  } else {
  /* ORRI.W #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
  }
}

static void l0_ANDw7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) { /* AND TO SR */
    uint68_t a;
    a = get_nextw();
    REG68.sr &= a;
  } else {
  /* ANDI.W #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
  }
}

static void l0_EORw7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) { /* EOR TO SR */
    uint68_t a;
    a = get_nextw();
    REG68.sr ^= a;
  } else {
  /* EORI.W #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
  }
}

static void l0_ADDw7(emu68_t * const emu68, int reg0)
{
  /* ADDI.W #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_SUBw7(emu68_t * const emu68, int reg0)
{
  /* SUBI.W #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

static void l0_CMPw7(emu68_t * const emu68, int reg0)
{
  /* CMPI.W #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextw() << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  CMPW(s,d);
}

static void l0_ORRl7(emu68_t * const emu68, int reg0)
{
  /* ORRI.L #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ANDl7(emu68_t * const emu68, int reg0)
{
  /* ANDI.L #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_EORl7(emu68_t * const emu68, int reg0)
{
  /* EORI.L #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_ADDl7(emu68_t * const emu68, int reg0)
{
  /* ADDI.L #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_SUBl7(emu68_t * const emu68, int reg0)
{
  /* SUBI.L #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

static void l0_CMPl7(emu68_t * const emu68, int reg0)
{
  /* CMPI.L #I,<Ae> */
  const uint68_t s = ( (int68_t)   get_nextl() << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  CMPL(s,d);
}

static void (*const line0_imm[8][32])(emu68_t * const emu68, int) =
{
/* ORR */
  {
  l0_ORRb0,l0_ill,l0_ORRb2,l0_ORRb3,l0_ORRb4,l0_ORRb5,l0_ORRb6,l0_ORRb7,
  l0_ORRw0,l0_ill,l0_ORRw2,l0_ORRw3,l0_ORRw4,l0_ORRw5,l0_ORRw6,l0_ORRw7,
  l0_ORRl0,l0_ill,l0_ORRl2,l0_ORRl3,l0_ORRl4,l0_ORRl5,l0_ORRl6,l0_ORRl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* AND */
  {
  l0_ANDb0,l0_ill,l0_ANDb2,l0_ANDb3,l0_ANDb4,l0_ANDb5,l0_ANDb6,l0_ANDb7,
  l0_ANDw0,l0_ill,l0_ANDw2,l0_ANDw3,l0_ANDw4,l0_ANDw5,l0_ANDw6,l0_ANDw7,
  l0_ANDl0,l0_ill,l0_ANDl2,l0_ANDl3,l0_ANDl4,l0_ANDl5,l0_ANDl6,l0_ANDl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* SUB */
  {
  l0_SUBb0,l0_ill,l0_SUBb2,l0_SUBb3,l0_SUBb4,l0_SUBb5,l0_SUBb6,l0_SUBb7,
  l0_SUBw0,l0_ill,l0_SUBw2,l0_SUBw3,l0_SUBw4,l0_SUBw5,l0_SUBw6,l0_SUBw7,
  l0_SUBl0,l0_ill,l0_SUBl2,l0_SUBl3,l0_SUBl4,l0_SUBl5,l0_SUBl6,l0_SUBl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* ADD */
  {
  l0_ADDb0,l0_ill,l0_ADDb2,l0_ADDb3,l0_ADDb4,l0_ADDb5,l0_ADDb6,l0_ADDb7,
  l0_ADDw0,l0_ill,l0_ADDw2,l0_ADDw3,l0_ADDw4,l0_ADDw5,l0_ADDw6,l0_ADDw7,
  l0_ADDl0,l0_ill,l0_ADDl2,l0_ADDl3,l0_ADDl4,l0_ADDl5,l0_ADDl6,l0_ADDl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* ??? */
  {
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* EOR */
  {
  l0_EORb0,l0_ill,l0_EORb2,l0_EORb3,l0_EORb4,l0_EORb5,l0_EORb6,l0_EORb7,
  l0_EORw0,l0_ill,l0_EORw2,l0_EORw3,l0_EORw4,l0_EORw5,l0_EORw6,l0_EORw7,
  l0_EORl0,l0_ill,l0_EORl2,l0_EORl3,l0_EORl4,l0_EORl5,l0_EORl6,l0_EORl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* CMP */
  {
  l0_CMPb0,l0_ill,l0_CMPb2,l0_CMPb3,l0_CMPb4,l0_CMPb5,l0_CMPb6,l0_CMPb7,
  l0_CMPw0,l0_ill,l0_CMPw2,l0_CMPw3,l0_CMPw4,l0_CMPw5,l0_CMPw6,l0_CMPw7,
  l0_CMPl0,l0_ill,l0_CMPl2,l0_CMPl3,l0_CMPl4,l0_CMPl5,l0_CMPl6,l0_CMPl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* ??? */
  {
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
};

DECL_LINE68(line000)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_reg(emu68, bit, reg0);
  } else {
    line0_imm[reg9][0](emu68,reg0);
  }
}

DECL_LINE68(line001)
{
  if (reg9 == 4) {
    ILLEGAL; /* BTST #xx,An (op:00000000000000000000000000000001) */
    assert(EMU68_BREAK);
  } else {
    line0_imm[reg9][1](emu68,reg0);
  }
}

DECL_LINE68(line002)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_mem(emu68, bit, 2, reg0);
  } else {
    line0_imm[reg9][2](emu68,reg0);
  }
}

DECL_LINE68(line003)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_mem(emu68, bit, 3, reg0);
  } else {
    line0_imm[reg9][3](emu68,reg0);
  }
}

DECL_LINE68(line004)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_mem(emu68, bit, 4, reg0);
  } else {
    line0_imm[reg9][4](emu68,reg0);
  }
}

DECL_LINE68(line005)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_mem(emu68, bit, 5, reg0);
  } else {
    line0_imm[reg9][5](emu68,reg0);
  }
}

DECL_LINE68(line006)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_mem(emu68, bit, 6, reg0);
  } else {
    line0_imm[reg9][6](emu68,reg0);
  }
}

DECL_LINE68(line007)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BTST_mem(emu68, bit, 7, reg0);
  } else {
    line0_imm[reg9][7](emu68,reg0);
  }
}

DECL_LINE68(line008)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_reg(emu68, bit, reg0);
  } else {
    line0_imm[reg9][8](emu68,reg0);
  }
}

DECL_LINE68(line009)
{
  if (reg9 == 4) {
    ILLEGAL; /* BCHG #xx,An (op:00000000000000000000000000000011) */
    assert(EMU68_BREAK);
  } else {
    line0_imm[reg9][9](emu68,reg0);
  }
}

DECL_LINE68(line00A)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_mem(emu68, bit, 2, reg0);
  } else {
    line0_imm[reg9][10](emu68,reg0);
  }
}

DECL_LINE68(line00B)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_mem(emu68, bit, 3, reg0);
  } else {
    line0_imm[reg9][11](emu68,reg0);
  }
}

DECL_LINE68(line00C)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_mem(emu68, bit, 4, reg0);
  } else {
    line0_imm[reg9][12](emu68,reg0);
  }
}

DECL_LINE68(line00D)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_mem(emu68, bit, 5, reg0);
  } else {
    line0_imm[reg9][13](emu68,reg0);
  }
}

DECL_LINE68(line00E)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_mem(emu68, bit, 6, reg0);
  } else {
    line0_imm[reg9][14](emu68,reg0);
  }
}

DECL_LINE68(line00F)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCHG_mem(emu68, bit, 7, reg0);
  } else {
    line0_imm[reg9][15](emu68,reg0);
  }
}

DECL_LINE68(line010)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_reg(emu68, bit, reg0);
  } else {
    line0_imm[reg9][16](emu68,reg0);
  }
}

DECL_LINE68(line011)
{
  if (reg9 == 4) {
    ILLEGAL; /* BCLR #xx,An (op:00000000000000000000000000000021) */
    assert(EMU68_BREAK);
  } else {
    line0_imm[reg9][17](emu68,reg0);
  }
}

DECL_LINE68(line012)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_mem(emu68, bit, 2, reg0);
  } else {
    line0_imm[reg9][18](emu68,reg0);
  }
}

DECL_LINE68(line013)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_mem(emu68, bit, 3, reg0);
  } else {
    line0_imm[reg9][19](emu68,reg0);
  }
}

DECL_LINE68(line014)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_mem(emu68, bit, 4, reg0);
  } else {
    line0_imm[reg9][20](emu68,reg0);
  }
}

DECL_LINE68(line015)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_mem(emu68, bit, 5, reg0);
  } else {
    line0_imm[reg9][21](emu68,reg0);
  }
}

DECL_LINE68(line016)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_mem(emu68, bit, 6, reg0);
  } else {
    line0_imm[reg9][22](emu68,reg0);
  }
}

DECL_LINE68(line017)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BCLR_mem(emu68, bit, 7, reg0);
  } else {
    line0_imm[reg9][23](emu68,reg0);
  }
}

DECL_LINE68(line018)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_reg(emu68, bit, reg0);
  } else {
    line0_imm[reg9][24](emu68,reg0);
  }
}

DECL_LINE68(line019)
{
  if (reg9 == 4) {
    ILLEGAL; /* BSET #xx,An (op:00000000000000000000000000000031) */
    assert(EMU68_BREAK);
  } else {
    line0_imm[reg9][25](emu68,reg0);
  }
}

DECL_LINE68(line01A)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_mem(emu68, bit, 2, reg0);
  } else {
    line0_imm[reg9][26](emu68,reg0);
  }
}

DECL_LINE68(line01B)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_mem(emu68, bit, 3, reg0);
  } else {
    line0_imm[reg9][27](emu68,reg0);
  }
}

DECL_LINE68(line01C)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_mem(emu68, bit, 4, reg0);
  } else {
    line0_imm[reg9][28](emu68,reg0);
  }
}

DECL_LINE68(line01D)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_mem(emu68, bit, 5, reg0);
  } else {
    line0_imm[reg9][29](emu68,reg0);
  }
}

DECL_LINE68(line01E)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_mem(emu68, bit, 6, reg0);
  } else {
    line0_imm[reg9][30](emu68,reg0);
  }
}

DECL_LINE68(line01F)
{
  if (reg9 == 4) {
    const int bit = get_nextw();
    BSET_mem(emu68, bit, 7, reg0);
  } else {
    line0_imm[reg9][31](emu68,reg0);
  }
}

DECL_LINE68(line020)
{
  /* BTST.L Dx,Dy */
  int68_t   y = REG68.d[reg0];
  const int x = REG68.d[reg9];
  BTSTL(y,y,x);
  ADDCYCLE(2);
}

DECL_LINE68(line021)
{
  /* MOVEP.W d(An),Dn */
  const addr68_t l = REG68.a[reg0] + get_nextw();
        uint68_t a;
  a  = read_B( l + 0 ) << 8;
  a += read_B( l + 2 ) << 0;
  REG68.d[reg9] = ( REG68.d[reg9] & ~0xFFFF ) + a;
  ADDCYCLE(8);
}

DECL_LINE68(line022)
{
  /* BTST.B Dn,(An) */
  const addr68_t l = get_EAB(2,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BTSTB(y,y,x);
  ADDCYCLE(0);
}

DECL_LINE68(line023)
{
  /* BTST.B Dn,(An)+ */
  const addr68_t l = get_EAB(3,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BTSTB(y,y,x);
  ADDCYCLE(0);
}

DECL_LINE68(line024)
{
  /* BTST.B Dn,-(An) */
  const addr68_t l = get_EAB(4,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BTSTB(y,y,x);
  ADDCYCLE(0);
}

DECL_LINE68(line025)
{
  /* BTST.B Dn,d(An) */
  const addr68_t l = get_EAB(5,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BTSTB(y,y,x);
  ADDCYCLE(0);
}

DECL_LINE68(line026)
{
  /* BTST.B Dn,d(An,Xi) */
  const addr68_t l = get_EAB(6,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BTSTB(y,y,x);
  ADDCYCLE(0);
}

DECL_LINE68(line027)
{
  /* BTST.B Dn,<Ae> */
  const addr68_t l = get_EAB(7,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BTSTB(y,y,x);
  ADDCYCLE(0);
}

DECL_LINE68(line028)
{
  /* BCHG.L Dx,Dy */
  int68_t   y = REG68.d[reg0];
  const int x = REG68.d[reg9];
  BCHGL(y,y,x);
  REG68.d[reg0] = (u32) y;
  ADDCYCLE(4);
}

DECL_LINE68(line029)
{
  /* MOVEP.L d(An),Dn */
  const addr68_t l = REG68.a[reg0] + get_nextw();
        uint68_t a;
  a  = read_B( l + 0 ) << 24;
  a += read_B( l + 2 ) << 16;
  a += read_B( l + 4 ) << 8;
  a += read_B( l + 6 ) << 0;
  REG68.d[reg9] = a;
  ADDCYCLE(16);
}

DECL_LINE68(line02A)
{
  /* BCHG.B Dn,(An) */
  const addr68_t l = get_EAB(2,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCHGB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line02B)
{
  /* BCHG.B Dn,(An)+ */
  const addr68_t l = get_EAB(3,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCHGB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line02C)
{
  /* BCHG.B Dn,-(An) */
  const addr68_t l = get_EAB(4,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCHGB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line02D)
{
  /* BCHG.B Dn,d(An) */
  const addr68_t l = get_EAB(5,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCHGB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line02E)
{
  /* BCHG.B Dn,d(An,Xi) */
  const addr68_t l = get_EAB(6,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCHGB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line02F)
{
  /* BCHG.B Dn,<Ae> */
  const addr68_t l = get_EAB(7,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCHGB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line030)
{
  /* BCLR.L Dx,Dy */
  int68_t   y = REG68.d[reg0];
  const int x = REG68.d[reg9];
  BCLRL(y,y,x);
  REG68.d[reg0] = (u32) y;
  ADDCYCLE(6);
}

DECL_LINE68(line031)
{
  /* MOVEP.W Dn,d(An) */
  const addr68_t l = REG68.a[reg0] + get_nextw();
  const uint68_t a = REG68.d[reg9];
  write_B( l + 0, a >> 8);
  write_B( l + 2, a >> 0);
  ADDCYCLE(8);
}

DECL_LINE68(line032)
{
  /* BCLR.B Dn,(An) */
  const addr68_t l = get_EAB(2,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCLRB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line033)
{
  /* BCLR.B Dn,(An)+ */
  const addr68_t l = get_EAB(3,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCLRB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line034)
{
  /* BCLR.B Dn,-(An) */
  const addr68_t l = get_EAB(4,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCLRB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line035)
{
  /* BCLR.B Dn,d(An) */
  const addr68_t l = get_EAB(5,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCLRB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line036)
{
  /* BCLR.B Dn,d(An,Xi) */
  const addr68_t l = get_EAB(6,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCLRB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line037)
{
  /* BCLR.B Dn,<Ae> */
  const addr68_t l = get_EAB(7,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BCLRB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line038)
{
  /* BSET.L Dx,Dy */
  int68_t   y = REG68.d[reg0];
  const int x = REG68.d[reg9];
  BSETL(y,y,x);
  REG68.d[reg0] = (u32) y;
  ADDCYCLE(4);
}

DECL_LINE68(line039)
{
  /* MOVEP.L Dn,d(An) */
  const addr68_t l = REG68.a[reg0] + get_nextw();
  const uint68_t a = REG68.d[reg9];
  write_B( l + 0, a >> 24);
  write_B( l + 2, a >> 16);
  write_B( l + 4, a >> 8);
  write_B( l + 6, a >> 0);
  ADDCYCLE(16);
}

DECL_LINE68(line03A)
{
  /* BSET.B Dn,(An) */
  const addr68_t l = get_EAB(2,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BSETB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line03B)
{
  /* BSET.B Dn,(An)+ */
  const addr68_t l = get_EAB(3,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BSETB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line03C)
{
  /* BSET.B Dn,-(An) */
  const addr68_t l = get_EAB(4,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BSETB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line03D)
{
  /* BSET.B Dn,d(An) */
  const addr68_t l = get_EAB(5,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BSETB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line03E)
{
  /* BSET.B Dn,d(An,Xi) */
  const addr68_t l = get_EAB(6,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BSETB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

DECL_LINE68(line03F)
{
  /* BSET.B Dn,<Ae> */
  const addr68_t l = get_EAB(7,reg0);
        int68_t  y = read_B(l);
  const int      x = REG68.d[reg9];
  BSETB(y,y,x);
  write_B(l,y);
  ADDCYCLE(4);
}

