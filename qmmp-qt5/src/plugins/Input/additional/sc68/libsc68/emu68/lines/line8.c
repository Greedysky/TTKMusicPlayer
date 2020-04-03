/* line8.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line8.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 8: OR/DIV/SBCD */

DECL_LINE68(line800)
{
  /* ORR.B Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line801)
{
  /* ORR.B An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line802)
{
  /* ORR.B (An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(2,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line803)
{
  /* ORR.B (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAB(3,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line804)
{
  /* ORR.B -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(4,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line805)
{
  /* ORR.B d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(5,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line806)
{
  /* ORR.B d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAB(6,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line807)
{
  /* ORR.B <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAB(7,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ORRB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line808)
{
  /* ORR.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line809)
{
  /* ORR.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line80A)
{
  /* ORR.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line80B)
{
  /* ORR.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line80C)
{
  /* ORR.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line80D)
{
  /* ORR.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line80E)
{
  /* ORR.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line80F)
{
  /* ORR.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ORRW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line810)
{
  /* ORR.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line811)
{
  /* ORR.L An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line812)
{
  /* ORR.L (An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line813)
{
  /* ORR.L (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line814)
{
  /* ORR.L -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line815)
{
  /* ORR.L d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line816)
{
  /* ORR.L d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line817)
{
  /* ORR.L <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line818)
{
  /* DIVU.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line819)
{
  /* DIVU.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line81A)
{
  /* DIVU.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line81B)
{
  /* DIVU.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line81C)
{
  /* DIVU.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line81D)
{
  /* DIVU.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line81E)
{
  /* DIVU.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line81F)
{
  /* DIVU.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line820)
{
  /* SBCD Dy,Dx */
  int s = (u8) REG68.d[reg0];
  int d = (u8) REG68.d[reg9];
  SBCDB(d,s,d);
  REG68.d[reg9] = (REG68.d[reg9] & 0xFFFFFF00) | d;
}

DECL_LINE68(line821)
{
  /* SBCD -(Ay),-(Ax) */
  const addr68_t l0 = REG68.a[reg0] = (u32) ( REG68.a[reg0] - 1 );
  const addr68_t l9 = REG68.a[reg9] = (u32) ( REG68.a[reg9] - 1 );
  int s = read_B(l0);
  int d = read_B(l9);
  SBCDB(d,s,d);
  write_B(l9,d);
}

DECL_LINE68(line822)
{
  /* ORR.B Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line823)
{
  /* ORR.B Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line824)
{
  /* ORR.B Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line825)
{
  /* ORR.B Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line826)
{
  /* ORR.B Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line827)
{
  /* ORR.B Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ORRB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line828)
{
  ILLEGAL; /* EXG on other line (op:050) */
}

DECL_LINE68(line829)
{
  ILLEGAL; /* EXG on other line (op:051) */
}

DECL_LINE68(line82A)
{
  /* ORR.W Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line82B)
{
  /* ORR.W Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line82C)
{
  /* ORR.W Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line82D)
{
  /* ORR.W Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line82E)
{
  /* ORR.W Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line82F)
{
  /* ORR.W Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ORRW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line830)
{
  /* ORR.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  ORRL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line831)
{
  ILLEGAL; /* EXG on other line (op:061) */
}

DECL_LINE68(line832)
{
  /* ORR.L Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line833)
{
  /* ORR.L Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line834)
{
  /* ORR.L Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line835)
{
  /* ORR.L Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line836)
{
  /* ORR.L Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line837)
{
  /* ORR.L Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ORRL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line838)
{
  /* DIVS.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line839)
{
  /* DIVS.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line83A)
{
  /* DIVS.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line83B)
{
  /* DIVS.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line83C)
{
  /* DIVS.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line83D)
{
  /* DIVS.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line83E)
{
  /* DIVS.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line83F)
{
  /* DIVS.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  DIVSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

