/*-----------------------------------------------------------------------------

    ST-Sound ( YM files player library )

    YM Music Driver

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

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "ym_music.h"

#define	_LINEAR_OVRS				// Activate linear oversampling (best quality) Only used for DigiMix and UniversalTracker YM file type


// ATARI-ST MFP chip predivisor
static	const ymint	mfpPrediv[8] = {0,4,10,16,50,64,100,200};



CYmMusic::CYmMusic(ymint _replayRate)
{

    pBigMalloc = nullptr;
    pSongName = nullptr;
    pSongAuthor = nullptr;
    pSongComment = nullptr;
    pSongType = nullptr;
    pSongPlayer = nullptr;

    pBigSampleBuffer = nullptr;
    pMixBlock = nullptr;

    replayRate = _replayRate;
    innerSamplePos = 0;
    currentPos = 0;
    nbDrum = 0;
    pDrumTab = nullptr;
    setLoopMode(YMFALSE);

    m_pTimeInfo = nullptr;
}

void CYmMusic::setTimeControl(ymbool bTime)
{
    if (bTime)
        attrib |= A_TIMECONTROL;
    else
        attrib &= (~A_TIMECONTROL);
}

CYmMusic::~CYmMusic()
{
    stop();
    unLoad();
}

void CYmMusic::setLoopMode(ymbool bLoopMode)
{
    bLoop = bLoopMode;
}

void CYmMusic::setPlayerRate(ymint rate)
{
    playerRate = rate;
}

ymu32 CYmMusic::getPos()
{
    if ((songType>=YM_MIX1) && (songType<YM_MIXMAX))
    {
        // avoid overflow after 97 seconds and keep only 32bit computing
        return m_iMusicPosInMs;
    }
    else if ((nbFrame>0) && (playerRate>0))
    {
        return ((ymu32)currentFrame*1000)/(ymu32)playerRate;
    }
    else
        return 0;

}

ymu32 CYmMusic::getMusicTime(void)
{

    if ((songType>=YM_MIX1) && (songType<YM_MIXMAX))
    {
        return m_musicLenInMs;
    }
    else if ((nbFrame>0) && (playerRate>0))
    {
        return ((ymu32)nbFrame*1000)/(ymu32)playerRate;
    }
    else
        return 0;

}


void CYmMusic::setMixTime(ymu32 time)
{
    if (time > m_musicLenInMs)
        return;

    assert(m_pTimeInfo);
    for (int i=0;i<m_nbTimeKey;i++)
    {
        ymu32 tEnd = i < (m_nbTimeKey-1) ? m_pTimeInfo[i+1].time : m_musicLenInMs;
        if ((time >= m_pTimeInfo[i].time) && (time < tEnd))
        {
            mixPos = m_pTimeInfo[i].nBlock;
            pCurrentMixSample = pBigSampleBuffer + pMixBlock[mixPos].sampleStart;
            currentSampleLength = (pMixBlock[mixPos].sampleLength)<<12;
            currentPente = (((ymu32)pMixBlock[mixPos].replayFreq)<<12) / replayRate;

            ymu32 len = tEnd - m_pTimeInfo[i].time;
            ymu32 t0 = ((time - m_pTimeInfo[i].time) * pMixBlock[mixPos].sampleLength) / len;

            currentPos = t0 << 12;
            nbRepeat = m_pTimeInfo[i].nRepeat;
            break;
        }
    }

    m_iMusicPosInMs = time;
    m_iMusicPosAccurateSample = 0;
}

ymu32 CYmMusic::setMusicTime(ymu32 time)
{
    if (!isSeekable()) return 0;
    ymu32 newTime = 0;

    if ((songType>=YM_V2) && (songType<YM_VMAX))
    {
        newTime = time;
        if (newTime>=getMusicTime()) newTime = 0;
        currentFrame = (newTime*(ymu32)playerRate)/1000;
    }
    else if ((songType>=YM_TRACKER1) && (songType<YM_TRACKERMAX))
    {
        newTime = time;
        if (newTime>=getMusicTime()) newTime = 0;
        currentFrame = (newTime*(ymu32)playerRate)/1000;
    }
    else if ((songType>=YM_MIX1) && (songType<YM_MIXMAX))
    {
        assert(m_pTimeInfo);
        setMixTime(time);
    }

    return newTime;
}

void CYmMusic::restart(void)
{
    setMusicTime(0);
    bMusicOver = YMFALSE;
}



void CYmMusic::getMusicInfo(ymMusicInfo_t *pInfo)
{
    if (pInfo)
    {
        pInfo->pSongName = pSongName;
        pInfo->pSongAuthor = pSongAuthor;
        pInfo->pSongComment = pSongComment;
        pInfo->pSongType = pSongType;
        pInfo->pSongPlayer = pSongPlayer;

        pInfo->musicTimeInMs = getMusicTime();
        pInfo->musicTimeInSec = pInfo->musicTimeInMs / 1000;
    }
}


void CYmMusic::setAttrib(ymint _attrib)
{
    attrib = _attrib;
}

ymint	 CYmMusic::getAttrib(void)
{
    return attrib;
}

ymbool CYmMusic::isSeekable(void)
{
    return getAttrib()&A_TIMECONTROL;
}

void CYmMusic::setLastError(const char *pError)
{
    pLastError = pError;
}

const char	*CYmMusic::getLastError(void)
{
    return pLastError;
}

void	bufferClear(ymsample *pBuffer,ymint nbSample)
{
    memset((void*)pBuffer,0,nbSample*sizeof(ymsample));
}

ymbool CYmMusic::update(ymsample *sampleBuffer,ymint nbSample)
{
    ymint sampleToCompute;
    ymint	vblNbSample;


    if ((!bMusicOk) ||
            (bPause) ||
            (bMusicOver))
    {
        bufferClear(sampleBuffer,nbSample);
        if (bMusicOver)
            return YMFALSE;
        else
            return YMTRUE;
    }

    if ((songType >= YM_MIX1) && (songType < YM_MIXMAX))
    {
        stDigitMix(sampleBuffer,nbSample);


    }
    else if ((songType >= YM_TRACKER1) && (songType<YM_TRACKERMAX))
    {
        ymTrackerUpdate(sampleBuffer,nbSample);
    }
    else
    {
        ymsample *pOut = sampleBuffer;
        ymint nbs = nbSample;
        vblNbSample = replayRate/playerRate;
        do
        {
            // Nb de sample ?calculer avant l'appel de Player
            sampleToCompute = vblNbSample-innerSamplePos;
            // Test si la fin du buffer arrive avant la fin de sampleToCompute
            if (sampleToCompute>nbs) sampleToCompute = nbs;
            innerSamplePos += sampleToCompute;
            if (innerSamplePos>=vblNbSample)
            {
                player();			// Lecture de la partition (playerRate Hz)
                innerSamplePos -= vblNbSample;
            }
            if (sampleToCompute>0)
            {
                ymChip.update(pOut,sampleToCompute);	// YM Emulation.
                pOut += sampleToCompute;
            }
            nbs -= sampleToCompute;
        }
        while (nbs>0);
    }


    return YMTRUE;
}



void CYmMusic::readYm6Effect(unsigned char *pReg,ymint code,ymint prediv,ymint count)
{
    ymint voice;
    ymint ndrum;

    code = pReg[code]&0xf0;
    prediv = (pReg[prediv]>>5)&7;
    count = pReg[count];

    if (code&0x30)
    {
        ymu32 tmpFreq;
        // Ici il y a un effet sur la voie:

        voice = ((code&0x30)>>4)-1;
        switch (code&0xc0)
        {
        case 0x00:		// SID
        case 0x80:		// Sinus-SID

            prediv = mfpPrediv[prediv];
            prediv *= count;
            tmpFreq = 0;
            if (prediv)
            {
                tmpFreq = 2457600L / prediv;
                if ((code&0xc0)==0x00)
                    ymChip.sidStart(voice,tmpFreq,pReg[voice+8]&15);
                else
                    ymChip.sidSinStart(voice,tmpFreq,pReg[voice+8]&15);
            }
            break;

        case 0x40:		// DigiDrum
            ndrum = pReg[voice+8]&31;
            if ((ndrum>=0) && (ndrum<nbDrum))
            {
                prediv = mfpPrediv[prediv];
                prediv *= count;
                if (prediv>0)
                {
                    tmpFreq = 2457600L / prediv;
                    ymChip.drumStart(voice,pDrumTab[ndrum].pData,pDrumTab[ndrum].size,tmpFreq);
                }
            }
            break;

        case 0xc0:		// Sync-Buzzer.

            prediv = mfpPrediv[prediv];
            prediv *= count;
            tmpFreq = 0;
            if (prediv)
            {
                tmpFreq = 2457600L / prediv;
                ymChip.syncBuzzerStart(tmpFreq,pReg[voice+8]&15);
            }
            break;


        }

    }
}

void CYmMusic::setVolume(ymint )
{
    //		ymChip.setGlobalVolume(volume);
}

void CYmMusic::player(void)
{
    ymu8	*ptr;
    ymu32 prediv;
    ymint voice;
    ymint ndrum;


    if (currentFrame<0) currentFrame = 0;

    if (currentFrame>=nbFrame)
    {
        if (bLoop)
        {
            currentFrame = loopFrame;
        }
        else
        {
            bMusicOver = YMTRUE;
            ymChip.reset();
            return;
        }
    }

    ptr = pDataStream+currentFrame*streamInc;

    for (ymint i=0;i<=10;i++)
        ymChip.writeRegister(i,ptr[i]);

    ymChip.sidStop(0);
    ymChip.sidStop(1);
    ymChip.sidStop(2);
    ymChip.syncBuzzerStop();

    //---------------------------------------------
    // Check digi-drum
    //---------------------------------------------
    if (songType == YM_V2)		// MADMAX specific !
    {
        if (ptr[13]!=0xff)
        {
            ymChip.writeRegister(11,ptr[11]);
            ymChip.writeRegister(12,0);
            ymChip.writeRegister(13,10);				// MADMAX specific !!
        }
        if (ptr[10]&0x80)					// bit 7 volume canal C pour annoncer une digi-drum madmax.
        {
            ymint	sampleNum;
            ymu32 sampleFrq;
            ymChip.writeRegister(7,ymChip.readRegister(7)|0x24)	;	// Coupe TONE + NOISE canal C.
            sampleNum = ptr[10]&0x7f;		// Numero du sample

            if (ptr[12])
            {
                sampleFrq = (MFP_CLOCK / ptr[12]);
                ymChip.drumStart(	2,							// Voice C
                                    sampleAdress[sampleNum],
                                    sampleLen[sampleNum],
                                    sampleFrq);
            }
        }
    }
    else if (songType >= YM_V3)
    {
        ymChip.writeRegister(11,ptr[11]);
        ymChip.writeRegister(12,ptr[12]);
        if (ptr[13]!=0xff)
        {
            ymChip.writeRegister(13,ptr[13]);
        }

        if (songType >= YM_V5)
        {
            ymint code;

            if (songType == YM_V6)
            {
                readYm6Effect(ptr,1,6,14);
                readYm6Effect(ptr,3,8,15);
            }
            else
            {	// YM5 effect decoding

                //------------------------------------------------------
                // Sid Voice !!
                //------------------------------------------------------
                code = (ptr[1]>>4)&3;
                if (code!=0)
                {
                    ymu32 tmpFreq;
                    voice = code-1;
                    prediv = mfpPrediv[(ptr[6]>>5)&7];
                    prediv *= ptr[14];
                    tmpFreq = 0;
                    if (prediv)
                    {
                        tmpFreq = 2457600L / prediv;
                        ymChip.sidStart(voice,tmpFreq,ptr[voice+8]&15);
                    }
                }

                //------------------------------------------------------
                // YM5 Digi Drum.
                //------------------------------------------------------
                code = (ptr[3]>>4)&3;
                if (code!=0)
                {	// Ici un digidrum demarre sur la voie voice.
                    voice = code-1;
                    ndrum = ptr[8+voice]&31;
                    if ((ndrum>=0) && (ndrum<nbDrum))
                    {
                        ymu32 sampleFrq;
                        prediv = mfpPrediv[(ptr[8]>>5)&7];
                        prediv *= ptr[15];
                        if (prediv)
                        {
                            sampleFrq = MFP_CLOCK / prediv;
                            ymChip.drumStart(voice,pDrumTab[ndrum].pData,pDrumTab[ndrum].size,sampleFrq);
                        }
                    }
                }
            }
        }
    }
    currentFrame++;
}

/*

    x x x x x x x x	r0
    0 0 0 0 x x x x	r1		// Special FX 1a
    x x x x x x x x	r2
    0 0 0 0 x x x x r3		// Special FX 2a
    x x x x x x x x r4
    0 0 0 0 x x x x r5
    0 0 0 x x x x x r6		// Special FX 1b
    0 0 x x x x x x r7
    0 0 0 x x x x x r8		// Special FX 2b
    0 0 0 x x x x x r9
    0 0 0 x x x x x r10
    x x x x x x x x r11
    x x x x x x x x r12
    0 0 0 0 x x x x r13
    0 0 0 0 0 0 0 0 r14		// Special FX 1c
    0 0 0 0 0 0 0 0 r15		// Special FX 2c


  Special Fx ?a
    0 0 0 0	: No special FX running
    0 0 0 1 : Sid Voice A
    0 0 1 0 : Sid Voice B
    0 0 1 1 : Sid Voice C
    0 1 0 0 : Extended Fx voice A
    0 1 0 1 : Digidrum voice A
    0 1 1 0 : Digidrum voice B
    0 1 1 1 : Digidrum voice C
    1 0 0 0 : Extended Fx voice B
    1 0 0 1 : Sinus SID voice A
    1 0 1 0 : Sinus SID voice B
    1 0 1 1 : Sinus SID voice C
    1 1 0 0 : Extended Fx voice C
    1 1 0 1 : Sync Buzzer voice A
    1 1 1 0 : Sync Buzzer voice B
    1 1 1 1 : Sync Buzzer voice C



*/

void CYmMusic::computeTimeInfo(void)
{

    assert(nullptr == m_pTimeInfo);

    //-------------------------------------------
    // Compute nb of mixblock
    //-------------------------------------------
    m_nbTimeKey = 0;
    ymint i;
    for (i=0;i<nbMixBlock;i++)
    {
        if (pMixBlock[i].nbRepeat >= 32)
            pMixBlock[i].nbRepeat = 32;

        m_nbTimeKey += pMixBlock[i].nbRepeat;
    }

    //-------------------------------------------
    // Parse all mixblock keys
    //-------------------------------------------
    m_pTimeInfo = (TimeKey*)malloc(sizeof(TimeKey) * m_nbTimeKey);
    TimeKey *pKey = m_pTimeInfo;
    ymu32 time = 0;

    for (i=0;i<nbMixBlock;i++)
    {
        for (ymint j=0;j<pMixBlock[i].nbRepeat;j++)
        {
            pKey->time = time;
            pKey->nRepeat = pMixBlock[i].nbRepeat - j;
            pKey->nBlock = i;
            pKey++;

            time += (pMixBlock[i].sampleLength * 1000) / pMixBlock[i].replayFreq;
        }
    }
    m_musicLenInMs = time;

}

void CYmMusic::readNextBlockInfo(void)
{
    nbRepeat--;
    if (nbRepeat<=0)
    {
        mixPos++;
        if (mixPos >= nbMixBlock)
        {
            mixPos = 0;
            if (!bLoop) bMusicOver = YMTRUE;

            m_iMusicPosAccurateSample = 0;
            m_iMusicPosInMs = 0;
        }
        nbRepeat = pMixBlock[mixPos].nbRepeat;
    }
    pCurrentMixSample = pBigSampleBuffer + pMixBlock[mixPos].sampleStart;
    currentSampleLength = (pMixBlock[mixPos].sampleLength)<<12;
    currentPente = (((ymu32)pMixBlock[mixPos].replayFreq)<<12) / replayRate;
    currentPos &= ((1<<12)-1);
}

void CYmMusic::stDigitMix(ymsample *pWrite16,ymint nbs)
{


    if (bMusicOver) return;

    if (mixPos == -1)
    {
        nbRepeat = -1;
        readNextBlockInfo();
    }

    m_iMusicPosAccurateSample += nbs * 1000;
    m_iMusicPosInMs += ((m_iMusicPosAccurateSample) / (replayRate));
    m_iMusicPosAccurateSample %= (replayRate);

    if (nbs) do
    {

        ymint sa = (ymint)(ymsample)(pCurrentMixSample[currentPos>>12]<<8);
#ifdef _LINEAR_OVRS
        ymint sb = sa;
        if ((currentPos>>12)<((currentSampleLength>>12)-1))
            sb = (ymint)(ymsample)(pCurrentMixSample[(currentPos>>12)+1]<<8);
        ymint frac = currentPos&((1<<12)-1);
        sa += (((sb-sa)*frac)>>12);
#endif
        *pWrite16++ = sa;

        currentPos += currentPente;
        if (currentPos>=currentSampleLength)
        {
            readNextBlockInfo();
            if (bMusicOver) return;
        }
    }
    while (--nbs);
}

void CYmMusic::ymTrackerDesInterleave(void)
{
    unsigned char *a0,*a1,*a2;
    unsigned char *pNewBuffer;
    ymint	step;
    ymu32 n1,n2;


    if (attrib&A_STREAMINTERLEAVED)
    {
        a0 = pDataStream;
        ymint size = sizeof(ymTrackerLine_t)*nbVoice*nbFrame;
        pNewBuffer = (unsigned char*)malloc(size);
        step = sizeof(ymTrackerLine_t)*nbVoice;
        n1 = step;
        a2 = pNewBuffer;
        do
        {
            n2 = nbFrame;
            a1 = a2;
            do
            {
                *a1 = *a0++;
                a1 += step;
            }
            while (--n2);
            a2++;
        }
        while (--n1);
        memcpy(pDataStream,pNewBuffer,size);
        free(pNewBuffer);
        attrib &= (~A_STREAMINTERLEAVED);
    }
}


void CYmMusic::ymTrackerInit(ymint volMaxPercent)
{
    ymint i,s;
    ymint vol;
    ymint scale;
    ymsample *pTab;



    for (i=0;i<MAX_VOICE;i++)
        ymTrackerVoice[i].bRunning = 0;

    ymTrackerNbSampleBefore = 0;

    scale = (256*volMaxPercent) / (nbVoice*100);
    pTab = ymTrackerVolumeTable;

    // Construit la table de volume.
    for (vol=0;vol<64;vol++)
    {
        for (s=-128;s<128;s++)
        {
            *pTab++ = (s*(ymint)scale*vol)/64;
        }
    }

    // Des-interleave si necessaire.
    ymTrackerDesInterleave();

}


void CYmMusic::ymTrackerPlayer(ymTrackerVoice_t *pVoice)
{
    ymint i;
    ymTrackerLine_t *pLine;


    pLine = (ymTrackerLine_t*)pDataStream;
    pLine += (currentFrame*nbVoice);
    for (i=0;i<nbVoice;i++)
    {
        ymint n;
        pVoice[i].sampleFreq = ((ymint)pLine->freqHigh<<8) | pLine->freqLow;
        if (pVoice[i].sampleFreq)
        {
            pVoice[i].sampleVolume = pLine->volume&63;
            pVoice[i].bLoop = (pLine->volume&0x40);
            n = pLine->noteOn;
            if (n != 0xff)		// Note ON.
            {
                pVoice[i].bRunning = 1;
                pVoice[i].pSample = pDrumTab[n].pData;
                pVoice[i].sampleSize = pDrumTab[n].size;
                pVoice[i].repLen = pDrumTab[n].repLen;
                pVoice[i].samplePos = 0;
            }
        }
        else
        {
            pVoice[i].bRunning = 0;
        }
        pLine++;
    }

    currentFrame++;
    if (currentFrame >= nbFrame)
    {
        if (!bLoop)
        {
            bMusicOver = YMTRUE;
        }
        currentFrame = 0;
    }
}


void CYmMusic::ymTrackerVoiceAdd(ymTrackerVoice_t *pVoice,ymsample *pBuffer,ymint nbs)
{
    ymsample *pVolumeTab;
    ymu8 *pSample;
    ymu32 samplePos;
    ymu32 sampleEnd;
    ymu32 sampleInc;
    ymu32 repLen;
    double	step;


    if (!(pVoice->bRunning)) return;

    pVolumeTab = &ymTrackerVolumeTable[256*(pVoice->sampleVolume&63)];
    pSample = pVoice->pSample;
    samplePos = pVoice->samplePos;

    step = (double)(pVoice->sampleFreq<<YMTPREC);
    step *= (double)(1<<ymTrackerFreqShift);
    step /= (double)replayRate;
    sampleInc = (ymu32)step;

    sampleEnd = (pVoice->sampleSize<<YMTPREC);
    repLen = (pVoice->repLen<<YMTPREC);
    if (nbs>0) do
    {
        ymint va = pVolumeTab[pSample[samplePos>>YMTPREC]];
#ifdef _LINEAR_OVRS
        ymint vb = va;
        if (samplePos < (sampleEnd-(1<<YMTPREC)))
            vb = pVolumeTab[pSample[(samplePos>>YMTPREC)+1]];

        ymint frac = samplePos & ((1<<YMTPREC)-1);
        va += (((vb-va)*frac)>>YMTPREC);
#endif
        (*pBuffer++) += va;

        samplePos += sampleInc;
        if (samplePos>=sampleEnd)
        {
            if (pVoice->bLoop)
            {
                samplePos -= repLen;
            }
            else
            {
                pVoice->bRunning = 0;
                return;
            }
        }
    }
    while (--nbs);
    pVoice->samplePos = samplePos;
}

void CYmMusic::ymTrackerUpdate(ymsample *pBuffer,ymint nbSample)
{
    ymint i;
    ymint _nbs;

    // Clear les buffers.
    memset(pBuffer,0,sizeof(ymsample)*nbSample);
    if (bMusicOver) return;

    do
    {
        if (ymTrackerNbSampleBefore == 0)
        {
            // Lit la partition ymTracker
            ymTrackerPlayer(ymTrackerVoice);
            if (bMusicOver) return;
            ymTrackerNbSampleBefore = replayRate / playerRate;
        }
        _nbs = ymTrackerNbSampleBefore;		// nb avant playerUpdate.
        if (_nbs>nbSample) _nbs = nbSample;
        ymTrackerNbSampleBefore -= _nbs;
        if (_nbs>0)
        {
            // Genere les samples.
            for (i=0;i<nbVoice;i++)
            {
                ymTrackerVoiceAdd(&ymTrackerVoice[i],pBuffer,_nbs);
            }
            pBuffer += _nbs;
            nbSample -= _nbs;
        }
    }
    while (nbSample>0);
}
