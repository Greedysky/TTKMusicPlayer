/* lineD.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: lineD.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line D: ADD/ADDX */

DECL_LINE68(lineD00)
{
  /* ADD.B Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD01)
{
  /* ADD.B An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD02)
{
  /* ADD.B (An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(2,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD03)
{
  /* ADD.B (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAB(3,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD04)
{
  /* ADD.B -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(4,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD05)
{
  /* ADD.B d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(5,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD06)
{
  /* ADD.B d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAB(6,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD07)
{
  /* ADD.B <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAB(7,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(lineD08)
{
  /* ADD.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD09)
{
  /* ADD.W An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD0A)
{
  /* ADD.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD0B)
{
  /* ADD.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD0C)
{
  /* ADD.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD0D)
{
  /* ADD.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD0E)
{
  /* ADD.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD0F)
{
  /* ADD.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(lineD10)
{
  /* ADD.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD11)
{
  /* ADD.L An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD12)
{
  /* ADD.L (An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD13)
{
  /* ADD.L (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD14)
{
  /* ADD.L -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD15)
{
  /* ADD.L d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD16)
{
  /* ADD.L d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD17)
{
  /* ADD.L <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD18)
{
  /* ADDA.W Dn,An */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD19)
{
  /* ADDA.W An,An */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD1A)
{
  /* ADDA.W (An),An */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD1B)
{
  /* ADDA.W (An)+,An */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD1C)
{
  /* ADDA.W -(An),An */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD1D)
{
  /* ADDA.W d(An),An */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD1E)
{
  /* ADDA.W d(An,Xi),An */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD1F)
{
  /* ADDA.W <Ae>,An */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD20)
{
  int68_t a,b,s;
  a = (int68_t)REG68.d[reg0]<<BYTE_FIX;
  b = (int68_t)REG68.d[reg9]<<BYTE_FIX;
  ADDXB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9] & BYTE_MSK) + ((uint68_t)s>>BYTE_FIX);
}

DECL_LINE68(lineD21)
{
  int68_t a,b,s;
  a = read_B(REG68.a[reg0]-=1)<<BYTE_FIX;
  b = read_B(REG68.a[reg9]-=1)<<BYTE_FIX;
  ADDXB(s,a,b);
  write_B(REG68.a[reg9],(uint68_t)s>>BYTE_FIX);
}

DECL_LINE68(lineD22)
{
  /* ADD.B Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineD23)
{
  /* ADD.B Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineD24)
{
  /* ADD.B Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineD25)
{
  /* ADD.B Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineD26)
{
  /* ADD.B Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineD27)
{
  /* ADD.B Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(lineD28)
{
  int68_t a,b,s;
  a = (int68_t)REG68.d[reg0]<<WORD_FIX;
  b = (int68_t)REG68.d[reg9]<<WORD_FIX;
  ADDXW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9] & WORD_MSK) + ((uint68_t)s>>WORD_FIX);
}

DECL_LINE68(lineD29)
{
  int68_t a,b,s;
  a = read_W(REG68.a[reg0]-=2)<<WORD_FIX;
  b = read_W(REG68.a[reg9]-=2)<<WORD_FIX;
  ADDXW(s,a,b);
  write_W(REG68.a[reg9],(uint68_t)s>>WORD_FIX);
}

DECL_LINE68(lineD2A)
{
  /* ADD.W Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineD2B)
{
  /* ADD.W Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineD2C)
{
  /* ADD.W Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineD2D)
{
  /* ADD.W Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineD2E)
{
  /* ADD.W Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineD2F)
{
  /* ADD.W Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(lineD30)
{
  int68_t a,b,s;
  a = (int68_t)REG68.d[reg0]<<LONG_FIX;
  b = (int68_t)REG68.d[reg9]<<LONG_FIX;
  ADDXL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9] & LONG_MSK) + ((uint68_t)s>>LONG_FIX);
}

DECL_LINE68(lineD31)
{
  int68_t a,b,s;
  a = read_L(REG68.a[reg0]-=4)<<LONG_FIX;
  b = read_L(REG68.a[reg9]-=4)<<LONG_FIX;
  ADDXL(s,a,b);
  write_L(REG68.a[reg9],(uint68_t)s>>LONG_FIX);
}

DECL_LINE68(lineD32)
{
  /* ADD.L Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineD33)
{
  /* ADD.L Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineD34)
{
  /* ADD.L Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineD35)
{
  /* ADD.L Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineD36)
{
  /* ADD.L Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineD37)
{
  /* ADD.L Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(lineD38)
{
  /* ADDA.L Dn,An */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD39)
{
  /* ADDA.L An,An */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD3A)
{
  /* ADDA.L (An),An */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD3B)
{
  /* ADDA.L (An)+,An */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD3C)
{
  /* ADDA.L -(An),An */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD3D)
{
  /* ADDA.L d(An),An */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD3E)
{
  /* ADDA.L d(An,Xi),An */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(lineD3F)
{
  /* ADDA.L <Ae>,An */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

