/* lineC.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: lineC.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line C: AND/MUL/ABCD/EXG */

DECL_LINE68(lineC00)
{
  /* AND.B Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC01)
{
  /* AND.B An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(lineC02)
{
  /* AND.B (An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(2,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC03)
{
  /* AND.B (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAB(3,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC04)
{
  /* AND.B -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(4,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC05)
{
  /* AND.B d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(5,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC06)
{
  /* AND.B d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAB(6,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC07)
{
  /* AND.B <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAB(7,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ANDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineC08)
{
  /* AND.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC09)
{
  /* AND.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(lineC0A)
{
  /* AND.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC0B)
{
  /* AND.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC0C)
{
  /* AND.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC0D)
{
  /* AND.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC0E)
{
  /* AND.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC0F)
{
  /* AND.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ANDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineC10)
{
  /* AND.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC11)
{
  /* AND.L An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(lineC12)
{
  /* AND.L (An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC13)
{
  /* AND.L (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC14)
{
  /* AND.L -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC15)
{
  /* AND.L d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC16)
{
  /* AND.L d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC17)
{
  /* AND.L <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC18)
{
  /* MULU.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC19)
{
  /* MULU.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(lineC1A)
{
  /* MULU.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC1B)
{
  /* MULU.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC1C)
{
  /* MULU.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC1D)
{
  /* MULU.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC1E)
{
  /* MULU.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC1F)
{
  /* MULU.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULUW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC20)
{
  /* ABCD Dy,Dx */
  int s = (u8) REG68.d[reg0];
  int d = (u8) REG68.d[reg9];
  ABCDB(d,s,d);
  REG68.d[reg9] = (REG68.d[reg9] & 0xFFFFFF00) | d;
}

DECL_LINE68(lineC21)
{
  /* ABCD -(Ay),-(Ax) */
  const addr68_t l0 = REG68.a[reg0] = (u32) ( REG68.a[reg0] - 1 );
  const addr68_t l9 = REG68.a[reg9] = (u32) ( REG68.a[reg9] - 1 );
  int s = read_B(l0);
  int d = read_B(l9);
  ABCDB(d,s,d);
  write_B(l9,d);
}

DECL_LINE68(lineC22)
{
  /* AND.B Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineC23)
{
  /* AND.B Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineC24)
{
  /* AND.B Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineC25)
{
  /* AND.B Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineC26)
{
  /* AND.B Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineC27)
{
  /* AND.B Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ANDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineC28)
{
  /* EXG Dx,Dy */
  EXG(reg9,reg0);
}

DECL_LINE68(lineC29)
{
  /* EXG Ax,Ay */
  EXG(reg9+8,reg0+8);
}

DECL_LINE68(lineC2A)
{
  /* AND.W Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineC2B)
{
  /* AND.W Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineC2C)
{
  /* AND.W Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineC2D)
{
  /* AND.W Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineC2E)
{
  /* AND.W Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineC2F)
{
  /* AND.W Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ANDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineC30)
{
  /* AND.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  ANDL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC31)
{
  /* EXG Dx,Ay */
  EXG(reg9,reg0+8);
}

DECL_LINE68(lineC32)
{
  /* AND.L Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineC33)
{
  /* AND.L Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineC34)
{
  /* AND.L Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineC35)
{
  /* AND.L Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineC36)
{
  /* AND.L Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineC37)
{
  /* AND.L Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ANDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineC38)
{
  /* MULS.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC39)
{
  /* MULS.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(lineC3A)
{
  /* MULS.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC3B)
{
  /* MULS.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC3C)
{
  /* MULS.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC3D)
{
  /* MULS.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC3E)
{
  /* MULS.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineC3F)
{
  /* MULS.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  MULSW(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

