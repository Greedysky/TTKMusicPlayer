/*
 * @file    unice68_unpack.c
 * @brief   Ice Depacker 2.40 (native version)
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "unice68.h"

#ifdef TYPE_U8
typedef TYPE_U8 u8
#else
typedef unsigned char u8;
#endif

#ifdef TYPE_S8
typedef TYPE_S8 s8
#else
typedef signed char s8;
#endif

#ifdef TYPE_S16
typedef TYPE_S16 s16
#else
typedef signed short s16;
#endif

typedef u8 * areg_t;
typedef int dreg_t;

typedef struct {
  areg_t a0,a1,a2,a3,a4,a5,a6,a7;
  dreg_t d0,d1,d2,d3,d4,d5,d6,d7;
  areg_t srcbuf,srcend,dstbuf,dstend;
  int overflow;
} all_regs_t;

#define ICE_MAGIC 0x49434521 /* 'ICE!' */

#define B_CC(CC, LABEL) if (CC) {goto LABEL;} else
#define DB_CC(CC, REG, LABEL) if (!(CC) && --REG >= 0) {goto LABEL;} else
#define DBF(REG,LABEL) DB_CC(0, REG, LABEL)
#define GET_1_BIT_BCC(LABEL) B_CC(!get_1_bit(R), LABEL)
#define DBF_COUNT(REG) ( ( (REG) & 0xFFFF ) + 1 )

static const int direkt_tab[] = {
  0x7fff000e,0x00ff0007,0x00070002,0x00030001,0x00030001,
  270-1,        15-1,    8-1,    5-1,    2-1
};

static const u8 length_tab[] = {
  9,1,0,-1,-1,
  8,4,2,1,0
};

static const s8 more_offset[] = {
  11,   4,   7,  0,
  0x01,0x1f, -1,-1, 0x00,0x1F
};

static const int more_offset_word[] = {
  0x0b04,0x0700,0x011f,-1,0x001f
};

#if 0
static void dump_regs(const char * label)
{
  if (label) {
    fprintf(stderr, "%s:\n", label);
  }
#define DUMP(N) fprintf(stderr,"d%d=%08X a%d=%p\n",N,d##N,N,a##N)
  DUMP(0);
  DUMP(1);
  DUMP(2);
  DUMP(3);
  DUMP(4);
  DUMP(5);
  DUMP(6);
  DUMP(7);
}
#endif


static void strings(all_regs_t *);
static void normal_bytes(all_regs_t *);
static int get_d0_bits(all_regs_t *, int d0);

static inline int chk_dst_range(all_regs_t *R, const areg_t a, const areg_t b)
{
  R->overflow |= (a <  R->dstbuf) << 0;
  R->overflow |= (b >= R->dstend) << 1;
  return R->overflow;
}

static inline int chk_src_range(all_regs_t *R, const areg_t a, const areg_t b)
{
  R->overflow |= (a <  R->srcbuf) << 2;
  R->overflow |= (b >= R->srcend) << 3;
  return R->overflow;
}

/* getinfo:     moveq   #3,d1 */
/* getbytes: lsl.l      #8,d0 */
/*      move.b  (a0)+,d0 */
/*      dbf     d1,getbytes */
/*      rts */
static inline int getinfo(all_regs_t *R)
{
  const areg_t a0 = R->a0;
  const int r0 = (0[a0]<<24) + (1[a0]<<16) + (2[a0]<<8) + 3[a0];
  R->a0 = a0+4;
  return r0;
}

/* get_1_bit: */
/*      add.b   d7,d7 */
/*      bne.s   bitfound */
/*      move.b  -(a5),d7 */
/*      addx.b  d7,d7 */
/* bitfound: */
/*      rts */

static inline int get_1_bit(all_regs_t *R)
{
  int r;

  r = (R->d7 & 255) << 1;
  B_CC(r & 255, bitfound);

  chk_src_range(R,R->a5-1,R->a5-1);

  r = (r>>8) + (*(--R->a5) << 1);
bitfound:
  R->d7 = (R->d7 & ~0xFF) | (r & 0xFF);
  return r >> 8;
}


/* ice_decrunch:
 *  a0 = Pointer to packed data
 *  a1 = Address to which the data is unpacked
 *
 * During depacking:
 *
 *  a5 : current packed data pointer (read from bottom to top)
 *  a6 : current depacked data pointer (written from bottom to top)
 *  a4 : depack buffer top, used to detect end of depacking.
 */

static int ice_decrunch(all_regs_t *R)
{
  int id;
  int csize;
  int dsize;

/*      movem.l d0-a6,-(a7) */
/*      bsr.s   getinfo */
/*      cmpi.l  #'ICE!',d0 */
/*      bne     not_packed */
/*      bsr.s   getinfo  */
/*      lea.l   -8(a0,d0.l),a5 */
/*      bsr.s   getinfo */
/*      move.l  d0,(a7) */
/*      move.l  a1,a4 */
/*      move.l  a1,a6 */
/*      adda.l  d0,a6 */
/*      move.l  a6,a3 */
/*      move.b  -(a5),d7 */
/*      bsr     normal_bytes */

  R->srcbuf = R->a0;
  R->dstbuf = R->a1;

  id = getinfo(R);        /* Works with 'Ice!' too */
  if ( ( id & ~0x202000 ) != ICE_MAGIC) {
    return -1;
  }
  csize = getinfo(R);
  R->srcend = R->a5 = R->a0 - 8 + csize;
  R->d0 = dsize = getinfo(R);
  R->a6 = R->a4 = R->a1;
  R->a6 += R->d0;
  R->dstend = R->a3 = R->a6;

  R->d7 = *(--R->a5);
  normal_bytes(R);

/*      move.l  a3,a6 */
/*      bsr     get_1_bit */
/*      bcc.s   not_packed */
/*      move.w  #$0f9f,d7 */
/*      bsr     get_1_bit */
/*      bcc.s   ice_00 */
/*      moveq   #15,d0   */
/*      bsr     get_d0_bits */
/*      move.w  d1,d7 */

  R->a6 = R->a3;
  GET_1_BIT_BCC(not_packed);
  R->d7 = 0x0f9f;
  GET_1_BIT_BCC(ice_00);
  R->d7 = R->d1 = get_d0_bits(R, 15);

/* ice_00:      moveq   #3,d6 */
/* ice_01:      move.w  -(a3),d4 */
/*      moveq   #3,d5 */
/* ice_02:      add.w   d4,d4 */
/*      addx.w  d0,d0 */
/*      add.w   d4,d4 */
/*      addx.w  d1,d1 */
/*      add.w   d4,d4 */
/*      addx.w  d2,d2 */
/*      add.w   d4,d4 */
/*      addx.w  d3,d3 */
/*      dbra    d5,ice_02 */
/*      dbra    d6,ice_01 */
/*      movem.w d0-d3,(a3) */
/*      dbra    d7,ice_00 */

ice_00:
  R->d6 = 3;
ice_01:
  R->a3 -= 2;
  R->d4 = (R->a3[0]<<8) | R->a3[1];
  R->d5 = 3;
ice_02:
  R->d4 += R->d4;
  R->d0 += R->d0 + (R->d4>>16);
  R->d4 &= 0xFFFF;

  R->d4 += R->d4;
  R->d1 += R->d1 + (R->d4>>16);
  R->d4 &= 0xFFFF;

  R->d4 += R->d4;
  R->d2 += R->d2 + (R->d4>>16);
  R->d4 &= 0xFFFF;

  R->d4 += R->d4;
  R->d3 += R->d3 + (R->d4>>16);
  R->d4 &= 0xFFFF;

  DBF(R->d5,ice_02);
  DBF(R->d6,ice_01);

  if (chk_src_range(R, R->a3, R->a3+7)) {
    goto not_packed;
  }

  0[R->a3] = R->d0 >> 8;
  1[R->a3] = R->d0;

  2[R->a3] = R->d1 >> 8;
  3[R->a3] = R->d1;

  4[R->a3] = R->d2 >> 8;
  5[R->a3] = R->d2;

  6[R->a3] = R->d3 >> 8;
  7[R->a3] = R->d3;

  DBF(R->d7,ice_00);

not_packed:
  return -!!R->overflow;
}

static void normal_bytes(all_regs_t *R)
{
/* normal_bytes:         */
/*      bsr.s   get_1_bit */
/*      bcc.s   test_if_end */
/*      moveq.l #0,d1 */
/*      bsr.s   get_1_bit */
/*      bcc.s   copy_direkt */
/*      lea.l   direkt_tab+20(pc),a1 */
/*      moveq.l #4,d3 */
/* nextgb:      move.l  -(a1),d0 */
/*      bsr.s   get_d0bits */
/*      swap.w  d0 */
/*      cmp.w   d0,d1 */
/*      dbne    d3,nextgb */
/* no_more:     add.l   20(a1),d1 */
/* copy_direkt:  */
/*      move.b  -(a5),-(a6) */
/*      dbf     d1,copy_direkt */
/* test_if_end:  */
/*      cmpa.l  a4,a6 */
/*      bgt.s   strings */
/*      rts      */


  while (1) {
    const int * tab;

    GET_1_BIT_BCC(test_if_end);
    R->d1 = 0;
    GET_1_BIT_BCC(copy_direkt);

    tab = direkt_tab + (20>>2);
    R->d3 = 4;
 nextgb:
    R->d0 = * (--tab);
    R->d1 = get_d0_bits(R, R->d0);
    R->d0 = (R->d0 >> 16) | ~0xFFFF;
    DB_CC((R->d0^R->d1)&0xFFFF, R->d3, nextgb);
/*   no_more: */
    R->d1 += tab[(20>>2)];

 copy_direkt:
    {
      const int cnt = DBF_COUNT(R->d1);
      if (chk_dst_range(R, R->a6-cnt, R->a6-1) |
          chk_src_range(R, R->a5-cnt, R->a5-1)) {
        break;
      }
    }
 lp_copy:
    *(--R->a6) = *(--R->a5);
    DBF(R->d1,lp_copy);

 test_if_end:
    if (R->a6 <= R->a4) {
      if (R->a6 < R->a4) {
        chk_dst_range(R, R->a6, R->a6);
      }
      break;
    }
    strings(R);
  }
}



/* get_d0_bits: */
/*      moveq.l #0,d1 */
/* hole_bit_loop:        */
/*      add.b   d7,d7 */
/*      bne.s   on_d0 */
/*      move.b  -(a5),d7 */
/*      addx.b  d7,d7 */
/* on_d0:       addx.w  d1,d1 */
/*      dbf     d0,hole_bit_loop */
/*      rts      */

static int get_d0_bits(all_regs_t *R, int r0)
{
  int r7 = R->d7;
  int r1 = 0;

  r0 &= 0xFFFF;
  if (r0 > 15) {
    R->overflow |= (1 << 4);
    return 0;
  }

hole_bit_loop:
  r7 = (r7 & 255) << 1;
  B_CC(r7 & 255, on_d0);

  chk_src_range(R,R->a5-1,R->a5-1);

  r7 = (*(--R->a5) << 1) + (r7>>8);
on_d0:
  r1 += r1 + (r7>>8);
  DBF(r0,hole_bit_loop);
  R->d7 = (R->d7 &~0xFF) | (r7 & 0xFF);
  R->d0 |= 0xFFFF;
  return r1;
}

static void strings(all_regs_t *R)
{
/* strings: */
/*      lea.l   length_tab(pc),a1 */
/*      moveq.l #3,d2 */
/* get_length_bit:       */
/*      bsr.s   get_1_bit */
/*      dbcc    d2,get_length_bit */
/* no_length_bit:        */
/*      moveq.l #0,d4 */
/*      moveq.l #0,d1 */
/*      move.b  1(a1,d2.w),d0 */
/*      ext.w   d0 */
/*      bmi.s   no_Ober */
/* get_Ober: bsr.s      get_d0_bits */
/* no_Ober:     move.b  6(a1,d2.w),d4 */
/*      add.w   d1,d4 */
/*      beq.s   get_offset_2 */

  R->a1 = (areg_t)length_tab;
  R->d2 = 3;
get_length_bit:
  DB_CC(get_1_bit(R)==0, R->d2, get_length_bit);
/* no_length_bit:        */
  R->d4 = R->d1 = 0; /* $$$ d4 is not needed here. */
  R->d0 = (R->d0 & ~0xFFFF) | (0xFFFF & (s8)R->a1[ 1 + (s16)R->d2 ]);
  B_CC(R->d0&0x8000, no_Ober);
/*  get_Ober: */
  R->d1 = get_d0_bits(R, R->d0);
  R->d0 |= 0xFFFF;
no_Ober:
  R->d4 = R->a1 [ 6 + (s16)R->d2 ];
  R->d4 += R->d1;
  B_CC(R->d4==0,get_offset_2);

/*      lea.l   more_offset(pc),a1 */
/*      moveq.l #1,d2 */
/* getoffs:     bsr.s   get_1_bit */
/*      dbcc    d2,getoffs */
/*      moveq.l #0,d1 */
/*      move.b  1(a1,d2.w),d0 */
/*      ext.w   d0 */
/*      bsr.s   get_d0_bits */
/*      add.w   d2,d2 */
/*      add.w   6(a1,d2.w),d1 */
/*      bpl.s   depack_bytes */
/*      sub.w   d4,d1 */
/*      bra.s   depack_bytes */

  R->a1 = (areg_t)more_offset;
  R->d2 = 1;
getoffs:
  DB_CC(get_1_bit(R)==0,R->d2,getoffs);

  R->d1 = get_d0_bits(R,(int)(s8)more_offset[1+(s16)R->d2]);
  R->d0 |= 0xFFFF;
  R->d1 += more_offset_word[3+(s16)R->d2];
  if (R->d1 < 0) {
    R->d1 -= R->d4;
  }
  goto depack_bytes;


  /* get_offset_2:       */
  /*    moveq.l #0,d1 */
  /*    moveq.l #5,d0 */
  /*    moveq.l #-1,d2 */
  /*    bsr.s   get_1_bit */
  /*    bcc.s   less_40 */
  /*    moveq.l #8,d0 */
  /*    moveq.l #$3f,d2 */
  /* less_40:   bsr.s   get_d0_bits */
  /*    add.w   d2,d1 */

get_offset_2:
  R->d1 = 0;
  R->d0 = 5;
  R->d2 = -1;
  GET_1_BIT_BCC(less_40);
  R->d0 = 8;
  R->d2 = 0x3f;
less_40:
  R->d1 = get_d0_bits(R, R->d0);
  R->d0 |= 0xFFFF;
  R->d1 += R->d2;

  /* depack_bytes: */
  /*    lea.l   2(a6,d4.w),a1 */
  /*    adda.w  d1,a1 */
  /*    move.b  -(a1),-(a6) */
  /* dep_b:     move.b  -(a1),-(a6) */
  /*    dbf     d4,dep_b */
  /*    bra     normal_bytes */

depack_bytes:
  R->a1 = R->a6 + 2 + (s16)R->d4 + (s16)R->d1;
  chk_dst_range(R, R->a6 - DBF_COUNT(R->d4) - 1, R->a6-1);
  if (R->a6>R->a4) *(--R->a6) = *(--R->a1);
dep_b:
  if (R->a6>R->a4) *(--R->a6) = *(--R->a1);
  DBF(R->d4,dep_b);
}

int unice68_depacked_size(const void * buffer, int * p_csize)
{
  int id, csize, dsize;
  int csize_verif = p_csize ? *p_csize : 0;
  all_regs_t allregs;

  if (csize_verif && csize_verif<12) {
    return -1;
  }

  allregs.a0 = (areg_t)buffer;
  id = getinfo(&allregs);
  if ( ( id & ~0x202000 ) != ICE_MAGIC) {
    return -1;
  }
  csize = getinfo(&allregs);
  if (csize < 12) {
    return -2;
  }
  /* csize -= 12; */
  dsize = getinfo(&allregs);
  if (p_csize) {
    *p_csize = csize;
  }
  if (csize_verif && (csize != csize_verif)) {
    dsize = ~dsize;
  }
  return dsize;
}

int unice68_depacker(void * dest, const void * src)
{
  all_regs_t allregs;

  allregs.d0 =
    allregs.d1 =
    allregs.d2 =
    allregs.d3 =
    allregs.d4 =
    allregs.d5 =
    allregs.d6 =
    allregs.d7 = 0;

  allregs.a2 =
    allregs.a3 =
    allregs.a4 =
    allregs.a5 =
    allregs.a6 =
    allregs.a7 = 0;

  allregs.a0 = (areg_t)src;
  allregs.a1 = dest;
  allregs.overflow = 0;

  return ice_decrunch(&allregs);
}
