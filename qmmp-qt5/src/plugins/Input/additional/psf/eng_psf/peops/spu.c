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

#define _IN_SPU

#include "../peops/stdafx.h"
#include "../peops/externals.h"
#include "../peops/regs.h"
#include "../peops/registers.h"
#include "../peops/spu.h"

void SPUirq(void) ;

//#include "PsxMem.h"
//#include "driver.h"

////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////
static const int f[5][2] = {   
    {    0,  0  },
    {   60,  0  },
    {  115, -52 },
    {   98, -55 },
    {  122, -60 } };

////////////////////////////////////////////////////////////////////////
// CODE AREA
////////////////////////////////////////////////////////////////////////

// dirty inline func includes

#include "ao.h"

#include "../peops/reverb.c"        
#include "../peops/adsr.c"

// Try this to increase speed.
#include "../peops/registers.c"
#include "../peops/dma.c"

////////////////////////////////////////////////////////////////////////
// helpers for so-called "gauss interpolation"

#define gval0 (((int *)(&spu->s_chan[ch].SB[29]))[gpos])
#define gval(x) (((int *)(&spu->s_chan[ch].SB[29]))[(gpos+x)&3])

#include "gauss_i.h"

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// START SOUND... called by main thread to setup a new sound on a channel
////////////////////////////////////////////////////////////////////////

static INLINE void StartSound(spu_state_t *spu, int ch)
{
 StartADSR(spu, ch);

 spu->s_chan[ch].pCurr=spu->s_chan[ch].pStart;                   // set sample start
                         
 spu->s_chan[ch].s_1=0;                                     // init mixing vars
 spu->s_chan[ch].s_2=0;
 spu->s_chan[ch].iSBPos=28;

 spu->s_chan[ch].bNew=0;                                    // init channel flags
 spu->s_chan[ch].bStop=0;                                   
 spu->s_chan[ch].bOn=1;

 spu->s_chan[ch].SB[29]=0;                                  // init our interpolation helpers
 spu->s_chan[ch].SB[30]=0;

 spu->s_chan[ch].spos=0x40000L;spu->s_chan[ch].SB[28]=0;  // -> start with more decoding
}

////////////////////////////////////////////////////////////////////////
// MAIN SPU FUNCTION
// here is the main job handler... thread, timer or direct func call
// basically the whole sound processing is done in this fat func!
////////////////////////////////////////////////////////////////////////

// Counting to 65536 results in full volume offage.
void setlength(spu_state_t *spu, s32 stop, s32 fade)
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

#define CLIP(_x) {if(_x>32767) _x=32767; if(_x<-32767) _x=-32767;}
int SPUasync(mips_cpu_context *cpu, u32 cycles)
{
    spu_state_t *spu = cpu->spu;
 int volmul=spu->iVolume;
 s32 temp;

 spu->ttemp+=cycles;
 spu->dosampies=spu->ttemp/384;
 if(!spu->dosampies) return(1);
 spu->ttemp-=spu->dosampies*384;
 temp=spu->dosampies;

 while(temp)
 {
   s32 revLeft=0, revRight=0;
   s32 sl=0, sr=0;
   int ch,fa;

   temp--;
   //--------------------------------------------------//
   //- main channel loop                              -// 
   //--------------------------------------------------//
    {
     for(ch=0;ch<MAXCHAN;ch++)                         // loop em all.
      {
       if(spu->s_chan[ch].bNew) StartSound(spu, ch);             // start new sound
       if(!spu->s_chan[ch].bOn) continue;                   // channel not playing? next


       if(spu->s_chan[ch].iActFreq!=spu->s_chan[ch].iUsedFreq)   // new psx frequency?
        {
         spu->s_chan[ch].iUsedFreq=spu->s_chan[ch].iActFreq;     // -> take it and calc steps
         spu->s_chan[ch].sinc=spu->s_chan[ch].iRawPitch<<4;
         if(!spu->s_chan[ch].sinc) spu->s_chan[ch].sinc=1;
        }

         while(spu->s_chan[ch].spos>=0x10000L)
          {
           if(spu->s_chan[ch].iSBPos==28)                   // 28 reached?
            {
	     int predict_nr,shift_factor,flags,d,s;
	     u8* start;unsigned int nSample;
	     int s_1,s_2;

             start=spu->s_chan[ch].pCurr;                   // set up the current pos

             if (start == (u8*)-1)          // special "stop" sign
              {
               spu->s_chan[ch].bOn=0;                       // -> turn everything off
               spu->s_chan[ch].ADSRX.lVolume=0;
               spu->s_chan[ch].ADSRX.EnvelopeVol=0;
               goto ENDX;                              // -> and done for this channel
              }

             spu->s_chan[ch].iSBPos=0;	// Reset buffer play index.

             //////////////////////////////////////////// spu irq handler here? mmm... do it later

             s_1=spu->s_chan[ch].s_1;
             s_2=spu->s_chan[ch].s_2;

             predict_nr=(int)*start;start++;           
             shift_factor=predict_nr&0xf;
             predict_nr >>= 4;
             flags=(int)*start;start++;

             // -------------------------------------- // 
	     // Decode new samples into spu->s_chan[ch].SB[0 through 27]
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

             if(spu->spuCtrl&0x40)         			// irq active?
              {
               if((spu->pSpuIrq >  start-16 &&              // irq address reached?
                   spu->pSpuIrq <= start) ||
                  ((flags&1) &&                        // special: irq on looping addr, when stop/loop flag is set 
                   (spu->pSpuIrq >  spu->s_chan[ch].pLoop-16 && 
                    spu->pSpuIrq <= spu->s_chan[ch].pLoop)))
               {
		 //extern s32 spuirqvoodoo;
                 spu->s_chan[ch].iIrqDone=1;                // -> debug flag
		 SPUirq();
		//puts("IRQ");
		 //if(spuirqvoodoo!=-1)
		 //{
		 // spuirqvoodoo=temp*384;
		 // temp=0;
		 //}
                }
              }
      
             //////////////////////////////////////////// flag handler

             if((flags&4) && (!spu->s_chan[ch].bIgnoreLoop))
              spu->s_chan[ch].pLoop=start-16;               // loop adress

             if(flags&1)                               // 1: stop/loop
              {
               // We play this block out first...
               //if(!(flags&2))                          // 1+2: do loop... otherwise: stop
               if(flags!=3 || spu->s_chan[ch].pLoop==NULL)  // PETE: if we don't check exactly for 3, loop hang ups will happen (DQ4, for example)
                {                                      // and checking if pLoop is set avoids crashes, yeah
                 start = (u8*)-1;
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
            }

           fa=spu->s_chan[ch].SB[spu->s_chan[ch].iSBPos++];      // get sample data

           if((spu->spuCtrl&0x4000)==0) fa=0;               // muted?
	   else CLIP(fa);

	    {
	     int gpos;
             gpos = spu->s_chan[ch].SB[28];
             gval0 = fa;
             gpos = (gpos+1) & 3;
             spu->s_chan[ch].SB[28] = gpos;
	    }
           spu->s_chan[ch].spos -= 0x10000L;
          }

         ////////////////////////////////////////////////
         // noise handler... just produces some noise data
         // surely wrong... and no noise frequency (spuCtrl&0x3f00) will be used...
         // and sometimes the noise will be used as fmod modulation... pfff

         if(spu->s_chan[ch].bNoise)
          {
	   //puts("Noise");
           if((spu->dwNoiseVal<<=1)&0x80000000L)
            {
             spu->dwNoiseVal^=0x0040001L;
             fa=((spu->dwNoiseVal>>2)&0x7fff);
             fa=-fa;
            }
           else fa=(spu->dwNoiseVal>>2)&0x7fff;

           // mmm... depending on the noise freq we allow bigger/smaller changes to the previous val
           fa=spu->s_chan[ch].iOldNoise+((fa-spu->s_chan[ch].iOldNoise)/((0x001f-((spu->spuCtrl&0x3f00)>>9))+1));
           if(fa>32767L)  fa=32767L;
           if(fa<-32767L) fa=-32767L;              
           spu->s_chan[ch].iOldNoise=fa;

          }                                            //----------------------------------------
         else                                         // NO NOISE (NORMAL SAMPLE DATA) HERE 
          {
             int vl, vr, gpos;
             vl = (spu->s_chan[ch].spos >> 6) & ~3;
             gpos = spu->s_chan[ch].SB[28];
             vr=(gauss[vl]*gval0)>>9;
             vr+=(gauss[vl+1]*gval(1))>>9;
             vr+=(gauss[vl+2]*gval(2))>>9;
             vr+=(gauss[vl+3]*gval(3))>>9;
             fa = vr>>2;
          }

         spu->s_chan[ch].sval = (MixADSR(spu, ch) * fa)>>10;     // / 1023;  // add adsr
         if(spu->s_chan[ch].bFMod==2)                       // fmod freq channel
         {
           int NP=spu->s_chan[ch+1].iRawPitch;
           NP=((32768L+spu->s_chan[ch].sval)*NP)>>15; ///32768L;

           if(NP>0x3fff) NP=0x3fff;
           if(NP<0x1)    NP=0x1;
                                                        
	   // mmmm... if I do this, all is screwed              
	  //           spu->s_chan[ch+1].iRawPitch=NP;

           NP=(44100L*NP)/(4096L);                     // calc frequency

           spu->s_chan[ch+1].iActFreq=NP;
           spu->s_chan[ch+1].iUsedFreq=NP;
           spu->s_chan[ch+1].sinc=(((NP/10)<<16)/4410);
           if(!spu->s_chan[ch+1].sinc) spu->s_chan[ch+1].sinc=1;

		// mmmm... set up freq decoding positions?
		//           spu->s_chan[ch+1].iSBPos=28;
		//           spu->s_chan[ch+1].spos=0x10000L;
          }                    
         else
          {                                          
           //////////////////////////////////////////////
           // ok, left/right sound volume (psx volume goes from 0 ... 0x3fff)
	   int tmpl,tmpr;

		if (1) //ao_channel_enable[ch+PSF_1]) {
		{
			tmpl=(spu->s_chan[ch].sval*spu->s_chan[ch].iLeftVolume)>>14;
			tmpr=(spu->s_chan[ch].sval*spu->s_chan[ch].iRightVolume)>>14;
		} else {
			tmpl = 0;
			tmpr = 0;
		}
	   sl+=tmpl;
	   sr+=tmpr;

	   if(((spu->rvb.Enabled>>ch)&1) && (spu->spuCtrl&0x80))
	   {
	    revLeft+=tmpl;
	    revRight+=tmpr;
	   }
          }

         spu->s_chan[ch].spos += spu->s_chan[ch].sinc;             
 ENDX:   ;                                                      
      }
    }                                                         

  ///////////////////////////////////////////////////////
  // mix all channels (including reverb) into one buffer
  MixREVERBLeftRight(spu, &sl,&sr,revLeft,revRight);
//  printf("sampcount %d decaybegin %d decayend %d\n", sampcount, decaybegin, decayend);
  if(spu->sampcount>=spu->decaybegin)
  {
   s32 dmul;
   if(spu->decaybegin!=~0) // Is anyone REALLY going to be playing a song
		      // for 13 hours?
   {
    if(spu->sampcount>=spu->decayend) 
    {
//       	    ao_song_done = 1;
	    return(0);
    }
    dmul=256-(256*(spu->sampcount-spu->decaybegin)/(spu->decayend-spu->decaybegin));
    sl=(sl*dmul)>>8;
    sr=(sr*dmul)>>8;
   }
  }

  spu->sampcount++;
  sl=(sl*volmul)>>8;
  sr=(sr*volmul)>>8;

  //{
  // static double asl=0;
  // static double asr=0;
   
  // asl+=(sl-asl)/5;
  // asr+=(sl-asr)/5;

   //sl-=asl;
   //sr-=asr;

  // if(sl>32767 || sl < -32767) printf("Left: %d, %f\n",sl,asl);
  // if(sr>32767 || sr < -32767) printf("Right: %d, %f\n",sl,asl);
  //}

  if(sl>32767) sl=32767; if(sl<-32767) sl=-32767;
  if(sr>32767) sr=32767; if(sr<-32767) sr=-32767;

  *spu->pS++=sl;
  *spu->pS++=sr;
 }

 return(1);
}

void SPU_flushboot(mips_cpu_context *cpu)
{
    spu_state_t *spu = cpu->spu;
   if((u8*)spu->pS>((u8*)spu->pSpuBuffer+1024))
   {
    //spu_update(cpu, (u8*)pSpuBuffer,(u8*)pS-(u8*)pSpuBuffer);
    cpu->spu_callback ((u8*)spu->pSpuBuffer,(u8*)spu->pS-(u8*)spu->pSpuBuffer, cpu->spu_callback_data);
    spu->pS=(s16 *)spu->pSpuBuffer;
   }
}   

#ifdef TIMEO
static u64 begintime;
static u64 gettime64(void)
{
 struct timeval tv;
 u64 ret;

 gettimeofday(&tv,0);
 ret=tv.tv_sec;
 ret*=1000000;
 ret+=tv.tv_usec;
 return(ret);
}
#endif
////////////////////////////////////////////////////////////////////////
// INIT/EXIT STUFF
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SPUINIT: this func will be called first by the main emu
////////////////////////////////////////////////////////////////////////
              
int SPUinit(mips_cpu_context *cpu, void (*update_cb)(unsigned char *pSound, long lBytes, void *data), void *data)
{
    cpu->spu_callback = update_cb;
    cpu->spu_callback_data = data;
    cpu->spu = malloc (sizeof (spu_state_t));
    memset (cpu->spu, 0, sizeof (spu_state_t));
    cpu->spu->dwNoiseVal = 1;
    cpu->spu->spuAddr = 0xffffffff;
 cpu->spu->spuMemC=(u8*)cpu->spu->spuMem;                      // just small setup
 memset((void *)cpu->spu->s_chan,0,MAXCHAN*sizeof(SPUCHAN));
 memset((void *)&cpu->spu->rvb,0,sizeof(REVERBInfo));
 memset(cpu->spu->regArea,0,sizeof(cpu->spu->regArea));
 memset(cpu->spu->spuMem,0,sizeof(cpu->spu->spuMem));
 InitADSR();
 cpu->spu->sampcount = cpu->spu->ttemp=0;
 #ifdef TIMEO
 begintime=gettime64();
 #endif
 return 0;
}

////////////////////////////////////////////////////////////////////////
// SETUPSTREAMS: init most of the spu buffers
////////////////////////////////////////////////////////////////////////

void SetupStreams(spu_state_t *spu)
{ 
 int i;

 spu->pSpuBuffer=(u8*)malloc(32768);            // alloc mixing buffer
 spu->pS=(s16 *)spu->pSpuBuffer;

 for(i=0;i<MAXCHAN;i++)                                // loop sound channels
  {
   spu->s_chan[i].ADSRX.SustainLevel = 1024;                // -> init sustain
   spu->s_chan[i].iIrqDone=0;
   spu->s_chan[i].pLoop=spu->spuMemC;
   spu->s_chan[i].pStart=spu->spuMemC;
   spu->s_chan[i].pCurr=spu->spuMemC;
  }
}

////////////////////////////////////////////////////////////////////////
// REMOVESTREAMS: free most buffer
////////////////////////////////////////////////////////////////////////

void RemoveStreams(spu_state_t *spu)
{ 
 free(spu->pSpuBuffer);                                     // free mixing buffer
 spu->pSpuBuffer=NULL;

 #ifdef TIMEO
 {
  u64 tmp;
  tmp=gettime64();
  tmp-=begintime;
  if(tmp)
   tmp=(u64)sampcount*1000000/tmp;
  printf("%lld samples per second\n",tmp);
 }
 #endif
}


////////////////////////////////////////////////////////////////////////
// SPUOPEN: called by main emu after init
////////////////////////////////////////////////////////////////////////
   
int SPUopen(mips_cpu_context *cpu)
{
    spu_state_t *spu = cpu->spu;
 if(spu->bSPUIsOpen) return 0;                              // security for some stupid main emus
 spu->spuIrq=0;                       

 spu->spuStat=spu->spuCtrl=0;
 spu->spuAddr=0xffffffff;
 spu->dwNoiseVal=1;

 spu->spuMemC=(u8*)spu->spuMem;      
 memset((void *)spu->s_chan,0,(MAXCHAN+1)*sizeof(SPUCHAN));
 spu->pSpuIrq=0;

 spu->iVolume=255; //85;
 SetupStreams(spu);                                       // prepare streaming

 spu->bSPUIsOpen=1;

 return 1;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SPUCLOSE: called before shutdown
////////////////////////////////////////////////////////////////////////

int SPUclose(mips_cpu_context *cpu)
{
    spu_state_t *spu = cpu->spu;
    if (!spu) {
        return 0;
    }
 if(!spu->bSPUIsOpen) return 0;                             // some security

 spu->bSPUIsOpen=0;                                         // no more open

 RemoveStreams(spu);                                      // no more streaming
 free (spu);
 cpu->spu = NULL;

 return 0;
}

////////////////////////////////////////////////////////////////////////
// SPUSHUTDOWN: called by main emu on final exit
////////////////////////////////////////////////////////////////////////

int SPUshutdown(mips_cpu_context *cpu)
{
 return 0;
}

void SPUinjectRAMImage(mips_cpu_context *cpu, u16 *pIncoming)
{
    spu_state_t *spu = cpu->spu;
	int i;

	for (i = 0; i < (256*1024); i++)
	{
		spu->spuMem[i] = pIncoming[i];
	}
}
