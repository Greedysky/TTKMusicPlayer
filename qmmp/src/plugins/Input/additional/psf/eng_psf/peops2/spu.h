/***************************************************************************
                            spu.h  -  description
                             -------------------
    begin                : Wed May 15 2002
    copyright            : (C) 2002 by Pete Bernert
    email                : BlackDove@addcom.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version. See also the license.txt file for *
 *   additional informations.                                              *
 *                                                                         *
 ***************************************************************************/

//*************************************************************************//
// History of changes:
//
// 2004/04/04 - Pete
// - changed plugin to emulate PS2 spu
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

#include "../psx.h"

typedef struct spu2_state_s {
    // psx buffer / addresses

    unsigned short  regArea[32*1024];                        
    unsigned short  spuMem[1*1024*1024];
    unsigned char * spuMemC;
    unsigned char * pSpuIrq[2];
    unsigned char * pSpuBuffer;

    // user settings          

    int             iUseXA;//=0;
    int             iVolume;//=3;
    int             iXAPitch;//=1;
    int             iUseTimer;//=2;
    int             iSPUIRQWait;//=1;
    int             iDebugMode;//=0;
    int             iRecordMode;//=0;
    int             iUseReverb;//=1;
    int             iUseInterpolation;//=2;
                                 
    // MAIN infos struct for each channel

    SPUCHAN         s_chan[MAXCHAN+1];                     // channel + 1 infos (1 is security for fmod handling)
    REVERBInfo      rvb[2];

    unsigned long   dwNoiseVal;//=1;                          // global noise generator

    unsigned short  spuCtrl2[2];                           // some vars to store psx reg infos
    unsigned short  spuStat2[2];
    unsigned long   spuIrq2[2];             
    unsigned long   spuAddr2[2];                           // address into spu mem
    unsigned long   spuRvbAddr2[2];
    unsigned long   spuRvbAEnd2[2];
    int             bEndThread;                          // thread handlers
    int             bThreadEnded;
    int             bSpuInit;
    int             bSPUIsOpen;

    unsigned long dwNewChannel2[2];                        // flags for faster testing, if new channel starts
    unsigned long dwEndChannel2[2];

    // UNUSED IN PS2 YET
    void (CALLBACK *irqCallback)(void);                  // func of main emu, called on spu irq
    void (CALLBACK *cddavCallback)(unsigned short,unsigned short);

    int SSumR[NSSIZE];
    int SSumL[NSSIZE];
    int iCycle;
    short * pS;

    int lastch;//=-1;      // last channel processed on spu irq in timer mode
    int lastns;       // last ns pos
    int iSecureStart; // secure start counter

    u32 sampcount;
    u32 decaybegin;
    u32 decayend;

    // REVERB info and timing vars...
    int *          sRVBPlay[2];
    int *          sRVBEnd[2];
    int *          sRVBStart[2];

    int iSpuAsyncWait;
} spu2_state_t;


//void SetupTimer(mips_cpu_context *cpu);
//void RemoveTimer(mips_cpu_context *cpu);
//EXPORT_GCC void CALLBACK SPU2playADPCMchannel(mips_cpu_context *cpu, xa_decode_t *xap);

EXPORT_GCC long CALLBACK SPU2init(mips_cpu_context *cpu, void (*callback)(unsigned char *, long, void *), void *data);
EXPORT_GCC void CALLBACK SPU2free(mips_cpu_context *cpu);
EXPORT_GCC long CALLBACK SPU2open(mips_cpu_context *cpu, void *pDsp);
EXPORT_GCC void CALLBACK SPU2async(mips_cpu_context *cpu, unsigned long cycle);
EXPORT_GCC void CALLBACK SPU2close(mips_cpu_context *cpu);
void setlength2(spu2_state_t *spu, s32 stop, s32 fade);

