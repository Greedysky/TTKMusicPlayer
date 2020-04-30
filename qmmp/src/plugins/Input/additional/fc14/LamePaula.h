// Simple AMIGA Paula Audio channel mixer -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#ifndef LAMEPAULA_H
#define LAMEPAULA_H

#include "Paula.h"

class LamePaulaMixer;

class LamePaulaVoice : public PaulaVoice
{
 public:
    LamePaulaVoice();
    ~LamePaulaVoice();

    void on();
    void off();
    void takeNextBuf();    // take parameters from paula.* (or just to repeat.*)

    friend class LamePaulaMixer;

 private:
    bool isOn;
    bool looping;  // whether to loop sample buffer continously (PAULA emu)
    
    const ubyte* start;
    const ubyte* end;
    udword length;
    
    const ubyte* repeatStart;
    const ubyte* repeatEnd;
    udword repeatLength;
    
    uword curPeriod;
    udword stepSpeed;
    udword stepSpeedPnt;
    udword stepSpeedAddPnt;
};

class LamePaulaMixer : public PaulaMixer
{
 public:
    LamePaulaMixer();
    ~LamePaulaMixer();
    void init(udword freq, ubyte bits, ubyte channels, uword zero);
    void init(ubyte voices);
    PaulaVoice* getVoice(ubyte); 

    void fillBuffer(void* buffer, udword bufferLen, PaulaPlayer *player);

 private:
    void setReplayingSpeed();
    void setBpm(uword bpm);
    void end();

    void* (LamePaulaMixer::*_fillFunc)(void*, udword);

    void* fill8bitMono(void*, udword);
    void* fill8bitStereo(void*, udword);
    void* fill16bitMono(void*, udword);
    void* fill16bitStereo(void*, udword);

    static const int _maxVoices = 32;
    LamePaulaVoice* _voice[_maxVoices];
    int _voices;

    udword _pcmFreq;
    ubyte _bitsPerSample;
    ubyte _channels;
    uword _zero;

    static const udword AMIGA_CLOCK_PAL = 3546895;
    static const udword AMIGA_CLOCK_NTSC = 3579546;
    const udword AMIGA_CLOCK;

    sbyte mix8[256];
    sword mix16[256];

    ubyte zero8bit;   // ``zero''-sample
    uword zero16bit;  // either signed or unsigned
    
    ubyte bufferScale;
    
    udword samplesAdd;
    udword samplesPnt;
    uword samples, samplesOrg;
    
    udword toFill;
    
    ubyte emptySample;
};

#endif  // LAMEPAULA_H
