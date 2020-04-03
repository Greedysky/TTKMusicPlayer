/* lineB.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: lineB.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line B: CMP/EOR */

DECL_LINE68(lineB00)
{
  /* CMP.B Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB01)
{
  /* CMP.B An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB02)
{
  /* CMP.B (An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(2,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB03)
{
  /* CMP.B (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAB(3,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB04)
{
  /* CMP.B -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(4,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB05)
{
  /* CMP.B d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(5,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB06)
{
  /* CMP.B d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAB(6,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB07)
{
  /* CMP.B <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAB(7,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  CMPB(s,d);
}

DECL_LINE68(lineB08)
{
  /* CMP.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB09)
{
  /* CMP.W An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB0A)
{
  /* CMP.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB0B)
{
  /* CMP.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB0C)
{
  /* CMP.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB0D)
{
  /* CMP.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB0E)
{
  /* CMP.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB0F)
{
  /* CMP.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CMPW(s,d);
}

DECL_LINE68(lineB10)
{
  /* CMP.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB11)
{
  /* CMP.L An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB12)
{
  /* CMP.L (An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB13)
{
  /* CMP.L (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB14)
{
  /* CMP.L -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB15)
{
  /* CMP.L d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB16)
{
  /* CMP.L d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB17)
{
  /* CMP.L <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CMPL(s,d);
}

DECL_LINE68(lineB18)
{
  /* CMPA.W Dn,An */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB19)
{
  /* CMPA.W An,An */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB1A)
{
  /* CMPA.W (An),An */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB1B)
{
  /* CMPA.W (An)+,An */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB1C)
{
  /* CMPA.W -(An),An */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB1D)
{
  /* CMPA.W d(An),An */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB1E)
{
  /* CMPA.W d(An,Xi),An */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB1F)
{
  /* CMPA.W <Ae>,An */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAW(s,d);
}

DECL_LINE68(lineB20)
{
  /* EOR.B Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  EORB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineB21)
{
  /* CMPM.B (Ay)+,(Ax)+ */
  int68_t y0, x9; addr68_t l;
  l = (s32) REG68.a[reg0];
  REG68.a[reg0] = (u32) ( REG68.a[reg0] + 1 );
  y0 = read_B(l) << BYTE_FIX;
  l = (s32) REG68.a[reg9];
  REG68.a[reg9] = (u32) ( REG68.a[reg9] + 1 );
  x9 = read_B(l) << BYTE_FIX;
  CMPB(y0,x9);
}

DECL_LINE68(lineB22)
{
  /* EOR.B Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineB23)
{
  /* EOR.B Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineB24)
{
  /* EOR.B Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineB25)
{
  /* EOR.B Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineB26)
{
  /* EOR.B Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineB27)
{
  /* EOR.B Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  EORB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineB28)
{
  /* EOR.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  EORW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineB29)
{
  /* CMPM.W (Ay)+,(Ax)+ */
  int68_t y0, x9; addr68_t l;
  l = (s32) REG68.a[reg0];
  REG68.a[reg0] = (u32) ( REG68.a[reg0] + 2 );
  y0 = read_W(l) << WORD_FIX;
  l = (s32) REG68.a[reg9];
  REG68.a[reg9] = (u32) ( REG68.a[reg9] + 2 );
  x9 = read_W(l) << WORD_FIX;
  CMPW(y0,x9);
}

DECL_LINE68(lineB2A)
{
  /* EOR.W Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineB2B)
{
  /* EOR.W Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineB2C)
{
  /* EOR.W Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineB2D)
{
  /* EOR.W Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineB2E)
{
  /* EOR.W Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineB2F)
{
  /* EOR.W Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  EORW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineB30)
{
  /* EOR.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  EORL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(lineB31)
{
  /* CMPM.L (Ay)+,(Ax)+ */
  int68_t y0, x9; addr68_t l;
  l = (s32) REG68.a[reg0];
  REG68.a[reg0] = (u32) ( REG68.a[reg0] + 4 );
  y0 = read_L(l) << LONG_FIX;
  l = (s32) REG68.a[reg9];
  REG68.a[reg9] = (u32) ( REG68.a[reg9] + 4 );
  x9 = read_L(l) << LONG_FIX;
  CMPL(y0,x9);
}

DECL_LINE68(lineB32)
{
  /* EOR.L Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineB33)
{
  /* EOR.L Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineB34)
{
  /* EOR.L Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineB35)
{
  /* EOR.L Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineB36)
{
  /* EOR.L Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineB37)
{
  /* EOR.L Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  EORL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineB38)
{
  /* CMPA.L Dn,An */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB39)
{
  /* CMPA.L An,An */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB3A)
{
  /* CMPA.L (An),An */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB3B)
{
  /* CMPA.L (An)+,An */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB3C)
{
  /* CMPA.L -(An),An */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB3D)
{
  /* CMPA.L d(An),An */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB3E)
{
  /* CMPA.L d(An,Xi),An */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

DECL_LINE68(lineB3F)
{
  /* CMPA.L <Ae>,An */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  CMPAL(s,d);
}

