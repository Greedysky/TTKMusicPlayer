// Future Composer audio decoder -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef FC_H
#define FC_H

#include <string>

#include "MyTypes.h"
#include "MyEndian.h"
#include "SmartPtr.h"
#include "Paula.h"

class FC : public PaulaPlayer {
 public:
    FC();
    ~FC();

    void setMixer(PaulaMixer*);
    bool isOurData(void*,unsigned long int);
    bool init(void*,udword,int=0,int=0);
    void run();
    void restart(int=0,int=0);
    void off();
    bool songEnd; // whether song end has been reached

    bool isSMOD;  // whether file is in Future Composer 1.0 - 1.3 format
    bool isFC14;  // whether file is in Future Composer 1.4 format

    std::string formatName;
    static const std::string SMOD_FORMAT_NAME;
    static const std::string FC14_FORMAT_NAME;
    static const std::string UNKNOWN_FORMAT_NAME;

    static const uword SMOD_SONGTAB_OFFSET = 0x0064;      // 100

    static const uword FC14_SMPHEADERS_OFFSET = 0x0028;   // 40
    static const uword FC14_WAVEHEADERS_OFFSET = 0x0064;  // 100
    static const uword FC14_SONGTAB_OFFSET = 0x00b4;      // 180

    static const uword TRACKTAB_ENTRY_LENGTH = 0x000d;    // 3*4+1
    static const uword PATTERN_LENGTH = 0x0040;           // 32*2
    static const ubyte PATTERN_BREAK = 0x49;

    static const ubyte SEQ_END = 0xE1;

    static const ubyte SNDMOD_LOOP = 0xE0;
    static const ubyte SNDMOD_END = SEQ_END;
    static const ubyte SNDMOD_SETWAVE = 0xE2;
    static const ubyte SNDMOD_CHANGEWAVE = 0xE4;
    static const ubyte SNDMOD_NEWVIB = 0xE3;
    static const ubyte SNDMOD_SUSTAIN = 0xE8;
    static const ubyte SNDMOD_NEWSEQ = 0xE7;
    static const ubyte SNDMOD_SETPACKWAVE = 0xE9;
    static const ubyte SNDMOD_PITCHBEND = 0xEA;

    static const ubyte ENVELOPE_LOOP = 0xE0;
    static const ubyte ENVELOPE_END = SEQ_END;
    static const ubyte ENVELOPE_SUSTAIN = 0xE8;
    static const ubyte ENVELOPE_SLIDE = 0xEA;

    static const int channels = 4;

 private:
    PaulaVoice _dummyVoices[channels];

    ubyte *input;
    udword inputLen;

    smartPtr<ubyte> fcBuf;   // for safe unsigned access
    smartPtr<sbyte> fcBufS;  // for safe signed access

    // This array will be moved behind the input file. So don't forget
    // to allocate additional sizeof(..) bytes.
    static const ubyte silenceData[8];

    // Index is AND 0x7f. Table is longer.
    static const uword periods[(5+6)*12+4];

    static const uword SMOD_waveInfo[47*4];
    static const ubyte SMOD_waveforms[];

    struct Admin {
        uword dmaFlags;  // which audio channels to turn on (AMIGA related)
        ubyte count;     // speed count
        ubyte speed;     // speed
        ubyte RScount;
        bool initialized;  // true => restartable
        bool isEnabled;    // player on => true, else false
    
        struct _moduleOffsets {
            udword trackTable;
            udword patterns;
            udword sndModSeqs;
            udword volModSeqs;
            udword silence;
        } offsets;

        int usedPatterns;
        int usedSndModSeqs;
        int usedVolModSeqs;
    } _admin;

    struct Sound {
        const ubyte* start;
        uword len, repOffs, repLen;
        // rest was place-holder (6 bytes)
    };
    // 10 samples/sample-packs
    // 80 waveforms
    Sound _sounds[10+80];

    struct CHdata
    {
        PaulaVoice *ch;  // paula and mixer interface
    
        uword dmaMask;
    
        udword trackStart;     // track/step pattern table
        udword trackEnd;
        uword trackPos;

        udword pattStart;
        uword pattPos;
    
        sbyte transpose;       // TR
        sbyte soundTranspose;  // ST
        sbyte seqTranspose;    // from sndModSeq
    
        ubyte noteValue;
    
        sbyte pitchBendSpeed;
        ubyte pitchBendTime, pitchBendDelayFlag;
    
        ubyte portaInfo, portDelayFlag;
        sword portaOffs;
    
        udword volSeq;
        uword volSeqPos;
    
        ubyte volSlideSpeed, volSlideTime, volSustainTime,
            volSlideDelayFlag;
    
        ubyte envelopeSpeed, envelopeCount;
    
        udword sndSeq;
        uword sndSeqPos;
    
        ubyte sndModSustainTime;
    
        ubyte vibFlag, vibDelay, vibSpeed,
            vibAmpl, vibCurOffs;
    
        sbyte volume;
        uword period;
    
        const ubyte* pSampleStart;
        uword repeatOffset;
        uword repeatLength;
        uword repeatDelay;
    };

    struct CHdata _CHdata[channels];

    void killChannel(CHdata&);
    void nextNote(CHdata&);
    void processModulation(CHdata&);
    void readModCommand(CHdata&);
    void processPerVol(CHdata&);
    inline void setWave(CHdata&, ubyte num);
    inline void readSeqTranspose(CHdata&);
    void volSlide(CHdata&);
};

#endif  // FC_H
