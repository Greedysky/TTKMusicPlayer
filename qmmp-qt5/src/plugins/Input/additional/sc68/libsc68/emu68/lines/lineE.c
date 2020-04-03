/* lineE.c - EMU68 generated code by
 * gen68 2013-05-25 00:26:49
 * Copyright (c) 1998-2013 Benjamin Gerard
 *
 * $Id: lineE.c 458 2014-07-03 05:29:47Z benjihan $
 */

/* Line E: Shift/Rotate/Bit Field */

static void ASR_mem(emu68_t * const emu68, int reg, int mode)
{
  /* ASR.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  ASRW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void LSR_mem(emu68_t * const emu68, int reg, int mode)
{
  /* LSR.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  LSRW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void ROXR_mem(emu68_t * const emu68, int reg, int mode)
{
  /* ROXR.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  ROXRW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void ROR_mem(emu68_t * const emu68, int reg, int mode)
{
  /* ROR.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  RORW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void ASL_mem(emu68_t * const emu68, int reg, int mode)
{
  /* ASL.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  ASLW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void LSL_mem(emu68_t * const emu68, int reg, int mode)
{
  /* LSL.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  LSLW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void ROXL_mem(emu68_t * const emu68, int reg, int mode)
{
  /* ROXL.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  ROXLW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void ROL_mem(emu68_t * const emu68, int reg, int mode)
{
  /* ROL.W <Ae> */
  const addr68_t l = get_EAW(mode,reg);
         int68_t a = read_W(l)<<WORD_FIX;
  ROLW(a,a,1);
  write_W(l,a>>WORD_FIX);
}

static void (*const lslmemR_fc[4])(emu68_t *const,int,int)=
{
  ASR_mem,  LSR_mem,  ROXR_mem,  ROR_mem,
};

static void (*const lslmemL_fc[4])(emu68_t *const,int,int)=
{
  ASL_mem,  LSL_mem,  ROXL_mem,  ROL_mem,
};

DECL_LINE68(lineE00)
{
  /* ASR.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ASRB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE01)
{
  /* LSR.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  LSRB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE02)
{
  /* ROXR.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ROXRB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE03)
{
  /* ROR.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  RORB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE04)
{
  /* ASR.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ASRB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE05)
{
  /* LSR.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  LSRB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE06)
{
  /* ROXR.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ROXRB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE07)
{
  /* ROR.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  RORB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE08)
{
  /* ASR.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ASRW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE09)
{
  /* LSR.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  LSRW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE0A)
{
  /* ROXR.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ROXRW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE0B)
{
  /* ROR.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  RORW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE0C)
{
  /* ASR.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ASRW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE0D)
{
  /* LSR.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  LSRW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE0E)
{
  /* ROXR.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ROXRW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE0F)
{
  /* ROR.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  RORW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE10)
{
  /* ASR.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ASRL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE11)
{
  /* LSR.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  LSRL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE12)
{
  /* ROXR.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ROXRL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE13)
{
  /* ROR.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  RORL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE14)
{
  /* ASR.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ASRL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE15)
{
  /* LSR.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  LSRL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE16)
{
  /* ROXR.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ROXRL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE17)
{
  /* ROR.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  RORL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE18)
{
  lslmemR_fc[reg9&3](emu68,reg0,0);
}

DECL_LINE68(lineE19)
{
  lslmemR_fc[reg9&3](emu68,reg0,1);
}

DECL_LINE68(lineE1A)
{
  lslmemR_fc[reg9&3](emu68,reg0,2);
}

DECL_LINE68(lineE1B)
{
  lslmemR_fc[reg9&3](emu68,reg0,3);
}

DECL_LINE68(lineE1C)
{
  lslmemR_fc[reg9&3](emu68,reg0,4);
}

DECL_LINE68(lineE1D)
{
  lslmemR_fc[reg9&3](emu68,reg0,5);
}

DECL_LINE68(lineE1E)
{
  lslmemR_fc[reg9&3](emu68,reg0,6);
}

DECL_LINE68(lineE1F)
{
  lslmemR_fc[reg9&3](emu68,reg0,7);
}

DECL_LINE68(lineE20)
{
  /* ASL.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ASLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE21)
{
  /* LSL.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  LSLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE22)
{
  /* ROXL.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ROXLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE23)
{
  /* ROL.B #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ROLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE24)
{
  /* ASL.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ASLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE25)
{
  /* LSL.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  LSLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE26)
{
  /* ROXL.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ROXLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE27)
{
  /* ROL.B Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<BYTE_FIX;
  ROLB(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & BYTE_MSK) + (a>>BYTE_FIX);
}

DECL_LINE68(lineE28)
{
  /* ASL.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ASLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE29)
{
  /* LSL.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  LSLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE2A)
{
  /* ROXL.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ROXLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE2B)
{
  /* ROL.W #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ROLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE2C)
{
  /* ASL.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ASLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE2D)
{
  /* LSL.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  LSLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE2E)
{
  /* ROXL.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ROXLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE2F)
{
  /* ROL.W Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<WORD_FIX;
  ROLW(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & WORD_MSK) + (a>>WORD_FIX);
}

DECL_LINE68(lineE30)
{
  /* ASL.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ASLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE31)
{
  /* LSL.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  LSLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE32)
{
  /* ROXL.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ROXLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE33)
{
  /* ROL.L #d,Dn */
  const int d = ((reg9-1)&7)+1;
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ROLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE34)
{
  /* ASL.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ASLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE35)
{
  /* LSL.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  LSLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE36)
{
  /* ROXL.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ROXLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE37)
{
  /* ROL.L Dn,Dn */
  const int d = REG68.d[reg9];
   uint68_t a = (uint68_t)REG68.d[reg0]<<LONG_FIX;
  ROLL(a,a,d);
  REG68.d[reg0] = (REG68.d[reg0] & LONG_MSK) + (a>>LONG_FIX);
}

DECL_LINE68(lineE38)
{
  lslmemL_fc[reg9&3](emu68,reg0,0);
}

DECL_LINE68(lineE39)
{
  lslmemL_fc[reg9&3](emu68,reg0,1);
}

DECL_LINE68(lineE3A)
{
  lslmemL_fc[reg9&3](emu68,reg0,2);
}

DECL_LINE68(lineE3B)
{
  lslmemL_fc[reg9&3](emu68,reg0,3);
}

DECL_LINE68(lineE3C)
{
  lslmemL_fc[reg9&3](emu68,reg0,4);
}

DECL_LINE68(lineE3D)
{
  lslmemL_fc[reg9&3](emu68,reg0,5);
}

DECL_LINE68(lineE3E)
{
  lslmemL_fc[reg9&3](emu68,reg0,6);
}

DECL_LINE68(lineE3F)
{
  lslmemL_fc[reg9&3](emu68,reg0,7);
}

