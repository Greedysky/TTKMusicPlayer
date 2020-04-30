/*
 * @file    desa68.c
 * @brief   68000 disassembler
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "desa68.h"

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

/* ======================================================================
 * Integer types.
 *
 * Only tested with proper stdint.h types, but i should work without
 * in most cases.
 * ====================================================================== */

#if defined(HAVE_STDINT_H)
#include <stdint.h>
typedef int8_t   s8;
typedef uint8_t  u8;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;
#define U32(v) ( (u32) (v) )
#define U16(v) ( (u16) (v) )
#define U8(v)  ( (u8)  (v) )
#define S32(v) ( (s32) (v) )
#define S16(v) ( (s16) (v) )
#define S8(v)  ( (s8)  (v) )
#else
typedef signed char    s8;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef short          s16;
typedef int            s32;
typedef unsigned int   u32;
#define U32(v) ((v) & 0xFFFFFFFF)
#define U16(v) ((v) & 0x0000FFFF)
#define U8(v)  ((v) & 0x000000FF)
#define S32(v) ((s32)(((s32)(v) << (sizeof(s32)-4)) >> (sizeof(s32)-4)))
#define S16(v) ((s16)(((s16)(v) << (sizeof(s16)-2)) >> (sizeof(s16)-2)))
#define S8(v)  ((s8)(v))
#endif

typedef unsigned int uint;

/* ======================================================================
 * Opcode decoding helpers
 * ====================================================================== */

#define REG0(W)  (((W))&7)
#define REG9(W)  (((W)>>9)&7)
#define OPSZ(W)  (((W)>>6)&3)
#define LINE(W)  (((W)>>12)&15)
#define MODE3(W) (((W)>>3)&7)
#define MODE6(W) (((W)>>6)&7)

/* Bit macros */
#define BTST(V,N) ( (V) & ( 1u << (N) ) ) /* bit test */
#define BITV(V,N) ( ( (V) >> (N) ) & 1u ) /* vit value */
#define BIT8(V)   BITV(V,8)


/* ======================================================================
 * Addressing modes
 * ====================================================================== */

/* Junk address for assertion only. ~0 is used as final value but it's
 * mostly the backward compatibility. Only desa68_ref::type should be
 * used to test if the address is valid. */
#define NDEF_ADDR (0x55555555 | (0x55555555 << ((sizeof(int)-4)*8 )))


/* Extended adress mode number. */
enum {
  MODE_DN,   MODE_AN,  MODE_iAN,   MODE_ANp,
  MODE_pAN,  MODE_dAN, MODE_dANXI, MODE_ABSW,
  MODE_ABSL, MODE_dPC, MODE_dPCXI, MODE_IMM,
  MODE_LAST
};

/* Addressing modes bit mask. */
enum {
  MBIT_DN    = 1 << MODE_DN,
  MBIT_AN    = 1 << MODE_AN,
  MBIT_iAN   = 1 << MODE_iAN,
  MBIT_ANp   = 1 << MODE_ANp,
  MBIT_pAN   = 1 << MODE_pAN,
  MBIT_dAN   = 1 << MODE_dAN,
  MBIT_dANXI = 1 << MODE_dANXI,
  MBIT_ABSW  = 1 << MODE_ABSW,
  MBIT_ABSL  = 1 << MODE_ABSL,
  MBIT_dPC   = 1 << MODE_dPC,
  MBIT_dPCXI = 1 << MODE_dPCXI,
  MBIT_IMM   = 1 << MODE_IMM,

  MBIT_SRC   = MBIT_dPC | MBIT_dPCXI | MBIT_IMM,
  MBIT_ALL   = ( 1 << MODE_LAST ) - 1,
  MBIT_DST   = MBIT_ALL & ~MBIT_SRC
};

enum {
  SZ_BYTE = DESA68_OP_B,
  SZ_WORD = DESA68_OP_W,
  SZ_LONG = DESA68_OP_L,
  SZ_IMPL = DESA68_OP_A,
  SZ_ADDR = DESA68_OP_A,
  SZ_NDEF = DESA68_OP_NDEF
};


/* ======================================================================
 * Disassembler string tables
 * ====================================================================== */

/* Hexa digit table */
static const char Thex[16] = {
  '0','1','2','3','4','5','6','7',
  '8','9','A','B','C','D','E','F'
};

/* Branch Condition Code String Tables */
static const u16 bcc_ascii[16] = {
  'RA','SR','HI','LS',
  'CC','CS','NE','EQ',
  'VC','VS','PL','MI',
  'GE','LT','GT','LE'
};

/* Scc Condition Code String Tables */
static const u16 scc_ascii[16] = {
  'T', 'F','HI','LS',
  'CC','CS','NE','EQ',
  'VC','VS','PL','MI',
  'GE','LT','GT','LE'
};

/* Dbcc Condition Code String Tables */
static const u16 dbcc_ascii[16] = {
  'RA', 'F','HI','LS',
  'CC','CS','NE','EQ',
  'VC','VS','PL','MI',
  'GE','LT','GT','LE'
};

/* ======================================================================
 * String functions
 * ====================================================================== */


static int my_isfalse(desa68_t * d, int c)
{
  return 0;
}

static int my_isalnum(desa68_t * d, int c)
{
  return 0
    || (c>='a' && c<='z')
    || (c>='A' && c<='Z')
    || (c>='0' && c<='9');
}

static int my_isgraph(desa68_t * d, int c)
{
  return c >= 32 && c < 127 && c != '\'';
}

/* not really ASCII ... just a name */
static int my_isascii(desa68_t * d, int c)
{
  return
    c == '-' || c=='_' || c==' ' || c == '!' || c == '.' || c == '#'
    || (c>='a' && c<='z')
    || (c>='A' && c<='Z')
    || (c>='0' && c<='9');
}

/* #define desa_char(D,C) (D)->strput((D),(C)) */

/* Add a char to disassembly string */
static void desa_char(desa68_t *d, int c)
{
  if (d->_quote == c)
    d->_quote = 0;
  else if (!d->_quote) {
    if (c == '\'')
      d->_quote = c;
    else if ( (d->flags & DESA68_LCASE_FLAG) && c >= 'A' && c <= 'Z')
      c += 'a'-'A';
  }
  d->strput(d,c);
}

static void def_strput(desa68_t * d, int c)
{
  if (d->out < d->strmax)
    d->str[d->out++] = c;
  else if (d->str) {
    d->error |= DESA68_ERR_OUT;
    if (d->strmax > 0)
      d->str[d->strmax-1] = 0;
  }
}

#if 0
static void def_strput(desa68_t * d, int c)
{
  uint nxt;
  if (c != ' ')
    nxt = d->out + 1;
  else {
    const uint tabs = 13;
    nxt = d->out + tabs - (d->out % tabs);
  }
  while (d->out < nxt) {
    if (d->out < d->strmax)
      d->str[d->out++] = c;
    else {
      if (d->str) {
        d->error |= DESA68_ERR_OUT;
        if (d->strmax > 0)
          d->str[d->strmax-1] = 0;
      }
      nxt = d->out;
    }
  }
}
#endif

/* ======================================================================
 * symbol functions
 * ====================================================================== */

static const char * def_symget(desa68_t * d, uint v, int type)
{
  uint min, max;
  const int flag = (type == DESA68_SYM_DABW || type == DESA68_SYM_DABL)
    ? DESA68_DSTSYM_FLAG : DESA68_SRCSYM_FLAG;

  assert ( d->flags & DESA68_SYMBOL_FLAG );

  /* Select range for symbol candidat */
  if (type == DESA68_SYM_SIMM) {
    min = d->immsym_min;
    max = d->immsym_max;
  } else {
    min = d->memorg;
    max = min+d->memlen;
  }

  if ( (d->flags & flag) || (v >= min && v < max ) ) {
    char * s = d->_tmp; int j;
    *s++ = 'L';
    j = (v >= 0x1000000 ? 5 : 7 ) << 2;
    do {
      *s++ = Thex[ 15 & (v>>j) ];
    } while ( j -= 4 >= 0);
    *s = 0;
    return d->_tmp;
  }
  return 0;
}

static const char * desa_symget(desa68_t * d, uint v, int stype)
{
  assert ( stype > DESA68_SYM_NDEF && stype < DESA68_SYM_LAST );
  return (d->flags & DESA68_SYMBOL_FLAG)
    ? d->symget(d,v,stype) : 0;
}

/* ======================================================================
 * Disassembly output helpers
 * ====================================================================== */

static inline void desa_space(desa68_t * d)
{
  desa_char(d,' ');
}

static inline void desa_comma(desa68_t * d)
{
  desa_char(d,',');
}

/* Add a string to disassembly string */
static void desa_str(desa68_t * d, const char * str)
{
  char c;
  while (c=*str++, c)
    desa_char(d, c);
}

static void desa_str_notr(desa68_t * d, const char * str)
{
  int flags = d->flags;
  d->flags &= ~DESA68_LCASE_FLAG;
  desa_str(d,str);
  d->flags = flags;
}

/* Add a string to disassembly string */
static void desa_ascii(desa68_t * d, uint n)
{
  int shift;
  for (shift=24; shift>=0; shift-=8) {
    u8 c = U8(n >> shift);
    if (c)
      desa_char(d, c);
  }
}

/* Add a N-digit unsigned hexa number with header char
   to disassembly string */
static void desa_uhexacat(desa68_t * d, uint n,
                          int ndigit, int header_char)
{
  int shf;
  desa_char(d,header_char);
  for (shf=(ndigit-1)*4; shf>=0; shf-=4) {
    desa_char(d,Thex[(n>>shf)&15] );
  }
}

/* Add a signifiant digit only unsigned hexa number
 * with heading '$' to disassembly string
 */
static void desa_usignifiant(desa68_t * d, uint n)
{
  int shf;
  desa_char(d,'$');
  for (shf=(sizeof(int)*2-1)*4; shf>=0 && !(n>>shf); shf-=4)
    ;
  if (shf<0) shf=0;
  for (; shf>=0; shf-=4)
    desa_char(d,Thex[(n>>shf)&15] );
}

/* idem desa_usignifiant, but signed */
static void desa_signifiant(desa68_t * d, int n)
{
  if ( n < 0 ) {
    desa_char(d,'-');
    n = -n;
  }
  desa_usignifiant(d,n);
}

/* ======================================================================
 * Memory access
 * ====================================================================== */

static int def_memget(desa68_t * d, uint addr, int flag)
{
  addr &= d->memmsk;
  return (addr >= d->memlen)
    ? -1
    : U8(d->memptr[addr])
    ;
}

static int _uB(desa68_t * d, uint addr, int flag)
{
  int c = d->memget(d,addr,flag);
  if (c < 0) {
    d->error |= DESA68_ERR_MEM;
    c = 0;
  }
  return c;
}

static int _sB(desa68_t * d, uint addr, int flag)
{
  return S8(_uB(d,addr,flag));
}

static int _uW(desa68_t * d, uint addr)
{
  if ( addr & 1 )
    d->error |= DESA68_ERR_ODD;
  return ( _uB(d,addr,2) << 8 ) | _uB(d,addr+1,0);
}

static int _sW(desa68_t * d, uint addr)
{
  return S16(_uW(d,addr));
}

static int _sL(desa68_t * d, uint addr)
{
  if ( addr & 1 )
    d->error |= DESA68_ERR_ODD;
  return 0
    | (_sB(d, addr+0,4)<<24)
    | (_uB(d, addr+1,0)<<16)
    | (_uB(d, addr+2,0)<<8)
    | (_uB(d, addr+3,0)<<0)
    ;
}

/* Read next word and increment pc */
static int _pcW(desa68_t * d)
{
  d->_w = _sW(d, d->pc);
  d->pc += 2;
  return d->_w;
}

/* Read next long and increment pc */
static int _pcL(desa68_t * d)
{
  int r = _sL(d, d->pc);
  d->_w = S16(r);
  d->pc += 4;
  return r;
}

static int immB(desa68_t * d)
{
  int w = _pcW(d), v = S8(w);

  /* How to handle the MSB ?  Need testing on a real 68k but it's most
   * probable that it's simply ignored. However when it comes to
   * disassembly and more specially to sourcing it can be a problem.
   * A re-assembly could differ because of that. Moreover if the MSB
   * is not $00 or the sign extension it is highly probable we are not
   * dealing with a real instruction.
   */
  if (v != w) {
    w &= 0xFF00;                        /* sign extension */
    if (w && w != ( v & 0xff00 ) )
      d->error |= DESA68_ERR_IMM;
  }
  return v;
}

static int immW(desa68_t * d)
{
  return _pcW(d);
}

static int immL(desa68_t * d)
{
  return _pcL(d);
}

static int adrW(desa68_t * d)
{
  return immW(d);
}

static int adrL(desa68_t * d)
{
  return immL(d);
}

static int relPC(desa68_t * d)
{
  const int w = _pcW(d);
  return (d->pc + w - 2) & d->memmsk;
}

/* ======================================================================
 * General disassembly function
 * ====================================================================== */

static inline void ref_set(struct desa68_ref * ref, uint v, int rtype)
{
  v = U32(v);
  /* This assert can be triggered in rare legit cases only if host
   * integers are 32 bits. */
  assert (v != NDEF_ADDR);
  assert (ref->addr == NDEF_ADDR);
  assert (rtype != SZ_NDEF);
  ref->type = rtype;
  ref->addr = v;
}

static void set_branch(desa68_t * d, uint addr, int itype)
{
  assert(itype == DESA68_INT || itype ==DESA68_BRA || itype ==DESA68_BSR);
  d->itype = itype;
  d->dref.type = SZ_ADDR;
  d->dref.addr = addr;
  assert (addr != NDEF_ADDR);
}

static void set_interrupt(desa68_t * d, uint vector)
{
  set_branch(d,vector,DESA68_INT);
}

static void desa_op_DN(desa68_t * d, u8 reg)
{
  assert(reg < 8);
  desa_char(d,'D');
  desa_char(d,'0'+reg);
  d->regs |= 1 << (DESA68_REG_DN+reg);
}

static void desa_op_AN(desa68_t * d, u8 reg)
{
  assert(reg < 8);
  desa_char(d,'A');
  desa_char(d,'0'+reg);
  d->regs |= 1 << (DESA68_REG_AN+reg);
}

static void desa_op_RN(desa68_t * d, u8 reg)
{
  assert(reg < 16);
  desa_char(d,"DA"[reg>>3]);
  desa_char(d,'0'+(reg&7));
  d->regs |= 1 << (DESA68_REG_DN+reg);
}

static void desa_op_iAN(desa68_t * d, u8 reg)
{
  desa_char(d,'(');
  desa_op_AN(d,reg);
  desa_char(d,')');
}

static void desa_op_pAN(desa68_t * d, u8 reg)
{
  desa_char(d,'-');
  desa_op_iAN(d,reg);
}

static void desa_op_ANp(desa68_t * d, u8 reg)
{
  desa_op_iAN(d,reg);
  desa_char(d,'+');
}

/* Could use a table for that. */
static void desa_op_anyreg(desa68_t * d, u8 reg)
{
  uint name;
  if (reg < DESA68_REG_AN)
    name = 'D0'+(reg-DESA68_REG_DN);
  else if (reg <= DESA68_REG_SP)
    name = 'A0'+(reg-DESA68_REG_AN);
  else
    switch (reg) {
    case DESA68_REG_USP: name = 'USP'; break;
    case DESA68_REG_CCR: name = 'CCR'; break;
    case DESA68_REG_SR:  name = 'SR';  break;
    case DESA68_REG_PC:  name = 'PC';  break;
    default:
      assert(!"invalid register");
      name = 'R?';
    }
  desa_ascii(d,name);
  d->regs |= 1 << reg;
}

static void desa_op_xi(desa68_t * d, u16 w)
{
  int r = (w >> 12) & 7;                /* register base */
  if (w & 0x8000)
    desa_op_AN(d,r);
  else
    desa_op_DN(d,r);
  desa_char(d,'.');
  desa_char(d,"WL"[ 1 & (w >> 11)]);
}

static void desa_opsz(desa68_t *d, u8 size)
{
  if (size <= SZ_LONG) {
    desa_char(d,'.');
    desa_char(d,"BWL"[size]);
  }
}


/* Immediate long values can sometime be disassembled as symbols. The
 * default symbol lookup function will do that if the source symbol
 * disassembly is forced (with DESA68_SRCSYM_FLAG) or if the value is
 * in the range [immsym_min:immsym_max-1].
 *
 * It means the source reference is only set if the immediate value is
 * detected as a symbol. It can only happen if DESA68_SYMBOL_FLAG is
 * set. Not sure this is the best way to do it. It could be useful to
 * have eligible address value to the source reference even if the
 * disassembler is not producing auto symbols.
 */
static void desa_immL(desa68_t * d, uint v)
{
  const char * symb = desa_symget(d,v,DESA68_SYM_SIMM);

  assert(!d->_quote);

  if (symb) {
    ref_set(&d->sref, v, SZ_ADDR);
    desa_str_notr(d,symb);
#if 0                                   /* See comment paragraph 2. */
  } else if ( v >= d->immsym_min && v < d->immsym_max ) {
    ref_set(&d->sref, v, SZ_ADDR);
    desa_signifiant(d,S32(v));
#endif
  } else {
    const int q = '\'';
    if (
      d->ischar(d,U8(v)) && d->ischar(d,U8(v>>8)) &&
      d->ischar(d,U8(v>>16)) && d->ischar(d,U8(v>>24))) {
      desa_char(d,q);
      d->_quote = q;
      desa_ascii(d,U32(v));
      /* d->_quote = 0; */
      desa_char(d,q);
    } else {
      desa_signifiant(d,S32(v));
    }
  }
}

static void desa_addr(desa68_t * d, uint v, int stype)
{
  const char * symb = desa_symget(d,v,stype);
  if (symb)
    desa_str_notr(d,symb);
  else
    desa_usignifiant(d, v);
}

static void desa_absL(desa68_t * d, uint v, int stype)
{
  desa_addr(d,v,stype);
}

static void desa_jmp_label(desa68_t * d, uint v)
{
  desa_addr(d,v,DESA68_SYM_SPCR);
}

static void get_ea_2(desa68_t * d,
                     struct desa68_ref * ref, u8 rtype,
                     u8 mode, u8 reg, u8 immsz)
{
  const u8 is_src = ref == &d->sref;
  int v;

  assert(ref == &d->sref || ref == &d->dref);
  assert(ref->type == SZ_NDEF);
  assert(ref->addr == NDEF_ADDR);
  if (mode == MODE_ABSW)
    mode += reg;

  assert( is_src || !( MBIT_SRC & (1<<mode) ) );

  switch (mode) {
  case MODE_DN:
    desa_op_DN(d, reg);
    break;
  case MODE_AN:
    desa_op_AN(d, reg);
    break;
  case MODE_pAN:
    desa_op_pAN(d,reg);
    break;
  case MODE_ANp:
    desa_op_ANp(d,reg);
    break;
  case MODE_dAN:
    _pcW(d);
    desa_signifiant(d,d->_w);
    /* continue on MODE_iAN */
  case MODE_iAN:
    desa_op_iAN(d,reg);
    break;
  case MODE_dANXI:
    v = _pcW(d);                       /* control word */
    desa_signifiant(d,S8(v));
    desa_char(d,'(');
    desa_op_AN(d,reg);
    desa_comma(d);
    desa_op_xi(d,v);
    desa_char(d,')');
    break;
  case MODE_ABSW:
    /* ea_set(d, v = adrW(d)); DELME */
    v = adrW(d);
    desa_absL(d, v, is_src ? DESA68_SYM_SABW : DESA68_SYM_DABW);
    desa_ascii(d,'.W');
    ref_set(ref, v, rtype);
    break;
  case MODE_ABSL:
    /* ea_set(d, v = adrL(d)); DELME */
    v = adrL(d);
    desa_absL(d, v, is_src ? DESA68_SYM_SABL : DESA68_SYM_DABL);
    if ( S16(v) == v )
      desa_ascii(d,'.L');
    ref_set(ref, v, rtype);
    break;
  case MODE_dPC:
    assert(is_src);
    /* ea_set(d, v = relPC(d)); DELME */
    v = relPC(d);
    desa_addr(d,v,DESA68_SYM_SPCR);
    /* Don't use DESA68_REG_PC here, only for (PC,Xi). We don't really
     * need to know if the addressing is PCR but we need to know if
     * the reference is an effective address or a base address.
     *
     * desa_op_anyreg(d, DESA68_REG_PC);
     */
    desa_ascii(d,'(PC)');
    ref_set(ref, v, rtype);
    break;
  case MODE_dPCXI: {
    uint addr;
    assert(is_src);
    v = _pcW(d);
    addr = d->pc-2+S8(v);
    ref_set(ref,addr,rtype);
    desa_addr(d, addr, DESA68_SYM_SPCI);
    desa_char(d,'(');
    desa_op_anyreg(d, DESA68_REG_PC);
    desa_comma(d);
    desa_op_xi(d, v);
    desa_char(d,')');
  } break;
  case MODE_IMM:
    assert(is_src);
    desa_char(d,'#');
    switch (immsz) {
    case SZ_BYTE:
      v = immB(d);
      if (v < -0x3f) v = U8(v);
      desa_signifiant(d,v);
      break;
    case SZ_WORD:
      v = immW(d);
      if (v < -0x3fff) v = U16(v);
      desa_signifiant(d,v);
      break;
    case SZ_LONG:
      v = U32(immL(d));
      desa_immL(d, v);                 /* set the source reference */
      break;

    default:
      assert(!"invalid immediat size");
      desa_ascii(d,'#?');
      break;
    }
    break;

  default:
    assert(!"invalid addressing mode");
    desa_char(d,'?');
    desa_usignifiant(d, mode);
    desa_char(d,'?');
    break;
  }
}

/* Get move operand
 * bit : 0/6 -> src/dest
 * w   : current opcode
 */
static void get_ea_move(desa68_t * d, int bit, int w, u8 easz)
{
  int ea = (w>>bit) & 63;
  assert(bit == 0 || bit == 6);

  if (!bit)
    get_ea_2(d, &d->sref, easz, ea>>3, ea&7, easz);
  else
    get_ea_2(d, &d->dref, easz, ea&7, ea>>3, easz);
}

/* Used with ABCD, SBCD, ADDX, SUBX */
static void desa_ry_rx(desa68_t * d, int inst, int size)
{
  desa_ascii(d,inst);
  if (size != SZ_IMPL)
    desa_opsz(d,size);                  /* ADDX */
  /* else */
  /*   d->opsz = SZ_BYTE; */            /* ABCD */
  desa_space(d);
  if (d->_mode3&1) {             /* -(Ay),-(Ax) */
    desa_op_pAN(d,d->_reg0);
    desa_comma(d);
    desa_op_pAN(d,d->_reg9);
  } else {                      /* Dy,Dx */
    desa_op_DN(d, d->_reg0);
    desa_comma(d);
    desa_op_DN(d, d->_reg9);
  }
  /* d->opsz = size; */

}

/* Used by ALU (add/sub/or/eor/and) */
static void desa_dn_ae(desa68_t * d, int name)
{
  desa_ascii(d,name);
  desa_opsz(d,d->_opsz);
  desa_space(d);
  /*  dn,<ae> */
  if (d->_opw&0400) {
    desa_op_DN(d,d->_reg9);
    desa_comma(d);
    get_ea_2(d, &d->dref, d->_opsz, d->_mode3, d->_reg0, d->_opsz);
  }

  /*  <ae>,dn */
  else {
    get_ea_2(d, &d->sref, d->_opsz, d->_mode3, d->_reg0, d->_opsz);
    desa_comma(d);
    desa_op_DN(d, d->_reg9);
  }
}


static void desa_dcw(desa68_t * d)
{
  desa_ascii(d,'DC.W');
  desa_space(d);
  desa_uhexacat(d,U16(d->_opw),4,'$');
  d->itype = DESA68_DCW;
}

/* ======================================================================
 *
 *   LINE 0 :
 *   -Immediat operations
 *   -SR & CCR operations
 *   -Bit operations
 *   -Movep
 *
 * ====================================================================== */

static int desa_check_bitop(desa68_t * d)
{
  static u32 fn[] = { 'BTST', 'BCHG', 'BCLR', 'BSET' };
  int modemsk = 00775;
  uint inst;

  if (!(modemsk&(1<<d->_adrm0)))
    return 0;

  inst = fn[d->_opsz];
  if (!(d->_opw&0400)) {
    /* B... #x,<AE>*/
    if ((d->_opw&0xF00)!=0x800)
      return 0;
    desa_ascii(d,inst);
    desa_space(d);
    desa_char(d,'#');
    _pcW(d);
    desa_usignifiant(d,U8(d->_w));
  } else {
    /* B... dn,<AE>*/
    desa_ascii(d,inst);
    desa_space(d);
    desa_op_DN(d, d->_reg9);
  }
  desa_comma(d);

  if (!d->_mode3)
    ;
  else
    ;
  get_ea_2(d, &d->dref, SZ_BYTE, d->_mode3, d->_reg0, SZ_NDEF);

  /* if (d->_mode3) */
  /*   set_dst_ref(d, SZ_BYTE);            /\* AE is not a register *\/ */
  /* d->opsz = d->_mode3 ? SZ_BYTE : SZ_LONG; */

  return 1;
}

static int desa_check_IMM_SR(desa68_t * d)
{
  int modemsk = 1 + (1<<2) + (1<<10);
  int mode = (d->_opw>>8)&15, inst='ORI';

  if ((d->_opw&0677)!=0074 || !(modemsk&(1<<mode)))
    return 0;

  switch(mode) {
  case 0xA: /* EORI */
    inst = 'EORI';
    break;
#if 0
  case 0x0: /* ORI */
    inst = 'ORI';
    break;
#endif
  case 0x2: /* ANDI */
    inst = 'ANDI';
    break;
  }
  desa_ascii(d,inst);
  desa_space(d);
  get_ea_2(d, &d->sref, SZ_WORD, MODE_ABSW, MODE_IMM-MODE_ABSW, SZ_WORD);
  desa_comma(d);
  desa_op_anyreg(d, (d->_mode6&1) ? DESA68_REG_SR : DESA68_REG_CCR);
  return 1;
}

static int desa_check_movep(desa68_t * d)
{
  int opsz;
  if ((d->_opw & 0470) != 0410 )
    return 0;

  opsz = SZ_WORD + (d->_opsz & 1);
  desa_str(d,"MOVEP");
  desa_opsz(d,opsz);
  desa_space(d);

  if (!(d->_opw&(1<<7))) {
    /* MOVEP.? d(Ax),Dy */
    get_ea_2(d, &d->sref, opsz, MODE_dAN, d->_reg0, SZ_NDEF);
    desa_comma(d);
    desa_op_DN(d,d->_reg9);
  } else {
    /* MOVEP.? Dx,d(Ay) */
    desa_op_DN(d,d->_reg9);
    desa_comma(d);
    get_ea_2(d, &d->dref, opsz, MODE_dAN, d->_reg0, SZ_NDEF);
  }
  return 1;
}


static int desa_check_imm_op(desa68_t * d)
{
  /* FEDC BA9 8 76 543 210
   * 0000 INS 0 SZ MOD REG */
  static u32 fn[8] = {
    'ORI', 'ANDI', 'SUBI', 'ADDI', 0, 'EORI', 'CMPI', 0
  };
  const int modes = MBIT_DST & ~MBIT_AN;

  if ( (d->_opw & 256) || !( modes & (1<<d->_adrm0) )
       || d->_opsz==3 || !fn[d->_reg9] )
    return 0;

  desa_ascii(d,fn[d->_reg9]);
  desa_opsz(d,d->_opsz);
  desa_space(d);
  /* LONG immediate can be address */
  get_ea_2(d, &d->sref, SZ_ADDR, MODE_ABSW, MODE_IMM-MODE_ABSW, d->_opsz);
  desa_comma(d);
  get_ea_2(d, &d->dref, d->_opsz, d->_mode3, d->_reg0, SZ_NDEF);
  return 1;
}

static void desa_line0(desa68_t * d)
{
  if (!desa_check_movep(d)  &&
      !desa_check_bitop(d)  &&
      !desa_check_IMM_SR(d) &&
      !desa_check_imm_op(d) )
    desa_dcw(d);
}

/* General move(a) disassemble
 */
static void desa_move(desa68_t * d)
{
  static const u8 mvsz[4] = {
    SZ_NDEF, SZ_BYTE, SZ_LONG, SZ_WORD
  };
  const int w = d->_opw;
  const int rtype = mvsz[3 & (w>>12)];
  desa_ascii(d,'MOVE');
  if (d->_adrm6==MODE_AN)
    desa_char(d,'A');                   /* MOVEA */
  desa_opsz(d,rtype);
  desa_space(d);
  get_ea_move(d,0,w,rtype);
  /* set_src_ref(d,rtype); */

  desa_comma(d);
  get_ea_move(d,6,w,rtype);
  /* set_dst_ref(d,rtype); */
}

/* Check and disassemble a valid move
 * return TRUE if valid move
 */
static int desa_check_move(desa68_t * d)
{
  u32 srcmsk = 0xFFF, dstmsk = 0x1FF;
  /* Remove An source & destination addressing mode for byte access */
  if (d->_line==0x1) {
    srcmsk &= ~(1<<MODE_AN);
    dstmsk &= ~(1<<MODE_AN);
  }
  if ((srcmsk&(1<<d->_adrm0)) && (dstmsk&(1<<d->_adrm6)) ) {
    desa_move(d);
    return 1;
  }
  return 0;
}

/* ======================================================================
 *
 *   LINE 1 :
 *   -MOVE.B
 *   LINE 2 :
 *   -MOVE.L
 *   LINE 3 :
 *   -MOVE.W
 *
 * ====================================================================== */

static void desa_li123(desa68_t * d)
{
  if (!desa_check_move(d))
    desa_dcw(d);
}

/* ======================================================================
 *
 *   LINE 4 :
 *   -Other instructions
 *
 * ====================================================================== */

static void get_movemsub(desa68_t * d, u8 i, u8 j)
{
  assert(j <= DESA68_REG_SP);
  assert(i <= j);
  desa_op_anyreg(d, i);
  if ( i != j ) {
    desa_char(d,'-');
    desa_op_anyreg(d, j);
  }
}

static void get_movemreg(desa68_t * d, uint v, uint rev)
{
  int i,j,p=0;

  assert(v < 0x10000);
  assert(rev == 0 || rev == 15);

  for (i=0 ; i<16; i++) {
    for (; i<16 && (v&(1<<(i^rev)))==0; i++);
    if (i==16) break;
    j = i;
    for (; i<16 && (v&(1<<(i^rev))); i++)
      d->regs |= 1<<(DESA68_REG_DN+i);
    if (p) desa_char(d,'/');
    get_movemsub(d,j,(i-1));
    p = 1;
  }
}

static int desa_check_imp(desa68_t * d, uint name, int mskmode,
                          u8 opsz, u8 memsz)
{
  if ((d->_opw&0400) || !(mskmode&(1<<d->_adrm0)))
    return 0;
  desa_ascii(d,name);
  desa_opsz(d,opsz);
  desa_space(d);

  get_ea_2(d, &d->sref, opsz, d->_mode3, d->_reg0, SZ_NDEF);

  if (opsz <= SZ_LONG)
    d->dref = d->sref;
  return 1;
}

static int desa_check_lea(desa68_t * d)
{
  int modemsk = 03744;
  if ((d->_opw&0700) != 0700 || !(modemsk&(1<<d->_adrm0)))
    return 0;
  desa_ascii(d,'LEA');
  desa_space(d);

  get_ea_2(d, &d->sref, SZ_ADDR, d->_mode3, d->_reg0, SZ_NDEF);
  desa_comma(d);
  desa_op_AN(d,d->_reg9);
  return 1;
}

static int desa_check_chk(desa68_t * d)
{
  int modemsk = 07775;
  if ((d->_opw&0700) != 0600 || !(modemsk&(1<<d->_adrm0)))
    return 0;
  desa_ascii(d,'CHK');
  desa_space(d);
  get_ea_2(d, &d->sref, SZ_WORD, d->_mode3, d->_reg0, SZ_WORD);
  desa_comma(d);
  desa_op_DN(d, d->_reg9);
  set_interrupt(d, 0x18);
  return 1;
}

static int desa_check_ext(desa68_t * d)
{
  if ((d->_opw&07670)!=04200)
    return 0;
  desa_ascii(d,'EXT');
  desa_opsz(d,1+(1&(d->_opw>>6)));
  desa_space(d);
  desa_op_DN(d,d->_reg0);
  return 1;
}

static int desa_check_movem(desa68_t * d)
{
  int modemsk, regmsk;
  int mode3, reg0, w, opsz;
  if ((d->_opw&05600)!=04200)
    return 0;
  modemsk = !(d->_opw&02000) ? 00764 : 03754;
  if (!(modemsk&(1<<d->_adrm0)))
    return 0;

  desa_str(d,"MOVEM");
  opsz = 1 + (1 & (d->_opw>>6));
  desa_opsz(d,opsz);
  desa_space(d);

  w = d->_opw;
  mode3 = d->_mode3;
  reg0 = d->_reg0;
  regmsk = U16(immW(d));

  if (w&02000) {
    /*  -> reg */
    get_ea_2(d, &d->sref, opsz, mode3, reg0, SZ_NDEF);
    desa_comma(d);
    get_movemreg(d,regmsk,0);
  } else {
    /* -> mem */
    get_movemreg(d, regmsk, (mode3==MODE_pAN) ? 15 : 0);
    desa_comma(d);
    get_ea_2(d, &d->dref, opsz, mode3, reg0, SZ_NDEF);
  }
  return 1;
}

static int desa_check_jmp(desa68_t * d)
{
  /* FEDCBA987 6 543 210
   * 010011101 T MOD REG T:= (0:JSR 1:JMP) */
  int type;
  const int modes =
    MBIT_ALL & ~(MBIT_DN|MBIT_AN|MBIT_ANp|MBIT_pAN|MBIT_IMM);

  if ( (d->_opw & 0xFF80) != 0x4E80 || ! (modes & (1<<d->_adrm0)) )
    return 0;

  desa_char(d,'J');
  if (d->_opsz==2) {
    type = DESA68_BSR;
    desa_ascii(d,'SR');
  } else {
    type = DESA68_BRA;
    desa_ascii(d,'MP');
  }
  desa_space(d);
  get_ea_2(d, &d->sref, SZ_ADDR, d->_mode3, d->_reg0, SZ_NDEF);
  if (d->sref.type != SZ_NDEF)
    set_branch(d, d->sref.addr, type);
  return 1;
}

static int desa_check_line4_mode3(desa68_t * d)
{
  if (d->_mode6 != 3)
    return 0;

  switch(d->_reg9) {

  case 0:                               /* MOVE FROM SR */
    if (!(00775 & (1<<d->_adrm0)))
      break;
    desa_ascii(d,'MOVE');
    desa_space(d);
    desa_op_anyreg(d, DESA68_REG_SR);
    desa_comma(d);
    get_ea_2(d, &d->dref, SZ_WORD, d->_mode3, d->_reg0, SZ_WORD);
    return 1;

  case 2:                               /* MOVE TO CCR */
    if (!(07775 & (1<<d->_adrm0)))
      break;
    desa_ascii(d,'MOVE');
    desa_space(d);
    /* Word indeed ;) */
    get_ea_2(d, &d->sref, SZ_WORD, d->_mode3, d->_reg0, SZ_BYTE);
    desa_comma(d);
    desa_op_anyreg(d,DESA68_REG_CCR);
    return 1;

  case 3:                               /* MOVE TO SR */
    if (!(07775 & (1<<d->_adrm0)))
      break;
    desa_ascii(d,'MOVE');
    desa_space(d);
    get_ea_2(d, &d->sref, SZ_WORD, d->_mode3, d->_reg0, SZ_WORD);
    desa_comma(d);
    desa_op_anyreg(d,DESA68_REG_SR);
    return 1;

  case 5:                               /* TAS */
    if (desa_check_imp(d, 'TAS', 00775, SZ_IMPL, SZ_BYTE)) {
      return 1;
    } else if (d->_w == 0x4AFC) {
      desa_str(d,"ILLEGAL");
      set_interrupt(d, 0x10);
      return 1;
    }
  }
  return 0;
}


static void desa_line4(desa68_t * d)
{
  switch (d->_mode6) {

  case 7:
    if (desa_check_lea(d)) return;      /* LEA <ea>,An */
    /* TODO for 32bit support */        /* EXTB.L (>=68020) */
    break;

  case 6:                               /* CHK <ea>,Dn*/
    if (desa_check_chk(d)) return;
    break;

  case 4:                               /* CHK.L (>=68020) */
    break;

  case 3:
  case 2:
    if (desa_check_ext(d))   return;
    if (desa_check_movem(d)) return;
    if (desa_check_jmp(d))   return;
    if (desa_check_line4_mode3(d)) return; /* mode 3 only ? */

  default:
    switch(d->_reg9) {
    case 0:                             /* NEGX */
      if (desa_check_imp(d, 'NEGX', 00775, d->_opsz, d->_opsz))
        return;
      break;

    case 1:                             /* CLR */
      if (desa_check_imp(d, 'CLR', 00775, d->_opsz, d->_opsz)) {
        /* d->sref.type = SZ_NDEF; */ /* to keep only dst ref */
        return;
      }
      break;

    case 2:                             /* NEG */
      if (desa_check_imp(d, 'NEG', 00775, d->_opsz, d->_opsz))
        return;
      break;

    case 3:                             /* NOT */
      if (desa_check_imp(d, 'NOT', 00775, d->_opsz, d->_opsz))
        return;
      break;

    case 4:
      if (d->_mode6==0) {
        /* NBCD */
        if (desa_check_imp(d, 'NBCD', 00775, SZ_IMPL, SZ_BYTE))
          return;
      } else if (d->_mode6==1) {
        if (d->_mode3 == MODE_DN) {
          /* SWAP */
          desa_ascii(d,'SWAP');
          desa_space(d);
          desa_op_DN(d, d->_reg0);
          return;
        } else {
          /* PEA */
          if (desa_check_imp(d, 'PEA', 0x7E4, SZ_IMPL, SZ_ADDR))
            return;
        }
      }
      break;

    case 5:                             /* TST */
      if (desa_check_imp(d, 'TST', 00775, d->_opsz, d->_opsz)) {
        /* d->dref.type = SZ_NDEF; */ /* to keep only src ref */
        return;
      }
      break;

    case 6:
      break;

    case 7:
      if (d->_mode6 == 1) {
        /* FUNKY LINE 4 */
        switch(d->_mode3) {
        case 0:
        case 1: {
          const int num = d->_opw & 0xF;
          /* TRAP #$X 0100 1110 0100 TRAP */
          assert ( (d->_opw & 0xFFF0) == 0x4E40);
          desa_ascii(d,'TRAP');
          desa_space(d);
          desa_ascii(d,'#$');
          desa_char(d, Thex[num]);
          set_interrupt(d,0x80 + (num<<2));
          /* d->opsz = SZ_IMPL; */
        } return;
        case 2:
          /* LINK An,#d 0100 1110 0101 0 REG */
          assert ( (d->_opw & 0xFFF8) == 0x4E50);
          desa_ascii(d,'LINK');
          desa_space(d);
          desa_op_AN(d,d->_reg0);
          desa_comma(d);
          get_ea_2(d, &d->sref, SZ_IMPL, MODE_ABSW, MODE_IMM-MODE_ABSW, SZ_WORD);
          d->regs |= 1 << DESA68_REG_SP;
          /* d->opsz = SZ_IMPL; */
          return;
        case 3:
          /* UNLK An 0100 1110 0101 1 REG */
          assert ( (d->_opw & 0xFFF8) == 0x4E58);
          desa_ascii(d,'UNLK');
          desa_space(d);
          desa_op_AN(d,d->_reg0);
          d->regs |= 1 << DESA68_REG_SP;
          /* d->opsz = SZ_IMPL; */
          return;
        case 4:
          /* MOVE An,USP 0100 1110 0110 0 REG */
          assert ( (d->_opw & 0xFFF8) == 0x4E60);
          desa_ascii(d,'MOVE');
          desa_space(d);
          desa_op_AN(d,d->_reg0);
          desa_comma(d);
          desa_op_anyreg(d,DESA68_REG_USP);
          /* d->opsz = SZ_LONG; */
          return;

        case 5:
          /* MOVE USP,An  0100 1110 0110 1 REG */
          assert ( (d->_opw & 0xFFF8) == 0x4E68);
          desa_ascii(d,'MOVE');
          desa_space(d);
          desa_op_anyreg(d,DESA68_REG_USP);
          desa_comma(d);
          desa_op_AN(d,d->_reg0);
          /* d->opsz = SZ_LONG; */
          return;

        case 6: {
          /* FUNKY LINE 4 MODE 6 (4E */
          static char *str[8] = { /* $4E70 - $4E77 */
            /* 0      1     2       3   4   5      6      7 */
            "RESET","NOP","STOP ","RTE",0,"RTS","TRAPV","RTR"
          };
          assert ( (d->_opw & 0xFFF8) == 0x4E70);

          if (d->_reg0 == 4)
            break;

          if ((d->_reg0 & 1))
            d->itype = (d->_reg0 == 1) ? DESA68_NOP : DESA68_RTS;

          desa_str(d, str[d->_reg0]);
          if (d->_reg0 == 2)
            /* STOP */
            get_ea_2(d, &d->sref, SZ_WORD, MODE_IMM, 0,SZ_WORD);
        } return;
        }
      }
    }
  }
  desa_dcw(d);
}


/* ======================================================================
 *
 *   LINE 5:
 *   - ADDQ #x,<ea>
 *   - SUBQ #x,<ea>
 *   - Scc <ea>
 *   - Dcc dn,Addr
 *
 * ====================================================================== */

static int desa_check_addq_subq(desa68_t * d)
{
  int v;
  const int modes = MBIT_DST;

  /* ADDQ #VAL,<ea> 0101 VAL 0 SZ ADRMOD
   * SUBQ #VAL,<ea> 0101 VAL 1 SZ ADRMOD */

  assert (d->_opsz != 3);      /* tested upstream */
  if ( ! (modes & (1<<d->_adrm0) ) )
    return 0;

  v = d->_reg9;
  v += (!v) << 3;
  desa_ascii(d, BTST(d->_opw,8) ? 'SUBQ':'ADDQ');
  desa_opsz(d,d->_opsz);
  desa_space(d);
  desa_ascii(d,'#0' + v);
  desa_comma(d);
  get_ea_2(d, &d->dref, d->_opsz, d->_mode3, d->_reg0, d->_opsz);

  return 1;
}

static void desa_Dcc(desa68_t * d)
{
  int addr;

  /* DBcc Dn,<addr> 0101 COND 11001 REG <offset.w> */

  assert ( (d->_opw & 0xF0F8) == 0x50C8 );

  desa_ascii(d,('DB'<<16) + dbcc_ascii[(d->_opw>>8)&15]);
  desa_space(d);
  desa_op_DN(d,d->_reg0);
  desa_comma(d);
  addr = relPC(d);
  desa_jmp_label(d, addr);
  set_branch(d,addr,DESA68_BSR);
}

static int desa_check_Scc(desa68_t * d)
{
  const int modes = MBIT_DST & ~MBIT_AN;

  /* Scc <ae> 0101 COND 11 ADRMOD */

  assert ( ( d->_opw & 0xF0C0 ) == 0x50C0 );

  if ( !( modes & (1<<d->_adrm0) ) )
    return 0;

  desa_char(d,'S');
  desa_ascii(d,scc_ascii[(d->_opw>>8)&15]);
  desa_space(d);
  get_ea_2(d, &d->dref, SZ_BYTE, d->_mode3, d->_reg0, SZ_NDEF);
  return 1;
}

static void desa_line5(desa68_t * d)
{
  if (d->_opsz == 3) {
    if (d->_adrm0 == MODE_AN)
      desa_Dcc(d);
    else if (!desa_check_Scc(d))
      desa_dcw(d);
  } else if (!desa_check_addq_subq(d)) {
    desa_dcw(d);
  }
}


/* ======================================================================
 *
 *   LINE 6 :
 *   -Bcc
 *
 * Format 0110 COND OFFSET08 [OFFSET16 if OFFSET08==0]
 * !!! COND=0001(false) => BSR
 *
 * ====================================================================== */

static void desa_line6(desa68_t * d)
{
  int a, cond = (d->_opw>>8) & 0xF;

  desa_ascii(d,('B'<<16) + bcc_ascii[cond]);

  /* Bcc.S */
  if (d->_opw & 255) {
    desa_ascii(d,'.S');
    a = (int)S8(d->_opw);
    a += (int)d->pc;
  }
  /* Bcc.W */
  else {
    desa_ascii(d,'.W');
    a = relPC(d);
  }
  desa_space(d);
  desa_jmp_label(d, a);
  set_branch(d, a, !cond ? DESA68_BRA : DESA68_BSR);

}

/* ======================================================================
 *
 *   LINE 7 - MOVEQ
 *
 *
 ***************/
static void desa_line7(desa68_t * d)
{
  /* MOVEQ #VAL,Dn 01111 REG 0 8-BITVAL */

  if ( BTST(d->_opw,8) ) {
    desa_dcw(d);
  } else {
    int v = S8(d->_opw);
    desa_str(d,"MOVEQ");
    desa_space(d);
    desa_char(d,'#');
    desa_signifiant(d,v);
    desa_comma(d);
    desa_op_DN(d,d->_reg9);
  }
}


/* ======================================================================
 *
 *   LINE B :
 *   -CMP  <ea>,Dn
 *   -CMPA <ea>,An
 *   -CMPM (An)+,(An)+,
 *   -EOR  <ea>,Dn
 *
 *
 * ====================================================================== */

static int desa_check_cmpa(desa68_t * d)
{
  int modes = MBIT_ALL, opsz;

  /* CMPA <ea>,An 1011 REG S11 ADRMOD */
  if ( d->_opsz != 3 || !(modes & (1<<d->_adrm0)) )
    return 0;
  desa_ascii(d,'CMPA');
  opsz = SZ_WORD + BIT8(d->_opw);
  desa_opsz(d,opsz);
  desa_space(d);
  get_ea_2(d, &d->sref, opsz, d->_mode3, d->_reg0, opsz);
  desa_comma(d);
  desa_op_AN(d,d->_reg9);
  return 1;
}

static int desa_check_eor_cmp(desa68_t * d)
{
  int modes, inst;

  if (d->_opsz == 3)
    return 0;

  /* EOR Dn,<ea> 1011 REG 1SZ ADRMOD */
    if (d->_opw & 256) {
    modes = MBIT_ALL & ~(MBIT_AN|MBIT_SRC);
    inst = 'EOR';
  }
  /* CMP <ea>,Dn 1011 REG 0SZ ADRMOD */
  else {
    modes = MBIT_ALL & ~(!d->_opsz ? MBIT_AN : 0);
    inst = 'CMP';
  }
  if ( ! (modes & (1<<d->_adrm0) ) )
    return 0;

  desa_dn_ae(d,inst);
  return 1;
}

static int desa_check_cmpm(desa68_t * d)
{
  /* CMPM (Ay)+,(Ax)+ 1011 RAX 1 SZ 001 RAY */
  /* Equiv EOR Dn,An */
  if ((d->_opw&0470) != 0410)
    return 0;
  desa_ascii(d,'CMPM');
  desa_space(d);
  desa_op_ANp(d, d->_reg0);
  desa_comma(d);
  desa_op_ANp(d, d->_reg9);
  return 1;
}

static void desa_lineB(desa68_t * d)
{
  if ( !desa_check_cmpm(d) &&
       !desa_check_cmpa(d) &&
       !desa_check_eor_cmp(d)
    )   desa_dcw(d);
}

/* ======================================================================
 *
 * LINE 8 -  OR / SBCD / DIV
 * LINE C - AND / ABCD / MUL / EXG
 *
 * ====================================================================== */

static int desa_check_exg(desa68_t * d)
{
  /* EXG Dx,Dy 1100 RGx 101000 RGy
   * EXG Ax,Ay 1100 RGx 101001 RGy
   * EXG Dx,Ay 1100 RGx 110001 RGy */
  u8 rx,ry;
  switch ( d->_opw & 0770) {
  case 0500:
    rx = ry = 0;
    break;
  case 0510:
    rx = ry = 8;
    break;
  case 0610:
    rx = 0; ry = 8;
    break;
  default:
    return 0;
  }
  desa_ascii(d,'EXG');
  desa_space(d);
  desa_op_RN(d, d->_reg9 + rx);
  desa_comma(d);
  desa_op_RN(d, d->_reg0 + ry);
  return 1;
}

static int desa_check_mul_div(desa68_t * d)
{
  /* MUL <ea>,Dn 1100 REG U11 ADRMOD U:{Unsigned,Signed)
   * DIV <ea>,Dn 1000 REG U11 ADRMOD */

  const int modes = MBIT_ALL & ~MBIT_AN;

  if ( d->_opsz != 3 || !( modes & (1<<d->_adrm0) ) )
    return 0;
  desa_ascii(d, BTST(d->_opw,14) ? 'MUL' : 'DIV');
  desa_char(d, BTST(d->_opw,8) ? 'S' : 'U');
  desa_space(d);
  get_ea_2(d, &d->sref, SZ_WORD, d->_mode3, d->_reg0, SZ_WORD);
  desa_comma(d);
  desa_op_DN(d,d->_reg9);
  return 1;
}

static int desa_check_abcd_sbcd(desa68_t * d)
{
  if ( (d->_opw&0x1f0) != 0x100 )
    return 0;
  desa_ry_rx(d,(d->_line==0xC) ? 'ABCD' : 'SBCD', SZ_IMPL);
  return 1;
}

static int desa_check_and_or(desa68_t * d)
{
  int modemsk = !(d->_opw&0400)? 0xFFD : 0x1FC;
  if ( ! (modemsk & (1<<d->_adrm0) ) )
    return 0;
  desa_dn_ae(d, (d->_line==0xC) ? 'AND' : 'OR');
  return 1;
}

static void desa_lin8C(desa68_t * d)
{
  if (!desa_check_abcd_sbcd(d) &&
      !desa_check_mul_div(d) &&
      !desa_check_exg(d) &&
      !desa_check_and_or(d))
    desa_dcw(d);
}

/* ======================================================================
 *
 *   LINE 9 - SUB / SUBX / SUBA
 *   LINE D - ADD / ADDX / ADDA
 *
 * ====================================================================== */

static int desa_check_addx(desa68_t * d)
{
  /* ADDX Dy,Dx       1101 RGx 1 SZ 00 0 RGy
   * ADDX -(Ay),-(Ax) 1101 RGx 1 SZ 00 1 RGy
   * SUBX Dy,Dx       1001 RGx 1 SZ 00 0 RGy
   * SUBX -(Ay),-(Ax) 1001 RGx 1 SZ 00 1 RGy */

  assert( d->_opsz != 3 );               /* tested upstream */

  if ( (d->_opw & 0460) != 0400 )
    return 0;
  desa_ry_rx(d, BTST(d->_opw,14) ? 'ADDX' : 'SUBX', d->_opsz);
  return 1;
}

static int desa_check_adda(desa68_t * d)
{
  /* ADDA 1101 REG S11 ADRMOD  S: {.W,.L}
   * SUBA 1001 REG S11 ADRMOD
   */
  const int modes = MBIT_ALL;
  int opsz;

  assert(d->_opsz == 3);                /* tested ipstream */

  if (!(modes & (1<<d->_adrm0)))
    return 0;

  opsz = SZ_WORD + BIT8(d->_opw);
  desa_ascii(d, BTST(d->_opw,14) ? 'ADDA' : 'SUBA');
  desa_opsz(d,opsz);
  desa_space(d);
  get_ea_2(d, &d->sref, opsz, d->_mode3, d->_reg0, opsz);
  desa_comma(d);
  desa_op_AN(d, d->_reg9);
  return 1;
}

static int desa_check_add(desa68_t * d)
{
  /* SUB <ea>,Dn 1001 REG 0SZ ADRMOD
   * SUB Dn,<ea> 1001 REG 1SZ ADRMOD
   * ADD <ea>,Dn 1101 REG 0SZ ADRMOD
   * ADD Dn,<ea> 1101 REG 1SZ ADRMOD */

  int modes = BTST(d->_opw,8) ? MBIT_DST : MBIT_ALL;

  assert( d->_opsz != 3 );              /* tested upstream */

  if (d->_opsz == 0)             /* Forbid An for byte access size. */
    modes &= ~MBIT_AN;

  if ( ! (modes & (1<<d->_adrm0) ) )
    return 0;
  desa_dn_ae(d, BTST(d->_opw,14) ? 'ADD' : 'SUB');
  return 1;
}

static void desa_lin9D(desa68_t * d)
{
  if (d->_opsz == 3) {
    if (desa_check_adda(d))
      return;
  } else {
    if (desa_check_addx(d) ||
        desa_check_add(d))
      return;
  }
  desa_dcw(d);
}

/* ======================================================================
 *
 * LINE E - bit Shift
 *
 * ====================================================================== */

static void desa_lineE(desa68_t * d)
{
  /* SHIFT Dx,Dy 1110 RGx D SZ 1 TY RGy TY:type D:direction
   * SHIFT #N,Dy 1110 VAL D SZ 0 TY RGy
   * SHIFT <ea>  1110 0TY D 11 -ADRMOD- */

  static u16 shf_ascii[4] = { 'AS', 'LS', 'RO', 'RO' };

  /* <ea> */
  if (d->_opsz == 3) {
    const int modes = MBIT_DST & ~(MBIT_DN|MBIT_AN);
    const int type = d->_reg9;
    if ( !(modes & (1<<d->_adrm0)) || (type&4)) {
      desa_dcw(d);
      return;
    }
    desa_ascii(d,shf_ascii[type]);
    if (type == 2) desa_char(d,'X');
    desa_char(d, BTST(d->_opw,8) ? 'L' :'R');
    desa_space(d);
    get_ea_2(d, &d->sref, SZ_BYTE, d->_mode3, d->_reg0, SZ_NDEF);
    d->dref = d->sref;
  }

  /* Dn */
  else {
    const int type = ( d->_opw >> 3) & 3;
    desa_ascii (d,shf_ascii[type]);
    if (type==2) desa_char(d,'X');
    desa_char (d, (d->_opw&0400) ? 'L' :'R');
    desa_opsz(d,d->_opsz);
    desa_space(d);
    /* Dn,Dn */
    if (d->_opw&(1<<5))
      desa_op_DN(d,d->_reg9);
    /* #N,Dn */
    else {
      desa_char(d,'#');
      desa_char(d,'1' + ( ( d->_reg9 - 1 ) & 7 ));
    }
    desa_comma(d);
    desa_op_DN(d,d->_reg0);
  }
}

/* ======================================================================
 *
 * LINE A/F - instruction emulator
 *
 * ====================================================================== */

static void desa_linAF(desa68_t * d)
{
  desa_dcw(d);                      /* before set_interrupt() call. */
  set_interrupt(d, 0x40 + ( BITV(d->_opw,12) << 2 ) );
}

/* ======================================================================
 *
 * Main
 *
 * ====================================================================== */

static void (*desa_line[16])(desa68_t *) =
{
  desa_line0, desa_li123, desa_li123, desa_li123,
  desa_line4, desa_line5, desa_line6, desa_line7,
  desa_lin8C, desa_lin9D, desa_linAF, desa_lineB,
  desa_lin8C, desa_lin9D, desa_lineE, desa_linAF,
};

static void rtcheck(void)
{
  assert( sizeof(s8)  == 1 );
  assert( sizeof(u8)  == 1 );
  assert( sizeof(s16) >= 2 );
  assert( sizeof(u16) >= 2 );
  assert( sizeof(s32) >= 4 );
  assert( sizeof(u32) >= 4 );
  assert( U8(0x18D) == 0x8D );
  assert( S8(0x181) == -127 );
  assert( U16(0x15D5D) == 0x5D5D );
  assert( S16(0x18001) == -32767 );
  assert( U32(~0) == 0xFFFFFFFF );
  assert( S32(~1) == -2 );
}

int desa68(desa68_t * d)
{
  rtcheck();

  /* Clean slate. */
  d->sref.type = SZ_NDEF;
  d->sref.addr = NDEF_ADDR;
  d->dref  = d->sref;
  d->regs  = 0;                /* clear used registers. */
  d->itype = DESA68_INS;       /* assume valid generic instruction. */
  d->error = 0;                /* clear error flags. */

  /* Setup 68k memory access. */
  if (!d->memget)
    d->memget = def_memget;
  if (!d->memmsk)
    d->memmsk = (1<<24)-1;

  /* Setup symbol control */
  if (!d->symget)
    d->symget = def_symget;
  if (!d->immsym_min && !d->immsym_max) {
    d->immsym_min = d->memorg;
    d->immsym_max = d->memorg + d->memlen;
  }

  /* Setup output string writer. */
  if (!d->strput)
    d->strput = def_strput;
  if (!d->str)
    d->strmax = 0;
  d->out = 0;

  /* Setup the ASCII disassembly. */
  if (!d->ischar)
    switch(d->flags & DESA68_GRAPH_FLAG) {
    case DESA68_ASCII_FLAG:
      d->ischar = my_isascii; break;
    case DESA68_ALNUM_FLAG:
      d->ischar = my_isalnum; break;
    case DESA68_GRAPH_FLAG:
      d->ischar = my_isgraph; break;
    case 0:
    default:
      d->ischar = my_isfalse; break;
    }

  /* Setup instruction decoder. */
  d->_pc = d->pc &= d->memmsk;
  _pcW(d);

  d->_quote = 0;
  d->_opw   = U16(d->_w);
  d->_reg0  = REG0(d->_opw);
  d->_reg9  = REG9(d->_opw);
  d->_mode3 = MODE3(d->_opw);
  d->_mode6 = MODE6(d->_opw);
  d->_line  = LINE(d->_opw);
  d->_opsz  = OPSZ(d->_opw);
  d->_adrm0 = d->_mode3 + ((d->_mode3==MODE_ABSW)? d->_reg0 : 0);
  d->_adrm6 = d->_mode6 + ((d->_mode6==MODE_ABSW)? d->_reg9 : 0);

  (desa_line[d->_line])(d);
  desa_char(d,0);

  /* Restore current status to caller struct */
  d->pc &= d->memmsk;

  assert(d->sref.type == SZ_NDEF || d->sref.addr != NDEF_ADDR);
  assert(d->dref.type == SZ_NDEF || d->dref.addr != NDEF_ADDR);

  if (d->sref.type != SZ_NDEF)
    d->sref.addr &= d->memmsk;
  else
    d->sref.addr = ~0;

  if (d->dref.type != SZ_NDEF)
    d->dref.addr &= d->memmsk;
  else
    d->dref.addr = ~0;

  return d->error ? DESA68_ERR : d->itype;
}

int desa68_version(void)
{
#ifndef PACKAGE_VERNUM
# define PACKAGE_VERNUM 0
#endif
  return PACKAGE_VERNUM;
}

const char * desa68_versionstr(void)
{
#ifndef PACKAGE_STRING
# define PACKAGE_STRING "desa68 n/a"
#endif
  return PACKAGE_STRING;
}
