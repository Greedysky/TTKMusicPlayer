/* line3.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line3.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 3: Move Word */

DECL_LINE68(line300)
{
  /* MOVE.W Dx,Dy */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line301)
{
  /* MOVE.W Ax,Dy */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line302)
{
  /* MOVE.W (Ax),Dy */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line303)
{
  /* MOVE.W (Ax)+,Dy */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line304)
{
  /* MOVE.W -(Ax),Dy */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line305)
{
  /* MOVE.W d(Ax),Dy */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line306)
{
  /* MOVE.W d(Ax,Xi),Dy */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line307)
{
  /* MOVE.W <Ae>,Dy */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  REG68.d[reg9] = ( REG68.d[reg9] & WORD_MSK ) + ( (uint68_t) a >> WORD_FIX );
}

DECL_LINE68(line308)
{
  /* MOVEA.W Dx,An */
  REG68.a[reg9] = (u32)(s16) REG68.d[reg0];
}

DECL_LINE68(line309)
{
  /* MOVEA.W Ax,An */
  REG68.a[reg9] = (u32)(s16) REG68.a[reg0];
}

DECL_LINE68(line30A)
{
  /* MOVEA.W (Ax),An */
  REG68.a[reg9] = (u32)(s16) read_EAW(2,reg0);
}

DECL_LINE68(line30B)
{
  /* MOVEA.W (Ax)+,An */
  REG68.a[reg9] = (u32)(s16) read_EAW(3,reg0);
}

DECL_LINE68(line30C)
{
  /* MOVEA.W -(Ax),An */
  REG68.a[reg9] = (u32)(s16) read_EAW(4,reg0);
}

DECL_LINE68(line30D)
{
  /* MOVEA.W d(Ax),An */
  REG68.a[reg9] = (u32)(s16) read_EAW(5,reg0);
}

DECL_LINE68(line30E)
{
  /* MOVEA.W d(Ax,Xi),An */
  REG68.a[reg9] = (u32)(s16) read_EAW(6,reg0);
}

DECL_LINE68(line30F)
{
  /* MOVEA.W <Ae>,An */
  REG68.a[reg9] = (u32)(s16) read_EAW(7,reg0);
}

DECL_LINE68(line310)
{
  /* MOVE.W Dx,(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line311)
{
  /* MOVE.W Ax,(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line312)
{
  /* MOVE.W (Ax),(Ay) */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line313)
{
  /* MOVE.W (Ax)+,(Ay) */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line314)
{
  /* MOVE.W -(Ax),(Ay) */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line315)
{
  /* MOVE.W d(Ax),(Ay) */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line316)
{
  /* MOVE.W d(Ax,Xi),(Ay) */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line317)
{
  /* MOVE.W <Ae>,(Ay) */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(2, reg9, a >> WORD_FIX);
}

DECL_LINE68(line318)
{
  /* MOVE.W Dx,(Ay)+ */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line319)
{
  /* MOVE.W Ax,(Ay)+ */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line31A)
{
  /* MOVE.W (Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line31B)
{
  /* MOVE.W (Ax)+,(Ay)+ */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line31C)
{
  /* MOVE.W -(Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line31D)
{
  /* MOVE.W d(Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line31E)
{
  /* MOVE.W d(Ax,Xi),(Ay)+ */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line31F)
{
  /* MOVE.W <Ae>,(Ay)+ */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(3, reg9, a >> WORD_FIX);
}

DECL_LINE68(line320)
{
  /* MOVE.W Dx,-(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line321)
{
  /* MOVE.W Ax,-(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line322)
{
  /* MOVE.W (Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line323)
{
  /* MOVE.W (Ax)+,-(Ay) */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line324)
{
  /* MOVE.W -(Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line325)
{
  /* MOVE.W d(Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line326)
{
  /* MOVE.W d(Ax,Xi),-(Ay) */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line327)
{
  /* MOVE.W <Ae>,-(Ay) */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(4, reg9, a >> WORD_FIX);
}

DECL_LINE68(line328)
{
  /* MOVE.W Dx,d(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line329)
{
  /* MOVE.W Ax,d(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line32A)
{
  /* MOVE.W (Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line32B)
{
  /* MOVE.W (Ax)+,d(Ay) */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line32C)
{
  /* MOVE.W -(Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line32D)
{
  /* MOVE.W d(Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line32E)
{
  /* MOVE.W d(Ax,Xi),d(Ay) */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line32F)
{
  /* MOVE.W <Ae>,d(Ay) */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(5, reg9, a >> WORD_FIX);
}

DECL_LINE68(line330)
{
  /* MOVE.W Dx,d(Ay,Xi) */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line331)
{
  /* MOVE.W Ax,d(Ay,Xi) */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line332)
{
  /* MOVE.W (Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line333)
{
  /* MOVE.W (Ax)+,d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line334)
{
  /* MOVE.W -(Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line335)
{
  /* MOVE.W d(Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line336)
{
  /* MOVE.W d(Ax,Xi),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line337)
{
  /* MOVE.W <Ae>,d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(6, reg9, a >> WORD_FIX);
}

DECL_LINE68(line338)
{
  /* MOVE.W Dx,<Ae> */
  const int68_t a = (int68_t) REG68.d[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line339)
{
  /* MOVE.W Ax,<Ae> */
  const int68_t a = (int68_t) REG68.a[reg0] << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line33A)
{
  /* MOVE.W (Ax),<Ae> */
  const int68_t a = (int68_t) read_EAW(2,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line33B)
{
  /* MOVE.W (Ax)+,<Ae> */
  const int68_t a = (int68_t) read_EAW(3,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line33C)
{
  /* MOVE.W -(Ax),<Ae> */
  const int68_t a = (int68_t) read_EAW(4,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line33D)
{
  /* MOVE.W d(Ax),<Ae> */
  const int68_t a = (int68_t) read_EAW(5,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line33E)
{
  /* MOVE.W d(Ax,Xi),<Ae> */
  const int68_t a = (int68_t) read_EAW(6,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

DECL_LINE68(line33F)
{
  /* MOVE.W <Ae>,<Ae> */
  const int68_t a = (int68_t) read_EAW(7,reg0) << WORD_FIX;
  MOVEW(a);
  write_EAW(7, reg9, a >> WORD_FIX);
}

