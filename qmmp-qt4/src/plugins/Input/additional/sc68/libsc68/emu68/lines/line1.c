/* line1.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line1.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 1: Move Byte */

DECL_LINE68(line100)
{
  /* MOVE.B Dx,Dy */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line101)
{
  /* MOVE.B Ax,Dy */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line102)
{
  /* MOVE.B (Ax),Dy */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line103)
{
  /* MOVE.B (Ax)+,Dy */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line104)
{
  /* MOVE.B -(Ax),Dy */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line105)
{
  /* MOVE.B d(Ax),Dy */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line106)
{
  /* MOVE.B d(Ax,Xi),Dy */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line107)
{
  /* MOVE.B <Ae>,Dy */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  REG68.d[reg9] = ( REG68.d[reg9] & BYTE_MSK ) + ( (uint68_t) a >> BYTE_FIX );
}

DECL_LINE68(line108)
{
  ILLEGAL; /* MOVEA.B Dx,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line109)
{
  ILLEGAL; /* MOVEA.B Ax,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line10A)
{
  ILLEGAL; /* MOVEA.B (Ax),An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line10B)
{
  ILLEGAL; /* MOVEA.B (Ax)+,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line10C)
{
  ILLEGAL; /* MOVEA.B -(Ax),An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line10D)
{
  ILLEGAL; /* MOVEA.B d(Ax),An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line10E)
{
  ILLEGAL; /* MOVEA.B d(Ax,Xi),An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line10F)
{
  ILLEGAL; /* MOVEA.B <Ae>,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line110)
{
  /* MOVE.B Dx,(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line111)
{
  /* MOVE.B Ax,(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line112)
{
  /* MOVE.B (Ax),(Ay) */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line113)
{
  /* MOVE.B (Ax)+,(Ay) */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line114)
{
  /* MOVE.B -(Ax),(Ay) */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line115)
{
  /* MOVE.B d(Ax),(Ay) */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line116)
{
  /* MOVE.B d(Ax,Xi),(Ay) */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line117)
{
  /* MOVE.B <Ae>,(Ay) */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(2, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line118)
{
  /* MOVE.B Dx,(Ay)+ */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line119)
{
  /* MOVE.B Ax,(Ay)+ */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line11A)
{
  /* MOVE.B (Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line11B)
{
  /* MOVE.B (Ax)+,(Ay)+ */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line11C)
{
  /* MOVE.B -(Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line11D)
{
  /* MOVE.B d(Ax),(Ay)+ */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line11E)
{
  /* MOVE.B d(Ax,Xi),(Ay)+ */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line11F)
{
  /* MOVE.B <Ae>,(Ay)+ */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(3, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line120)
{
  /* MOVE.B Dx,-(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line121)
{
  /* MOVE.B Ax,-(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line122)
{
  /* MOVE.B (Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line123)
{
  /* MOVE.B (Ax)+,-(Ay) */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line124)
{
  /* MOVE.B -(Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line125)
{
  /* MOVE.B d(Ax),-(Ay) */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line126)
{
  /* MOVE.B d(Ax,Xi),-(Ay) */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line127)
{
  /* MOVE.B <Ae>,-(Ay) */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(4, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line128)
{
  /* MOVE.B Dx,d(Ay) */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line129)
{
  /* MOVE.B Ax,d(Ay) */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line12A)
{
  /* MOVE.B (Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line12B)
{
  /* MOVE.B (Ax)+,d(Ay) */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line12C)
{
  /* MOVE.B -(Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line12D)
{
  /* MOVE.B d(Ax),d(Ay) */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line12E)
{
  /* MOVE.B d(Ax,Xi),d(Ay) */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line12F)
{
  /* MOVE.B <Ae>,d(Ay) */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(5, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line130)
{
  /* MOVE.B Dx,d(Ay,Xi) */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line131)
{
  /* MOVE.B Ax,d(Ay,Xi) */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line132)
{
  /* MOVE.B (Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line133)
{
  /* MOVE.B (Ax)+,d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line134)
{
  /* MOVE.B -(Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line135)
{
  /* MOVE.B d(Ax),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line136)
{
  /* MOVE.B d(Ax,Xi),d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line137)
{
  /* MOVE.B <Ae>,d(Ay,Xi) */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(6, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line138)
{
  /* MOVE.B Dx,<Ae> */
  const int68_t a = (int68_t) REG68.d[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line139)
{
  /* MOVE.B Ax,<Ae> */
  const int68_t a = (int68_t) REG68.a[reg0] << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line13A)
{
  /* MOVE.B (Ax),<Ae> */
  const int68_t a = (int68_t) read_EAB(2,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line13B)
{
  /* MOVE.B (Ax)+,<Ae> */
  const int68_t a = (int68_t) read_EAB(3,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line13C)
{
  /* MOVE.B -(Ax),<Ae> */
  const int68_t a = (int68_t) read_EAB(4,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line13D)
{
  /* MOVE.B d(Ax),<Ae> */
  const int68_t a = (int68_t) read_EAB(5,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line13E)
{
  /* MOVE.B d(Ax,Xi),<Ae> */
  const int68_t a = (int68_t) read_EAB(6,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

DECL_LINE68(line13F)
{
  /* MOVE.B <Ae>,<Ae> */
  const int68_t a = (int68_t) read_EAB(7,reg0) << BYTE_FIX;
  MOVEB(a);
  write_EAB(7, reg9, a >> BYTE_FIX);
}

