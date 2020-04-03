/* line4.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: line4.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line 4: Miscellaneous */

static void funky4_m6_0(emu68_t * const emu68)
{
  RESET;
}

static void funky4_m6_1(emu68_t * const emu68)
{
  NOP;
}

static void funky4_m6_2(emu68_t * const emu68)
{
  STOP;
}

static void funky4_m6_3(emu68_t * const emu68)
{
  RTE;
}

static void funky4_m6_4(emu68_t * const emu68)
{
  ILLEGAL;
}

static void funky4_m6_5(emu68_t * const emu68)
{
  RTS;
}

static void funky4_m6_6(emu68_t * const emu68)
{
  TRAPV;
}

static void funky4_m6_7(emu68_t * const emu68)
{
  RTR;
}


static void (*const funky4_m6_func[8])(emu68_t * const) = {
  funky4_m6_0,funky4_m6_1,funky4_m6_2,funky4_m6_3,
  funky4_m6_4,funky4_m6_5,funky4_m6_6,funky4_m6_7,
};

static void funky4_m0(emu68_t * const emu68, int reg0)
{
  const int a = ( 0 << 3 ) + reg0;
  TRAP(a);
}

static void funky4_m1(emu68_t * const emu68, int reg0)
{
  const int a = ( 1 << 3 ) + reg0;
  TRAP(a);
}

static void funky4_m2(emu68_t * const emu68, int reg0)
{
  LINK(reg0);
}

static void funky4_m3(emu68_t * const emu68, int reg0)
{
  UNLK(reg0);
}

static void funky4_m4(emu68_t * const emu68, int reg0)
{
  REG68.usp = REG68.a[reg0];
}

static void funky4_m5(emu68_t * const emu68, int reg0)
{
  REG68.a[reg0] = REG68.usp;
}

static void funky4_m6(emu68_t * const emu68, int reg0)
{
  funky4_m6_func[reg0](emu68);
}

static void funky4_m7(emu68_t * const emu68, int reg0)
{
  ILLEGAL;
}

static void (* const funky4_func[8])(emu68_t * const, int) = {
  funky4_m0,funky4_m1,funky4_m2,funky4_m3,
  funky4_m4,funky4_m5,funky4_m6,funky4_m7,
};

static void movemregw(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16) get_nextw(), addr;
  s32 * r = REG68.d;
  addr = get_eaw68[mode](emu68,reg0);
  for(; m; r++, m>>=1)
    if ( m & 1 ){ *r = read_W(addr); addr += 2; }
  if ( mode == 3 ) REG68.a[reg0] = addr;
}

static void movemregl(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16) get_nextw(), addr;
  s32 * r = REG68.d;
  addr = get_eal68[mode](emu68,reg0);
  for(; m; r++, m>>=1)
    if ( m & 1 ){ *r = read_L(addr); addr += 4; }
  if ( mode == 3 ) REG68.a[reg0] = addr;
}

static void movemmemw(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16)get_nextw(), addr;
  if (mode==4) {
    s32 * r = REG68.a+7;
    addr = get_eaw68[3](emu68,reg0);
    for(; m; r--, m>>=1)
      if (m&1) write_W(addr-=2,*r);
    REG68.a[reg0] = addr;
  } else {
    s32 * r = REG68.d;
    addr = get_eaw68[mode](emu68,reg0);
    for(; m; r++, m>>=1)
      if (m&1) { write_W(addr,*r); addr+=2; }
  }
}

static void movemmeml(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16)get_nextw(), addr;
  if (mode==4) {
    s32 * r = REG68.a+7;
    addr = get_eal68[3](emu68,reg0);
    for(; m; r--, m>>=1)
      if (m&1) write_L(addr-=4,*r);
    REG68.a[reg0] = addr;
  } else {
    s32 * r = REG68.d;
    addr = get_eal68[mode](emu68,reg0);
    for(; m; r++, m>>=1)
      if (m&1) { write_L(addr,*r); addr+=4; }
  }
}

static void line4_r0_s0(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NEGX.B Dn */
    b = (uint68_t) REG68.d[reg0] << BYTE_FIX;
    NEGXB(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( b >> BYTE_FIX );
  } else {
    /* NEGX.B <Ae> */
    const addr68_t addr = get_eab68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_B(addr) << BYTE_FIX;
    NEGXB(b,b);
    write_B(addr, b >> BYTE_FIX);
  }
}

static void line4_r0_s1(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NEGX.W Dn */
    b = (uint68_t) REG68.d[reg0] << WORD_FIX;
    NEGXW(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( b >> WORD_FIX );
  } else {
    /* NEGX.W <Ae> */
    const addr68_t addr = get_eaw68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_W(addr) << WORD_FIX;
    NEGXW(b,b);
    write_W(addr, b >> WORD_FIX);
  }
}

static void line4_r0_s2(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NEGX.L Dn */
    b = (uint68_t) REG68.d[reg0] << LONG_FIX;
    NEGXL(b,b);
    REG68.d[reg0] = ( b >> LONG_FIX );
  } else {
    /* NEGX.L <Ae> */
    const addr68_t addr = get_eal68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_L(addr) << LONG_FIX;
    NEGXL(b,b);
    write_L(addr, b >> LONG_FIX);
  }
}

static void line4_r0_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVE FROM SR */
  if (mode)
    write_W(get_eaw68[mode](emu68,reg0),REG68.sr);
  else
    REG68.d[reg0] = (REG68.d[reg0]&0xFFFF0000) + (u16)REG68.sr;
}

static void line4_r1_s0(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* CLR.B Dn */
    CLRB(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( b >> BYTE_FIX );
  } else {
    /* CLR.B <Ae> */
    const addr68_t addr = get_eab68[mode](emu68,reg0);
    assert(mode != 1);
    CLRB(b,b);
    write_B(addr, b >> BYTE_FIX);
  }
}

static void line4_r1_s1(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* CLR.W Dn */
    CLRW(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( b >> WORD_FIX );
  } else {
    /* CLR.W <Ae> */
    const addr68_t addr = get_eaw68[mode](emu68,reg0);
    assert(mode != 1);
    CLRW(b,b);
    write_W(addr, b >> WORD_FIX);
  }
}

static void line4_r1_s2(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* CLR.L Dn */
    CLRL(b,b);
    REG68.d[reg0] = ( b >> LONG_FIX );
  } else {
    /* CLR.L <Ae> */
    const addr68_t addr = get_eal68[mode](emu68,reg0);
    assert(mode != 1);
    CLRL(b,b);
    write_L(addr, b >> LONG_FIX);
  }
}

static void line4_r1_s3(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r2_s0(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NEG.B Dn */
    b = (uint68_t) REG68.d[reg0] << BYTE_FIX;
    NEGB(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( b >> BYTE_FIX );
  } else {
    /* NEG.B <Ae> */
    const addr68_t addr = get_eab68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_B(addr) << BYTE_FIX;
    NEGB(b,b);
    write_B(addr, b >> BYTE_FIX);
  }
}

static void line4_r2_s1(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NEG.W Dn */
    b = (uint68_t) REG68.d[reg0] << WORD_FIX;
    NEGW(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( b >> WORD_FIX );
  } else {
    /* NEG.W <Ae> */
    const addr68_t addr = get_eaw68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_W(addr) << WORD_FIX;
    NEGW(b,b);
    write_W(addr, b >> WORD_FIX);
  }
}

static void line4_r2_s2(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NEG.L Dn */
    b = (uint68_t) REG68.d[reg0] << LONG_FIX;
    NEGL(b,b);
    REG68.d[reg0] = ( b >> LONG_FIX );
  } else {
    /* NEG.L <Ae> */
    const addr68_t addr = get_eal68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_L(addr) << LONG_FIX;
    NEGL(b,b);
    write_L(addr, b >> LONG_FIX);
  }
}

static void line4_r2_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVE TO CCR */
  if (mode)
    SET_CCR(REG68.sr,read_W(get_eaw68[mode](emu68,reg0)));
  else
    SET_CCR(REG68.sr,REG68.d[reg0]);
}

static void line4_r3_s0(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NOT.B Dn */
    b = (uint68_t) REG68.d[reg0] << BYTE_FIX;
    NOTB(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( b >> BYTE_FIX );
  } else {
    /* NOT.B <Ae> */
    const addr68_t addr = get_eab68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_B(addr) << BYTE_FIX;
    NOTB(b,b);
    write_B(addr, b >> BYTE_FIX);
  }
}

static void line4_r3_s1(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NOT.W Dn */
    b = (uint68_t) REG68.d[reg0] << WORD_FIX;
    NOTW(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & WORD_MSK ) + ( b >> WORD_FIX );
  } else {
    /* NOT.W <Ae> */
    const addr68_t addr = get_eaw68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_W(addr) << WORD_FIX;
    NOTW(b,b);
    write_W(addr, b >> WORD_FIX);
  }
}

static void line4_r3_s2(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NOT.L Dn */
    b = (uint68_t) REG68.d[reg0] << LONG_FIX;
    NOTL(b,b);
    REG68.d[reg0] = ( b >> LONG_FIX );
  } else {
    /* NOT.L <Ae> */
    const addr68_t addr = get_eal68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_L(addr) << LONG_FIX;
    NOTL(b,b);
    write_L(addr, b >> LONG_FIX);
  }
}

static void line4_r3_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVE TO SR */
  if (mode)
    REG68.sr = read_W(get_eaw68[mode](emu68,reg0));
  else
    REG68.sr = (u16)REG68.d[reg0];
}

static void line4_r4_s0(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* NBCD.B Dn */
    b = (uint68_t) REG68.d[reg0] << BYTE_FIX;
    NBCDB(b,b);
    REG68.d[reg0] = ( REG68.d[reg0] & BYTE_MSK ) + ( b >> BYTE_FIX );
  } else {
    /* NBCD.B <Ae> */
    const addr68_t addr = get_eab68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_B(addr) << BYTE_FIX;
    NBCDB(b,b);
    write_B(addr, b >> BYTE_FIX);
  }
}

static void line4_r4_s1(emu68_t * const emu68, int mode, int reg0)
{
  if (!mode) {
    /* SWAP */
    SWAP(reg0);
  } else {
    /* PEA */
    pushl(get_eal68[mode](emu68,reg0));
  }
}

static void line4_r4_s2(emu68_t * const emu68, int mode, int reg0)
{
  if (!mode) {
    /* EXT.W Dn */
    const int68_t d = (int68_t) (s8) REG68.d[reg0] << WORD_FIX;
    EXTW(d);
    REG68.d[reg0] &= 0xFFFF0000;
    REG68.d[reg0] |= (uint68_t) d >> WORD_FIX;
  } else {
    /* MOVEM.W REGS,<AE> */
    movemmemw(emu68, mode, reg0);
  }
}

static void line4_r4_s3(emu68_t * const emu68, int mode, int reg0)
{
  if (!mode) {
    /* EXT.L Dn */
    const int68_t d = (int68_t) (s16) REG68.d[reg0] << LONG_FIX;
    EXTL(d);
    REG68.d[reg0] = d>>LONG_FIX;
  } else {
    /* MOVEM.L REGS,<AE> */
    movemmeml(emu68, mode, reg0);
  }
}

static void line4_r5_s0(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* TST.B Dn */
    b = (uint68_t) REG68.d[reg0] << BYTE_FIX;
    TSTB(b,b);
  } else {
    /* TST.B <Ae> */
    const addr68_t addr = get_eab68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_B(addr) << BYTE_FIX;
    TSTB(b,b);
  }
}

static void line4_r5_s1(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* TST.W Dn */
    b = (uint68_t) REG68.d[reg0] << WORD_FIX;
    TSTW(b,b);
  } else {
    /* TST.W <Ae> */
    const addr68_t addr = get_eaw68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_W(addr) << WORD_FIX;
    TSTW(b,b);
  }
}

static void line4_r5_s2(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t b;

  if ( !mode ) {
    /* TST.L Dn */
    b = (uint68_t) REG68.d[reg0] << LONG_FIX;
    TSTL(b,b);
  } else {
    /* TST.L <Ae> */
    const addr68_t addr = get_eal68[mode](emu68,reg0);
    assert(mode != 1);
    b = read_L(addr) << LONG_FIX;
    TSTL(b,b);
  }
}

static void line4_r5_s3(emu68_t * const emu68, int mode, int reg0)
{
  if (mode<2) {
    /* TAS.B Dn */
    int68_t a = (int68_t) REG68.d[reg0];
    TASB(a,a);
    REG68.d[reg0] = a;
  } else {
    if ( mode == 7 && reg0 > 1 ) {
      ILLEGAL;
    } else {
      /* TAS.B <Ae> */
      const addr68_t l = get_eab68[mode](emu68,reg0);
             int68_t a = read_B(l);
      TASB(a,a);
      write_B(l,a);
    }
  }
}

static void line4_r6_s0(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r6_s1(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r6_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVEM.W <AE>,REGS */
  movemregw(emu68,mode,reg0);
}

static void line4_r6_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVEM.L <AE>,REGS */
  movemregl(emu68,mode,reg0);
}

static void line4_r7_s0(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r7_s1(emu68_t * const emu68, int mode, int reg0)
{
  funky4_func[mode](emu68,reg0);
}

static void line4_r7_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* JSR <Ae> */
  const addr68_t pc = get_eal68[mode](emu68,reg0);
  JSR(pc);
}

static void line4_r7_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* JMP <Ae> */
  const addr68_t pc = get_eal68[mode](emu68,reg0);
  JMP(pc);
}

DECL_STATIC_LINE68((* const line4_0_func[8])) = {
  line4_r0_s0,line4_r1_s0,line4_r2_s0,line4_r3_s0,
  line4_r4_s0,line4_r5_s0,line4_r6_s0,line4_r7_s0,
};

DECL_STATIC_LINE68((* const line4_1_func[8])) = {
  line4_r0_s1,line4_r1_s1,line4_r2_s1,line4_r3_s1,
  line4_r4_s1,line4_r5_s1,line4_r6_s1,line4_r7_s1,
};

DECL_STATIC_LINE68((* const line4_2_func[8])) = {
  line4_r0_s2,line4_r1_s2,line4_r2_s2,line4_r3_s2,
  line4_r4_s2,line4_r5_s2,line4_r6_s2,line4_r7_s2,
};

DECL_STATIC_LINE68((* const line4_3_func[8])) = {
  line4_r0_s3,line4_r1_s3,line4_r2_s3,line4_r3_s3,
  line4_r4_s3,line4_r5_s3,line4_r6_s3,line4_r7_s3,
};

DECL_LINE68(line400)
{
  line4_0_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line401)
{
  line4_0_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line402)
{
  line4_0_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line403)
{
  line4_0_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line404)
{
  line4_0_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line405)
{
  line4_0_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line406)
{
  line4_0_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line407)
{
  line4_0_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line408)
{
  line4_1_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line409)
{
  line4_1_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line40A)
{
  line4_1_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line40B)
{
  line4_1_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line40C)
{
  line4_1_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line40D)
{
  line4_1_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line40E)
{
  line4_1_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line40F)
{
  line4_1_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line410)
{
  line4_2_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line411)
{
  line4_2_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line412)
{
  line4_2_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line413)
{
  line4_2_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line414)
{
  line4_2_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line415)
{
  line4_2_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line416)
{
  line4_2_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line417)
{
  line4_2_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line418)
{
  line4_3_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line419)
{
  line4_3_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line41A)
{
  line4_3_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line41B)
{
  line4_3_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line41C)
{
  line4_3_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line41D)
{
  line4_3_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line41E)
{
  line4_3_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line41F)
{
  line4_3_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line420)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:040 */
#else
  /* CHK.L Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line421)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:041 */
#else
  /* CHK.L An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
#endif
}

DECL_LINE68(line422)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:042 */
#else
  /* CHK.L (An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(2,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line423)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:043 */
#else
  /* CHK.L (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAL(3,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line424)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:044 */
#else
  /* CHK.L -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(4,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line425)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:045 */
#else
  /* CHK.L d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAL(5,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line426)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:046 */
#else
  /* CHK.L d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAL(6,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line427)
{
#ifndef EMU68_68020
  ILLEGAL; /* CHK.L op:047 */
#else
  /* CHK.L <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAL(7,reg0) << LONG_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << LONG_FIX;
  CHKL(s,d);
#endif
}

DECL_LINE68(line428)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line429)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line42A)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line42B)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line42C)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line42D)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line42E)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line42F)
{
  ILLEGAL; /* line:4 op:050 */
  assert(EMU68_BREAK);
}

DECL_LINE68(line430)
{
  /* CHK.W Dn,Dn */
  const uint68_t s = ( (int68_t) REG68.d[reg0] << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line431)
{
  /* CHK.W An,Dn */
  ILLEGAL; /* source An not allowed */
  assert(EMU68_BREAK);
}

DECL_LINE68(line432)
{
  /* CHK.W (An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(2,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line433)
{
  /* CHK.W (An)+,Dn */
  const uint68_t s = ( (int68_t) read_EAW(3,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line434)
{
  /* CHK.W -(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(4,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line435)
{
  /* CHK.W d(An),Dn */
  const uint68_t s = ( (int68_t) read_EAW(5,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line436)
{
  /* CHK.W d(An,Xi),Dn */
  const uint68_t s = ( (int68_t) read_EAW(6,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line437)
{
  /* CHK.W <Ae>,Dn */
  const uint68_t s = ( (int68_t) read_EAW(7,reg0) << WORD_FIX );
        uint68_t d = (int68_t) REG68.d[reg9] << WORD_FIX;
  CHKW(s,d);
}

DECL_LINE68(line438)
{
  /* LEA Dn,An */
  ILLEGAL; /* LEA Dn,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line439)
{
  /* LEA An,An */
  ILLEGAL; /* LEA An,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line43A)
{
  /* LEA (An),An */
  REG68.a[reg9] = (u32) REG68.a[reg0];
}

DECL_LINE68(line43B)
{
  /* LEA (An)+,An */
  ILLEGAL; /* LEA (An)+,An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line43C)
{
  /* LEA -(An),An */
  ILLEGAL; /* LEA -(An),An */
  assert(EMU68_BREAK);
}

DECL_LINE68(line43D)
{
  /* LEA d(An),An */
  REG68.a[reg9] = (u32) ( REG68.a[reg0] + get_nextw() );
}

DECL_LINE68(line43E)
{
  /* LEA d(An,Xi),An */
  REG68.a[reg9] = (u32) get_eal68[6](emu68,reg0);
}

DECL_LINE68(line43F)
{
  /* LEA <Ae>,An */
  REG68.a[reg9] = (u32) get_eal68[7](emu68,reg0);
}

