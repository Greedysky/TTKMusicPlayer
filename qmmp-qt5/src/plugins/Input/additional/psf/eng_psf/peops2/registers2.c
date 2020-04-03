/***************************************************************************
                         registers.c  -  description
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
// 2003/02/09 - kode54
// - removed &0x3fff from reverb volume registers, fixes a few games,
//   hopefully won't be breaking anything
//
// 2003/01/19 - Pete
// - added Neill's reverb
//
// 2003/01/06 - Pete
// - added Neill's ADSR timings
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

#include "stdafx.h"

#define _IN_REGISTERS

#include "../peops2/externals.h"
#include "../peops2/registers.h"
#include "../peops2/regs.h"
#include "../peops2/reverb.h"
#include "../peops2/spu.h"
#include "../psx.h"

/*
// adsr time values (in ms) by James Higgs ... see the end of
// the adsr.c source for details

#define ATTACK_MS     514L
#define DECAYHALF_MS  292L
#define DECAY_MS      584L
#define SUSTAIN_MS    450L
#define RELEASE_MS    446L
*/

// we have a timebase of 1.020408f ms, not 1 ms... so adjust adsr defines
#define ATTACK_MS      494L
#define DECAYHALF_MS   286L
#define DECAY_MS       572L
#define SUSTAIN_MS     441L
#define RELEASE_MS     437L

////////////////////////////////////////////////////////////////////////
// WRITE REGISTERS: called by main emu
////////////////////////////////////////////////////////////////////////

EXPORT_GCC void CALLBACK SPU2write(mips_cpu_context *cpu, unsigned long reg, unsigned short val)
{
    spu2_state_t *spu = cpu->spu2;
 long r=reg&0xffff;

 spu->regArea[r>>1] = val;

//	printf("SPU2: %04x to %08x\n", val, reg);

 if((r>=0x0000 && r<0x0180)||(r>=0x0400 && r<0x0580))  // some channel info?
  {
   int ch=(r>>4)&0x1f;
   if(r>=0x400) ch+=24;
   
   switch(r&0x0f)
    {
     //------------------------------------------------// r volume
     case 0:                                           
       SetVolumeL(spu, (unsigned char)ch,val);
       break;
     //------------------------------------------------// l volume
     case 2:                                           
       SetVolumeR(spu, (unsigned char)ch,val);
       break;
     //------------------------------------------------// pitch
     case 4:                                           
       SetPitch(spu, ch,val);
       break;
     //------------------------------------------------// level with pre-calcs
     case 6:
       {
        const unsigned long lval=val;unsigned long lx;
        //---------------------------------------------//
        spu->s_chan[ch].ADSRX.AttackModeExp=(lval&0x8000)?1:0; 
        spu->s_chan[ch].ADSRX.AttackRate=(lval>>8) & 0x007f;
        spu->s_chan[ch].ADSRX.DecayRate=(lval>>4) & 0x000f;
        spu->s_chan[ch].ADSRX.SustainLevel=lval & 0x000f;
        //---------------------------------------------//
        if(!spu->iDebugMode) break;
        //---------------------------------------------// stuff below is only for debug mode

        spu->s_chan[ch].ADSR.AttackModeExp=(lval&0x8000)?1:0;        //0x007f

        lx=(((lval>>8) & 0x007f)>>2);                  // attack time to run from 0 to 100% volume
        lx=min(31,lx);                                 // no overflow on shift!
        if(lx) 
         { 
          lx = (1<<lx);
          if(lx<2147483) lx=(lx*ATTACK_MS)/10000L;     // another overflow check
          else           lx=(lx/10000L)*ATTACK_MS;
          if(!lx) lx=1;
         }
        spu->s_chan[ch].ADSR.AttackTime=lx;                

        spu->s_chan[ch].ADSR.SustainLevel=                 // our adsr vol runs from 0 to 1024, so scale the sustain level
         (1024*((lval) & 0x000f))/15;

        lx=(lval>>4) & 0x000f;                         // decay:
        if(lx)                                         // our const decay value is time it takes from 100% to 0% of volume
         {
          lx = ((1<<(lx))*DECAY_MS)/10000L;
          if(!lx) lx=1;
         }
        spu->s_chan[ch].ADSR.DecayTime =                   // so calc how long does it take to run from 100% to the wanted sus level
         (lx*(1024-spu->s_chan[ch].ADSR.SustainLevel))/1024;
       }
      break;
     //------------------------------------------------// adsr times with pre-calcs
     case 8:
      {
       const unsigned long lval=val;unsigned long lx;

       //----------------------------------------------//
       spu->s_chan[ch].ADSRX.SustainModeExp = (lval&0x8000)?1:0;
       spu->s_chan[ch].ADSRX.SustainIncrease= (lval&0x4000)?0:1;
       spu->s_chan[ch].ADSRX.SustainRate = (lval>>6) & 0x007f;
       spu->s_chan[ch].ADSRX.ReleaseModeExp = (lval&0x0020)?1:0;
       spu->s_chan[ch].ADSRX.ReleaseRate = lval & 0x001f;
       //----------------------------------------------//
       if(!spu->iDebugMode) break;
       //----------------------------------------------// stuff below is only for debug mode

       spu->s_chan[ch].ADSR.SustainModeExp = (lval&0x8000)?1:0;
       spu->s_chan[ch].ADSR.ReleaseModeExp = (lval&0x0020)?1:0;
                   
       lx=((((lval>>6) & 0x007f)>>2));                 // sustain time... often very high
       lx=min(31,lx);                                  // values are used to hold the volume
       if(lx)                                          // until a sound stop occurs
        {                                              // the highest value we reach (due to 
         lx = (1<<lx);                                 // overflow checking) is: 
         if(lx<2147483) lx=(lx*SUSTAIN_MS)/10000L;     // 94704 seconds = 1578 minutes = 26 hours... 
         else           lx=(lx/10000L)*SUSTAIN_MS;     // should be enuff... if the stop doesn't 
         if(!lx) lx=1;                                 // come in this time span, I don't care :)
        }
       spu->s_chan[ch].ADSR.SustainTime = lx;

       lx=(lval & 0x001f);
       spu->s_chan[ch].ADSR.ReleaseVal     =lx;
       if(lx)                                          // release time from 100% to 0%
        {                                              // note: the release time will be
         lx = (1<<lx);                                 // adjusted when a stop is coming,
         if(lx<2147483) lx=(lx*RELEASE_MS)/10000L;     // so at this time the adsr vol will 
         else           lx=(lx/10000L)*RELEASE_MS;     // run from (current volume) to 0%
         if(!lx) lx=1;
        }
       spu->s_chan[ch].ADSR.ReleaseTime=lx;

       if(lval & 0x4000)                               // add/dec flag
            spu->s_chan[ch].ADSR.SustainModeDec=-1;
       else spu->s_chan[ch].ADSR.SustainModeDec=1;
      }
     break;
     //------------------------------------------------//
    }

   spu->iSpuAsyncWait=0;

   return;
  }

 if((r>=0x01c0 && r<0x02E0)||(r>=0x05c0 && r<0x06E0))  // some channel info?
  {
   int ch=0;
   if(r>=0x400) {ch=24;r-=0x400;}

   ch+=(r-0x1c0)/12;
   r-=(ch%24)*12;
   switch(r)
    {
     //------------------------------------------------//
     case 0x1C0:
      spu->s_chan[ch].iStartAdr=(((unsigned long)val&0xf)<<16)|(spu->s_chan[ch].iStartAdr&0xFFFF);
      spu->s_chan[ch].pStart=spu->spuMemC+(spu->s_chan[ch].iStartAdr<<1);
      break;
     case 0x1C2:
      spu->s_chan[ch].iStartAdr=(spu->s_chan[ch].iStartAdr & 0xF0000) | (val & 0xFFFF);
      spu->s_chan[ch].pStart=spu->spuMemC+(spu->s_chan[ch].iStartAdr<<1);
      break;
     //------------------------------------------------//
     case 0x1C4:
      spu->s_chan[ch].iLoopAdr=(((unsigned long)val&0xf)<<16)|(spu->s_chan[ch].iLoopAdr&0xFFFF);
      spu->s_chan[ch].pLoop=spu->spuMemC+(spu->s_chan[ch].iLoopAdr<<1);
      spu->s_chan[ch].bIgnoreLoop=1;
      break;
     case 0x1C6:
      spu->s_chan[ch].iLoopAdr=(spu->s_chan[ch].iLoopAdr & 0xF0000) | (val & 0xFFFF);
      spu->s_chan[ch].pLoop=spu->spuMemC+(spu->s_chan[ch].iLoopAdr<<1);
      spu->s_chan[ch].bIgnoreLoop=1;
      break;
     //------------------------------------------------//
     case 0x1C8:
      // unused... check if it gets written as well
      spu->s_chan[ch].iNextAdr=(((unsigned long)val&0xf)<<16)|(spu->s_chan[ch].iNextAdr&0xFFFF);
      break;
     case 0x1CA:
      // unused... check if it gets written as well
      spu->s_chan[ch].iNextAdr=(spu->s_chan[ch].iNextAdr & 0xF0000) | (val & 0xFFFF);
      break;
     //------------------------------------------------//
    }

   spu->iSpuAsyncWait=0;

   return;
  } 

 switch(r)
   {
    //-------------------------------------------------//
    case PS2_C0_SPUaddr_Hi:
      spu->spuAddr2[0] = (((unsigned long)val&0xf)<<16)|(spu->spuAddr2[0]&0xFFFF);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUaddr_Lo:
      spu->spuAddr2[0] = (spu->spuAddr2[0] & 0xF0000) | (val & 0xFFFF);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUaddr_Hi:
      spu->spuAddr2[1] = (((unsigned long)val&0xf)<<16)|(spu->spuAddr2[1]&0xFFFF);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUaddr_Lo:
      spu->spuAddr2[1] = (spu->spuAddr2[1] & 0xF0000) | (val & 0xFFFF);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUdata:
      spu->spuMem[spu->spuAddr2[0]] = val;
      spu->spuAddr2[0]++;
      if(spu->spuAddr2[0]>0xfffff) spu->spuAddr2[0]=0;
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUdata:
      spu->spuMem[spu->spuAddr2[1]] = val;
      spu->spuAddr2[1]++;
      if(spu->spuAddr2[1]>0xfffff) spu->spuAddr2[1]=0;
      break;
    //-------------------------------------------------//
    case PS2_C0_ATTR:
      spu->spuCtrl2[0]=val;
      break;
    //-------------------------------------------------//
    case PS2_C1_ATTR:
      spu->spuCtrl2[1]=val;
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUstat:
      spu->spuStat2[0]=val;
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUstat:
      spu->spuStat2[1]=val;
      break;
    //-------------------------------------------------//
    case PS2_C0_ReverbAddr_Hi:
      spu->spuRvbAddr2[0] = (((unsigned long)val&0xf)<<16)|(spu->spuRvbAddr2[0]&0xFFFF);
      SetReverbAddr(spu, 0);
      break;
    //-------------------------------------------------//
    case PS2_C0_ReverbAddr_Lo:
      spu->spuRvbAddr2[0] = (spu->spuRvbAddr2[0] & 0xF0000) | (val & 0xFFFF);
      SetReverbAddr(spu, 0);
      break;
    //-------------------------------------------------//
    case PS2_C0_ReverbAEnd_Hi:
      spu->spuRvbAEnd2[0] = (((unsigned long)val&0xf)<<16)|(/*spu->spuRvbAEnd2[0]&*/0xFFFF);
      spu->rvb[0].EndAddr=spu->spuRvbAEnd2[0];
      break;
    //-------------------------------------------------//
    case PS2_C1_ReverbAEnd_Hi:
      spu->spuRvbAEnd2[1] = (((unsigned long)val&0xf)<<16)|(/*spu->spuRvbAEnd2[1]&*/0xFFFF);
      spu->rvb[1].EndAddr=spu->spuRvbAEnd2[1];
      break;
    //-------------------------------------------------//
    case PS2_C1_ReverbAddr_Hi:
      spu->spuRvbAddr2[1] = (((unsigned long)val&0xf)<<16)|(spu->spuRvbAddr2[1]&0xFFFF);
      SetReverbAddr(spu, 1);
      break;
    //-------------------------------------------------//
    case PS2_C1_ReverbAddr_Lo:
      spu->spuRvbAddr2[1] = (spu->spuRvbAddr2[1] & 0xF0000) | (val & 0xFFFF);
      SetReverbAddr(spu, 1);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUirqAddr_Hi:
      spu->spuIrq2[0] = (((unsigned long)val&0xf)<<16)|(spu->spuIrq2[0]&0xFFFF);
      spu->pSpuIrq[0]=spu->spuMemC+(spu->spuIrq2[0]<<1);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUirqAddr_Lo:
      spu->spuIrq2[0] = (spu->spuIrq2[0] & 0xF0000) | (val & 0xFFFF);
      spu->pSpuIrq[0]=spu->spuMemC+(spu->spuIrq2[0]<<1);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUirqAddr_Hi:
      spu->spuIrq2[1] = (((unsigned long)val&0xf)<<16)|(spu->spuIrq2[1]&0xFFFF);
      spu->pSpuIrq[1]=spu->spuMemC+(spu->spuIrq2[1]<<1);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUirqAddr_Lo:
      spu->spuIrq2[1] = (spu->spuIrq2[1] & 0xF0000) | (val & 0xFFFF);
      spu->pSpuIrq[1]=spu->spuMemC+(spu->spuIrq2[1]<<1);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUrvolL:
      spu->rvb[0].VolLeft=val;
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUrvolR:
      spu->rvb[0].VolRight=val;
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUrvolL:
      spu->rvb[1].VolLeft=val;
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUrvolR:
      spu->rvb[1].VolRight=val;
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUon1:
      SoundOn(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUon2:
      SoundOn(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUon1:
      SoundOn(spu, 24,40,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUon2:
      SoundOn(spu, 40,48,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUoff1:
      SoundOff(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUoff2:
      SoundOff(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUoff1:
      SoundOff(spu, 24,40,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUoff2:
      SoundOff(spu, 40,48,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_SPUend1:
    case PS2_C0_SPUend2:
      if(val) spu->dwEndChannel2[0]=0;
      break;
    //-------------------------------------------------//
    case PS2_C1_SPUend1:
    case PS2_C1_SPUend2:
      if(val) spu->dwEndChannel2[1]=0;
      break;
    //-------------------------------------------------//
    case PS2_C0_FMod1:
      FModOn(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_FMod2:
      FModOn(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_FMod1:
      FModOn(spu, 24,40,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_FMod2:
      FModOn(spu, 40,48,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_Noise1:
      NoiseOn(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_Noise2:
      NoiseOn(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_Noise1:
      NoiseOn(spu, 24,40,val);
      break;
    //-------------------------------------------------//
    case PS2_C1_Noise2:
      NoiseOn(spu, 40,48,val);
      break;
    //-------------------------------------------------//
    case PS2_C0_DryL1:
      VolumeOn(spu, 0,16,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C0_DryL2:
      VolumeOn(spu, 16,24,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C1_DryL1:
      VolumeOn(spu, 24,40,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C1_DryL2:
      VolumeOn(spu, 40,48,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C0_DryR1:
      VolumeOn(spu, 0,16,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C0_DryR2:
      VolumeOn(spu, 16,24,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C1_DryR1:
      VolumeOn(spu, 24,40,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C1_DryR2:
      VolumeOn(spu, 40,48,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C0_RVBon1_L:
      ReverbOn(spu, 0,16,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C0_RVBon2_L:
      ReverbOn(spu, 16,24,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C1_RVBon1_L:
      ReverbOn(spu, 24,40,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C1_RVBon2_L:
      ReverbOn(spu, 40,48,val,0);
      break;
    //-------------------------------------------------//
    case PS2_C0_RVBon1_R:
      ReverbOn(spu, 0,16,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C0_RVBon2_R:
      ReverbOn(spu, 16,24,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C1_RVBon1_R:
      ReverbOn(spu, 24,40,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C1_RVBon2_R:
      ReverbOn(spu, 40,48,val,1);
      break;
    //-------------------------------------------------//
    case PS2_C0_Reverb+0:
      spu->rvb[0].FB_SRC_A=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].FB_SRC_A&0xFFFF);
      break;
    case PS2_C0_Reverb+2:
      spu->rvb[0].FB_SRC_A=(spu->rvb[0].FB_SRC_A & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+4:
      spu->rvb[0].FB_SRC_B=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].FB_SRC_B&0xFFFF);
      break;
    case PS2_C0_Reverb+6:
      spu->rvb[0].FB_SRC_B=(spu->rvb[0].FB_SRC_B & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+8:
      spu->rvb[0].IIR_DEST_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_DEST_A0&0xFFFF);
      break;
    case PS2_C0_Reverb+10:
      spu->rvb[0].IIR_DEST_A0=(spu->rvb[0].IIR_DEST_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+12:
      spu->rvb[0].IIR_DEST_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_DEST_A1&0xFFFF);
      break;
    case PS2_C0_Reverb+14:
      spu->rvb[0].IIR_DEST_A1=(spu->rvb[0].IIR_DEST_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+16:
      spu->rvb[0].ACC_SRC_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_A0&0xFFFF);
      break;
    case PS2_C0_Reverb+18:
      spu->rvb[0].ACC_SRC_A0=(spu->rvb[0].ACC_SRC_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+20:
      spu->rvb[0].ACC_SRC_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_A1&0xFFFF);
      break;
    case PS2_C0_Reverb+22:
      spu->rvb[0].ACC_SRC_A1=(spu->rvb[0].ACC_SRC_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+24:
      spu->rvb[0].ACC_SRC_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_B0&0xFFFF);
      break;
    case PS2_C0_Reverb+26:
      spu->rvb[0].ACC_SRC_B0=(spu->rvb[0].ACC_SRC_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+28:
      spu->rvb[0].ACC_SRC_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_B1&0xFFFF);
      break;
    case PS2_C0_Reverb+30:
      spu->rvb[0].ACC_SRC_B1=(spu->rvb[0].ACC_SRC_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+32:
      spu->rvb[0].IIR_SRC_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_SRC_A0&0xFFFF);
      break;
    case PS2_C0_Reverb+34:
      spu->rvb[0].IIR_SRC_A0=(spu->rvb[0].IIR_SRC_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+36:
      spu->rvb[0].IIR_SRC_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_SRC_A1&0xFFFF);
      break;
    case PS2_C0_Reverb+38:
      spu->rvb[0].IIR_SRC_A1=(spu->rvb[0].IIR_SRC_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+40:
      spu->rvb[0].IIR_DEST_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_DEST_B0&0xFFFF);
      break;
    case PS2_C0_Reverb+42:
      spu->rvb[0].IIR_DEST_B0=(spu->rvb[0].IIR_DEST_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+44:
      spu->rvb[0].IIR_DEST_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_DEST_B1&0xFFFF);
      break;
    case PS2_C0_Reverb+46:
      spu->rvb[0].IIR_DEST_B1=(spu->rvb[0].IIR_DEST_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+48:
      spu->rvb[0].ACC_SRC_C0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_C0&0xFFFF);
      break;
    case PS2_C0_Reverb+50:
      spu->rvb[0].ACC_SRC_C0=(spu->rvb[0].ACC_SRC_C0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+52:
      spu->rvb[0].ACC_SRC_C1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_C1&0xFFFF);
      break;
    case PS2_C0_Reverb+54:
      spu->rvb[0].ACC_SRC_C1=(spu->rvb[0].ACC_SRC_C1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+56:
      spu->rvb[0].ACC_SRC_D0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_D0&0xFFFF);
      break;
    case PS2_C0_Reverb+58:
      spu->rvb[0].ACC_SRC_D0=(spu->rvb[0].ACC_SRC_D0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+60:
      spu->rvb[0].ACC_SRC_D1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].ACC_SRC_D1&0xFFFF);
      break;
    case PS2_C0_Reverb+62:
      spu->rvb[0].ACC_SRC_D1=(spu->rvb[0].ACC_SRC_D1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+64:
      spu->rvb[0].IIR_SRC_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_SRC_B1&0xFFFF);
      break;
    case PS2_C0_Reverb+66:
      spu->rvb[0].IIR_SRC_B1=(spu->rvb[0].IIR_SRC_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+68:
      spu->rvb[0].IIR_SRC_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].IIR_SRC_B0&0xFFFF);
      break;
    case PS2_C0_Reverb+70:
      spu->rvb[0].IIR_SRC_B0=(spu->rvb[0].IIR_SRC_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+72:
      spu->rvb[0].MIX_DEST_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].MIX_DEST_A0&0xFFFF);
      break;
    case PS2_C0_Reverb+74:
      spu->rvb[0].MIX_DEST_A0=(spu->rvb[0].MIX_DEST_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+76:
      spu->rvb[0].MIX_DEST_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].MIX_DEST_A1&0xFFFF);
      break;
    case PS2_C0_Reverb+78:
      spu->rvb[0].MIX_DEST_A1=(spu->rvb[0].MIX_DEST_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+80:
      spu->rvb[0].MIX_DEST_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].MIX_DEST_B0&0xFFFF);
      break;
    case PS2_C0_Reverb+82:
      spu->rvb[0].MIX_DEST_B0=(spu->rvb[0].MIX_DEST_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_Reverb+84:
      spu->rvb[0].MIX_DEST_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[0].MIX_DEST_B1&0xFFFF);
      break;
    case PS2_C0_Reverb+86:
      spu->rvb[0].MIX_DEST_B1=(spu->rvb[0].MIX_DEST_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C0_ReverbX+0:  spu->rvb[0].IIR_ALPHA=(short)val;      break;
    case PS2_C0_ReverbX+2:  spu->rvb[0].ACC_COEF_A=(short)val;     break;
    case PS2_C0_ReverbX+4:  spu->rvb[0].ACC_COEF_B=(short)val;     break;
    case PS2_C0_ReverbX+6:  spu->rvb[0].ACC_COEF_C=(short)val;     break;
    case PS2_C0_ReverbX+8:  spu->rvb[0].ACC_COEF_D=(short)val;     break;
    case PS2_C0_ReverbX+10: spu->rvb[0].IIR_COEF=(short)val;       break;
    case PS2_C0_ReverbX+12: spu->rvb[0].FB_ALPHA=(short)val;       break;
    case PS2_C0_ReverbX+14: spu->rvb[0].FB_X=(short)val;           break;
    case PS2_C0_ReverbX+16: spu->rvb[0].IN_COEF_L=(short)val;      break;
    case PS2_C0_ReverbX+18: spu->rvb[0].IN_COEF_R=(short)val;      break;
    //-------------------------------------------------//
    case PS2_C1_Reverb+0:
      spu->rvb[1].FB_SRC_A=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].FB_SRC_A&0xFFFF);
      break;
    case PS2_C1_Reverb+2:
      spu->rvb[1].FB_SRC_A=(spu->rvb[1].FB_SRC_A & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+4:
      spu->rvb[1].FB_SRC_B=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].FB_SRC_B&0xFFFF);
      break;
    case PS2_C1_Reverb+6:
      spu->rvb[1].FB_SRC_B=(spu->rvb[1].FB_SRC_B & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+8:
      spu->rvb[1].IIR_DEST_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_DEST_A0&0xFFFF);
      break;
    case PS2_C1_Reverb+10:
      spu->rvb[1].IIR_DEST_A0=(spu->rvb[1].IIR_DEST_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+12:
      spu->rvb[1].IIR_DEST_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_DEST_A1&0xFFFF);
      break;
    case PS2_C1_Reverb+14:
      spu->rvb[1].IIR_DEST_A1=(spu->rvb[1].IIR_DEST_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+16:
      spu->rvb[1].ACC_SRC_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_A0&0xFFFF);
      break;
    case PS2_C1_Reverb+18:
      spu->rvb[1].ACC_SRC_A0=(spu->rvb[1].ACC_SRC_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+20:
      spu->rvb[1].ACC_SRC_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_A1&0xFFFF);
      break;
    case PS2_C1_Reverb+22:
      spu->rvb[1].ACC_SRC_A1=(spu->rvb[1].ACC_SRC_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+24:
      spu->rvb[1].ACC_SRC_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_B0&0xFFFF);
      break;
    case PS2_C1_Reverb+26:
      spu->rvb[1].ACC_SRC_B0=(spu->rvb[1].ACC_SRC_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+28:
      spu->rvb[1].ACC_SRC_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_B1&0xFFFF);
      break;
    case PS2_C1_Reverb+30:
      spu->rvb[1].ACC_SRC_B1=(spu->rvb[1].ACC_SRC_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+32:
      spu->rvb[1].IIR_SRC_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_SRC_A0&0xFFFF);
      break;
    case PS2_C1_Reverb+34:
      spu->rvb[1].IIR_SRC_A0=(spu->rvb[1].IIR_SRC_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+36:
      spu->rvb[1].IIR_SRC_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_SRC_A1&0xFFFF);
      break;
    case PS2_C1_Reverb+38:
      spu->rvb[1].IIR_SRC_A1=(spu->rvb[1].IIR_SRC_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+40:
      spu->rvb[1].IIR_DEST_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_DEST_B0&0xFFFF);
      break;
    case PS2_C1_Reverb+42:
      spu->rvb[1].IIR_DEST_B0=(spu->rvb[1].IIR_DEST_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+44:
      spu->rvb[1].IIR_DEST_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_DEST_B1&0xFFFF);
      break;
    case PS2_C1_Reverb+46:
      spu->rvb[1].IIR_DEST_B1=(spu->rvb[1].IIR_DEST_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+48:
      spu->rvb[1].ACC_SRC_C0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_C0&0xFFFF);
      break;
    case PS2_C1_Reverb+50:
      spu->rvb[1].ACC_SRC_C0=(spu->rvb[1].ACC_SRC_C0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+52:
      spu->rvb[1].ACC_SRC_C1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_C1&0xFFFF);
      break;
    case PS2_C1_Reverb+54:
      spu->rvb[1].ACC_SRC_C1=(spu->rvb[1].ACC_SRC_C1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+56:
      spu->rvb[1].ACC_SRC_D0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_D0&0xFFFF);
      break;
    case PS2_C1_Reverb+58:
      spu->rvb[1].ACC_SRC_D0=(spu->rvb[1].ACC_SRC_D0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+60:
      spu->rvb[1].ACC_SRC_D1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].ACC_SRC_D1&0xFFFF);
      break;
    case PS2_C1_Reverb+62:
      spu->rvb[1].ACC_SRC_D1=(spu->rvb[1].ACC_SRC_D1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+64:
      spu->rvb[1].IIR_SRC_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_SRC_B1&0xFFFF);
      break;
    case PS2_C1_Reverb+66:
      spu->rvb[1].IIR_SRC_B1=(spu->rvb[1].IIR_SRC_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+68:
      spu->rvb[1].IIR_SRC_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].IIR_SRC_B0&0xFFFF);
      break;
    case PS2_C1_Reverb+70:
      spu->rvb[1].IIR_SRC_B0=(spu->rvb[1].IIR_SRC_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+72:
      spu->rvb[1].MIX_DEST_A0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].MIX_DEST_A0&0xFFFF);
      break;
    case PS2_C1_Reverb+74:
      spu->rvb[1].MIX_DEST_A0=(spu->rvb[1].MIX_DEST_A0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+76:
      spu->rvb[1].MIX_DEST_A1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].MIX_DEST_A1&0xFFFF);
      break;
    case PS2_C1_Reverb+78:
      spu->rvb[1].MIX_DEST_A1=(spu->rvb[1].MIX_DEST_A1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+80:
      spu->rvb[1].MIX_DEST_B0=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].MIX_DEST_B0&0xFFFF);
      break;
    case PS2_C1_Reverb+82:
      spu->rvb[1].MIX_DEST_B0=(spu->rvb[1].MIX_DEST_B0 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_Reverb+84:
      spu->rvb[1].MIX_DEST_B1=(((unsigned long)val&0xf)<<16)|(spu->rvb[1].MIX_DEST_B1&0xFFFF);
      break;
    case PS2_C1_Reverb+86:
      spu->rvb[1].MIX_DEST_B1=(spu->rvb[1].MIX_DEST_B1 & 0xF0000) | ((val) & 0xFFFF);
      break;
    case PS2_C1_ReverbX+0:  spu->rvb[1].IIR_ALPHA=(short)val;      break;
    case PS2_C1_ReverbX+2:  spu->rvb[1].ACC_COEF_A=(short)val;     break;
    case PS2_C1_ReverbX+4:  spu->rvb[1].ACC_COEF_B=(short)val;     break;
    case PS2_C1_ReverbX+6:  spu->rvb[1].ACC_COEF_C=(short)val;     break;
    case PS2_C1_ReverbX+8:  spu->rvb[1].ACC_COEF_D=(short)val;     break;
    case PS2_C1_ReverbX+10: spu->rvb[1].IIR_COEF=(short)val;       break;
    case PS2_C1_ReverbX+12: spu->rvb[1].FB_ALPHA=(short)val;       break;
    case PS2_C1_ReverbX+14: spu->rvb[1].FB_X=(short)val;           break;
    case PS2_C1_ReverbX+16: spu->rvb[1].IN_COEF_L=(short)val;      break;
    case PS2_C1_ReverbX+18: spu->rvb[1].IN_COEF_R=(short)val;      break;
   }

 spu->iSpuAsyncWait=0;

}

////////////////////////////////////////////////////////////////////////
// READ REGISTER: called by main emu
////////////////////////////////////////////////////////////////////////

EXPORT_GCC unsigned short CALLBACK SPU2read(mips_cpu_context *cpu, unsigned long reg)
{
    spu2_state_t *spu = cpu->spu2;
 long r=reg&0xffff;

#ifdef _WINDOWS
// if(iDebugMode==1) logprintf("R_REG %X\r\n",reg&0xFFFF);
#endif

 spu->iSpuAsyncWait=0;

 if((r>=0x0000 && r<0x0180)||(r>=0x0400 && r<0x0580))  // some channel info?
  {
   switch(r&0x0f)
    {
     //------------------------------------------------// env value
     case 10:                                           
      {
       int ch=(r>>4)&0x1f;
       if(r>=0x400) ch+=24;
       if(spu->s_chan[ch].bNew) return 1;                   // we are started, but not processed? return 1
       if(spu->s_chan[ch].ADSRX.lVolume &&                  // same here... we haven't decoded one sample yet, so no envelope yet. return 1 as well
          !spu->s_chan[ch].ADSRX.EnvelopeVol)                   
        return 1;
       return (unsigned short)(spu->s_chan[ch].ADSRX.EnvelopeVol>>16);
      }break;
    }
  }

 if((r>=0x01c0 && r<0x02E0)||(r>=0x05c0 && r<0x06E0))  // some channel info?
  {
   int ch=0;unsigned long rx=r;
   if(rx>=0x400) {ch=24;rx-=0x400;}
        
   ch+=(rx-0x1c0)/12;
   rx-=(ch%24)*12;
   
   switch(rx)
    {
     //------------------------------------------------//
     case 0x1C4:
      return (((spu->s_chan[ch].pLoop-spu->spuMemC)>>17)&0xF);
      break;
     case 0x1C6:
      return (((spu->s_chan[ch].pLoop-spu->spuMemC)>>1)&0xFFFF);
      break;
     //------------------------------------------------//
     case 0x1C8:
      return (((spu->s_chan[ch].pCurr-spu->spuMemC)>>17)&0xF);
      break;
     case 0x1CA:
      return (((spu->s_chan[ch].pCurr-spu->spuMemC)>>1)&0xFFFF);
      break;
     //------------------------------------------------//
    }
  }

 switch(r)
  {
   //--------------------------------------------------//
   case PS2_C0_SPUend1:
     return (unsigned short)((spu->dwEndChannel2[0]&0xFFFF));
   case PS2_C0_SPUend2:
     return (unsigned short)((spu->dwEndChannel2[0]>>16));
   //--------------------------------------------------//
   case PS2_C1_SPUend1:
     return (unsigned short)((spu->dwEndChannel2[1]&0xFFFF));
   case PS2_C1_SPUend2:
     return (unsigned short)((spu->dwEndChannel2[1]>>16));
   //--------------------------------------------------//
   case PS2_C0_ATTR:
     return spu->spuCtrl2[0];
     break;
   //--------------------------------------------------//
   case PS2_C1_ATTR:
     return spu->spuCtrl2[1];
     break;
   //--------------------------------------------------//
   case PS2_C0_SPUstat:
     return spu->spuStat2[0];
     break;
   //--------------------------------------------------//
   case PS2_C1_SPUstat:
     return spu->spuStat2[1];
     break;
   //--------------------------------------------------//
   case PS2_C0_SPUdata:
     {
      unsigned short s=spu->spuMem[spu->spuAddr2[0]];
      spu->spuAddr2[0]++;
      if(spu->spuAddr2[0]>0xfffff) spu->spuAddr2[0]=0;
      return s;
     }
   //--------------------------------------------------//
   case PS2_C1_SPUdata:
     {
      unsigned short s=spu->spuMem[spu->spuAddr2[1]];
      spu->spuAddr2[1]++;
      if(spu->spuAddr2[1]>0xfffff) spu->spuAddr2[1]=0;
      return s;
     }
   //--------------------------------------------------//
   case PS2_C0_SPUaddr_Hi:
     return (unsigned short)((spu->spuAddr2[0]>>16)&0xF);
     break;
   case PS2_C0_SPUaddr_Lo:
     return (unsigned short)((spu->spuAddr2[0]&0xFFFF));
     break;
   //--------------------------------------------------//
   case PS2_C1_SPUaddr_Hi:
     return (unsigned short)((spu->spuAddr2[1]>>16)&0xF);
     break;
   case PS2_C1_SPUaddr_Lo:
     return (unsigned short)((spu->spuAddr2[1]&0xFFFF));
     break;
   //--------------------------------------------------//
  }

 return spu->regArea[r>>1];
}

EXPORT_GCC void CALLBACK SPU2writePS1Port(mips_cpu_context *cpu, unsigned long reg, unsigned short val)
{
    spu2_state_t *spu = cpu->spu2;
 const u32 r=reg&0xfff;

 if(r>=0xc00 && r<0xd80)	// channel info
 {
 	SPU2write(cpu, r-0xc00, val);
 	return;
 }

 switch(r)
   {
    //-------------------------------------------------//
    case H_SPUaddr:
      spu->spuAddr2[0] = (u32) val<<2;
      break;
    //-------------------------------------------------//
    case H_SPUdata:
      spu->spuMem[spu->spuAddr2[0]] = BFLIP16(val);
      spu->spuAddr2[0]++;
      if(spu->spuAddr2[0]>0xfffff) spu->spuAddr2[0]=0;
      break;
    //-------------------------------------------------//
    case H_SPUctrl:
//      spuCtrl=val;
      break;
    //-------------------------------------------------//
    case H_SPUstat:
      spu->spuStat2[0]=val & 0xf800;
      break;
    //-------------------------------------------------//
    case H_SPUReverbAddr:
      spu->spuRvbAddr2[0] = val;
      SetReverbAddr(spu, 0);
      break;
    //-------------------------------------------------//
    case H_SPUirqAddr:
      spu->spuIrq2[0] = val<<2;
      spu->pSpuIrq[0]=spu->spuMemC+((u32) val<<1);
      break;
    //-------------------------------------------------//
    /* Volume settings appear to be at least 15-bit unsigned in this case.  
       Definitely NOT 15-bit signed.  Probably 16-bit signed, so s16 type cast.
       Check out "Chrono Cross:  Shadow's End Forest"
    */
    case H_SPUrvolL:
      spu->rvb[0].VolLeft=(s16)val;
      //printf("%d\n",val);
      break;
    //-------------------------------------------------//
    case H_SPUrvolR:
      spu->rvb[0].VolRight=(s16)val;
      //printf("%d\n",val);
      break;
    //-------------------------------------------------//

/*
    case H_ExtLeft:
     //auxprintf("EL %d\n",val);
      break;
    //-------------------------------------------------//
    case H_ExtRight:
     //auxprintf("ER %d\n",val);
      break;
    //-------------------------------------------------//
    case H_SPUmvolL:
     //auxprintf("ML %d\n",val);
      break;
    //-------------------------------------------------//
    case H_SPUmvolR:
     //auxprintf("MR %d\n",val);
      break;
    //-------------------------------------------------//
    case H_SPUMute1:
     //printf("M0 %04x\n",val);
      break;
    //-------------------------------------------------//
    case H_SPUMute2:
    // printf("M1 %04x\n",val);
      break;
*/
    //-------------------------------------------------//
    case H_SPUon1:
      SoundOn(spu, 0,16,val);
      break;
    //-------------------------------------------------//
     case H_SPUon2:
      //printf("Boop: %08x: %04x\n",reg,val);
      SoundOn(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case H_SPUoff1:
      SoundOff(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case H_SPUoff2:
      SoundOff(spu, 16,24,val);
	// printf("Boop: %08x: %04x\n",reg,val);
      break;
    //-------------------------------------------------//
    case H_FMod1:
      FModOn(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case H_FMod2:
      FModOn(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case H_Noise1:
      NoiseOn(spu, 0,16,val);
      break;
    //-------------------------------------------------//
    case H_Noise2:
      NoiseOn(spu, 16,24,val);
      break;
    //-------------------------------------------------//
    case H_RVBon1:
      ReverbOn(spu, 0,16,val,0);
      break;

    //-------------------------------------------------//
    case H_RVBon2:
      ReverbOn(spu, 16,24,val,0);
      break;

    //-------------------------------------------------//
    case H_Reverb+0:
      spu->rvb[0].FB_SRC_A=val;
      break;

    case H_Reverb+2   : spu->rvb[0].FB_SRC_B=(s16)val;       break;
    case H_Reverb+4   : spu->rvb[0].IIR_ALPHA=(s16)val;      break;
    case H_Reverb+6   : spu->rvb[0].ACC_COEF_A=(s16)val;     break;
    case H_Reverb+8   : spu->rvb[0].ACC_COEF_B=(s16)val;     break;
    case H_Reverb+10  : spu->rvb[0].ACC_COEF_C=(s16)val;     break;
    case H_Reverb+12  : spu->rvb[0].ACC_COEF_D=(s16)val;     break;
    case H_Reverb+14  : spu->rvb[0].IIR_COEF=(s16)val;       break;
    case H_Reverb+16  : spu->rvb[0].FB_ALPHA=(s16)val;       break;
    case H_Reverb+18  : spu->rvb[0].FB_X=(s16)val;           break;
    case H_Reverb+20  : spu->rvb[0].IIR_DEST_A0=(s16)val;    break;
    case H_Reverb+22  : spu->rvb[0].IIR_DEST_A1=(s16)val;    break;
    case H_Reverb+24  : spu->rvb[0].ACC_SRC_A0=(s16)val;     break;
    case H_Reverb+26  : spu->rvb[0].ACC_SRC_A1=(s16)val;     break;
    case H_Reverb+28  : spu->rvb[0].ACC_SRC_B0=(s16)val;     break;
    case H_Reverb+30  : spu->rvb[0].ACC_SRC_B1=(s16)val;     break;
    case H_Reverb+32  : spu->rvb[0].IIR_SRC_A0=(s16)val;     break;
    case H_Reverb+34  : spu->rvb[0].IIR_SRC_A1=(s16)val;     break;
    case H_Reverb+36  : spu->rvb[0].IIR_DEST_B0=(s16)val;    break;
    case H_Reverb+38  : spu->rvb[0].IIR_DEST_B1=(s16)val;    break;
    case H_Reverb+40  : spu->rvb[0].ACC_SRC_C0=(s16)val;     break;
    case H_Reverb+42  : spu->rvb[0].ACC_SRC_C1=(s16)val;     break;
    case H_Reverb+44  : spu->rvb[0].ACC_SRC_D0=(s16)val;     break;
    case H_Reverb+46  : spu->rvb[0].ACC_SRC_D1=(s16)val;     break;
    case H_Reverb+48  : spu->rvb[0].IIR_SRC_B1=(s16)val;     break;
    case H_Reverb+50  : spu->rvb[0].IIR_SRC_B0=(s16)val;     break;
    case H_Reverb+52  : spu->rvb[0].MIX_DEST_A0=(s16)val;    break;
    case H_Reverb+54  : spu->rvb[0].MIX_DEST_A1=(s16)val;    break;
    case H_Reverb+56  : spu->rvb[0].MIX_DEST_B0=(s16)val;    break;
    case H_Reverb+58  : spu->rvb[0].MIX_DEST_B1=(s16)val;    break;
    case H_Reverb+60  : spu->rvb[0].IN_COEF_L=(s16)val;      break;
    case H_Reverb+62  : spu->rvb[0].IN_COEF_R=(s16)val;      break;
   }
}

EXPORT_GCC unsigned short CALLBACK SPU2readPS1Port(mips_cpu_context *cpu, unsigned long reg)
{
    spu2_state_t *spu = cpu->spu2;
 const u32 r=reg&0xfff;

 if(r>=0x0c00 && r<0x0d80)
  {
  	return SPU2read(cpu, r-0xc00);
  }

 switch(r)
  {
//    case H_SPUctrl:
//     return spuCtrl;
     break;

    case H_SPUstat:
     return spu->spuStat2[0];
     break;
        
    case H_SPUaddr:
     return (u16)(spu->spuAddr2[0]>>2);
     break;

    case H_SPUdata:
     {
      u16 s=BFLIP16(spu->spuMem[spu->spuAddr2[0]]);
      spu->spuAddr2[0]++;
      if(spu->spuAddr2[0]>0xfffff) spu->spuAddr2[0]=0;
      return s;
     }
     break;

    case H_SPUirqAddr:
     return spu->spuIrq2[0]>>2;
     break;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////
// SOUND ON register write
////////////////////////////////////////////////////////////////////////

void SoundOn(spu2_state_t *spu, int start,int end,unsigned short val)     // SOUND ON PSX COMAND
{
 int ch;

 for(ch=start;ch<end;ch++,val>>=1)                     // loop channels
  {
   if((val&1) && spu->s_chan[ch].pStart)                    // mmm... start has to be set before key on !?!
    {
     spu->s_chan[ch].bIgnoreLoop=0;
     spu->s_chan[ch].bNew=1;
     spu->dwNewChannel2[ch/24]|=(1<<(ch%24));               // bitfield for faster testing
    }
  }
}

////////////////////////////////////////////////////////////////////////
// SOUND OFF register write
////////////////////////////////////////////////////////////////////////

void SoundOff(spu2_state_t *spu, int start,int end,unsigned short val)    // SOUND OFF PSX COMMAND
{
 int ch;
 for(ch=start;ch<end;ch++,val>>=1)                     // loop channels
  {
   if(val&1)                                           // && spu->s_chan[i].bOn)  mmm...
    {
     spu->s_chan[ch].bStop=1;
    }                                                  
  }
}

////////////////////////////////////////////////////////////////////////
// FMOD register write
////////////////////////////////////////////////////////////////////////

void FModOn(spu2_state_t *spu, int start,int end,unsigned short val)      // FMOD ON PSX COMMAND
{
 int ch;

 for(ch=start;ch<end;ch++,val>>=1)                     // loop channels
  {
   if(val&1)                                           // -> fmod on/off
    {
     if(ch>0) 
      {
       spu->s_chan[ch].bFMod=1;                             // --> sound channel
       spu->s_chan[ch-1].bFMod=2;                           // --> freq channel
      }
    }
   else
    {
     spu->s_chan[ch].bFMod=0;                               // --> turn off fmod
    }
  }
}

////////////////////////////////////////////////////////////////////////
// NOISE register write
////////////////////////////////////////////////////////////////////////

void NoiseOn(spu2_state_t *spu, int start,int end,unsigned short val)     // NOISE ON PSX COMMAND
{
 int ch;

 for(ch=start;ch<end;ch++,val>>=1)                     // loop channels
  {
   if(val&1)                                           // -> noise on/off
    {
     spu->s_chan[ch].bNoise=1;
    }
   else 
    {
     spu->s_chan[ch].bNoise=0;
    }
  }
}

////////////////////////////////////////////////////////////////////////
// LEFT VOLUME register write
////////////////////////////////////////////////////////////////////////

// please note: sweep and phase invert are wrong... but I've never seen
// them used

void SetVolumeL(spu2_state_t *spu, unsigned char ch,short vol)            // LEFT VOLUME
{
 spu->s_chan[ch].iLeftVolRaw=vol;

 if(vol&0x8000)                                        // sweep?
  {
   short sInc=1;                                       // -> sweep up?
   if(vol&0x2000) sInc=-1;                             // -> or down?
   if(vol&0x1000) vol^=0xffff;                         // -> mmm... phase inverted? have to investigate this
   vol=((vol&0x7f)+1)/2;                               // -> sweep: 0..127 -> 0..64
   vol+=vol/(2*sInc);                                  // -> HACK: we don't sweep right now, so we just raise/lower the volume by the half!
   vol*=128;
  }
 else                                                  // no sweep:
  {
   if(vol&0x4000)                                      // -> mmm... phase inverted? have to investigate this
    //vol^=0xffff;
    vol=0x3fff-(vol&0x3fff);
  }

 vol&=0x3fff;
 spu->s_chan[ch].iLeftVolume=vol;                           // store volume
}

////////////////////////////////////////////////////////////////////////
// RIGHT VOLUME register write
////////////////////////////////////////////////////////////////////////

void SetVolumeR(spu2_state_t *spu, unsigned char ch,short vol)            // RIGHT VOLUME
{
 spu->s_chan[ch].iRightVolRaw=vol;

 if(vol&0x8000)                                        // comments... see above :)
  {
   short sInc=1;
   if(vol&0x2000) sInc=-1;
   if(vol&0x1000) vol^=0xffff;
   vol=((vol&0x7f)+1)/2;        
   vol+=vol/(2*sInc);
   vol*=128;
  }
 else            
  {
   if(vol&0x4000) //vol=vol^=0xffff;
    vol=0x3fff-(vol&0x3fff);
  }

 vol&=0x3fff;
 spu->s_chan[ch].iRightVolume=vol;
}

////////////////////////////////////////////////////////////////////////
// PITCH register write
////////////////////////////////////////////////////////////////////////

void SetPitch(spu2_state_t *spu, int ch,unsigned short val)               // SET PITCH
{
 int NP;
 double intr;

 if(val>0x3fff) NP=0x3fff;                             // get pitch val
 else           NP=val;

 intr = (double)48000.0f / (double)44100.0f * (double)NP;
 NP = (UINT32)intr;

 spu->s_chan[ch].iRawPitch=NP;

 NP=(44100L*NP)/4096L;                                 // calc frequency

 if(NP<1) NP=1;                                        // some security
 spu->s_chan[ch].iActFreq=NP;                               // store frequency
}

////////////////////////////////////////////////////////////////////////
// REVERB register write
////////////////////////////////////////////////////////////////////////

void ReverbOn(spu2_state_t *spu, int start,int end,unsigned short val,int iRight)  // REVERB ON PSX COMMAND
{
 int ch;

 for(ch=start;ch<end;ch++,val>>=1)                     // loop channels
  {
   if(val&1)                                           // -> reverb on/off
    {
     if(iRight) spu->s_chan[ch].bReverbR=1;
     else       spu->s_chan[ch].bReverbL=1;
    }
   else 
    {
     if(iRight) spu->s_chan[ch].bReverbR=0;
     else       spu->s_chan[ch].bReverbL=0;
    }
  }
}

////////////////////////////////////////////////////////////////////////
// REVERB START register write
////////////////////////////////////////////////////////////////////////

void SetReverbAddr(spu2_state_t *spu, int core)
{
 long val=spu->spuRvbAddr2[core];
 
 if(spu->rvb[core].StartAddr!=val)
  {
   if(val<=0x27ff)
    {
     spu->rvb[core].StartAddr=spu->rvb[core].CurrAddr=0;
    }
   else
    {
     spu->rvb[core].StartAddr=val;
     spu->rvb[core].CurrAddr=spu->rvb[core].StartAddr;
    } 
  }
}

////////////////////////////////////////////////////////////////////////
// DRY LEFT/RIGHT per voice switches
////////////////////////////////////////////////////////////////////////

void VolumeOn(spu2_state_t *spu, int start,int end,unsigned short val,int iRight)  // VOLUME ON PSX COMMAND
{
 int ch;

 for(ch=start;ch<end;ch++,val>>=1)                     // loop channels
  {
   if(val&1)                                           // -> reverb on/off
    {
     if(iRight) spu->s_chan[ch].bVolumeR=1;
     else       spu->s_chan[ch].bVolumeL=1;
    }
   else 
    {
     if(iRight) spu->s_chan[ch].bVolumeR=0;
     else       spu->s_chan[ch].bVolumeL=0;
    }
  }
}


