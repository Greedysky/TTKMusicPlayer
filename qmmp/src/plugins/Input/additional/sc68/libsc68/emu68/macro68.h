/**
 * @ingroup   lib_emu68
 * @file      emu68/macro68.h
 * @brief     68K instruction emulation macro definitions.
 * @author    Benjamin Gerard
 * @date      1999/13/03
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_MACRO68_H
#define EMU68_MACRO68_H

/**
 * @cond emu68macros
 */

/* Determine what instruction to inline.
 *
 * EMU68_INLINE_LVL
 */

#define EMU68_INLINE_MAX 1000           /**< Inlines all,                */
#define EMU68_INLINE_RAR 999            /**< Inlines almost all          */
#define EMU68_INLINE_MID 50             /**< Inlines balanced            */
#define EMU68_INLINE_NON 0              /**< Inlines not forced          */
#define EMU68_INLINE_YES 1              /**< Inline almost all */

#ifndef EMU68_INLINE_LVL
# if defined(EMU68_INLINE_ALL)
#  define EMU68_INLINE_LVL EMU68_INLINE_MAX /* All would be inlined      */
# elif defined(EMU68_MONOLITIC)
#  define EMU68_INLINE_LVL EMU68_INLINE_MAX /* Monolitic; compiler rules */
# elif defined(DEBUG)
#  define EMU68_INLINE_LVL EMU68_INLINE_NON /* Debug; do not force       */
# elif defined(LIBSC68_NDEBUG)
#  define EMU68_INLINE_LVL EMU68_INLINE_YES /* Release; almost all       */
# else
#  define EMU68_INLINE_LVL EMU68_INLINE_MID /* Standard; balanced        */
# endif
#endif


/*                                           Instruction family   (cnt)
                                             ------------------    ---  */
#define EMU68_INLINE_ARI 278              /* Arithmetic           (278) */
#define EMU68_INLINE_BCD EMU68_INLINE_RAR /* Binary coded decimal (  6) */
#define EMU68_INLINE_BIT 36               /* Bit Operation        ( 36) */
#define EMU68_INLINE_LOG 171              /* Logic                (171) */
#define EMU68_INLINE_SHT 56               /* Shifting             ( 56) */
#define EMU68_INLINE_TST 240              /* Test and move        (240) */
#define EMU68_INLINE_BSR EMU68_INLINE_YES /* Branch               (   ) */


#define ADDCYCLE(N) inl_addcycle68(emu68,N)
#define SETCYCLE(N) inl_setcycle68(emu68,N)
#define EXCEPTION(VECTOR,LEVEL)  inl_exception68(emu68,VECTOR,LEVEL)


/* ,---------------------------------------------------------------------.
 * |                              BRANCH                                 |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_BSR < EMU68_INLINE_LVL
# define JSR(PC)      inl_jsr68(emu68,PC)
# define JMP(PC)      inl_jmp68(emu68,PC)
#else
# define JSR(PC)          jsr68(emu68,PC)
# define JMP(PC)          jmp68(emu68,PC)
#endif

/* ,---------------------------------------------------------------------.
 * |                          BIT MANIPULATION                           |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_LOG < EMU68_INLINE_LVL
# define _BTST(V,BIT)  inl_btst68(emu68, V, BIT)
# define _BCHG(V,BIT)  inl_bchg68(emu68, V, BIT)
# define _BCLR(V,BIT)  inl_bclr68(emu68, V, BIT)
# define _BSET(V,BIT)  inl_bset68(emu68, V, BIT)
#else
# define _BTST(V,BIT)      btst68(emu68, V, BIT)
# define _BCHG(V,BIT)      bchg68(emu68, V, BIT)
# define _BCLR(V,BIT)      bclr68(emu68, V, BIT)
# define _BSET(V,BIT)      bset68(emu68, V, BIT)
#endif
#define BTSTB(R, V, BIT)     _BTST(V, BIT & 7)
#define BTSTL(R, V, BIT)     _BTST(V, BIT & 31)
#define BCHGB(R, V, BIT) R = _BCHG(V, BIT & 7)
#define BCHGL(R, V, BIT) R = _BCHG(V, BIT & 31)
#define BSETB(R, V, BIT) R = _BSET(V, BIT & 7)
#define BSETL(R, V, BIT) R = _BSET(V, BIT & 31)
#define BCLRB(R, V, BIT) R = _BCLR(V, BIT & 7)
#define BCLRL(R, V, BIT) R = _BCLR(V, BIT & 31)

/* ,---------------------------------------------------------------------.
 * |                            MOVE AND TEST                            |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_TST < EMU68_INLINE_LVL
# define _TST(A)  inl_tst68(emu68, A)
# define _TAS(A)  inl_tas68(emu68, A)
#else
# define _TST(A)      tst68(emu68, A)
# define _TAS(A)      tas68(emu68, A)
#endif
#define TSTB(R, A)      _TST(A)
#define TSTW(R, A)      _TST(A)
#define TSTL(R, A)      _TST(A)
#define TASB(R, A)  R = _TAS(A)

#define MOVEB(A)        _TST(A)
#define MOVEW(A)        _TST(A)
#define MOVEL(A)        _TST(A)

#define EXTW(A)         _TST(A)
#define EXTL(A)         _TST(A)


/* ,---------------------------------------------------------------------.
 * |                                LOGIC                                |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_LOG < EMU68_INLINE_LVL
# define _AND68(A,B) inl_and68(emu68, A, B)
# define _ORR68(A,B) inl_orr68(emu68, A, B)
# define _EOR68(A,B) inl_eor68(emu68, A, B)
# define _NOT68(A)   inl_not68(emu68, A)
#else
# define _AND68(A,B)     and68(emu68, A, B)
# define _ORR68(A,B)     orr68(emu68, A, B)
# define _EOR68(A,B)     eor68(emu68, A, B)
# define _NOT68(A)       not68(emu68, A)
#endif

#define ANDB(S, A, B)  S = _AND68(A, B)
#define ANDW(S, A, B)  S = _AND68(A, B)
#define ANDL(S, A, B)  S = _AND68(A, B)
#define ORRB(S, A, B)  S = _ORR68(A, B)
#define ORRW(S, A, B)  S = _ORR68(A, B)
#define ORRL(S, A, B)  S = _ORR68(A, B)
#define EORB(S, A, B)  S = _EOR68(A, B)
#define EORW(S, A, B)  S = _EOR68(A, B)
#define EORL(S, A, B)  S = _EOR68(A, B)
#define NOTB(S, A)     S = _NOT68(A|~NRM_BYTE_MSK)
#define NOTW(S, A)     S = _NOT68(A|~NRM_WORD_MSK)
#define NOTL(S, A)     S = _NOT68(A|~NRM_LONG_MSK)


#define BYTE_SR_X ( (int68_t)(REG68.sr&SR_X) << (BYTE_FIX-SR_X_BIT) )
#define WORD_SR_X ( (int68_t)(REG68.sr&SR_X) << (WORD_FIX-SR_X_BIT) )
#define LONG_SR_X ( (int68_t)((REG68.sr>>SR_X_BIT) & 1) << LONG_FIX )


/* ,---------------------------------------------------------------------.
 * |                              ARITHMETIC                             |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_ARI < EMU68_INLINE_LVL
# define _ADD68(A, B, X)  inl_add68(emu68, A, B, X)
# define _SUB68(A, B, X)  inl_sub68(emu68, A, B, X)
# define _CMP68(A, B)     inl_cmp68(emu68, A, B   )
# define _NEG68(B, X)     inl_neg68(emu68,    B, X)
# define _CLR68()         inl_clr68(emu68)
# define _MULS(A, B)      inl_muls68(emu68, A, B)
# define _MULU(A, B)      inl_mulu68(emu68, A, B)
# define _DIVS(A, B)      inl_divs68(emu68, A, B)
# define _DIVU(A, B)      inl_divu68(emu68, A, B)
#else
# define _ADD68(A, B, X)      add68(emu68, A, B, X)
# define _SUB68(A, B, X)      sub68(emu68, A, B, X)
# define _CMP68(A, B)         cmp68(emu68, A, B   )
# define _NEG68(B, X)         neg68(emu68,    B, X)
# define _CLR68()             clr68(emu68)
# define _MULS(A, B)          muls68(emu68, A, B)
# define _MULU(A, B)          mulu68(emu68, A, B)
# define _DIVS(A, B)          divs68(emu68, A, B)
# define _DIVU(A, B)          divu68(emu68, A, B)
#endif
#define  _ADDA(A, B)  (B) + (A)
#define  _SUBA(A, B)  (B) - (A)
#define  _CMPA(A, B)  _CMP68(A, B)

#define ADDB(S, A, B)   S = _ADD68(A, B, 0)
#define ADDW(S, A, B)   S = _ADD68(A, B, 0)
#define ADDL(S, A, B)   S = _ADD68(A, B, 0)
#define SUBB(S, A, B)   S = _SUB68(A, B, 0)
#define SUBW(S, A, B)   S = _SUB68(A, B, 0)
#define SUBL(S, A, B)   S = _SUB68(A, B, 0)
#define CMPB(A, B)          _CMP68(A, B   )
#define CMPW(A, B)          _CMP68(A, B   )
#define CMPL(A, B)          _CMP68(A, B   )
#define NEGB(S, B)      S = _NEG68(   B, 0)
#define NEGW(S, B)      S = _NEG68(   B, 0)
#define NEGL(S, B)      S = _NEG68(   B, 0)
#define CLRB(S, B)      S = _CLR68()
#define CLRW(S, B)      S = _CLR68()
#define CLRL(S, B)      S = _CLR68()

#define ADDXB(S, A, B)  S = _ADD68(A, B, BYTE_SR_X)
#define ADDXW(S, A, B)  S = _ADD68(A, B, WORD_SR_X)
#define ADDXL(S, A, B)  S = _ADD68(A, B, LONG_SR_X)
#define SUBXB(S, A, B)  S = _SUB68(A, B, BYTE_SR_X)
#define SUBXW(S, A, B)  S = _SUB68(A, B, WORD_SR_X)
#define SUBXL(S, A, B)  S = _SUB68(A, B, LONG_SR_X)
#define NEGXB(S, B)     S = _NEG68(   B, BYTE_SR_X)
#define NEGXW(S, B)     S = _NEG68(   B, WORD_SR_X)
#define NEGXL(S, B)     S = _NEG68(   B, LONG_SR_X)

#define ADDAW(S, A, B)  S = _ADDA(A, B)
#define ADDAL(S, A, B)  S = _ADDA(A, B)
#define SUBAW(S, A, B)  S = _SUBA(A, B)
#define SUBAL(S, A, B)  S = _SUBA(A, B)
#define CMPAW(A, B)         _CMPA(A, B)
#define CMPAL(A, B)         _CMPA(A, B)

#define MULSW(S, A, B)  S = _MULS(A, B)
#define MULUW(S, A, B)  S = _MULU(A, B)
#define DIVSW(S, A, B)  S = _DIVS(A, B)
#define DIVUW(S, A, B)  S = _DIVU(A, B)


/* ,---------------------------------------------------------------------.
 * |                               SHIFTING                              |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_SHT < EMU68_INLINE_LVL
# define _LSL(D,CNT,SZ)   inl_lsl68(emu68,D,CNT,SZ)
# define _LSR(D,CNT,SZ)   inl_lsr68(emu68,D,CNT,SZ)
# define _ASL(D,CNT,SZ)   inl_asl68(emu68,D,CNT,SZ)
# define _ASR(D,CNT,SZ)   inl_asr68(emu68,D,CNT,SZ)
# define _ROL(D,CNT,SZ)   inl_rol68(emu68,D,CNT,SZ)
# define _ROR(D,CNT,SZ)   inl_ror68(emu68,D,CNT,SZ)
# define _ROXL(D,CNT,SZ)  inl_roxl68(emu68,D,CNT,SZ)
# define _ROXR(D,CNT,SZ)  inl_roxr68(emu68,D,CNT,SZ)
#else
# define _LSL(D,CNT,SZ)       lsl68(emu68,D,CNT,SZ)
# define _LSR(D,CNT,SZ)       lsr68(emu68,D,CNT,SZ)
# define _ASL(D,CNT,SZ)       asl68(emu68,D,CNT,SZ)
# define _ASR(D,CNT,SZ)       asr68(emu68,D,CNT,SZ)
# define _ROL(D,CNT,SZ)       rol68(emu68,D,CNT,SZ)
# define _ROR(D,CNT,SZ)       ror68(emu68,D,CNT,SZ)
# define _ROXL(D,CNT,SZ)      roxl68(emu68,D,CNT,SZ)
# define _ROXR(D,CNT,SZ)      roxr68(emu68,D,CNT,SZ)
#endif


#define LSRB(R, A, B)   R = _LSR(A, B,  8-1)
#define LSRW(R, A, B)   R = _LSR(A, B, 16-1)
#define LSRL(R, A, B)   R = _LSR(A, B, 32-1)
#define LSLB(R, A, B)   R = _LSL(A, B,  8-1)
#define LSLW(R, A, B)   R = _LSL(A, B, 16-1)
#define LSLL(R, A, B)   R = _LSL(A, B, 32-1)

#define ASRB(R, A, B)   R = _ASR(A, B,  8-1)
#define ASRW(R, A, B)   R = _ASR(A, B, 16-1)
#define ASRL(R, A, B)   R = _ASR(A, B, 32-1)
#define ASLB(R, A, B)   R = _ASL(A, B,  8-1)
#define ASLW(R, A, B)   R = _ASL(A, B, 16-1)
#define ASLL(R, A, B)   R = _ASL(A, B, 32-1)

#define RORB(R, A, B)   R = _ROR(A, B,  8-1)
#define RORW(R, A, B)   R = _ROR(A, B, 16-1)
#define RORL(R, A, B)   R = _ROR(A, B, 32-1)
#define ROLB(R, A, B)   R = _ROL(A, B,  8-1)
#define ROLW(R, A, B)   R = _ROL(A, B, 16-1)
#define ROLL(R, A, B)   R = _ROL(A, B, 32-1)

#define ROXRB(R, A, B)  R = _ROXR(A ,B,  8-1)
#define ROXRW(R, A, B)  R = _ROXR(A ,B, 16-1)
#define ROXRL(R, A, B)  R = _ROXR(A ,B, 32-1)
#define ROXLB(R, A, B)  R = _ROXL(A ,B,  8-1)
#define ROXLW(R, A, B)  R = _ROXL(A ,B, 16-1)
#define ROXLL(R, A, B)  R = _ROXL(A ,B, 32-1)



/* ,---------------------------------------------------------------------.
 * |                         BINARY CODED DECIMAL                        |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_BCD < EMU68_INLINE_LVL
# define _ABCD(A,B)  inl_abcd68(emu68, A, B)
# define _SBCD(A,B)  inl_sbcd68(emu68, A, B)
# define _NBCD(A)    inl_nbcd68(emu68, A)
#else
# define _ABCD(A,B)      abcd68(emu68, A, B)
# define _SBCD(A,B)      sbcd68(emu68, A, B)
# define _NBCD(A)        nbcd68(emu68, A)
#endif
#define ABCDB(S, A, B)  S = _ABCD(A, B)
#define SBCDB(S, A, B)  S = _SBCD(A, B)
#define NBCDB(S, A)     S = _NBCD(A)


/* ,---------------------------------------------------------------------.
 * |                  INSTRUCTIONS ALMOST NEVER INLINED                  |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_RAR < EMU68_INLINE_LVL
# define CHKW(A,B)        inl_chk68(emu68, A, B)
#else
# define CHKW(A,B)            chk68(emu68, A, B)
#endif

/* ,---------------------------------------------------------------------.
 * |                 INSTRUCTIONS ALMOST ALWAYS INLINED                  |
 * `---------------------------------------------------------------------'
 */

#if EMU68_INLINE_ONE < EMU68_INLINE_LVL
# define SWAP(R)          inl_swap68(emu68,R)
# define RTS              inl_rts68(emu68)
# define RTE              inl_rte68(emu68)
# define RTR              inl_rtr68(emu68)
# define ILLEGAL          inl_illegal68(emu68)
# define TRAP(N)          inl_trap68(emu68,N)
# define TRAPV            inl_trapv68(emu68)
# define NOP              inl_nop68(emu68)
# define RESET            inl_reset68(emu68)
# define STOP             inl_stop68(emu68)
# define LINK(R)          inl_link68(emu68,R)
# define UNLK(R)          inl_unlk68(emu68,R)
# define LINEA            inl_linea68(emu68)
# define LINEF            inl_linef68(emu68)
# define EXG(RX,RY)       inl_exg68(emu68,RX,RY)
#else
# define SWAP(R)              swap68(emu68,R)
# define RTS                  rts68(emu68)
# define RTE                  rte68(emu68)
# define RTR                  rtr68(emu68)
# define ILLEGAL              illegal68(emu68)
# define TRAP(N)              trap68(emu68,N)
# define TRAPV                trapv68(emu68)
# define NOP                  nop68(emu68)
# define RESET                reset68(emu68)
# define STOP                 stop68(emu68)
# define LINK(R)              link68(emu68,R)
# define UNLK(R)              unlk68(emu68,R)
# define LINEA                linea68(emu68)
# define LINEF                linef68(emu68)
# define EXG(RX,RY)           exg68(emu68,RX,RY)
#endif

/* ,---------------------------------------------------------------------.
 * |                        CODE CONDITIONS TABLE                        |
 * `---------------------------------------------------------------------'
 */

#define SCC(CC)      scc68[CC](emu68)
#define BCC(PC,CC)   bcc68[CC](emu68,PC)
#define DBCC(DN,CC)  dbcc68[CC](emu68,DN)

/**
 * @endcond
 */

#endif
