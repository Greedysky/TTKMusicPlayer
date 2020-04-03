/* line2.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line2.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 2: Move Long */

DECL_LINE68(line200)
{
  /* MOVE.L Dx,Dy */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line201)
{
  /* MOVE.L Ax,Dy */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line202)
{
  /* MOVE.L (Ax),Dy */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line203)
{
  /* MOVE.L (Ax)+,Dy */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line204)
{
  /* MOVE.L -(Ax),Dy */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line205)
{
  /* MOVE.L d(Ax),Dy */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line206)
{
  /* MOVE.L d(Ax,Xi),Dy */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line207)
{
  /* MOVE.L <Ae>,Dy */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  REG68.d[reg9] = ( (uint68_t) a >> LONG_FIX );
}

DECL_LINE68(line208)
{
  /* MOVEA.L Dx,An */
  REG68.a[reg9] = REG68.d[reg0];
}

DECL_LINE68(line209)
{
  /* MOVEA.L Ax,An */
  REG68.a[reg9] = REG68.a[reg0];
}

DECL_LINE68(line20A)
{
  /* MOVEA.L (Ax),An */
  REG68.a[reg9] = read_EAL(2,reg0);
}

DECL_LINE68(line20B)
{
  /* MOVEA.L (Ax)+,An */
  REG68.a[reg9] = read_EAL(3,reg0);
}

DECL_LINE68(line20C)
{
  /* MOVEA.L -(Ax),An */
  REG68.a[reg9] = read_EAL(4,reg0);
}

DECL_LINE68(line20D)
{
  /* MOVEA.L d(Ax),An */
  REG68.a[reg9] = read_EAL(5,reg0);
}

DECL_LINE68(line20E)
{
  /* MOVEA.L d(Ax,Xi),An */
  REG68.a[reg9] = read_EAL(6,reg0);
}

DECL_LINE68(line20F)
{
  /* MOVEA.L <Ae>,An */
  REG68.a[reg9] = read_EAL(7,reg0);
}

DECL_LINE68(line210)
{
  /* MOVE.L Dx,(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line211)
{
  /* MOVE.L Ax,(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line212)
{
  /* MOVE.L (Ax),(Ay) */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line213)
{
  /* MOVE.L (Ax)+,(Ay) */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line214)
{
  /* MOVE.L -(Ax),(Ay) */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line215)
{
  /* MOVE.L d(Ax),(Ay) */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line216)
{
  /* MOVE.L d(Ax,Xi),(Ay) */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line217)
{
  /* MOVE.L <Ae>,(Ay) */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(2, reg9, a >> LONG_FIX);
}

DECL_LINE68(line218)
{
  /* MOVE.L Dx,(Ay)+ */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line219)
{
  /* MOVE.L Ax,(Ay)+ */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line21A)
{
  /* MOVE.L (Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line21B)
{
  /* MOVE.L (Ax)+,(Ay)+ */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line21C)
{
  /* MOVE.L -(Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line21D)
{
  /* MOVE.L d(Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line21E)
{
  /* MOVE.L d(Ax,Xi),(Ay)+ */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line21F)
{
  /* MOVE.L <Ae>,(Ay)+ */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(3, reg9, a >> LONG_FIX);
}

DECL_LINE68(line220)
{
  /* MOVE.L Dx,-(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line221)
{
  /* MOVE.L Ax,-(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line222)
{
  /* MOVE.L (Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line223)
{
  /* MOVE.L (Ax)+,-(Ay) */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line224)
{
  /* MOVE.L -(Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line225)
{
  /* MOVE.L d(Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line226)
{
  /* MOVE.L d(Ax,Xi),-(Ay) */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line227)
{
  /* MOVE.L <Ae>,-(Ay) */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(4, reg9, a >> LONG_FIX);
}

DECL_LINE68(line228)
{
  /* MOVE.L Dx,d(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line229)
{
  /* MOVE.L Ax,d(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line22A)
{
  /* MOVE.L (Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line22B)
{
  /* MOVE.L (Ax)+,d(Ay) */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line22C)
{
  /* MOVE.L -(Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line22D)
{
  /* MOVE.L d(Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line22E)
{
  /* MOVE.L d(Ax,Xi),d(Ay) */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line22F)
{
  /* MOVE.L <Ae>,d(Ay) */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(5, reg9, a >> LONG_FIX);
}

DECL_LINE68(line230)
{
  /* MOVE.L Dx,d(Ay,Xi) */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line231)
{
  /* MOVE.L Ax,d(Ay,Xi) */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line232)
{
  /* MOVE.L (Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line233)
{
  /* MOVE.L (Ax)+,d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line234)
{
  /* MOVE.L -(Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line235)
{
  /* MOVE.L d(Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line236)
{
  /* MOVE.L d(Ax,Xi),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line237)
{
  /* MOVE.L <Ae>,d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(6, reg9, a >> LONG_FIX);
}

DECL_LINE68(line238)
{
  /* MOVE.L Dx,<Ae> */
  const int68_t a = (int68_t) REG68.d[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line239)
{
  /* MOVE.L Ax,<Ae> */
  const int68_t a = (int68_t) REG68.a[reg0] << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line23A)
{
  /* MOVE.L (Ax),<Ae> */
  const int68_t a = (int68_t) read_EAL(2,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line23B)
{
  /* MOVE.L (Ax)+,<Ae> */
  const int68_t a = (int68_t) read_EAL(3,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line23C)
{
  /* MOVE.L -(Ax),<Ae> */
  const int68_t a = (int68_t) read_EAL(4,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line23D)
{
  /* MOVE.L d(Ax),<Ae> */
  const int68_t a = (int68_t) read_EAL(5,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line23E)
{
  /* MOVE.L d(Ax,Xi),<Ae> */
  const int68_t a = (int68_t) read_EAL(6,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

DECL_LINE68(line23F)
{
  /* MOVE.L <Ae>,<Ae> */
  const int68_t a = (int68_t) read_EAL(7,reg0) << LONG_FIX;
  MOVEL(a);
  write_EAL(7, reg9, a >> LONG_FIX);
}

