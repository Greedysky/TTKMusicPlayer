/*-----------------------------------------------------------------------------

    ST-Sound ( YM files player library )

    Extended YM-2149 Emulator, with ATARI music demos effects.
    (SID-Like, Digidrum, Sync Buzzer, Sinus SID and Pattern SID)

-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ST-Sound, ATARI-ST Music Emulator
* Copyright (c) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
-----------------------------------------------------------------------------*/


#ifndef YM_2149EX_H
#define YM_2149EX_H

#define	AMSTRAD_CLOCK	1000000L
#define	ATARI_CLOCK		2000000L
#define	SPECTRUM_CLOCK	1773400L
#define	MFP_CLOCK		2457600L
#define	NOISESIZE		16384
#define	DRUM_PREC		15

#define	PI				3.1415926
#define	SIDSINPOWER		0.7

enum
{
    VOICE_A=0,
    VOICE_B=1,
    VOICE_C=2,
};

struct	YmSpecialEffect
{

    ymbool		bDrum;
    ymu32		drumSize;
    ymu8	*	drumData;
    ymu32		drumPos;
    ymu32		drumStep;

    ymbool		bSid;
    ymu32		sidPos;
    ymu32		sidStep;
    ymint		sidVol;

};

static	const	ymint		DC_ADJUST_BUFFERLEN		=	512;

class	CDcAdjuster
{
public:
    CDcAdjuster();

    void	AddSample(ymint sample);
    ymint	GetDcLevel(void)			{ return m_sum / DC_ADJUST_BUFFERLEN; }
    void	Reset(void);

private:
    ymint	m_buffer[DC_ADJUST_BUFFERLEN];
    ymint		m_pos;
    ymint		m_sum;
};


class CYm2149Ex
{
public:
    CYm2149Ex(ymu32 masterClock=ATARI_CLOCK,ymint prediv=1,ymu32 playRate=44100);
    ~CYm2149Ex();

    void	reset(void);
    void	update(ymsample *pSampleBuffer,ymint nbSample);

    void	setClock(ymu32 _clock);
    void	writeRegister(ymint reg,ymint value);
    ymint	readRegister(ymint reg);
    void	drumStart(ymint voice,ymu8 *drumBuffer,ymu32 drumSize,ymint drumFreq);
    void	drumStop(ymint voice);
    void	sidStart(ymint voice,ymint freq,ymint vol);
    void	sidSinStart(ymint voice,ymint timerFreq,ymint sinPattern);
    void	sidStop(ymint voice);
    void	syncBuzzerStart(ymint freq,ymint envShape);
    void	syncBuzzerStop(void);

    void	setFilter(ymbool bFilter)		{ m_bFilter = bFilter; }

private:

    CDcAdjuster		m_dcAdjust;

    ymu32	frameCycle;
    ymu32	cyclePerSample;
    inline	ymsample nextSample(void);
    ymu32 toneStepCompute(ymu8 rHigh,ymu8 rLow);
    ymu32 noiseStepCompute(ymu8 rNoise);
    ymu32 envStepCompute(ymu8 rHigh,ymu8 rLow);
    void	updateEnvGen(ymint nbSample);
    void	updateNoiseGen(ymint nbSample);
    void	updateToneGen(ymint voice,ymint nbSample);
    ymu32	rndCompute(void);

    void	sidVolumeCompute(ymint voice,ymint *pVol);
    inline int		LowPassFilter(int in);

    ymint	replayFrequency;
    ymu32	internalClock;
    ymu8	registers[14];

    ymu32	cycleSample;
    ymu32	stepA,stepB,stepC;
    ymu32	posA,posB,posC;
    ymint				volA,volB,volC,volE;
    ymu32	mixerTA,mixerTB,mixerTC;
    ymu32	mixerNA,mixerNB,mixerNC;
    ymint				*pVolA,*pVolB,*pVolC;

    ymu32	noiseStep;
    ymu32 noisePos;
    ymu32	rndRack;
    ymu32	currentNoise;
    ymu32	bWrite13;

    ymu32	envStep;
    ymu32 envPos;
    ymint		envPhase;
    ymint		envShape;
    ymu8	envData[16][2][16*2];
    ymint	globalVolume;

    struct	YmSpecialEffect	specialEffect[3];
    ymbool	bSyncBuzzer;
    ymu32	syncBuzzerStep;
    ymu32	syncBuzzerPhase;
    ymint	syncBuzzerShape;

    int		m_lowPassFilter[2];
    ymbool	m_bFilter;
};

#endif
