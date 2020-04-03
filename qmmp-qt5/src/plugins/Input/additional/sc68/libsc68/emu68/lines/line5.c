/* line5.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line5.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 5: ADDQ/SUBQ/Scc/DBcc/TRAPcc */

DECL_LINE68(line500)
{
  /* ADDQ.B #Q,Dn */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  ADDB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line501)
{
  /* ADDQ.B #Q,An */
  ILLEGAL; /* .B not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line502)
{
  /* ADDQ.B #Q,(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line503)
{
  /* ADDQ.B #Q,(An)+ */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line504)
{
  /* ADDQ.B #Q,-(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line505)
{
  /* ADDQ.B #Q,d(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line506)
{
  /* ADDQ.B #Q,d(An,Xi) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line507)
{
  /* ADDQ.B #Q,<Ae> */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  ADDB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line508)
{
  /* ADDQ.W #Q,Dn */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  ADDW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line509)
{
  /* ADDQ.W #Q,An */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg0] << LONG_FIX;
  ADDAW(d,s,d);
  REG68.a[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line50A)
{
  /* ADDQ.W #Q,(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line50B)
{
  /* ADDQ.W #Q,(An)+ */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line50C)
{
  /* ADDQ.W #Q,-(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line50D)
{
  /* ADDQ.W #Q,d(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line50E)
{
  /* ADDQ.W #Q,d(An,Xi) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line50F)
{
  /* ADDQ.W #Q,<Ae> */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  ADDW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line510)
{
  /* ADDQ.L #Q,Dn */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  ADDL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line511)
{
  /* ADDQ.L #Q,An */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg0] << LONG_FIX;
  ADDAL(d,s,d);
  REG68.a[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line512)
{
  /* ADDQ.L #Q,(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line513)
{
  /* ADDQ.L #Q,(An)+ */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line514)
{
  /* ADDQ.L #Q,-(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line515)
{
  /* ADDQ.L #Q,d(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line516)
{
  /* ADDQ.L #Q,d(An,Xi) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line517)
{
  /* ADDQ.L #Q,<Ae> */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  ADDL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line518)
{
  /* Scc Dn */
  const int r = SCC((reg9<<1)+0);
  REG68.d[reg0] = (REG68.d[reg0]&0xFFFFFF00)+r;
}

DECL_LINE68(line519)
{
  /* DBcc Dn,<addr> */
  DBCC(reg0,(reg9<<1)+0);
}

DECL_LINE68(line51A)
{
  /* Scc (An) */
  const int r = SCC((reg9<<1)+0);
  write_EAB(2,reg0,r);
}

DECL_LINE68(line51B)
{
  /* Scc (An)+ */
  const int r = SCC((reg9<<1)+0);
  write_EAB(3,reg0,r);
}

DECL_LINE68(line51C)
{
  /* Scc -(An) */
  const int r = SCC((reg9<<1)+0);
  write_EAB(4,reg0,r);
}

DECL_LINE68(line51D)
{
  /* Scc d(An) */
  const int r = SCC((reg9<<1)+0);
  write_EAB(5,reg0,r);
}

DECL_LINE68(line51E)
{
  /* Scc d(An,Xi) */
  const int r = SCC((reg9<<1)+0);
  write_EAB(6,reg0,r);
}

DECL_LINE68(line51F)
{
  /* Scc <Ae> */
  const int r = SCC((reg9<<1)+0);
  write_EAB(7,reg0,r);
}

DECL_LINE68(line520)
{
  /* SUBQ.B #Q,Dn */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << BYTE_FIX;
  SUBB(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( d >> BYTE_FIX );
}

DECL_LINE68(line521)
{
  /* SUBQ.B #Q,An */
  ILLEGAL; /* .B not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line522)
{
  /* SUBQ.B #Q,(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(2,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line523)
{
  /* SUBQ.B #Q,(An)+ */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(3,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line524)
{
  /* SUBQ.B #Q,-(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(4,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line525)
{
  /* SUBQ.B #Q,d(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(5,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line526)
{
  /* SUBQ.B #Q,d(An,Xi) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(6,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line527)
{
  /* SUBQ.B #Q,<Ae> */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << BYTE_FIX );
  const addr68_t l = get_EAB(7,reg0);
        uint68_t d = read_B(l) << BYTE_FIX;
  SUBB(d,s,d);
  write_B(l, d >> BYTE_FIX);
}

DECL_LINE68(line528)
{
  /* SUBQ.W #Q,Dn */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << WORD_FIX;
  SUBW(d,s,d);
  REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( d >> WORD_FIX );
}

DECL_LINE68(line529)
{
  /* SUBQ.W #Q,An */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX ) >> 16;
        uint68_t d = (int68_t) REG68.a[reg0] << LONG_FIX;
  SUBAW(d,s,d);
  REG68.a[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line52A)
{
  /* SUBQ.W #Q,(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(2,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line52B)
{
  /* SUBQ.W #Q,(An)+ */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(3,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line52C)
{
  /* SUBQ.W #Q,-(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(4,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line52D)
{
  /* SUBQ.W #Q,d(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(5,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line52E)
{
  /* SUBQ.W #Q,d(An,Xi) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(6,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line52F)
{
  /* SUBQ.W #Q,<Ae> */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << WORD_FIX );
  const addr68_t l = get_EAW(7,reg0);
        uint68_t d = read_W(l) << WORD_FIX;
  SUBW(d,s,d);
  write_W(l, d >> WORD_FIX);
}

DECL_LINE68(line530)
{
  /* SUBQ.L #Q,Dn */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg0] << LONG_FIX;
  SUBL(d,s,d);
  REG68.d[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line531)
{
  /* SUBQ.L #Q,An */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
        uint68_t d = (int68_t) REG68.a[reg0] << LONG_FIX;
  SUBAL(d,s,d);
  REG68.a[reg0] = ( d >> LONG_FIX );
}

DECL_LINE68(line532)
{
  /* SUBQ.L #Q,(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(2,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line533)
{
  /* SUBQ.L #Q,(An)+ */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(3,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line534)
{
  /* SUBQ.L #Q,-(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(4,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line535)
{
  /* SUBQ.L #Q,d(An) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(5,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line536)
{
  /* SUBQ.L #Q,d(An,Xi) */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(6,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line537)
{
  /* SUBQ.L #Q,<Ae> */
  const uint68_t s = ( (int68_t) ( ( ( reg9 - 1 ) & 7 ) + 1 ) << LONG_FIX );
  const addr68_t l = get_EAL(7,reg0);
        uint68_t d = read_L(l) << LONG_FIX;
  SUBL(d,s,d);
  write_L(l, d >> LONG_FIX);
}

DECL_LINE68(line538)
{
  /* Scc Dn */
  const int r = SCC((reg9<<1)+1);
  REG68.d[reg0] = (REG68.d[reg0]&0xFFFFFF00)+r;
}

DECL_LINE68(line539)
{
  /* DBcc Dn,<addr> */
  DBCC(reg0,(reg9<<1)+1);
}

DECL_LINE68(line53A)
{
  /* Scc (An) */
  const int r = SCC((reg9<<1)+1);
  write_EAB(2,reg0,r);
}

DECL_LINE68(line53B)
{
  /* Scc (An)+ */
  const int r = SCC((reg9<<1)+1);
  write_EAB(3,reg0,r);
}

DECL_LINE68(line53C)
{
  /* Scc -(An) */
  const int r = SCC((reg9<<1)+1);
  write_EAB(4,reg0,r);
}

DECL_LINE68(line53D)
{
  /* Scc d(An) */
  const int r = SCC((reg9<<1)+1);
  write_EAB(5,reg0,r);
}

DECL_LINE68(line53E)
{
  /* Scc d(An,Xi) */
  const int r = SCC((reg9<<1)+1);
  write_EAB(6,reg0,r);
}

DECL_LINE68(line53F)
{
  /* Scc <Ae> */
  const int r = SCC((reg9<<1)+1);
  write_EAB(7,reg0,r);
}

