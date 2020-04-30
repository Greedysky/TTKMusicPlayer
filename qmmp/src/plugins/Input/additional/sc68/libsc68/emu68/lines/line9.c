/* line9.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line9.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 9: SUB/SUBX */

DECL_LINE68(line900)
{
  /* SUB.B Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line901)
{
  /* SUB.B An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line902)
{
  /* SUB.B (An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(2,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line903)
{
  /* SUB.B (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAB(3,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line904)
{
  /* SUB.B -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(4,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line905)
{
  /* SUB.B d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAB(5,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line906)
{
  /* SUB.B d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAB(6,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line907)
{
  /* SUB.B <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAB(7,reg0) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line908)
{
  /* SUB.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line909)
{
  /* SUB.W An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line90A)
{
  /* SUB.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line90B)
{
  /* SUB.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line90C)
{
  /* SUB.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line90D)
{
  /* SUB.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line90E)
{
  /* SUB.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line90F)
{
  /* SUB.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line910)
{
  /* SUB.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line911)
{
  /* SUB.L An,Dn */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line912)
{
  /* SUB.L (An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line913)
{
  /* SUB.L (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line914)
{
  /* SUB.L -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line915)
{
  /* SUB.L d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line916)
{
  /* SUB.L d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line917)
{
  /* SUB.L <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line918)
{
  /* SUBA.W Dn,An */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line919)
{
  /* SUBA.W An,An */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line91A)
{
  /* SUBA.W (An),An */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line91B)
{
  /* SUBA.W (An)+,An */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line91C)
{
  /* SUBA.W -(An),An */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line91D)
{
  /* SUBA.W d(An),An */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line91E)
{
  /* SUBA.W d(An,Xi),An */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line91F)
{
  /* SUBA.W <Ae>,An */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line920)
{
  int68_t a,b,s;
  a = (int68_t)REG68.d[reg0]<<BYTE_FIX;
  b = (int68_t)REG68.d[reg9]<<BYTE_FIX;
  SUBXB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9] & BYTE_MSK) + ((uint68_t)s>>BYTE_FIX);
}

DECL_LINE68(line921)
{
  int68_t a,b,s;
  a = read_B(REG68.a[reg0]-=1)<<BYTE_FIX;
  b = read_B(REG68.a[reg9]-=1)<<BYTE_FIX;
  SUBXB(s,a,b);
  write_B(REG68.a[reg9],(uint68_t)s>>BYTE_FIX);
}

DECL_LINE68(line922)
{
  /* SUB.B Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line923)
{
  /* SUB.B Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line924)
{
  /* SUB.B Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line925)
{
  /* SUB.B Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line926)
{
  /* SUB.B Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line927)
{
  /* SUB.B Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line928)
{
  int68_t a,b,s;
  a = (int68_t)REG68.d[reg0]<<WORD_FIX;
  b = (int68_t)REG68.d[reg9]<<WORD_FIX;
  SUBXW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9] & WORD_MSK) + ((uint68_t)s>>WORD_FIX);
}

DECL_LINE68(line929)
{
  int68_t a,b,s;
  a = read_W(REG68.a[reg0]-=2)<<WORD_FIX;
  b = read_W(REG68.a[reg9]-=2)<<WORD_FIX;
  SUBXW(s,a,b);
  write_W(REG68.a[reg9],(uint68_t)s>>WORD_FIX);
}

DECL_LINE68(line92A)
{
  /* SUB.W Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line92B)
{
  /* SUB.W Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line92C)
{
  /* SUB.W Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line92D)
{
  /* SUB.W Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line92E)
{
  /* SUB.W Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line92F)
{
  /* SUB.W Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line930)
{
  int68_t a,b,s;
  a = (int68_t)REG68.d[reg0]<<LONG_FIX;
  b = (int68_t)REG68.d[reg9]<<LONG_FIX;
  SUBXL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9] & LONG_MSK) + ((uint68_t)s>>LONG_FIX);
}

DECL_LINE68(line931)
{
  int68_t a,b,s;
  a = read_L(REG68.a[reg0]-=4)<<LONG_FIX;
  b = read_L(REG68.a[reg9]-=4)<<LONG_FIX;
  SUBXL(s,a,b);
  write_L(REG68.a[reg9],(uint68_t)s>>LONG_FIX);
}

DECL_LINE68(line932)
{
  /* SUB.L Dn,(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line933)
{
  /* SUB.L Dn,(An)+ */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line934)
{
  /* SUB.L Dn,-(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line935)
{
  /* SUB.L Dn,d(An) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line936)
{
  /* SUB.L Dn,d(An,Xi) */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line937)
{
  /* SUB.L Dn,<Ae> */
  const uint68_t s = ( (int68_t) REG68.d[reg9] << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line938)
{
  /* SUBA.L Dn,An */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line939)
{
  /* SUBA.L An,An */
  const uint68_t s = ( (int68_t) REG68.a[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line93A)
{
  /* SUBA.L (An),An */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line93B)
{
  /* SUBA.L (An)+,An */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line93C)
{
  /* SUBA.L -(An),An */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line93D)
{
  /* SUBA.L d(An),An */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line93E)
{
  /* SUBA.L d(An,Xi),An */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

DECL_LINE68(line93F)
{
  /* SUBA.L <Ae>,An */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg9] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg9] = ( d >> LONG_FIX );
}

