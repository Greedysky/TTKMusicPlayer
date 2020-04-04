// Simple AMIGA Paula Audio channel mixer -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// History: This once was a simple mixer (it still is ;) that was
// used in a private MOD/TFMX player and was configured at run-time
// to mix up to 32 individual audio channels. In this particular
// version, 16-bit and mono are moved back in. Most of that old code
// has not been touched though because it has done its job well even
// if it looks quite ugly. So, please bear with me, if you find things
// in here that are not used by the FC player.

#include "LamePaula.h"

LamePaulaMixer::LamePaulaMixer()
    : AMIGA_CLOCK(AMIGA_CLOCK_PAL)
{
    // Start with no voice ptrs.
    _voices = 0;
    for (ubyte v=0; v<_maxVoices; v++) {
        _voice[v] = 0;
    }

	samplesAdd = 0;
	bufferScale = 0;
	toFill = 0;
	_fillFunc = 0;
}


LamePaulaMixer::~LamePaulaMixer() {
    end();
	samplesAdd = 0;
	bufferScale = 0;
	toFill = 0;
	_fillFunc = 0;
}


void LamePaulaMixer::end() {
    for (ubyte v=0; v<_voices; v++) {
        delete _voice[v];
        _voice[v] = 0;
    }
    _voices = 0;
}


PaulaVoice* LamePaulaMixer::getVoice(ubyte n) {
    if (n < _maxVoices) {
        return _voice[n];
    }
    else {
        return 0;
    }
}


void LamePaulaMixer::init(ubyte voices) {
    if ((voices <= _maxVoices) && (voices != _voices)) {
        end();
        _voices = voices;
        for (ubyte v=0; v<_voices; v++) {
            _voice[v] = new LamePaulaVoice;
        }
    }
    for (ubyte v=0; v<_voices; v++) {
        LamePaulaVoice* pv = _voice[v];
        pv->start = &emptySample;
        pv->end = &emptySample+1;
        pv->repeatStart = &emptySample;
        pv->repeatEnd = &emptySample+1;
        pv->length = 1;
        pv->curPeriod = 0;
        pv->stepSpeed = 0;
        pv->stepSpeedPnt = 0;
        pv->stepSpeedAddPnt = 0;
        pv->off();
    }
}


void LamePaulaMixer::init(udword freq, ubyte bits, ubyte channels, uword zero)
{
    _pcmFreq = freq;
    _bitsPerSample = bits;
    _channels = channels;
    _zero = zero;

    setReplayingSpeed();
    
    if (bits == 8) {
        zero8bit = zero;
        if (channels == 1) {
            _fillFunc = &LamePaulaMixer::fill8bitMono;
        }
        else {  // if (channels == 2)
            _fillFunc = &LamePaulaMixer::fill8bitStereo;
            ++bufferScale;
        }
    }
    else {  // if (bits == 16)
        zero16bit = zero;
        ++bufferScale;
        if (channels == 1) {
            _fillFunc = &LamePaulaMixer::fill16bitMono;
        }
        else {  // if (channels == 2)
            _fillFunc = &LamePaulaMixer::fill16bitStereo;
            ++bufferScale;
        }
    }
    
	uword ui = 0;
    long si = 0;
	ubyte voicesPerChannel = _voices/_channels;
    // Input samples: 80,81,82,...,FE,FF,00,01,02,...,7E,7F
    // Array: 00/x, 01/x, 02/x,...,7E/x,7F/x,80/x,81/x,82/x,...,FE/x/,FF/x 
    while (si++ < 128) {
		mix8[ui++] = (sbyte)(si/voicesPerChannel);
    }
    si = -128;
    while (si++ < 0) {
		mix8[ui++] = (sbyte)(si/voicesPerChannel);
    }
    // Input samples: 80,81,82,...,FE,FF,00,01,02,...,7E,7F
    // Array: 0/x, 100/x, 200/x, ..., FF00/x
    ui = 0;
    si = 0;
    while (si < 128*256) {
		mix16[ui++] = (sword)(si/voicesPerChannel);
        si += 256;
    }
    si = -128*256;
    while (si < 0) {
		mix16[ui++] = (sword)(si/voicesPerChannel);
        si += 256;
    }
}


void LamePaulaMixer::setReplayingSpeed() {
    samples = ( samplesOrg = _pcmFreq / 50 );
    samplesPnt = (( _pcmFreq % 50 ) * 65536 ) / 50;  
}


void LamePaulaMixer::setBpm(uword bpm) {
    uword callsPerSecond = (bpm * 2) / 5;
    samples = ( samplesOrg = _pcmFreq / callsPerSecond );
    samplesPnt = (( _pcmFreq % callsPerSecond ) * 65536 ) / callsPerSecond;  
}


void LamePaulaMixer::fillBuffer(void* buffer, udword bufferLen, PaulaPlayer *player) {
    // Both, 16-bit and stereo samples take more memory.
    // Hence fewer samples fit into the buffer.
    bufferLen >>= bufferScale;

    while ( bufferLen > 0 ) {
        if ( toFill > bufferLen ) {
            buffer = (this->*_fillFunc)(buffer, bufferLen);
            toFill -= bufferLen;
            bufferLen = 0;
        }
        else if ( toFill > 0 ) {
            buffer = (this->*_fillFunc)(buffer, toFill);
            bufferLen -= toFill;
            toFill = 0;   
        }
        if ( toFill == 0 ) {
            player->run();

            udword temp = ( samplesAdd += samplesPnt );
            samplesAdd = temp & 0xFFFF;
            toFill = samples + ( temp > 65535 );
	  
            for (ubyte v=0; v<_voices; v++) {
                LamePaulaVoice *pv = _voice[v];
                if ( pv->paula.period != pv->curPeriod ) {
                    pv->curPeriod = pv->paula.period;
                    if (pv->curPeriod != 0) {
                        pv->stepSpeed = (AMIGA_CLOCK/_pcmFreq) / pv->curPeriod;
                        pv->stepSpeedPnt = (((AMIGA_CLOCK/_pcmFreq) % pv->curPeriod ) * 65536 ) / pv->curPeriod;
                    }
                    else {
                        pv->stepSpeed = pv->stepSpeedPnt = 0;
                    }
                }
            }  // for voices
        }
    }  // while bufferLen
}


void* LamePaulaMixer::fill8bitMono(void* buffer, udword numberOfSamples)
{
    ubyte* buffer8bit = static_cast<ubyte*>(buffer);
    for (ubyte v=0; v<_voices; v++) {
        buffer8bit = static_cast<ubyte*>(buffer);
        LamePaulaVoice *pv = _voice[v];
        for (udword n = numberOfSamples; n>0; n--) {
            if (v == 0) {
                *buffer8bit = zero8bit;
            }
            pv->stepSpeedAddPnt += pv->stepSpeedPnt;
            pv->start += ( pv->stepSpeed + ( pv->stepSpeedAddPnt > 65535 ) );
            pv->stepSpeedAddPnt &= 65535;
            if ( pv->start < pv->end ) {
                *buffer8bit += ( pv->paula.volume * mix8[*pv->start] ) >> 6;
            }
            else {
                if ( pv->looping ) {
                    pv->start = pv->repeatStart;
                    pv->end = pv->repeatEnd;
                    if ( pv->start < pv->end ) {
                        *buffer8bit += ( pv->paula.volume * mix8[*pv->start] ) >> 6;
                    }
                }
            }
            buffer8bit++;
        }
    }
    return(buffer8bit);
}


void* LamePaulaMixer::fill8bitStereo( void* buffer, udword numberOfSamples )
{
    ubyte* buffer8bit = (static_cast<ubyte*>(buffer))+1;
    for (ubyte v=1; v<_voices; v+=2)
    {
        buffer8bit = (static_cast<ubyte*>(buffer))+1;
        LamePaulaVoice *pv = _voice[v];
        for (udword n = numberOfSamples; n>0; n--) {
            if (v == 1) {
                *buffer8bit = zero8bit;
            }
            pv->stepSpeedAddPnt += pv->stepSpeedPnt;
            pv->start += ( pv->stepSpeed + ( pv->stepSpeedAddPnt > 65535 ) );
            pv->stepSpeedAddPnt &= 65535;
            if ( pv->start < pv->end ) {
                *buffer8bit += ( pv->paula.volume * mix8[*pv->start] ) >> 6;
            }
            else {
                if ( pv->looping ) {
                    pv->start = pv->repeatStart;
                    pv->end = pv->repeatEnd;
                    if ( pv->start < pv->end ) {
                        *buffer8bit += ( pv->paula.volume * mix8[*pv->start] ) >> 6;
                    }
                }
            }
            buffer8bit += 2;
        }
    }
    buffer8bit = static_cast<ubyte*>(buffer);
    for (ubyte v=0; v<_voices; v+=2) {
        buffer8bit = static_cast<ubyte*>(buffer);
        LamePaulaVoice *pv = _voice[v];
        for (udword n = numberOfSamples; n>0; n--) {
            if (v == 0) {
                *buffer8bit = zero8bit;
            }
            pv->stepSpeedAddPnt += pv->stepSpeedPnt;
            pv->start += ( pv->stepSpeed + ( pv->stepSpeedAddPnt > 65535 ) );
            pv->stepSpeedAddPnt &= 65535;
            if ( pv->start < pv->end ) {
                *buffer8bit += ( pv->paula.volume * mix8[*pv->start] ) >> 6;
            }
            else {
                if ( pv->looping ) {
                    pv->start = pv->repeatStart;
                    pv->end = pv->repeatEnd;
                    if ( pv->start < pv->end ) {
                        *buffer8bit += ( pv->paula.volume * mix8[*pv->start] ) >> 6;
                    }
                }
            }
            buffer8bit += 2;
        }
    }
    return(buffer8bit);
}


void* LamePaulaMixer::fill16bitMono( void* buffer, udword numberOfSamples )
{
    sword* buffer16bit = static_cast<sword*>(buffer);
    for (ubyte v=0; v<_voices; v++) {
        buffer16bit = static_cast<sword*>(buffer);
        LamePaulaVoice *pv = _voice[v];
        for (udword n = numberOfSamples; n>0; n--) {
            if (v == 0) {
                *buffer16bit = zero16bit;
            }
            pv->stepSpeedAddPnt += pv->stepSpeedPnt;
            pv->start += ( pv->stepSpeed + ( pv->stepSpeedAddPnt > 65535 ) );
            pv->stepSpeedAddPnt &= 65535;
            if ( pv->start < pv->end ) {
                *buffer16bit += ( pv->paula.volume * mix16[*pv->start] ) >> 6;
            }
            else {
                if ( pv->looping ) {
                    pv->start = pv->repeatStart;
                    pv->end = pv->repeatEnd;
                    if ( pv->start < pv->end ) {
                        *buffer16bit += ( pv->paula.volume * mix16[*pv->start] ) >> 6;
                    }
                }
            }
            buffer16bit++;
        }
    }
    return(buffer16bit);
}


void* LamePaulaMixer::fill16bitStereo( void *buffer, udword numberOfSamples )
{
    sword* buffer16bit = (static_cast<sword*>(buffer))+1;
    for (ubyte v=1; v<_voices; v+=2 ) {
        buffer16bit = (static_cast<sword*>(buffer))+1;
        LamePaulaVoice *pv = _voice[v];
        for (udword n = numberOfSamples; n>0; n--) {
            if (v == 1) {
                *buffer16bit = zero16bit;
            }
            pv->stepSpeedAddPnt += pv->stepSpeedPnt;
            pv->start += ( pv->stepSpeed + ( pv->stepSpeedAddPnt > 65535 ) );
            pv->stepSpeedAddPnt &= 65535;
            if ( pv->start < pv->end ) {
                *buffer16bit += ( pv->paula.volume * mix16[*pv->start] ) >> 6;
            }
            else {
                if ( pv->looping ) {
                    pv->start = pv->repeatStart;
                    pv->end = pv->repeatEnd;
                    if ( pv->start < pv->end )
                    {
                        *buffer16bit += ( pv->paula.volume * mix16[*pv->start] ) >> 6;
                    }
                }
            }
            buffer16bit += 2;
        }
    }
    buffer16bit = static_cast<sword*>(buffer);
    for (ubyte v=0; v<_voices; v+=2 ) {
        buffer16bit = static_cast<sword*>(buffer);
        LamePaulaVoice *pv = _voice[v];
        for (udword n = numberOfSamples; n>0; n--) {
            if (v == 0) {
                *buffer16bit = zero16bit;
            }
            pv->stepSpeedAddPnt += pv->stepSpeedPnt;
            pv->start += ( pv->stepSpeed + ( pv->stepSpeedAddPnt > 65535 ) );
            pv->stepSpeedAddPnt &= 65535;
            if ( pv->start < pv->end ) {
                *buffer16bit += ( pv->paula.volume * mix16[*pv->start] ) >> 6;
            }
            else {
                if ( pv->looping ) {
                    pv->start = pv->repeatStart;
                    pv->end = pv->repeatEnd;
                    if ( pv->start < pv->end ) {
                        *buffer16bit += ( pv->paula.volume * mix16[*pv->start] ) >> 6;
                    }
                }
            }
            buffer16bit += 2;
        }
    }
    return(buffer16bit);
}
