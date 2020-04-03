/***************************************************************************
                            spu.c  -  description
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
// 2005/08/29 - Pete
// - changed to 48Khz output
//
// 2004/12/25 - Pete
// - inc'd version for pcsx2-0.7
//
// 2004/04/18 - Pete
// - changed all kind of things in the plugin
//
// 2004/04/04 - Pete
// - changed plugin to emulate PS2 spu
//
// 2003/04/07 - Eric
// - adjusted cubic interpolation algorithm
//
// 2003/03/16 - Eric
// - added cubic interpolation
//
// 2003/03/01 - linuzappz
// - libraryName changes using ALSA
//
// 2003/02/28 - Pete
// - added option for type of interpolation
// - adjusted spu irqs again (Thousant Arms, Valkyrie Profile)
// - added MONO support for MSWindows DirectSound
//
// 2003/02/20 - kode54
// - amended interpolation code, goto GOON could skip initialization of gpos and cause segfault
//
// 2003/02/19 - kode54
// - moved SPU IRQ handler and changed sample flag processing
//
// 2003/02/18 - kode54
// - moved ADSR calculation outside of the sample decode loop, somehow I doubt that
//   ADSR timing is relative to the frequency at which a sample is played... I guess
//   this remains to be seen, and I don't know whether ADSR is applied to noise channels...
//
// 2003/02/09 - kode54
// - one-shot samples now process the end block before stopping
// - in light of removing fmod hack, now processing ADSR on frequency channel as well
//
// 2003/02/08 - kode54
// - replaced easy interpolation with gaussian
// - removed fmod averaging hack
// - changed .sinc to be updated from .iRawPitch, no idea why it wasn't done this way already (<- Pete: because I sometimes fail to see the obvious, haharhar :)
//
// 2003/02/08 - linuzappz
// - small bugfix for one usleep that was 1 instead of 1000
// - added iDisStereo for no stereo (Linux)
//
// 2003/01/22 - Pete
// - added easy interpolation & small noise adjustments
//
// 2003/01/19 - Pete
// - added Neill's reverb
//
// 2003/01/12 - Pete
// - added recording window handlers
//
// 2003/01/06 - Pete
// - added Neill's ADSR timings
//
// 2002/12/28 - Pete
// - adjusted spu irq handling, fmod handling and loop handling
//
// 2002/08/14 - Pete
// - added extra reverb
//
// 2002/06/08 - linuzappz
// - SPUupdate changed for SPUasync
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

#include "stdafx.h"

#define _IN_SPU

#include "../psx.h"
#include "../peops2/externals.h"
#include "../peops2/regs.h"
#include "../peops2/dma.h"
#include "../peops2/spu.h"
 
////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////


// certain globals (were local before, but with the new timeproc I need em global)

const int f[5][2] = {   {    0,  0  },
                        {   60,  0  },
                        {  115, -52 },
                        {   98, -55 },
                        {  122, -60 } };

////////////////////////////////////////////////////////////////////////
// CODE AREA
////////////////////////////////////////////////////////////////////////

// dirty inline func includes

#include "reverb2.c"        
#include "adsr2.c"

////////////////////////////////////////////////////////////////////////
// helpers for simple interpolation

//
// easy interpolation on upsampling, no special filter, just "Pete's common sense" tm
//
// instead of having n equal sample values in a row like:
//       ____
//           |____
//
// we compare the current delta change with the next delta change.
//
// if curr_delta is positive,
//
//  - and next delta is smaller (or changing direction):
//         \.
//          -__
//
//  - and next delta significant (at least twice) bigger:
//         --_
//            \.
//
//  - and next delta is nearly same:
//          \.
//           \.
//
//
// if curr_delta is negative,
//
//  - and next delta is smaller (or changing direction):
//          _--
//         /
//
//  - and next delta significant (at least twice) bigger:
//            /
//         __- 
//         
//  - and next delta is nearly same:
//           /
//          /
//     


static INLINE void InterpolateUp(spu2_state_t *spu, int ch)
{
 if(spu->s_chan[ch].SB[32]==1)                              // flag == 1? calc step and set flag... and don't change the value in this pass
  {
   const int id1=spu->s_chan[ch].SB[30]-spu->s_chan[ch].SB[29];  // curr delta to next val
   const int id2=spu->s_chan[ch].SB[31]-spu->s_chan[ch].SB[30];  // and next delta to next-next val :)

   spu->s_chan[ch].SB[32]=0;

   if(id1>0)                                           // curr delta positive
    {
     if(id2<id1)
      {spu->s_chan[ch].SB[28]=id1;spu->s_chan[ch].SB[32]=2;}
     else
     if(id2<(id1<<1))
      spu->s_chan[ch].SB[28]=(id1*spu->s_chan[ch].sinc)/0x10000L;
     else
      spu->s_chan[ch].SB[28]=(id1*spu->s_chan[ch].sinc)/0x20000L; 
    }
   else                                                // curr delta negative
    {
     if(id2>id1)
      {spu->s_chan[ch].SB[28]=id1;spu->s_chan[ch].SB[32]=2;}
     else
     if(id2>(id1<<1))
      spu->s_chan[ch].SB[28]=(id1*spu->s_chan[ch].sinc)/0x10000L;
     else
      spu->s_chan[ch].SB[28]=(id1*spu->s_chan[ch].sinc)/0x20000L; 
    }
  }
 else
 if(spu->s_chan[ch].SB[32]==2)                              // flag 1: calc step and set flag... and don't change the value in this pass
  {
   spu->s_chan[ch].SB[32]=0;

   spu->s_chan[ch].SB[28]=(spu->s_chan[ch].SB[28]*spu->s_chan[ch].sinc)/0x20000L;
   if(spu->s_chan[ch].sinc<=0x8000)
        spu->s_chan[ch].SB[29]=spu->s_chan[ch].SB[30]-(spu->s_chan[ch].SB[28]*((0x10000/spu->s_chan[ch].sinc)-1));
   else spu->s_chan[ch].SB[29]+=spu->s_chan[ch].SB[28];
  }
 else                                                  // no flags? add bigger val (if possible), calc smaller step, set flag1
  spu->s_chan[ch].SB[29]+=spu->s_chan[ch].SB[28];
}

//
// even easier interpolation on downsampling, also no special filter, again just "Pete's common sense" tm
//

static INLINE void InterpolateDown(spu2_state_t *spu, int ch)
{
 if(spu->s_chan[ch].sinc>=0x20000L)                                 // we would skip at least one val?
  {
   spu->s_chan[ch].SB[29]+=(spu->s_chan[ch].SB[30]-spu->s_chan[ch].SB[29])/2; // add easy weight
   if(spu->s_chan[ch].sinc>=0x30000L)                               // we would skip even more vals?
    spu->s_chan[ch].SB[29]+=(spu->s_chan[ch].SB[31]-spu->s_chan[ch].SB[30])/2;// add additional next weight
  }
}

////////////////////////////////////////////////////////////////////////
// helpers for gauss interpolation

#define gval0 (((short*)(&spu->s_chan[ch].SB[29]))[gpos])
#define gval(x) (((short*)(&spu->s_chan[ch].SB[29]))[(gpos+x)&3])

#include "gauss_i.h"

////////////////////////////////////////////////////////////////////////

//#include "xa.c"

////////////////////////////////////////////////////////////////////////
// START SOUND... called by main thread to setup a new sound on a channel
////////////////////////////////////////////////////////////////////////

static INLINE void StartSound(spu2_state_t *spu, int ch)
{
 spu->dwNewChannel2[ch/24]&=~(1<<(ch%24));                  // clear new channel bit
 spu->dwEndChannel2[ch/24]&=~(1<<(ch%24));                  // clear end channel bit

 StartADSR(spu, ch);
 StartREVERB(spu, ch);      
                          
 spu->s_chan[ch].pCurr=spu->s_chan[ch].pStart;                   // set sample start
                         
 spu->s_chan[ch].s_1=0;                                     // init mixing vars
 spu->s_chan[ch].s_2=0;
 spu->s_chan[ch].iSBPos=28;

 spu->s_chan[ch].bNew=0;                                    // init channel flags
 spu->s_chan[ch].bStop=0;                                   
 spu->s_chan[ch].bOn=1;

 spu->s_chan[ch].SB[29]=0;                                  // init our interpolation helpers
 spu->s_chan[ch].SB[30]=0;

 if(spu->iUseInterpolation>=2)                              // gauss interpolation?
      {spu->s_chan[ch].spos=0x30000L;spu->s_chan[ch].SB[28]=0;}  // -> start with more decoding
 else {spu->s_chan[ch].spos=0x10000L;spu->s_chan[ch].SB[31]=0;}  // -> no/simple interpolation starts with one 44100 decoding
}

////////////////////////////////////////////////////////////////////////
// MAIN SPU FUNCTION
// here is the main job handler... thread, timer or direct func call
// basically the whole sound processing is done in this fat func!
////////////////////////////////////////////////////////////////////////

// Counting to 65536 results in full volume offage.
void setlength2(spu2_state_t *spu, s32 stop, s32 fade)
{
 if(stop==~0)
 {
  spu->decaybegin=~0;
 }
 else
 {
  stop=(stop*441)/10;
  fade=(fade*441)/10;

  spu->decaybegin=stop;
  spu->decayend=stop+fade;
 }
}
// 5 ms waiting phase, if buffer is full and no new sound has to get started
// .. can be made smaller (smallest val: 1 ms), but bigger waits give
// better performance

#define PAUSE_W 5
#define PAUSE_L 5000

////////////////////////////////////////////////////////////////////////

static void *MAINThread(mips_cpu_context *cpu, int samp2run)
{
    spu2_state_t *spu = cpu->spu2;
 int s_1,s_2,fa,voldiv=spu->iVolume;
 unsigned char * start;unsigned int nSample;
 int ch,predict_nr,shift_factor,flags,d,d2,s;
 int gpos,bIRQReturn=0;

// while(!bEndThread)                                    // until we are shutting down
  {
   //--------------------------------------------------//
   // ok, at the beginning we are looking if there is
   // enuff free place in the dsound/oss buffer to
   // fill in new data, or if there is a new channel to start.
   // if not, we wait (thread) or return (timer/spuasync)
   // until enuff free place is available/a new channel gets
   // started

   if(spu->dwNewChannel2[0] || spu->dwNewChannel2[1])            // new channel should start immedately?
    {                                                  // (at least one bit 0 ... MAXCHANNEL is set?)
     spu->iSecureStart++;                                   // -> set iSecure
     if(spu->iSecureStart>5) spu->iSecureStart=0;                //    (if it is set 5 times - that means on 5 tries a new samples has been started - in a row, we will reset it, to give the sound update a chance)
    }
   else spu->iSecureStart=0;                                // 0: no new channel should start

/*	if (!iSecureStart)
	{
	     iSecureStart=0;                                   // reset secure
	     return;
	}*/

#if 0
   while(!iSecureStart && !bEndThread) // &&               // no new start? no thread end?
//         (SoundGetBytesBuffered()>TESTSIZE))           // and still enuff data in sound buffer?
    {
     iSecureStart=0;                                   // reset secure

     if(iUseTimer) return 0;                           // linux no-thread mode? bye

     if(dwNewChannel2[0] || dwNewChannel2[1]) 
      iSecureStart=1;                                  // if a new channel kicks in (or, of course, sound buffer runs low), we will leave the loop
    }
#endif

   //--------------------------------------------------// continue from irq handling in timer mode? 

   if(spu->lastch>=0)                                       // will be -1 if no continue is pending
    {
     ch=spu->lastch; spu->lastch=-1;                  // -> setup all kind of vars to continue
     goto GOON;                                        // -> directly jump to the continue point
    }

   //--------------------------------------------------//
   //- main channel loop                              -// 
   //--------------------------------------------------//
    {
     for(ch=0;ch<MAXCHAN;ch++)                         // loop em all... we will collect 1 ms of sound of each playing channel
      {
       if(spu->s_chan[ch].bNew) StartSound(spu, ch);             // start new sound
       if(!spu->s_chan[ch].bOn) continue;                   // channel not playing? next

       if(spu->s_chan[ch].iActFreq!=spu->s_chan[ch].iUsedFreq)   // new psx frequency?
        {
         spu->s_chan[ch].iUsedFreq=spu->s_chan[ch].iActFreq;     // -> take it and calc steps
         spu->s_chan[ch].sinc=spu->s_chan[ch].iRawPitch<<4;
         if(!spu->s_chan[ch].sinc) spu->s_chan[ch].sinc=1;
         if(spu->iUseInterpolation==1) spu->s_chan[ch].SB[32]=1; // -> freq change in simle imterpolation mode: set flag
        }
//       ns=0;
//       while(ns<NSSIZE)                                // loop until 1 ms of data is reached
        {
         while(spu->s_chan[ch].spos>=0x10000L)
          {
           if(spu->s_chan[ch].iSBPos==28)                   // 28 reached?
            {
             start=spu->s_chan[ch].pCurr;                   // set up the current pos

             if (start == (unsigned char*)-1)          // special "stop" sign
              {
               spu->s_chan[ch].bOn=0;                       // -> turn everything off
               spu->s_chan[ch].ADSRX.lVolume=0;
               spu->s_chan[ch].ADSRX.EnvelopeVol=0;
               goto ENDX;                              // -> and done for this channel
              }

             spu->s_chan[ch].iSBPos=0;

             //////////////////////////////////////////// spu irq handler here? mmm... do it later

             s_1=spu->s_chan[ch].s_1;
             s_2=spu->s_chan[ch].s_2;

             predict_nr=(int)*start;start++;
             shift_factor=predict_nr&0xf;
             predict_nr >>= 4;
             flags=(int)*start;start++;

             // -------------------------------------- // 

             for (nSample=0;nSample<28;start++)      
              {
               d=(int)*start;
               s=((d&0xf)<<12);
               if(s&0x8000) s|=0xffff0000;

               fa=(s >> shift_factor);
               fa=fa + ((s_1 * f[predict_nr][0])>>6) + ((s_2 * f[predict_nr][1])>>6);
               s_2=s_1;s_1=fa;
               s=((d & 0xf0) << 8);

               spu->s_chan[ch].SB[nSample++]=fa;

               if(s&0x8000) s|=0xffff0000;
               fa=(s>>shift_factor);              
               fa=fa + ((s_1 * f[predict_nr][0])>>6) + ((s_2 * f[predict_nr][1])>>6);
               s_2=s_1;s_1=fa;

               spu->s_chan[ch].SB[nSample++]=fa;
              }     

             //////////////////////////////////////////// irq check

             if(spu->spuCtrl2[ch/24]&0x40)                  // some irq active?
              {
               if((spu->pSpuIrq[ch/24] >  start-16 &&       // irq address reached?
                   spu->pSpuIrq[ch/24] <= start) ||
                  ((flags&1) &&                        // special: irq on looping addr, when stop/loop flag is set 
                   (spu->pSpuIrq[ch/24] >  spu->s_chan[ch].pLoop-16 &&
                    spu->pSpuIrq[ch/24] <= spu->s_chan[ch].pLoop)))
                {
                 spu->s_chan[ch].iIrqDone=1;                // -> debug flag

                 if(spu->irqCallback) spu->irqCallback();        // -> call main emu (not supported in SPU2 right now)
                 else
                  {
                   if(ch<24) InterruptDMA4(cpu);            // -> let's see what is happening if we call our irqs instead ;)
                   else      InterruptDMA7(cpu);
                  }
                  
                 if(spu->iSPUIRQWait)                       // -> option: wait after irq for main emu
                  {
                   spu->iSpuAsyncWait=1;
                   bIRQReturn=1;
                  }
                }
              }
      
             //////////////////////////////////////////// flag handler

             if((flags&4) && (!spu->s_chan[ch].bIgnoreLoop))
              spu->s_chan[ch].pLoop=start-16;               // loop adress

             if(flags&1)                               // 1: stop/loop
              {
               spu->dwEndChannel2[ch/24]|=(1<<(ch%24));
              
               // We play this block out first...
               //if(!(flags&2)|| spu->s_chan[ch].pLoop==NULL)   
                                                       // 1+2: do loop... otherwise: stop
               if(flags!=3 || spu->s_chan[ch].pLoop==NULL)  // PETE: if we don't check exactly for 3, loop hang ups will happen (DQ4, for example)
                {                                      // and checking if pLoop is set avoids crashes, yeah
                 start = (unsigned char*)-1;
                }
               else
                {
                 start = spu->s_chan[ch].pLoop;
                }
              }

             spu->s_chan[ch].pCurr=start;                   // store values for next cycle
             spu->s_chan[ch].s_1=s_1;
             spu->s_chan[ch].s_2=s_2;      

             ////////////////////////////////////////////

             if(bIRQReturn)                            // special return for "spu irq - wait for cpu action"
              {
               bIRQReturn=0;
                {
                 spu->lastch=ch; 
//                 lastns=ns;	// changemeback

                 return 0;
                }
              }

             ////////////////////////////////////////////

GOON: ;

            }

           fa=spu->s_chan[ch].SB[spu->s_chan[ch].iSBPos++];      // get sample data

//           if((spu->spuCtrl2[ch/24]&0x4000)==0) fa=0;       // muted?
//           else                                        // else adjust
            {
             if(fa>32767L)  fa=32767L;
             if(fa<-32767L) fa=-32767L;              
            }

           if(spu->iUseInterpolation>=2)                    // gauss/cubic interpolation
            {
             gpos = spu->s_chan[ch].SB[28];
             gval0 = fa;          
             gpos = (gpos+1) & 3;
             spu->s_chan[ch].SB[28] = gpos;
            }
           else
           if(spu->iUseInterpolation==1)                    // simple interpolation
            {
             spu->s_chan[ch].SB[28] = 0;                    
             spu->s_chan[ch].SB[29] = spu->s_chan[ch].SB[30];    // -> helpers for simple linear interpolation: delay real val for two slots, and calc the two deltas, for a 'look at the future behaviour'
             spu->s_chan[ch].SB[30] = spu->s_chan[ch].SB[31];
             spu->s_chan[ch].SB[31] = fa;
             spu->s_chan[ch].SB[32] = 1;                    // -> flag: calc new interolation
            }
           else spu->s_chan[ch].SB[29]=fa;                  // no interpolation

           spu->s_chan[ch].spos -= 0x10000L;
          }

         ////////////////////////////////////////////////
         // noise handler... just produces some noise data
         // surely wrong... and no noise frequency (spuCtrl&0x3f00) will be used...
         // and sometimes the noise will be used as fmod modulation... pfff

         if(spu->s_chan[ch].bNoise)
          {
           if((spu->dwNoiseVal<<=1)&0x80000000L)
            {
             spu->dwNoiseVal^=0x0040001L;
             fa=((spu->dwNoiseVal>>2)&0x7fff);
             fa=-fa;
            }
           else fa=(spu->dwNoiseVal>>2)&0x7fff;

           // mmm... depending on the noise freq we allow bigger/smaller changes to the previous val
           fa=spu->s_chan[ch].iOldNoise+((fa-spu->s_chan[ch].iOldNoise)/((0x001f-((spu->spuCtrl2[ch/24]&0x3f00)>>9))+1));
           if(fa>32767L)  fa=32767L;
           if(fa<-32767L) fa=-32767L;              
           spu->s_chan[ch].iOldNoise=fa;

           if(spu->iUseInterpolation<2)                     // no gauss/cubic interpolation?
            spu->s_chan[ch].SB[29] = fa;                    // -> store noise val in "current sample" slot
          }                                            //----------------------------------------
         else                                          // NO NOISE (NORMAL SAMPLE DATA) HERE 
          {//------------------------------------------//
           if(spu->iUseInterpolation==3)                    // cubic interpolation
            {
             long xd;  
             xd = ((spu->s_chan[ch].spos) >> 1)+1;
             gpos = spu->s_chan[ch].SB[28];

             fa  = gval(3) - 3*gval(2) + 3*gval(1) - gval0;
             fa *= (xd - (2<<15)) / 6;
             fa >>= 15;
             fa += gval(2) - gval(1) - gval(1) + gval0;
             fa *= (xd - (1<<15)) >> 1;
             fa >>= 15;
             fa += gval(1) - gval0;
             fa *= xd;
             fa >>= 15;
             fa = fa + gval0;
            }
           //------------------------------------------//
           else
           if(spu->iUseInterpolation==2)                    // gauss interpolation
            {
             int vl, vr;
             vl = (spu->s_chan[ch].spos >> 6) & ~3;
             gpos = spu->s_chan[ch].SB[28];
             vr=(gauss[vl]*gval0)&~2047;
             vr+=(gauss[vl+1]*gval(1))&~2047;
             vr+=(gauss[vl+2]*gval(2))&~2047;
             vr+=(gauss[vl+3]*gval(3))&~2047;
             fa = vr>>11;
/*
             vr=(gauss[vl]*gval0)>>9;
             vr+=(gauss[vl+1]*gval(1))>>9;
             vr+=(gauss[vl+2]*gval(2))>>9;
             vr+=(gauss[vl+3]*gval(3))>>9;
             fa = vr>>2;
*/
            }
           //------------------------------------------//
           else
           if(spu->iUseInterpolation==1)                    // simple interpolation
            {
             if(spu->s_chan[ch].sinc<0x10000L)              // -> upsampling?
                  InterpolateUp(spu, ch);                   // --> interpolate up
             else InterpolateDown(spu, ch);                 // --> else down
             fa=spu->s_chan[ch].SB[29];
            }
           //------------------------------------------//
           else fa=spu->s_chan[ch].SB[29];                  // no interpolation
          }

         spu->s_chan[ch].sval = (MixADSR(spu, ch) * fa) / 1023;  // add adsr

         if(spu->s_chan[ch].bFMod==2)                       // fmod freq channel
          {
           int NP=spu->s_chan[ch+1].iRawPitch;
           double intr;

           NP=((32768L+spu->s_chan[ch].sval)*NP)/32768L;    // mmm... I still need to adjust that to 1/48 khz... we will wait for the first game/demo using it to decide how to do it :)

           if(NP>0x3fff) NP=0x3fff;
           if(NP<0x1)    NP=0x1;
           
	   intr = (double)48000.0f / (double)44100.0f * (double)NP;
           NP = (UINT32)intr;

           NP=(44100L*NP)/(4096L);                     // calc frequency

           spu->s_chan[ch+1].iActFreq=NP;
           spu->s_chan[ch+1].iUsedFreq=NP;
           spu->s_chan[ch+1].sinc=(((NP/10)<<16)/4410);
           if(!spu->s_chan[ch+1].sinc) spu->s_chan[ch+1].sinc=1;
           if(spu->iUseInterpolation==1)                    // freq change in sipmle interpolation mode
            spu->s_chan[ch+1].SB[32]=1;

// mmmm... set up freq decoding positions?
//           spu->s_chan[ch+1].iSBPos=28;
//           spu->s_chan[ch+1].spos=0x10000L;
          }                    
         else                   
          {                                          
           //////////////////////////////////////////////
           // ok, left/right sound volume (psx volume goes from 0 ... 0x3fff)

           if(spu->s_chan[ch].iMute) 
            spu->s_chan[ch].sval=0;                         // debug mute
           else
            {
             if(spu->s_chan[ch].bVolumeL)
              spu->SSumL[0]+=(spu->s_chan[ch].sval*spu->s_chan[ch].iLeftVolume)/0x4000L;
             if(spu->s_chan[ch].bVolumeR)
              spu->SSumR[0]+=(spu->s_chan[ch].sval*spu->s_chan[ch].iRightVolume)/0x4000L;
            }
        
           //////////////////////////////////////////////
           // now let us store sound data for reverb    
                                                          
           if(spu->s_chan[ch].bRVBActive) StoreREVERB(spu, ch,0);
          }

         ////////////////////////////////////////////////
         // ok, go on until 1 ms data of this channel is collected
                                                       
         spu->s_chan[ch].spos += spu->s_chan[ch].sinc;             
                                                              
        }        
ENDX:   ;                                                      
      }
    }                                                         
   
  //---------------------------------------------------//
  //- here we have another 1 ms of sound data
  //---------------------------------------------------//

  ///////////////////////////////////////////////////////
  // mix all channels (including reverb) into one buffer

    spu->SSumL[0]+=MixREVERBLeft(spu, 0,0);
    spu->SSumL[0]+=MixREVERBLeft(spu, 0,1);
    spu->SSumR[0]+=MixREVERBRight(spu, 0);
    spu->SSumR[0]+=MixREVERBRight(spu, 1);
                                              
    d=spu->SSumL[0]/voldiv;spu->SSumL[0]=0;
    d2=spu->SSumR[0]/voldiv;spu->SSumR[0]=0;

    if(d<-32767) d=-32767;if(d>32767) d=32767;
    if(d2<-32767) d2=-32767;if(d2>32767) d2=32767;

    if(spu->sampcount>=spu->decaybegin)
    {
	s32 dmul;
	if(spu->decaybegin!=~0) // Is anyone REALLY going to be playing a song
		      // for 13 hours?
    	{
		if(spu->sampcount>=spu->decayend) 
		{
//		       	ao_song_done = 1;
		        return(0);
		}

		dmul=256-(256*(spu->sampcount-spu->decaybegin)/(spu->decayend-spu->decaybegin));
		d=(d*dmul)>>8;
		d2=(d2*dmul)>>8;
	}
    }
    spu->sampcount++;

    *spu->pS++=d;
    *spu->pS++=d2;

    InitREVERB(spu);

  //////////////////////////////////////////////////////                   
  // feed the sound
  // wanna have around 1/60 sec (16.666 ms) updates
	if ((((unsigned char *)spu->pS)-((unsigned char *)spu->pSpuBuffer)) == (735*4))
	{
	    	cpu->spu_callback((u8*)spu->pSpuBuffer,(u8*)spu->pS-(u8*)spu->pSpuBuffer, cpu->spu_callback_data);
	        spu->pS=(short *)spu->pSpuBuffer;					  
	}
 }

 // end of big main loop...

 spu->bThreadEnded=1;

 return 0;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SPU ASYNC... even newer epsxe func
//  1 time every 'cycle' cycles... harhar
////////////////////////////////////////////////////////////////////////

EXPORT_GCC void CALLBACK SPU2async(mips_cpu_context *cpu, unsigned long cycle)
{
    spu2_state_t *spu = cpu->spu2;
 if(spu->iSpuAsyncWait)
  {
   spu->iSpuAsyncWait++;
   if(spu->iSpuAsyncWait<=64) return;
   spu->iSpuAsyncWait=0;
  }

   MAINThread(cpu, 0);                                      // -> linux high-compat mode
}

////////////////////////////////////////////////////////////////////////
// INIT/EXIT STUFF
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SPUINIT: this func will be called first by the main emu
////////////////////////////////////////////////////////////////////////

              
EXPORT_GCC long CALLBACK SPU2init(mips_cpu_context *cpu, void (*callback)(unsigned char *, long, void *), void *data)
{
    cpu->spu2 = malloc (sizeof (spu2_state_t));
    memset (cpu->spu2, 0, sizeof (spu2_state_t));
    cpu->spu2->iUseXA=0;
    cpu->spu2->iVolume=3;
    cpu->spu2->iXAPitch=1;
    cpu->spu2->iUseTimer=2;
    cpu->spu2->iSPUIRQWait=1;
    cpu->spu2->iDebugMode=0;
    cpu->spu2->iRecordMode=0;
    cpu->spu2->iUseReverb=1;
    cpu->spu2->iUseInterpolation=2;
    cpu->spu2->dwNoiseVal=1;                          // global noise generator
    cpu->spu2->lastch=-1;

    cpu->spu_callback = callback;
    cpu->spu_callback_data = data;
 cpu->spu2->spuMemC=(unsigned char *)cpu->spu2->spuMem;                      // just small setup
 memset((void *)cpu->spu2->s_chan,0,MAXCHAN*sizeof(SPUCHAN));
 memset(cpu->spu2->rvb,0,2*sizeof(REVERBInfo));

 cpu->spu2->sampcount = 0;

 InitADSR();

 return 0;
}

EXPORT_GCC void CALLBACK SPU2free (mips_cpu_context *cpu) {
    if (cpu->spu2) {
        free (cpu->spu2);
        cpu->spu2 = NULL;
    }
}

////////////////////////////////////////////////////////////////////////
// SETUPTIMER: init of certain buffers and threads/timers
////////////////////////////////////////////////////////////////////////

static void SetupTimer(mips_cpu_context *cpu)
{
    spu2_state_t *spu = cpu->spu2;
 memset(spu->SSumR,0,NSSIZE*sizeof(int));                   // init some mixing buffers
 memset(spu->SSumL,0,NSSIZE*sizeof(int));
 spu->pS=(short *)spu->pSpuBuffer;                               // setup soundbuffer pointer

 spu->bEndThread=0;                                         // init thread vars
 spu->bThreadEnded=0; 
 spu->bSpuInit=1;                                           // flag: we are inited
}

////////////////////////////////////////////////////////////////////////
// REMOVETIMER: kill threads/timers
////////////////////////////////////////////////////////////////////////

static void RemoveTimer(mips_cpu_context *cpu)
{
    spu2_state_t *spu = cpu->spu2;
 spu->bEndThread=1;                                         // raise flag to end thread
 spu->bThreadEnded=0;                                       // no more spu is running
 spu->bSpuInit=0;
}

////////////////////////////////////////////////////////////////////////
// SETUPSTREAMS: init most of the spu buffers
////////////////////////////////////////////////////////////////////////

static void SetupStreams(mips_cpu_context *cpu)
{ 
    spu2_state_t *spu = cpu->spu2;
 int i;

 spu->pSpuBuffer=(unsigned char *)malloc(32768);            // alloc mixing buffer

 i=NSSIZE*2;

 spu->sRVBStart[0] = (int *)malloc(i*4);                    // alloc reverb buffer
 memset(spu->sRVBStart[0],0,i*4);
 spu->sRVBEnd[0]  = spu->sRVBStart[0] + i;
 spu->sRVBPlay[0] = spu->sRVBStart[0];
 spu->sRVBStart[1] = (int *)malloc(i*4);                    // alloc reverb buffer
 memset(spu->sRVBStart[1],0,i*4);
 spu->sRVBEnd[1]  = spu->sRVBStart[1] + i;
 spu->sRVBPlay[1] = spu->sRVBStart[1];

 for(i=0;i<MAXCHAN;i++)                                // loop sound channels
  {
// we don't use mutex sync... not needed, would only 
// slow us down:
//   spu->s_chan[i].hMutex=CreateMutex(NULL,FALSE,NULL);
   spu->s_chan[i].ADSRX.SustainLevel = 1024;                // -> init sustain
   spu->s_chan[i].iMute=0;
   spu->s_chan[i].iIrqDone=0;
   spu->s_chan[i].pLoop=spu->spuMemC;
   spu->s_chan[i].pStart=spu->spuMemC;
   spu->s_chan[i].pCurr=spu->spuMemC;
  }
}

////////////////////////////////////////////////////////////////////////
// REMOVESTREAMS: free most buffer
////////////////////////////////////////////////////////////////////////

static void RemoveStreams(mips_cpu_context *cpu)
{ 
    spu2_state_t *spu = cpu->spu2;
 free(spu->pSpuBuffer);                                     // free mixing buffer
 spu->pSpuBuffer=NULL;
 free(spu->sRVBStart[0]);                                   // free reverb buffer
 spu->sRVBStart[0]=0;
 free(spu->sRVBStart[1]);                                   // free reverb buffer
 spu->sRVBStart[1]=0;

/*
 int i;
 for(i=0;i<MAXCHAN;i++)
  {
   WaitForSingleObject(spu->s_chan[i].hMutex,2000);
   ReleaseMutex(spu->s_chan[i].hMutex);
   if(spu->s_chan[i].hMutex)    
    {CloseHandle(spu->s_chan[i].hMutex);spu->s_chan[i].hMutex=0;}
  }
*/
}


////////////////////////////////////////////////////////////////////////
// SPUOPEN: called by main emu after init
////////////////////////////////////////////////////////////////////////
   
EXPORT_GCC long CALLBACK SPU2open(mips_cpu_context *cpu, void *pDsp)                          
{
    spu2_state_t *spu = cpu->spu2;
 if(spu->bSPUIsOpen) return 0;                              // security for some stupid main emus

 spu->iUseXA=0;                                             // just small setup
 spu->iVolume=3;
 spu->bEndThread=0;
 spu->bThreadEnded=0;
 spu->spuMemC=(unsigned char *)spu->spuMem;      
 memset((void *)spu->s_chan,0,(MAXCHAN+1)*sizeof(SPUCHAN));
 spu->pSpuIrq[0]=0;
 spu->pSpuIrq[1]=0;
 spu->iSPUIRQWait=1;
 spu->dwNewChannel2[0]=0;
 spu->dwNewChannel2[1]=0;
 spu->dwEndChannel2[0]=0;
 spu->dwEndChannel2[1]=0;
 spu->spuCtrl2[0]=0;              
 spu->spuCtrl2[1]=0;              
 spu->spuStat2[0]=0;
 spu->spuStat2[1]=0;
 spu->spuIrq2[0]=0;             
 spu->spuIrq2[1]=0;             
 spu->spuAddr2[0]=0xffffffff;   
 spu->spuAddr2[1]=0xffffffff;   
 spu->spuRvbAddr2[0]=0;
 spu->spuRvbAddr2[1]=0;
 spu->spuRvbAEnd2[0]=0;
 spu->spuRvbAEnd2[1]=0;
 
// ReadConfig();                                         // read user stuff
 
// SetupSound();                                         // setup midas (before init!)

 SetupStreams(cpu);                                       // prepare streaming

 SetupTimer(cpu);                                         // timer for feeding data

 spu->bSPUIsOpen=1;

 return 0;        
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SPUCLOSE: called before shutdown
////////////////////////////////////////////////////////////////////////

EXPORT_GCC void CALLBACK SPU2close(mips_cpu_context *cpu)
{
    spu2_state_t *spu = cpu->spu2;
 if(!spu->bSPUIsOpen) return;                               // some security

 spu->bSPUIsOpen=0;                                         // no more open

 RemoveTimer(cpu);                                        // no more feeding

// RemoveSound(cpu);                                        // no more sound handling

 RemoveStreams(cpu);                                      // no more streaming
}

////////////////////////////////////////////////////////////////////////
// SPUSHUTDOWN: called by main emu on final exit
////////////////////////////////////////////////////////////////////////

EXPORT_GCC void CALLBACK SPU2shutdown(void)
{
 return;
}

////////////////////////////////////////////////////////////////////////
// SPUTEST: we don't test, we are always fine ;)
////////////////////////////////////////////////////////////////////////

EXPORT_GCC long CALLBACK SPU2test(void)
{
 return 0;
}

////////////////////////////////////////////////////////////////////////
// SETUP CALLBACKS
// this functions will be called once, 
// passes a callback that should be called on SPU-IRQ/cdda volume change
////////////////////////////////////////////////////////////////////////

#if 0
// not used yet
EXPORT_GCC void CALLBACK SPU2irqCallback(void (CALLBACK *callback)(void))
{
 irqCallback = callback;
}

// not used yet
EXPORT_GCC void CALLBACK SPU2registerCallback(void (CALLBACK *callback)(void))
{
 irqCallback = callback;
}

// not used yet
EXPORT_GCC void CALLBACK SPU2registerCDDAVolume(void (CALLBACK *CDDAVcallback)(unsigned short,unsigned short))
{
 cddavCallback = CDDAVcallback;
}
#endif
