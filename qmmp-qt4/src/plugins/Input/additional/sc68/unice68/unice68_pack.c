/*
 * @file    ice68_pack.c
 * @brief   Ice Packer 2.35 (native version)
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
#ifndef PACKAGE_NAME
# define PACKAGE_NAME "unice68"
#endif
#ifndef PACKAGE_VERSION
# define PACKAGE_VERSION __DATE__
#endif
#ifndef PACKAGE_VERNUM
# define PACKAGE_VERNUM 0
#endif

#include "unice68.h"

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

typedef uint8_t * areg_t;
typedef     int   dreg_t;

typedef struct {
  areg_t a0,a1,a2,a3,a4,a5,a6,ax;
  dreg_t d0,d1,d2,d3,d4,d5,d6,d7;
  areg_t srcbuf,srcend,dstbuf,dstend;
  int srclen, dstlen, dstmax;
  int error, optimize, maxlength, maxgleich, maxoffset;
} all_regs_t;

#define ICE_MAGIC 0x49636521 /* 'Ice!' */

#define EQ(A,B) ((A) == (B))
#define NE(A,B) ((A) != (B))

#define GE(A,B) ((A) <= (B))
#define LE(A,B) ((A) >= (B))
#define HI(A,B) ((A) <  (B))
#define LS(A,B) ((A) >= (B))
#define GT(A,B) ((A) <  (B))
#define LT(A,B) ((A) >  (B))

#define B_CC(CC, LABEL) if (CC) {goto LABEL;} else
#define DB_CC(CC, REG, LABEL) if (!(CC) && --REG >= 0) {goto LABEL;} else
#define DBF(REG,LABEL) DB_CC(0, REG, LABEL)
#define DBF_COUNT(REG) ( ( (REG) & 0xFFFF ) + 1 )

#define BRA(LABEL) goto LABEL
#define BEQ(A, B, LABEL) B_CC( EQ(A,B), LABEL )
#define BNE(A, B, LABEL) B_CC( NE(A,B), LABEL )
#define BHI(A, B, LABEL) B_CC( HI(A,B), LABEL )
#define BGE(A, B, LABEL) B_CC( GE(A,B), LABEL )
#define BGT(A, B, LABEL) B_CC( GT(A,B), LABEL )
#define BLS(A, B, LABEL) B_CC( LS(A,B), LABEL )
#define BLE(A, B, LABEL) B_CC( LE(A,B), LABEL )
#define BLT(A, B, LABEL) B_CC( LT(A,B), LABEL )

#define DBGE(A,B,REG,LABEL) DB_CC( GE(A,B), REG, LABEL )


static void longword_store(all_regs_t *R);
static void put_bits(all_regs_t * R);
static void make_stringlength(all_regs_t * R);
static void make_normal_bytes(all_regs_t * R);

/* Store d7.l
 */
static void longword_store(all_regs_t *R)
{
  *R->a1++ = R->d7 >> 24;
  *R->a1++ = R->d7 >> 16;
  *R->a1++ = R->d7 >>  8;
  *R->a1++ = R->d7;
}

/* Store bit field
 *
 * d1[d4+1] bit field
 * d7:      bit acu
 * d6:      free bit in d7 minus 1
 */
static void put_bits(all_regs_t *R)
{
  do {
    R->d7 |= (R->d1 & 1) << 8;
    R->d1 >>= 1;
    R->d7 >>= 1;
    if (--R->d6 < 0) {
      *R->a1++ = R->d7;
      R->d7 = 0;
      R->d6 = 7;
    }
  } while (--R->d4 >= 0);
}


static void make_offset_2(all_regs_t * R)
{
  /* move.w     d0,d1 */
  R->d1 = R->d0;
  /* cmpi.w     #$3f,d1 */
  /* ble.s      offs_3f */
  BLE ( 0x3f, R->d1, offs_3f );
  /* subi.w     #$40,d1 */
  R->d1 -= 0x40;
  /* moveq      #9,d4 */
  R->d4 = 9;
  /* bset       d4,d1 */
  R->d1 |= 1 << R->d4;
  /* bra        put_bits */
  goto put;
offs_3f:
  /* moveq      #6,d4 */
  R->d4 = 6;
put:
  /* bra.s      put_bits */
  put_bits(R);
}

static void make_offset_mehr(all_regs_t * R)
{
  static const int16_t table3[6] = {
    0x0000,0x0020,0x0120,
    0x0606,0x0908,0x0C0D
  };
  /* moveq        #2,d3 */
  R->d3 = 2;
look_on:
  /* add.w   d3,d3 */
  /* move.w  0(a3,d3.w),d4 */
  /* lsr.w   #1,d3 */
  R->d4 = table3[R->d3];

  /* cmp.w   d4,d0 */
  /* dbge    d3,look_on */
  DBGE ( R->d4, R->d0, R->d3, look_on );
  /* sub.w   d4,d0 */
  R->d0 -= R->d4;

  /* add.w   d3,d3 */
  /* move.w  6(a3,d3.w),d3 */
  /* move.w  d3,d4 */
  R->d4 = R->d3 = table3[3+R->d3];
  /* lsr.w   #8,d3 */
  R->d3 >>= 8;
  /* moveq   #-1,d1 */
  /* lsl.w   d3,d1 */
  /* or.w    d0,d1 */
  R->d1 = (-1 << R->d3) | R->d0;
  /* andi.w  #$f,d4 */
  R->d4 &= 0xf;
  /* bra     put_bits */
  put_bits(R);
}

static void make_stringlength(all_regs_t *R)
{
  static const int ta[5] = { 0x2, 0x3, 0x4, 0x6, 0xa };
  static const int tb[5] = { 0x1, 0x1, 0x2, 0x3, 0xa };
  int i, d4, d0;

  for (i=4, d0 = R->maxlength;
       d0 < (d4 = ta[i]);
       --i);
  d0 -= d4;
  d4 = tb[i];
  R->d1 = (-1 << d4) | d0;
  R->d4 = d4 + i - 1;
  put_bits(R);
}

static int ice_crunch(all_regs_t *R)
{
  R->a0 = R->srcbuf;
  R->d0 = R->srclen;

mainloop:

/***********************************************************************
 * 1. Sequence of identical bytes are looking for pay
 */

  /* lea        $409(a0),a4 */
  R->a4 = R->a0 + 0x409;            /* a4 = End of the search range */
  /* cmpa.l     src_ende,a4 */
  /* ble.s      gleich_ok */
  BLE ( R->srcend, R->a4, gleich_ok);
  /* movea.l    src_ende,a4 */
  R->a4 = R->srcend;

gleich_ok:
/* a0 : search start
 * a3 : search current
 * a4 : search end
 */

  /* move.l     a0,a3 */
  R->a3 = R->a0;              /* a3 = Beginning of the search range */
  /* move.b     (a3)+,d0 */
  R->d0 = *R->a3++;                     /* current byte */

  /* while ( R->d0 == *R->a3++ && R->a3 < R->a4) */
  /*   ; */

gleich_compare:
  /* cmp.b      (a3)+,d0 */
  /* bne.s      gleich_ende */
  BNE( *R->a3++, R->d0, gleich_ende );
  /* cmpa.l     a4,a3 */
  /* blt.s      gleich_compare */
  BLT ( R->a4, R->a3, gleich_compare );
gleich_ende:

  /* move.l     a3,d1 */
  /* sub.l      a0,d1 */
  /* subq.l     #2,d1 */
  /* move.l     d1,maxgleich */
  R->maxgleich = R->d1 = R->a3 - R->a0 - 2;

  /* cmp.l      #$409,d1 */
  /* beq        gleich_ablegen */
  BEQ (0x409, R->d1, gleich_ablegen);


/*************************************************************
 * 2. Search string with the greatest possible length and a small offset
 */

  /* move.l     a0,a3 */
  /* adda.l     optimize(pc),a3 */
  R->a3 = R->a0 + R->optimize;
  /* cmpa.l     src_ende,a3 */
  /* ble.s      offset_ende */
  BLE ( R->srcend, R->a3, offset_ende );
  /* movea.l    src_ende,a3 */
  R->a3 = R->srcend;

offset_ende:
  /* if (R->a3 > R->srcend) */
  /*   R->a3 = R->srcend; */

  /* moveq      #1,d4 */
  R->d4 = 1;
  /* lea        2(a0),a4 */
  R->a4 = R->a0 + 2;

weiter_mit_string:
  /* move.b     (a0),d0 */
  R->d0 = *R->a0;
  /* move.b     1(a0),d1 */
  R->d1 = R->a0[1];

search_string:
  /* cmp.b      (a4)+,d0 */
  /* beq.s      ein_byte_stimmt */
  BEQ ( *R->a4++, R->d0, ein_byte_stimmt );

no_string:
  /* cmpa.l     a4,a3 */
  /* bgt.s      search_string */
  BGT ( R->a4, R->a3, search_string );
  /* bra.s      string_suche_fertig */
  BRA(string_suche_fertig);

ein_byte_stimmt:
  /* cmp.b      (a4),d1 */
  /* bne.s      no_string */
  BNE ( *R->a4, R->d1, no_string );

/* string_start_found: */
  /* move.l     a4,a6 */
  /* subq.w     #1,a6 */
  R->a6 = R->a4 - 1;
  /* move.l     a6,d0 */
  R->ax = R->a6;
  /* movea.l    a0,a5 */
  R->a5 = R->a0;

string_compare:
  /* cmpm.b     (a5)+,(a6)+ */
  /* bne.s      string_zu_ende */
  BNE ( *R->a5++, *R->a6++, string_zu_ende);
  /* cmp.l      d0,a5 */
  /* bhi.s      string_zu_ende */
  BHI( R->ax, R->a5, string_zu_ende );
  /* cmpa.l     a6,a3 */
  /* bgt.s      string_compare */
  BGT ( R->a6, R->a3, string_compare );

string_zu_ende:
  /* move.l     a5,d1 */
  /* sub.l      a0,d1 */
  /* subq.l     #1,d1 */
  R->d1 = R->a5 - R->a0 - 1;

  /* move.l     a6,d2 */
  /* sub.l      a5,d2 */
  /* sub.l      d1,d2 */
  /* addq.l     #1,d2 */
  R->d2 = R->a6 - R->a5 - R->d1 + 1;

  /* move.l     #$409,d0 */
  R->d0 = 0x409;                        /* max string length */

  /* cmp.l      d0,d4 */
  /* beq.s      string_suche_fertig */
  BEQ ( R->d0, R->d4, string_suche_fertig );

  /* cmp.l      d0,d1 */
  /* bls.s      not_too_large */
  BLS ( R->d0, R->d1, not_too_large );

  /* sub.l      d0,d1 */
  R->d1 -= R->d0;
  /* add.l      d1,d2 */
  R->d2 += R->d1;

  /* move.l     d0,d1 */
  R->d1  = R->d0;
not_too_large:

  /* cmp.l      d1,d4 */
  /* bge.s      string_lohnt_nicht */
  BGE( R->d1, R->d4, string_lohnt_nicht );
  /* cmp.w      #2,d1 */
  /* bls.s      shortstring */
  BLS ( 2, R->d1, shortstring );
  /* cmp.w      #$111f,d2 */
  /* bhi.s      string_lohnt_nicht */
  BHI ( 0x111f, R->d2, string_lohnt_nicht );

shortstring:
  /* move.l     d1,d4 */
  /* move.w     d1,maxlength */
  /* move.w     d2,maxoffset */
  R->maxlength = R->d4 = R->d1;
  R->maxoffset = R->d2;

string_lohnt_nicht:
  /* cmpa.l     a4,a3 */
  /* bgt        weiter_mit_string */
  BGT ( R->a4, R->a3, weiter_mit_string );

string_suche_fertig:
  /* move.l     maxgleich,d0 */
  R->d0 = R->maxgleich;
  /* cmp.w      #1,d0 */
  /* ble.s      nogleich */
  BLE ( 1, R->d0, nogleich );
  /* cmp.l      d4,d0 */
  /* bge.s      gleich_ablegen */
  BGE( R->d4, R->d0, gleich_ablegen );

nogleich:

  /* move.w     maxoffset(pc),d0 */
  R->d0 = R->maxoffset;
  /* move.w     d4,d1 */
  R->d1 = R->d4;
  /* cmpi.w     #1,d1 */
  /* beq.s      ein_byte_ablegen */
  BEQ ( 1, R->d1, ein_byte_ablegen );
  /* cmpi.w     #2,d1 */
  /* beq.s      zwei_byte_ablegen */
  BEQ ( 2, R->d1, zwei_byte_ablegen );
  /* bra.s      mehr_bytes_ablegen */
  BRA(mehr_bytes_ablegen);

/* ====================================================================== */

ein_byte_ablegen:
  /* move.b     (a0)+,(a1)+ */

  /* fprintf(stdout,"%04x %02x (%s:%d) / ein_byte_ablegen\n", */
  /*         R->a1 - R->dstbuf, */
  /*         *R->a0, */
  /*         __FUNCTION__, __LINE__); */
  *R->a1++ = *R->a0++;
  /* addq.l     #1,d5 */
  R->d5++;
  /* bra.s      kein_byte_ablegen */
  BRA ( kein_byte_ablegen );

gleich_ablegen:
  /* move.l     maxgleich,d1 */
  R->d1 = R->maxgleich;
  /* move.w     d1,maxlength */
  R->maxlength = R->d1;
  /* moveq      #0,d0 */
  R->d0 = 0;
  /* subq.l     #2,d1 */
  /* bne.s      mehr_bytes_ablegen */
  if ( (R->d1 -= 2) != 0 )
    goto mehr_bytes_ablegen;

zwei_byte_ablegen:
  /* cmpi.w     #$23f,d0 */
  /* bhi.s      ein_byte_ablegen */
  BHI( 0x23f, R->d0, ein_byte_ablegen );
  /* bsr        make_normal_bytes */
  make_normal_bytes(R);
  /* cmpi.w     #$23f,d0 */
  /* bhi        ein_byte_ablegen */
  BHI( 0x23f, R->d0, ein_byte_ablegen );
  /* bsr        make_offset_2 */
  make_offset_2(R);
  /* bra.s      drop_length */
  BRA(drop_length);

mehr_bytes_ablegen:
  /* bsr        make_normal_bytes */
  make_normal_bytes(R);
  /* bsr        make_offset_mehr */
  make_offset_mehr(R);

drop_length:
  /* bsr        make_stringlength */
  make_stringlength(R);
  /* moveq      #0,d5 */
  R->d5 = 0;
  /* move.w     maxlength(pc),d0 */
  R->d0 = R->maxlength;
  /* add.w      d0,a0 */
  R->a0 += R->d0;

kein_byte_ablegen:
  /* move.l     src_ende,d0 */
  /* sub.l      a0,d0 */
  /* subq.l     #3,d0 */
  /* bpl        mainloop */
  if ( ( R->d0 = R->srcend - R->a0 - 3 ) >= 0 )
    goto mainloop;

still_packing:
  /* cmp.l      src_ende,a0 */
  /* bge.s      all_packed */
  BGE ( R->srcend, R->a0, all_packed );
  /* move.b     (a0)+,(a1)+ */
  /* fprintf(stdout,"%04x %02x (%s:%d) / still_packing\n", */
  /*         R->a1 - R->dstbuf, */
  /*         *R->a0, */
  /*         __FUNCTION__ , __LINE__); */
  *R->a1++ = *R->a0++;
  /* addq.l     #1,d5 */
  R->d5++;
  /* bra.s      still_packing */
  BRA(still_packing);

all_packed:
  /* bsr        make_normal_bytes */
  make_normal_bytes(R);
  /* bset       d6,d7 */
  /* move.b     d7,(a1)+ */

  /* fprintf(stdout,"%04x %02x (%s:%d) /all_packed\n", */
  /*         R->a1 - R->dstbuf, */
  /*         R->d7 | (1 << R->d6), */
  /*         __FUNCTION__ , __LINE__); */
  *R->a1++ = R->d7 |= (1 << R->d6);
  /* sub.l   packed_data(pc),a1 */
  /* move.l  a1,d0 */
  /* move.l  d0,d7 */
  R->d7 = R->d0 = R->a1 - R->dstbuf;
  /* move.l  packed_data,a1 */
  /* addq.l  #4,a1 */
  /* bsr     longword_store */
  R->a1 = R->dstbuf + 4;
  longword_store(R);
  /* rts */
  return R->d0;
}

static void make_normal_bytes(all_regs_t * R)
{
  static const int t1a[7] = { 0, 1, 2, 5, 8, 15, 270 };

  static const int tib[7][2] = {
    { 0x01,0x01 }, { 0x01, 0x02 }, { 0x02, 0x04 }, { 0x02, 0x06 },
    { 0x03,0x09 }, { 0x08, 0x11 }, { 0x0f, 0x20 }
  };

  /* cmp.l      #$810d,d5 */
  /* bls.s      noerror */
  BLS ( 0x810d, R->d5, noerror );

  /* move.w     #-1,error */
  /* move.l     a0,src_ende */
  R->error = -1;
  R->srcend = R->a0;
noerror:

  /* lea.l      table1(pc),a3 */
  /* moveq      #6,d3 */
  R->d3 = 6;
kleiner:
  /* move.w     -(a3),d4 */

  R->d4 = t1a[R->d3];
  /* cmp.w      d4,d5 */
  /* dbge       d3,kleiner */
  DBGE ( R->d4, R->d5, R->d3, kleiner );
  /* sub.w      d4,d5 */
  R->d5 -= R->d4;
  R->d2 = tib[R->d3][0];
  /* moveq      #-1,d1 */
  /* lsl.l      d2,d1 */
  /* or.w       d5,d1 */
  R->d1 = (-1 << R->d2) | R->d5;
  /* moveq      #0,d5 */
  R->d5 = 0;
  /* move.b     (a3)+,d4 */
  /* ext.w      d4 */
  /* subq.w     #1,d4 */
  R->d4 = tib[R->d3][1] - 1;
  /* bra        put_bits */
  put_bits(R);
}

int unice68_packer(void * dst, int dstsz, const void * src, int srcsz)
{
  all_regs_t allregs, *R = &allregs;


  /***********************************************************************
   * Save parameters
   */
  R->srcbuf = (areg_t) src;
  R->srcend = R->srcbuf + srcsz;
  R->srclen = srcsz;
  R->dstbuf = (areg_t) dst;
  R->dstend = R->dstbuf + dstsz;
  R->dstlen = 0;
  R->dstmax = dstsz;

  R->a0 = R->srcbuf;
  R->d0 = R->srclen;
  R->a1 = R->dstbuf;

  R->a2 = R->a3 = R->a4 = R->a5 = R->a6 = 0;

  R->error     = 0;
  R->optimize  = 0x1580;
  R->maxlength = 0;
  R->maxgleich = 0;
  R->maxoffset = 0;

  /***********************************************************************
   * Store header
   */

  /* move.l  #'Ice!',d7 */
  /* bsr     longword_store */
  /* addq.l  #4,a1 */
  /* move.l  filesize(pc),d7 */
  /* bsr     longword_store */

  R->d7 = ICE_MAGIC;               /* Store magic id 'Ice!'         */
  longword_store(R);
  R->a1 += 4;                      /* Leave space for packed length */
  R->d7 = R->srclen;               /* Store unpacked length         */
  longword_store(R);

  /* moveq   #0,d5 */
  /* moveq   #0,d7 */
  /* moveq   #0,d4 */
  /* moveq   #0,d1 */
  R->d1 = R->d2 = R->d3 = R->d4 = R->d5 = R->d7 = 0;
  /* moveq   #7,d6 */
  R->d6 = 7;
  /* bsr     put_bits */

  /* Store one bit for picture flag */
  put_bits(R);


  /* Main loop */
  ice_crunch(R);

  if (R->error)
    R->d0 = -1;

  return R->d0;
}
