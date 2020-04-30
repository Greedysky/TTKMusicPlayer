/*****************************************************************************
 *
 *	 z80dasm.c
 *	 Portable Z80 disassembler
 *
 *	 Copyright (C) 1998 Juergen Buchmueller, all rights reserved.
 *
 *	 - This source code is released as freeware for non-commercial purposes.
 *	 - You are free to use and redistribute this code in modified or
 *	   unmodified form, provided you list me in the credits.
 *	 - If you modify this source code, you must add a notice to each modified
 *	   source file that it has been changed.  If you're a nice person, you
 *	   will clearly mark each change too.  :)
 *	 - If you wish to use this for commercial purposes, please contact me at
 *	   pullmoll@t-online.de
 *	 - The author of this copywritten work reserves the right to change the
 *     terms of its usage and license at any time, including retroactively
 *   - This entire notice must remain in the source code.
 *
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef	MAME_DEBUG
#include "driver.h"
#include "mamedbg.h"
#include "z80.h"

enum e_mnemonics {
	zADC  ,zADD  ,zAND	,zBIT  ,zCALL ,zCCF  ,zCP	,zCPD  ,
	zCPDR ,zCPI  ,zCPIR ,zCPL  ,zDAA  ,zDB	 ,zDEC	,zDI   ,
	zDJNZ ,zEI	 ,zEX	,zEXX  ,zHLT  ,zIM	 ,zIN	,zINC  ,
	zIND  ,zINDR ,zINI	,zINIR ,zJP   ,zJR	 ,zLD	,zLDD  ,
	zLDDR ,zLDI  ,zLDIR ,zNEG  ,zNOP  ,zOR	 ,zOTDR ,zOTIR ,
	zOUT  ,zOUTD ,zOUTI ,zPOP  ,zPUSH ,zRES  ,zRET	,zRETI ,
	zRETN ,zRL	 ,zRLA	,zRLC  ,zRLCA ,zRLD  ,zRR	,zRRA  ,
	zRRC  ,zRRCA ,zRRD	,zRST  ,zSBC  ,zSCF  ,zSET	,zSLA  ,
	zSLL  ,zSRA  ,zSRL	,zSUB  ,zXOR
};

static const char *s_mnemonic[] = {
	"adc", "add", "and", "bit", "call","ccf", "cp",  "cpd",
	"cpdr","cpi", "cpir","cpl", "daa", "db",  "dec", "di",
	"djnz","ei",  "ex",  "exx", "halt","im",  "in",  "inc",
	"ind", "indr","ini", "inir","jp",  "jr",  "ld",  "ldd",
	"lddr","ldi", "ldir","neg", "nop", "or",  "otdr","otir",
	"out", "outd","outi","pop", "push","res", "ret", "reti",
	"retn","rl",  "rla", "rlc", "rlca","rld", "rr",  "rra",
	"rrc", "rrca","rrd", "rst", "sbc", "scf", "set", "sla",
	"sll", "sra", "srl", "sub", "xor "
};

typedef struct {
	UINT8	access;
	UINT8	mnemonic;
	const char *arguments;
}	z80dasm;

#define _0	EA_NONE
#define _JP EA_ABS_PC
#define _JR EA_REL_PC
#define _RM EA_MEM_RD
#define _WM EA_MEM_WR
#define _RW EA_MEM_RDWR
#define _RP EA_PORT_RD
#define _WP EA_PORT_WR

static z80dasm mnemonic_xx_cb[256]= {
	{_RW,zRLC,"b=Y"},   {_RW,zRLC,"c=Y"},   {_RW,zRLC,"d=Y"},   {_RW,zRLC,"e=Y"},
	{_RW,zRLC,"h=Y"},   {_RW,zRLC,"l=Y"},   {_RW,zRLC,"Y"},     {_RW,zRLC,"a=Y"},
	{_RW,zRRC,"b=Y"},   {_RW,zRRC,"c=Y"},   {_RW,zRRC,"d=Y"},   {_RW,zRRC,"e=Y"},
	{_RW,zRRC,"h=Y"},   {_RW,zRRC,"l=Y"},   {_RW,zRRC,"Y"},     {_RW,zRRC,"a=Y"},
	{_RW,zRL,"b=Y"},    {_RW,zRL,"c=Y"},    {_RW,zRL,"d=Y"},    {_RW,zRL,"e=Y"},
	{_RW,zRL,"h=Y"},    {_RW,zRL,"l=Y"},    {_RW,zRL,"Y"},      {_RW,zRL,"a=Y"},
	{_RW,zRR,"b=Y"},    {_RW,zRR,"c=Y"},    {_RW,zRR,"d=Y"},    {_RW,zRR,"e=Y"},
	{_RW,zRR,"h=Y"},    {_RW,zRR,"l=Y"},    {_RW,zRR,"Y"},      {_RW,zRR,"a=Y"},
	{_RW,zSLA,"b=Y"},   {_RW,zSLA,"c=Y"},   {_RW,zSLA,"d=Y"},   {_RW,zSLA,"e=Y"},
	{_RW,zSLA,"h=Y"},   {_RW,zSLA,"l=Y"},   {_RW,zSLA,"Y"},     {_RW,zSLA,"a=Y"},
	{_RW,zSRA,"b=Y"},   {_RW,zSRA,"c=Y"},   {_RW,zSRA,"d=Y"},   {_RW,zSRA,"e=Y"},
	{_RW,zSRA,"h=Y"},   {_RW,zSRA,"l=Y"},   {_RW,zSRA,"Y"},     {_RW,zSRA,"a=Y"},
	{_RW,zSLL,"b=Y"},   {_RW,zSLL,"c=Y"},   {_RW,zSLL,"d=Y"},   {_RW,zSLL,"e=Y"},
	{_RW,zSLL,"h=Y"},   {_RW,zSLL,"l=Y"},   {_RW,zSLL,"Y"},     {_RW,zSLL,"a=Y"},
	{_RW,zSRL,"b=Y"},   {_RW,zSRL,"c=Y"},   {_RW,zSRL,"d=Y"},   {_RW,zSRL,"e=Y"},
	{_RW,zSRL,"h=Y"},   {_RW,zSRL,"l=Y"},   {_RW,zSRL,"Y"},     {_RW,zSRL,"a=Y"},
	{_RM,zBIT,"b=0,Y"}, {_RM,zBIT,"c=0,Y"}, {_RM,zBIT,"d=0,Y"}, {_RM,zBIT,"e=0,Y"},
	{_RM,zBIT,"h=0,Y"}, {_RM,zBIT,"l=0,Y"}, {_RM,zBIT,"0,Y"},   {_RM,zBIT,"a=0,Y"},
	{_RM,zBIT,"b=1,Y"}, {_RM,zBIT,"c=1,Y"}, {_RM,zBIT,"d=1,Y"}, {_RM,zBIT,"e=1,Y"},
	{_RM,zBIT,"h=1,Y"}, {_RM,zBIT,"l=1,Y"}, {_RM,zBIT,"1,Y"},   {_RM,zBIT,"a=1,Y"},
	{_RM,zBIT,"b=2,Y"}, {_RM,zBIT,"c=2,Y"}, {_RM,zBIT,"d=2,Y"}, {_RM,zBIT,"e=2,Y"},
	{_RM,zBIT,"h=2,Y"}, {_RM,zBIT,"l=2,Y"}, {_RM,zBIT,"2,Y"},   {_RM,zBIT,"a=2,Y"},
	{_RM,zBIT,"b=3,Y"}, {_RM,zBIT,"c=3,Y"}, {_RM,zBIT,"d=3,Y"}, {_RM,zBIT,"e=3,Y"},
	{_RM,zBIT,"h=3,Y"}, {_RM,zBIT,"l=3,Y"}, {_RM,zBIT,"3,Y"},   {_RM,zBIT,"a=3,Y"},
	{_RM,zBIT,"b=4,Y"}, {_RM,zBIT,"c=4,Y"}, {_RM,zBIT,"d=4,Y"}, {_RM,zBIT,"e=4,Y"},
	{_RM,zBIT,"h=4,Y"}, {_RM,zBIT,"l=4,Y"}, {_RM,zBIT,"4,Y"},   {_RM,zBIT,"a=4,Y"},
	{_RM,zBIT,"b=5,Y"}, {_RM,zBIT,"c=5,Y"}, {_RM,zBIT,"d=5,Y"}, {_RM,zBIT,"e=5,Y"},
	{_RM,zBIT,"h=5,Y"}, {_RM,zBIT,"l=5,Y"}, {_RM,zBIT,"5,Y"},   {_RM,zBIT,"a=5,Y"},
	{_RM,zBIT,"b=6,Y"}, {_RM,zBIT,"c=6,Y"}, {_RM,zBIT,"d=6,Y"}, {_RM,zBIT,"e=6,Y"},
	{_RM,zBIT,"h=6,Y"}, {_RM,zBIT,"l=6,Y"}, {_RM,zBIT,"6,Y"},   {_RM,zBIT,"a=6,Y"},
	{_RM,zBIT,"b=7,Y"}, {_RM,zBIT,"c=7,Y"}, {_RM,zBIT,"d=7,Y"}, {_RM,zBIT,"e=7,Y"},
	{_RM,zBIT,"h=7,Y"}, {_RM,zBIT,"l=7,Y"}, {_RM,zBIT,"7,Y"},   {_RM,zBIT,"a=7,Y"},
	{_WM,zRES,"b=0,Y"}, {_WM,zRES,"c=0,Y"}, {_WM,zRES,"d=0,Y"}, {_WM,zRES,"e=0,Y"},
	{_WM,zRES,"h=0,Y"}, {_WM,zRES,"l=0,Y"}, {_WM,zRES,"0,Y"},   {_WM,zRES,"a=0,Y"},
	{_WM,zRES,"b=1,Y"}, {_WM,zRES,"c=1,Y"}, {_WM,zRES,"d=1,Y"}, {_WM,zRES,"e=1,Y"},
	{_WM,zRES,"h=1,Y"}, {_WM,zRES,"l=1,Y"}, {_WM,zRES,"1,Y"},   {_WM,zRES,"a=1,Y"},
	{_WM,zRES,"b=2,Y"}, {_WM,zRES,"c=2,Y"}, {_WM,zRES,"d=2,Y"}, {_WM,zRES,"e=2,Y"},
	{_WM,zRES,"h=2,Y"}, {_WM,zRES,"l=2,Y"}, {_WM,zRES,"2,Y"},   {_WM,zRES,"a=2,Y"},
	{_WM,zRES,"b=3,Y"}, {_WM,zRES,"c=3,Y"}, {_WM,zRES,"d=3,Y"}, {_WM,zRES,"e=3,Y"},
	{_WM,zRES,"h=3,Y"}, {_WM,zRES,"l=3,Y"}, {_WM,zRES,"3,Y"},   {_WM,zRES,"a=3,Y"},
	{_WM,zRES,"b=4,Y"}, {_WM,zRES,"c=4,Y"}, {_WM,zRES,"d=4,Y"}, {_WM,zRES,"e=4,Y"},
	{_WM,zRES,"h=4,Y"}, {_WM,zRES,"l=4,Y"}, {_WM,zRES,"4,Y"},   {_WM,zRES,"a=4,Y"},
	{_WM,zRES,"b=5,Y"}, {_WM,zRES,"c=5,Y"}, {_WM,zRES,"d=5,Y"}, {_WM,zRES,"e=5,Y"},
	{_WM,zRES,"h=5,Y"}, {_WM,zRES,"l=5,Y"}, {_WM,zRES,"5,Y"},   {_WM,zRES,"a=5,Y"},
	{_WM,zRES,"b=6,Y"}, {_WM,zRES,"c=6,Y"}, {_WM,zRES,"d=6,Y"}, {_WM,zRES,"e=6,Y"},
	{_WM,zRES,"h=6,Y"}, {_WM,zRES,"l=6,Y"}, {_WM,zRES,"6,Y"},   {_WM,zRES,"a=6,Y"},
	{_WM,zRES,"b=7,Y"}, {_WM,zRES,"c=7,Y"}, {_WM,zRES,"d=7,Y"}, {_WM,zRES,"e=7,Y"},
	{_WM,zRES,"h=7,Y"}, {_WM,zRES,"l=7,Y"}, {_WM,zRES,"7,Y"},   {_WM,zRES,"a=7,Y"},
	{_WM,zSET,"b=0,Y"}, {_WM,zSET,"c=0,Y"}, {_WM,zSET,"d=0,Y"}, {_WM,zSET,"e=0,Y"},
	{_WM,zSET,"h=0,Y"}, {_WM,zSET,"l=0,Y"}, {_WM,zSET,"0,Y"},   {_WM,zSET,"a=0,Y"},
	{_WM,zSET,"b=1,Y"}, {_WM,zSET,"c=1,Y"}, {_WM,zSET,"d=1,Y"}, {_WM,zSET,"e=1,Y"},
	{_WM,zSET,"h=1,Y"}, {_WM,zSET,"l=1,Y"}, {_WM,zSET,"1,Y"},   {_WM,zSET,"a=1,Y"},
	{_WM,zSET,"b=2,Y"}, {_WM,zSET,"c=2,Y"}, {_WM,zSET,"d=2,Y"}, {_WM,zSET,"e=2,Y"},
	{_WM,zSET,"h=2,Y"}, {_WM,zSET,"l=2,Y"}, {_WM,zSET,"2,Y"},   {_WM,zSET,"a=2,Y"},
	{_WM,zSET,"b=3,Y"}, {_WM,zSET,"c=3,Y"}, {_WM,zSET,"d=3,Y"}, {_WM,zSET,"e=3,Y"},
	{_WM,zSET,"h=3,Y"}, {_WM,zSET,"l=3,Y"}, {_WM,zSET,"3,Y"},   {_WM,zSET,"a=3,Y"},
	{_WM,zSET,"b=4,Y"}, {_WM,zSET,"c=4,Y"}, {_WM,zSET,"d=4,Y"}, {_WM,zSET,"e=4,Y"},
	{_WM,zSET,"h=4,Y"}, {_WM,zSET,"l=4,Y"}, {_WM,zSET,"4,Y"},   {_WM,zSET,"a=4,Y"},
	{_WM,zSET,"b=5,Y"}, {_WM,zSET,"c=5,Y"}, {_WM,zSET,"d=5,Y"}, {_WM,zSET,"e=5,Y"},
	{_WM,zSET,"h=5,Y"}, {_WM,zSET,"l=5,Y"}, {_WM,zSET,"5,Y"},   {_WM,zSET,"a=5,Y"},
	{_WM,zSET,"b=6,Y"}, {_WM,zSET,"c=6,Y"}, {_WM,zSET,"d=6,Y"}, {_WM,zSET,"e=6,Y"},
	{_WM,zSET,"h=6,Y"}, {_WM,zSET,"l=6,Y"}, {_WM,zSET,"6,Y"},   {_WM,zSET,"a=6,Y"},
	{_WM,zSET,"b=7,Y"}, {_WM,zSET,"c=7,Y"}, {_WM,zSET,"d=7,Y"}, {_WM,zSET,"e=7,Y"},
	{_WM,zSET,"h=7,Y"}, {_WM,zSET,"l=7,Y"}, {_WM,zSET,"7,Y"},   {_WM,zSET,"a=7,Y"}
};

static z80dasm mnemonic_cb[256] = {
	{_0, zRLC,"b"},     {_0, zRLC,"c"},     {_0, zRLC,"d"},     {_0, zRLC,"e"},
	{_0, zRLC,"h"},     {_0, zRLC,"l"},     {_RW,zRLC,"(hl)"},  {_0, zRLC,"a"},
	{_0, zRRC,"b"},     {_0, zRRC,"c"},     {_0, zRRC,"d"},     {_0, zRRC,"e"},
	{_0, zRRC,"h"},     {_0, zRRC,"l"},     {_RW,zRRC,"(hl)"},  {_0, zRRC,"a"},
	{_0, zRL,"b"},      {_0, zRL,"c"},      {_0, zRL,"d"},      {_0, zRL,"e"},
	{_0, zRL,"h"},      {_0, zRL,"l"},      {_RW,zRL,"(hl)"},   {_0, zRL,"a"},
	{_0, zRR,"b"},      {_0, zRR,"c"},      {_0, zRR,"d"},      {_0, zRR,"e"},
	{_0, zRR,"h"},      {_0, zRR,"l"},      {_RW,zRR,"(hl)"},   {_0, zRR,"a"},
	{_0, zSLA,"b"},     {_0, zSLA,"c"},     {_0, zSLA,"d"},     {_0, zSLA,"e"},
	{_0, zSLA,"h"},     {_0, zSLA,"l"},     {_RW,zSLA,"(hl)"},  {_0, zSLA,"a"},
	{_0, zSRA,"b"},     {_0, zSRA,"c"},     {_0, zSRA,"d"},     {_0, zSRA,"e"},
	{_0, zSRA,"h"},     {_0, zSRA,"l"},     {_RW,zSRA,"(hl)"},  {_0, zSRA,"a"},
	{_0, zSLL,"b"},     {_0, zSLL,"c"},     {_0, zSLL,"d"},     {_0, zSLL,"e"},
	{_0, zSLL,"h"},     {_0, zSLL,"l"},     {_RW,zSLL,"(hl)"},  {_0, zSLL,"a"},
	{_0, zSRL,"b"},     {_0, zSRL,"c"},     {_0, zSRL,"d"},     {_0, zSRL,"e"},
	{_0, zSRL,"h"},     {_0, zSRL,"l"},     {_RW,zSRL,"(hl)"},  {_0, zSRL,"a"},
	{_0, zBIT,"0,b"},   {_0, zBIT,"0,c"},   {_0, zBIT,"0,d"},   {_0, zBIT,"0,e"},
	{_0, zBIT,"0,h"},   {_0, zBIT,"0,l"},   {_RM,zBIT,"0,(hl)"},{_0, zBIT,"0,a"},
	{_0, zBIT,"1,b"},   {_0, zBIT,"1,c"},   {_0, zBIT,"1,d"},   {_0, zBIT,"1,e"},
	{_0, zBIT,"1,h"},   {_0, zBIT,"1,l"},   {_RM,zBIT,"1,(hl)"},{_0, zBIT,"1,a"},
	{_0, zBIT,"2,b"},   {_0, zBIT,"2,c"},   {_0, zBIT,"2,d"},   {_0, zBIT,"2,e"},
	{_0, zBIT,"2,h"},   {_0, zBIT,"2,l"},   {_RM,zBIT,"2,(hl)"},{_0, zBIT,"2,a"},
	{_0, zBIT,"3,b"},   {_0, zBIT,"3,c"},   {_0, zBIT,"3,d"},   {_0, zBIT,"3,e"},
	{_0, zBIT,"3,h"},   {_0, zBIT,"3,l"},   {_RM,zBIT,"3,(hl)"},{_0, zBIT,"3,a"},
	{_0, zBIT,"4,b"},   {_0, zBIT,"4,c"},   {_0, zBIT,"4,d"},   {_0, zBIT,"4,e"},
	{_0, zBIT,"4,h"},   {_0, zBIT,"4,l"},   {_RM,zBIT,"4,(hl)"},{_0, zBIT,"4,a"},
	{_0, zBIT,"5,b"},   {_0, zBIT,"5,c"},   {_0, zBIT,"5,d"},   {_0, zBIT,"5,e"},
	{_0, zBIT,"5,h"},   {_0, zBIT,"5,l"},   {_RM,zBIT,"5,(hl)"},{_0, zBIT,"5,a"},
	{_0, zBIT,"6,b"},   {_0, zBIT,"6,c"},   {_0, zBIT,"6,d"},   {_0, zBIT,"6,e"},
	{_0, zBIT,"6,h"},   {_0, zBIT,"6,l"},   {_RM,zBIT,"6,(hl)"},{_0, zBIT,"6,a"},
	{_0, zBIT,"7,b"},   {_0, zBIT,"7,c"},   {_0, zBIT,"7,d"},   {_0, zBIT,"7,e"},
	{_0, zBIT,"7,h"},   {_0, zBIT,"7,l"},   {_RM,zBIT,"7,(hl)"},{_0, zBIT,"7,a"},
	{_0, zRES,"0,b"},   {_0, zRES,"0,c"},   {_0, zRES,"0,d"},   {_0, zRES,"0,e"},
	{_0, zRES,"0,h"},   {_0, zRES,"0,l"},   {_WM,zRES,"0,(hl)"},{_0, zRES,"0,a"},
	{_0, zRES,"1,b"},   {_0, zRES,"1,c"},   {_0, zRES,"1,d"},   {_0, zRES,"1,e"},
	{_0, zRES,"1,h"},   {_0, zRES,"1,l"},   {_WM,zRES,"1,(hl)"},{_0, zRES,"1,a"},
	{_0, zRES,"2,b"},   {_0, zRES,"2,c"},   {_0, zRES,"2,d"},   {_0, zRES,"2,e"},
	{_0, zRES,"2,h"},   {_0, zRES,"2,l"},   {_WM,zRES,"2,(hl)"},{_0, zRES,"2,a"},
	{_0, zRES,"3,b"},   {_0, zRES,"3,c"},   {_0, zRES,"3,d"},   {_0, zRES,"3,e"},
	{_0, zRES,"3,h"},   {_0, zRES,"3,l"},   {_WM,zRES,"3,(hl)"},{_0, zRES,"3,a"},
	{_0, zRES,"4,b"},   {_0, zRES,"4,c"},   {_0, zRES,"4,d"},   {_0, zRES,"4,e"},
	{_0, zRES,"4,h"},   {_0, zRES,"4,l"},   {_WM,zRES,"4,(hl)"},{_0, zRES,"4,a"},
	{_0, zRES,"5,b"},   {_0, zRES,"5,c"},   {_0, zRES,"5,d"},   {_0, zRES,"5,e"},
	{_0, zRES,"5,h"},   {_0, zRES,"5,l"},   {_WM,zRES,"5,(hl)"},{_0, zRES,"5,a"},
	{_0, zRES,"6,b"},   {_0, zRES,"6,c"},   {_0, zRES,"6,d"},   {_0, zRES,"6,e"},
	{_0, zRES,"6,h"},   {_0, zRES,"6,l"},   {_WM,zRES,"6,(hl)"},{_0, zRES,"6,a"},
	{_0, zRES,"7,b"},   {_0, zRES,"7,c"},   {_0, zRES,"7,d"},   {_0, zRES,"7,e"},
	{_0, zRES,"7,h"},   {_0, zRES,"7,l"},   {_WM,zRES,"7,(hl)"},{_0, zRES,"7,a"},
	{_0, zSET,"0,b"},   {_0, zSET,"0,c"},   {_0, zSET,"0,d"},   {_0, zSET,"0,e"},
	{_0, zSET,"0,h"},   {_0, zSET,"0,l"},   {_WM,zSET,"0,(hl)"},{_0, zSET,"0,a"},
	{_0, zSET,"1,b"},   {_0, zSET,"1,c"},   {_0, zSET,"1,d"},   {_0, zSET,"1,e"},
	{_0, zSET,"1,h"},   {_0, zSET,"1,l"},   {_WM,zSET,"1,(hl)"},{_0, zSET,"1,a"},
	{_0, zSET,"2,b"},   {_0, zSET,"2,c"},   {_0, zSET,"2,d"},   {_0, zSET,"2,e"},
	{_0, zSET,"2,h"},   {_0, zSET,"2,l"},   {_WM,zSET,"2,(hl)"},{_0, zSET,"2,a"},
	{_0, zSET,"3,b"},   {_0, zSET,"3,c"},   {_0, zSET,"3,d"},   {_0, zSET,"3,e"},
	{_0, zSET,"3,h"},   {_0, zSET,"3,l"},   {_WM,zSET,"3,(hl)"},{_0, zSET,"3,a"},
	{_0, zSET,"4,b"},   {_0, zSET,"4,c"},   {_0, zSET,"4,d"},   {_0, zSET,"4,e"},
	{_0, zSET,"4,h"},   {_0, zSET,"4,l"},   {_WM,zSET,"4,(hl)"},{_0, zSET,"4,a"},
	{_0, zSET,"5,b"},   {_0, zSET,"5,c"},   {_0, zSET,"5,d"},   {_0, zSET,"5,e"},
	{_0, zSET,"5,h"},   {_0, zSET,"5,l"},   {_WM,zSET,"5,(hl)"},{_0, zSET,"5,a"},
	{_0, zSET,"6,b"},   {_0, zSET,"6,c"},   {_0, zSET,"6,d"},   {_0, zSET,"6,e"},
	{_0, zSET,"6,h"},   {_0, zSET,"6,l"},   {_WM,zSET,"6,(hl)"},{_0, zSET,"6,a"},
	{_0, zSET,"7,b"},   {_0, zSET,"7,c"},   {_0, zSET,"7,d"},   {_0, zSET,"7,e"},
	{_0, zSET,"7,h"},   {_0, zSET,"7,l"},   {_WM,zSET,"7,(hl)"},{_0, zSET,"7,a"}
};

static z80dasm mnemonic_ed[256]= {
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_RP,zIN,"b,(c)"},  {_WP,zOUT,"(c),b"}, {_0, zSBC,"hl,bc"}, {_WM,zLD,"(W),bc"},
	{_0, zNEG,0},		{_0, zRETN,0},		{_0, zIM,"0"},      {_0, zLD,"i,a"},
	{_RP,zIN,"c,(c)"},  {_WP,zOUT,"(c),c"}, {_0, zADC,"hl,bc"}, {_RM,zLD,"bc,(W)"},
	{_0, zNEG,"*"},     {_0, zRETI,0},      {_0, zIM,"0"},      {_0, zLD,"r,a"},
	{_RP,zIN,"d,(c)"},  {_WP,zOUT,"(c),d"}, {_0, zSBC,"hl,de"}, {_WM,zLD,"(W),de"},
	{_0, zNEG,"*"},     {_0, zRETN,0},      {_0, zIM,"1"},      {_0, zLD,"a,i"},
	{_RP,zIN,"e,(c)"},  {_WP,zOUT,"(c),e"}, {_0, zADC,"hl,de"}, {_RM,zLD,"de,(W)"},
	{_0, zNEG,"*"},     {_0, zRETI,0},      {_0, zIM,"2"},      {_0, zLD,"a,r"},
	{_RP,zIN,"h,(c)"},  {_WP,zOUT,"(c),h"}, {_0, zSBC,"hl,hl"}, {_WM,zLD,"(W),hl"},
	{_0, zNEG,"*"},     {_0, zRETN,0},      {_0, zIM,"0"},      {_RW,zRRD,"(hl)"},
	{_RP,zIN,"l,(c)"},  {_WP,zOUT,"(c),l"}, {_0, zADC,"hl,hl"}, {_RM,zLD,"hl,(W)"},
	{_0, zNEG,"*"},     {_0, zRETI,0},      {_0, zIM,"0"},      {_RW,zRLD,"(hl)"},
	{_RP,zIN,"0,(c)"},  {_WP,zOUT,"(c),0"}, {_0, zSBC,"hl,sp"}, {_WM,zLD,"(W),sp"},
	{_0, zNEG,"*"},     {_0, zRETN,0},      {_0, zIM,"1"},      {_0, zDB,"?"},
	{_RP,zIN,"a,(c)"},  {_WP,zOUT,"(c),a"}, {_0, zADC,"hl,sp"}, {_RM,zLD,"sp,(W)"},
	{_0, zNEG,"*"},     {_0, zRETI,0},      {_0, zIM,"2"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_RW,zLDI,0},		{_RM,zCPI,0},		{_RP,zINI,0},		{_WP,zOUTI,0},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_RW,zLDD,0},		{_RM,zCPD,0},		{_RP,zIND,0},		{_WP,zOUTD,0},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_RW,zLDIR,0},		{_RM,zCPIR,0},		{_RP,zINIR,0},		{_WP,zOTIR,0},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_RW,zLDDR,0},		{_RM,zCPDR,0},		{_RP,zINDR,0},		{_WP,zOTDR,0},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"}
};

static z80dasm mnemonic_xx[256]= {
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zADD,"I,bc"},  {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zADD,"I,de"},  {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zLD,"I,N"},    {_WM,zLD,"(W),I"},  {_0, zINC,"I"},
	{_0, zINC,"Ih"},    {_0, zDEC,"Ih"},    {_0, zLD,"Ih,B"},   {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zADD,"I,I"},   {_RM,zLD,"I,(W)"},  {_0, zDEC,"I"},
	{_0, zINC,"Il"},    {_0, zDEC,"Il"},    {_0, zLD,"Il,B"},   {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_RW,zINC,"X"},     {_RW,zDEC,"X"},     {_WM,zLD,"X,B"},    {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zADD,"I,sp"},  {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zLD,"b,Ih"},   {_0, zLD,"b,Il"},   {_RM,zLD,"b,X"},    {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zLD,"c,Ih"},   {_0, zLD,"c,Il"},   {_RM,zLD,"c,X"},    {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zLD,"d,Ih"},   {_0, zLD,"d,Il"},   {_RM,zLD,"d,X"},    {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zLD,"e,Ih"},   {_0, zLD,"e,Il"},   {_RM,zLD,"e,X"},    {_0, zDB,"?"},
	{_0, zLD,"Ih,b"},   {_0, zLD,"Ih,c"},   {_0, zLD,"Ih,d"},   {_0, zLD,"Ih,e"},
	{_0, zLD,"Ih,Ih"},  {_0, zLD,"Ih,Il"},  {_RM,zLD,"h,X"},    {_0, zLD,"Ih,a"},
	{_0, zLD,"Il,b"},   {_0, zLD,"Il,c"},   {_0, zLD,"Il,d"},   {_0, zLD,"Il,e"},
	{_0, zLD,"Il,Ih"},  {_0, zLD,"Il,Il"},  {_RM,zLD,"l,X"},    {_0, zLD,"Il,a"},
	{_WM,zLD,"X,b"},    {_WM,zLD,"X,c"},    {_WM,zLD,"X,d"},    {_WM,zLD,"X,e"},
	{_WM,zLD,"X,h"},    {_WM,zLD,"X,l"},    {_0, zDB,"?"},      {_WM,zLD,"X,a"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zLD,"a,Ih"},   {_0, zLD,"a,Il"},   {_RM,zLD,"a,X"},    {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zADD,"a,Ih"},  {_0, zADD,"a,Il"},  {_RM,zADD,"a,X"},   {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zADC,"a,Ih"},  {_0, zADC,"a,Il"},  {_RM,zADC,"a,X"},   {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zSUB,"Ih"},    {_0, zSUB,"Il"},    {_RM,zSUB,"X"},     {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zSBC,"a,Ih"},  {_0, zSBC,"a,Il"},  {_RM,zSBC,"a,X"},   {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zAND,"Ih"},    {_0, zAND,"Il"},    {_RM,zAND,"X"},     {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zXOR,"Ih"},    {_0, zXOR,"Il"},    {_RM,zXOR,"X"},     {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zOR,"Ih"},     {_0, zOR,"Il"},     {_RM,zOR,"X"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zCP,"Ih"},     {_0, zCP,"Il"},     {_RM,zCP,"X"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"cb"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zPOP,"I"},     {_0, zDB,"?"},      {_RW,zEX,"(sp),I"},
	{_0, zDB,"?"},      {_0, zPUSH,"I"},    {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_JP,zJP,"(I)"},    {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zLD,"sp,I"},   {_0, zDB,"?"},      {_0, zDB,"?"},
	{_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"},      {_0, zDB,"?"}
};

static z80dasm mnemonic_main[256]= {
	{_0, zNOP,0},		{_0, zLD,"bc,N"},   {_WM,zLD,"(bc),a"}, {_0, zINC,"bc"},
	{_0, zINC,"b"},     {_0, zDEC,"b"},     {_0, zLD,"b,B"},    {_0, zRLCA,0},
	{_0, zEX,"af,af'"}, {_0, zADD,"hl,bc"}, {_RM,zLD,"a,(bc)"}, {_0, zDEC,"bc"},
	{_0, zINC,"c"},     {_0, zDEC,"c"},     {_0, zLD,"c,B"},    {_0, zRRCA,0},
	{_JR,zDJNZ,"O"},    {_0, zLD,"de,N"},   {_WM,zLD,"(de),a"}, {_0, zINC,"de"},
	{_0, zINC,"d"},     {_0, zDEC,"d"},     {_0, zLD,"d,B"},    {_0, zRLA,0},
	{_JR,zJR,"O"},      {_0, zADD,"hl,de"}, {_RM,zLD,"a,(de)"}, {_0, zDEC,"de"},
	{_0, zINC,"e"},     {_0, zDEC,"e"},     {_0, zLD,"e,B"},    {_0, zRRA,0},
	{_JR,zJR,"nz,O"},   {_0, zLD,"hl,N"},   {_WM,zLD,"(W),hl"}, {_0, zINC,"hl"},
	{_0, zINC,"h"},     {_0, zDEC,"h"},     {_0, zLD,"h,B"},    {_0, zDAA,0},
	{_JR,zJR,"z,O"},    {_0, zADD,"hl,hl"}, {_RM,zLD,"hl,(W)"}, {_0, zDEC,"hl"},
	{_0, zINC,"l"},     {_0, zDEC,"l"},     {_0, zLD,"l,B"},    {_0, zCPL,0},
	{_JR,zJR,"nc,O"},   {_0, zLD,"sp,N"},   {_WM,zLD,"(W),a"},  {_0, zINC,"sp"},
	{_RW,zINC,"(hl)"},  {_RW,zDEC,"(hl)"},  {_WM,zLD,"(hl),B"}, {_0, zSCF,0},
	{_JR,zJR,"c,O"},    {_0, zADD,"hl,sp"}, {_RM,zLD,"a,(W)"},  {_0, zDEC,"sp"},
	{_0, zINC,"a"},     {_0, zDEC,"a"},     {_0, zLD,"a,B"},    {_0, zCCF,0},
	{_0, zLD,"b,b"},    {_0, zLD,"b,c"},    {_0, zLD,"b,d"},    {_0, zLD,"b,e"},
	{_0, zLD,"b,h"},    {_0, zLD,"b,l"},    {_RM,zLD,"b,(hl)"}, {_0, zLD,"b,a"},
	{_0, zLD,"c,b"},    {_0, zLD,"c,c"},    {_0, zLD,"c,d"},    {_0, zLD,"c,e"},
	{_0, zLD,"c,h"},    {_0, zLD,"c,l"},    {_RM,zLD,"c,(hl)"}, {_0, zLD,"c,a"},
	{_0, zLD,"d,b"},    {_0, zLD,"d,c"},    {_0, zLD,"d,d"},    {_0, zLD,"d,e"},
	{_0, zLD,"d,h"},    {_0, zLD,"d,l"},    {_RM,zLD,"d,(hl)"}, {_0, zLD,"d,a"},
	{_0, zLD,"e,b"},    {_0, zLD,"e,c"},    {_0, zLD,"e,d"},    {_0, zLD,"e,e"},
	{_0, zLD,"e,h"},    {_0, zLD,"e,l"},    {_RM,zLD,"e,(hl)"}, {_0, zLD,"e,a"},
	{_0, zLD,"h,b"},    {_0, zLD,"h,c"},    {_0, zLD,"h,d"},    {_0, zLD,"h,e"},
	{_0, zLD,"h,h"},    {_0, zLD,"h,l"},    {_RM,zLD,"h,(hl)"}, {_0, zLD,"h,a"},
	{_0, zLD,"l,b"},    {_0, zLD,"l,c"},    {_0, zLD,"l,d"},    {_0, zLD,"l,e"},
	{_0, zLD,"l,h"},    {_0, zLD,"l,l"},    {_RM,zLD,"l,(hl)"}, {_0, zLD,"l,a"},
	{_WM,zLD,"(hl),b"}, {_WM,zLD,"(hl),c"}, {_WM,zLD,"(hl),d"}, {_WM,zLD,"(hl),e"},
	{_WM,zLD,"(hl),h"}, {_WM,zLD,"(hl),l"}, {_0, zHLT,0},       {_WM,zLD,"(hl),a"},
	{_0, zLD,"a,b"},    {_0, zLD,"a,c"},    {_0, zLD,"a,d"},    {_0, zLD,"a,e"},
	{_0, zLD,"a,h"},    {_0, zLD,"a,l"},    {_RM,zLD,"a,(hl)"}, {_0, zLD,"a,a"},
	{_0, zADD,"a,b"},   {_0, zADD,"a,c"},   {_0, zADD,"a,d"},   {_0, zADD,"a,e"},
	{_0, zADD,"a,h"},   {_0, zADD,"a,l"},   {_RM,zADD,"a,(hl)"},{_0, zADD,"a,a"},
	{_0, zADC,"a,b"},   {_0, zADC,"a,c"},   {_0, zADC,"a,d"},   {_0, zADC,"a,e"},
	{_0, zADC,"a,h"},   {_0, zADC,"a,l"},   {_RM,zADC,"a,(hl)"},{_0, zADC,"a,a"},
	{_0, zSUB,"b"},     {_0, zSUB,"c"},     {_0, zSUB,"d"},     {_0, zSUB,"e"},
	{_0, zSUB,"h"},     {_0, zSUB,"l"},     {_RM,zSUB,"(hl)"},  {_0, zSUB,"a"},
	{_0, zSBC,"a,b"},   {_0, zSBC,"a,c"},   {_0, zSBC,"a,d"},   {_0, zSBC,"a,e"},
	{_0, zSBC,"a,h"},   {_0, zSBC,"a,l"},   {_RM,zSBC,"a,(hl)"},{_0, zSBC,"a,a"},
	{_0, zAND,"b"},     {_0, zAND,"c"},     {_0, zAND,"d"},     {_0, zAND,"e"},
	{_0, zAND,"h"},     {_0, zAND,"l"},     {_RM,zAND,"(hl)"},  {_0, zAND,"a"},
	{_0, zXOR,"b"},     {_0, zXOR,"c"},     {_0, zXOR,"d"},     {_0, zXOR,"e"},
	{_0, zXOR,"h"},     {_0, zXOR,"l"},     {_RM,zXOR,"(hl)"},  {_0, zXOR,"a"},
	{_0, zOR,"b"},      {_0, zOR,"c"},      {_0, zOR,"d"},      {_0, zOR,"e"},
	{_0, zOR,"h"},      {_0, zOR,"l"},      {_RM,zOR,"(hl)"},   {_0, zOR,"a"},
	{_0, zCP,"b"},      {_0, zCP,"c"},      {_0, zCP,"d"},      {_0, zCP,"e"},
	{_0, zCP,"h"},      {_0, zCP,"l"},      {_RM,zCP,"(hl)"},   {_0, zCP,"a"},
	{_0, zRET,"nz"},    {_0, zPOP,"bc"},    {_JP,zJP,"nz,A"},   {_JP,zJP,"A"},
	{_JP,zCALL,"nz,A"}, {_0, zPUSH,"bc"},   {_0, zADD,"a,B"},   {_JP,zRST,"V"},
	{_0, zRET,"z"},     {_0, zRET,0},       {_JP,zJP,"z,A"},    {_0, zDB,"cb"},
	{_JP,zCALL,"z,A"},  {_JP,zCALL,"A"},    {_0, zADC,"a,B"},   {_JP,zRST,"V"},
	{_0, zRET,"nc"},    {_0, zPOP,"de"},    {_JP,zJP,"nc,A"},   {_WP,zOUT,"(P),a"},
	{_JP,zCALL,"nc,A"}, {_0, zPUSH,"de"},   {_0, zSUB,"B"},     {_JP,zRST,"V"},
	{_0, zRET,"c"},     {_0, zEXX,0},       {_JP,zJP,"c,A"},    {_RP,zIN,"a,(P)"},
	{_JP,zCALL,"c,A"},  {_0, zDB,"dd"},     {_0, zSBC,"a,B"},   {_JP,zRST,"V"},
	{_0, zRET,"po"},    {_0, zPOP,"hl"},    {_JP,zJP,"po,A"},   {_RW,zEX,"(sp),hl"},
	{_JP,zCALL,"po,A"}, {_0, zPUSH,"hl"},   {_0, zAND,"B"},     {_JP,zRST,"V"},
	{_0, zRET,"pe"},    {_JP,zJP,"(hl)"},   {_JP,zJP,"pe,A"},   {_0, zEX,"de,hl"},
	{_JP,zCALL,"pe,A"}, {_0, zDB,"ed"},     {_0, zXOR,"B"},     {_JP,zRST,"V"},
	{_0, zRET,"p"},     {_0, zPOP,"af"},    {_JP,zJP,"p,A"},    {_0, zDI,0},
	{_JP,zCALL,"p,A"},  {_0, zPUSH,"af"},   {_0, zOR,"B"},      {_JP,zRST,"V"},
	{_0, zRET,"m"},     {_0, zLD,"sp,hl"},  {_JP,zJP,"m,A"},    {_0, zEI,0},
	{_JP,zCALL,"m,A"},  {_0, zDB,"fd"},     {_0, zCP,"B"},      {_JP,zRST,"V"}
};

static char sign(INT8 offset)
{
 return (offset < 0)? '-':'+';
}

static int offs(INT8 offset)
{
	if (offset < 0) return -offset;
	return offset;
}

/****************************************************************************
 * Disassemble opcode at PC and return number of bytes it takes
 ****************************************************************************/
unsigned DasmZ80( char *buffer, unsigned pc )
{
    z80dasm *d;
	const char *symbol, *src;
	const char *ixy;
	char *dst;
	unsigned PC = pc;
	INT8 offset = 0;
	UINT8 op, op1;
	UINT16 ea = 0, xy = 0;

	ixy = "oops!!";
	dst = buffer;
	symbol = NULL;

	op = cpu_readop( pc++ );
    op1 = 0; /* keep GCC happy */

    switch (op)
	{
	case 0xcb:
		op = cpu_readop(pc++);
        d = &mnemonic_cb[op];
		break;
	case 0xed:
		op1 = cpu_readop(pc++);
        d = &mnemonic_ed[op1];
		break;
	case 0xdd:
		ixy = "ix";
		op1 = cpu_readop(pc++);
		if( op1 == 0xcb )
		{
			offset = (INT8) cpu_readop_arg(pc++);
			op1 = cpu_readop_arg(pc++); /* fourth byte from OP_RAM! */
			xy = z80_get_reg( Z80_IX );
			ea = (xy + offset) & 0xffff;
			d = &mnemonic_xx_cb[op1];
		}
		else d = &mnemonic_xx[op1];
        break;
	case 0xfd:
		ixy = "iy";
		op1 = cpu_readop(pc++);
		if( op1 == 0xcb )
		{
			offset = (INT8) cpu_readop_arg(pc++);
			op1 = cpu_readop_arg(pc++); /* fourth byte from OP_RAM! */
			xy = z80_get_reg( Z80_IY );
			ea = (ea + offset) & 0xffff;
			d = &mnemonic_xx_cb[op1];
		}
		else d = &mnemonic_xx[op1];
        break;
	default:
		d = &mnemonic_main[op];
		break;
	}

    if( d->arguments )
	{
		dst += sprintf(dst, "%-4s ", s_mnemonic[d->mnemonic]);
		src = d->arguments;
		while( *src )
		{
			switch( *src )
			{
			case '?':   /* illegal opcode */
				dst += sprintf( dst, "$%02x,$%02x", op, op1);
				break;
			case 'A':
				ea = cpu_readop_arg(pc) + ( cpu_readop_arg((pc+1)&0xffff) << 8);
				pc += 2;
				symbol = set_ea_info(0, ea, EA_UINT16, d->access);
				dst += sprintf( dst, "%s", symbol );
                break;
            case 'B':   /* Byte op arg */
				ea = cpu_readop_arg( pc++ );
				symbol = set_ea_info(1, ea, EA_UINT8, EA_VALUE);
				dst += sprintf( dst, "%s", symbol );
				break;
			case '(':   /* Memory byte at (HL) */
                *dst++ = *src;
				if( !strncmp( src, "(bc)", 4) )
				{
					ea = z80_get_reg( Z80_BC );
					set_ea_info(0, ea, EA_UINT8, d->access);
				}
                else
				if( !strncmp( src, "(de)", 4) )
				{
					ea = z80_get_reg( Z80_DE );
					set_ea_info(0, ea, EA_UINT8, d->access);
                }
                else
                if( !strncmp( src, "(hl)", 4) )
				{
					ea = z80_get_reg( Z80_HL );
					if( d->access == EA_ABS_PC )
						set_ea_info(0, ea, EA_DEFAULT, EA_ABS_PC);
					else
						set_ea_info(0, ea, EA_UINT8, d->access);
                }
				else
				if( !strncmp( src, "(sp)", 4) )
				{
					ea = z80_get_reg( Z80_SP );
					set_ea_info(0, ea, EA_UINT16, d->access);
                }
				else
				if( !strncmp( src, "(P)", 3) )
				{
					ea = (z80_get_reg( Z80_AF ) & 0xff00) | cpu_readop_arg( pc );
                    set_ea_info(0, ea, EA_UINT16, d->access);
                }
                else
                if( !strncmp( src, "(c)", 3) )
				{
					ea = z80_get_reg( Z80_BC );
					set_ea_info(0, ea, EA_UINT16, d->access);
                }
                else
				if( !strncmp( src, "(I)", 3) )
				{
					ea = xy;
					set_ea_info(0, ea, EA_DEFAULT, d->access);
                }
                break;
			case 'N':   /* Immediate 16 bit */
				ea = cpu_readop_arg(pc) + ( cpu_readop_arg((pc+1)&0xffff) << 8 );
				pc += 2;
				symbol = set_ea_info(1, ea, EA_UINT16, EA_VALUE );
				dst += sprintf( dst, "%s", symbol );
                break;
			case 'O':   /* Offset relative to PC */
				offset = (INT8) cpu_readop_arg(pc++);
				symbol = set_ea_info(0, PC, offset + 2, d->access);
				dst += sprintf( dst, "%s", symbol );
				break;
			case 'P':   /* Port number */
				ea = cpu_readop_arg( pc++ );
				dst += sprintf( dst, "$%02X", ea );
                break;
            case 'V':   /* Restart vector */
				ea = op & 0x38;
				symbol = set_ea_info(0, ea, EA_UINT8, d->access);
				dst += sprintf( dst, "%s", symbol );
				break;
			case 'W':   /* Memory address word */
				ea = cpu_readop_arg(pc) + ( cpu_readop_arg((pc+1)&0xffff) << 8);
				pc += 2;
				symbol = set_ea_info(0, ea, EA_UINT16, d->access);
				dst += sprintf( dst, "%s", symbol );
				break;
			case 'X':
				offset = (INT8) cpu_readop_arg(pc++);
                ea = (xy + offset) & 0xffff;
            case 'Y':
				symbol = set_ea_info(0, ea, EA_UINT8, d->access);
				dst += sprintf( dst,"(%s%c$%02x)", ixy, sign(offset), offs(offset) );
				break;
			case 'I':
				dst += sprintf( dst, "%s", ixy);
				break;
			default:
				*dst++ = *src;
			}
			src++;
		}
		*dst = '\0';
	}
	else
	{
		dst += sprintf(dst, "%s", s_mnemonic[d->mnemonic]);
    }

    return pc - PC;
}

#endif

