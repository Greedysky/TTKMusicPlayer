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

#include "FC.h"

#include <cstring>
#ifdef FC_HAVE_NOTHROW
#include <new>
#endif

//#define DEBUG 1
//#define DEBUG2 1
//#define DEBUG3 1
#if defined(DEBUG) || defined(DEBUG2) || defined(DEBUG3)
#include <iomanip>
#include <iostream>
using namespace std;
#include "Dump.h"
#endif

FC::FC() {
    input = 0;
    inputLen = 0;
    _admin.initialized = false;

    // Set up some dummy voices to decouple the decoder from the mixer.
    for (int v=0; v<channels; v++) {
        _CHdata[v].ch = &_dummyVoices[v];
    }
    off();
}


FC::~FC() {
    delete[] input;
    inputLen = 0;
}


void FC::off() {
    _admin.isEnabled = false;
    _admin.dmaFlags = 0;
    
    for (int c=0; c<channels; c++) {
        _CHdata[c].ch->off();
    }
    // (AMIGA) Power-LED on = low-pass filter on.
    // May be simulated by external audio post-processing.
}


void FC::setMixer(PaulaMixer* mixer) {
    // Create needed number of voices and replace the dummies.
    mixer->init(channels);
    for (int v=0; v<channels; v++) {
        _CHdata[v].ch = mixer->getVoice(v);
        _CHdata[v].ch->off();
        killChannel(_CHdata[v]);
    }
}


bool FC::isOurData(void *data, unsigned long int length) {
    if ( length<5 ) {
        return false;
    }
    ubyte *d = static_cast<ubyte*>(data);
    // Check for "SMOD" ID (Future Composer 1.0 to 1.3).
    isSMOD = (d[0]==0x53 && d[1]==0x4D && d[2]==0x4F && d[3]==0x44 &&
              d[4]==0x00);
    // Check for "FC14" ID (Future Composer 1.4).
    isFC14 = (d[0]==0x46 && d[1]==0x43 && d[2]==0x31 && d[3]==0x34 &&
              d[4]==0x00);
    // (NOTE) A very few hacked "SMOD" modules exist which contain an ID
    // string "FC13". Although this could be supported easily, it should
    // NOT. Format detection must be able to rely on the ID field. As no
    // version of Future Composer has ever created a "FC13" ID, such hacked
    // modules are likely to be incompatible in other parts due to incorrect
    // conversion, e.g. effect parameters. It is like creating non-standard
    // module files whose only purpose is to confuse accurate music players.
    return (isSMOD || isFC14);
}


bool FC::init(void *data, udword length, int startStep, int endStep) {
    if ( !isOurData(data,length) ) {
        formatName = UNKNOWN_FORMAT_NAME;
        return false;
    }
    if (isSMOD)
        formatName = SMOD_FORMAT_NAME;
    else if (isFC14)
        formatName = FC14_FORMAT_NAME;
    
    udword copyLen = length+sizeof(silenceData);
    if (copyLen > inputLen) {
        delete[] input;
        inputLen = 0;
#ifdef FC_HAVE_NOTHROW
        if ( (input = new(std::nothrow) ubyte[copyLen]) == 0 ) {
#else
        if ( (input = new ubyte[copyLen]) == 0 ) {
#endif
            return false;
        }
    }
    memcpy(input,data,copyLen);
    inputLen = copyLen;

    // Set up smart pointers for signed and unsigned input buffer access.
    // Ought to be read-only (const), but this implementation appends
    // a few values to the end of the buffer (see further below).
    fcBufS.setBuffer((sbyte*)input,inputLen);
    fcBuf.setBuffer((ubyte*)input,inputLen);
    
    // (NOTE) This next bit is just for convenience.
    //
    // It is the only place where the module buffer is written to.
    //
    // Copy ``silent'' modulation sequence to end of FC module buffer so it
    // is in the address space of the FC module and thus allows using the
    // same smart pointers as throughout the rest of the code.
    _admin.offsets.silence = inputLen-sizeof(silenceData);
    for (ubyte i=0; i<sizeof(silenceData); i++) {
        fcBuf[_admin.offsets.silence+i] = silenceData[i];
    }

    // (NOTE) Instead of addresses (pointers) use 32-bit offsets everywhere.
    // This is just for added safety and convenience. One could range-check
    // each pointer/offset where appropriate to avoid segmentation faults
    // caused by damaged input data.
    
    if (isSMOD) {
        _admin.offsets.trackTable = SMOD_SONGTAB_OFFSET;
    }
    else {  // if (isFC14)
        _admin.offsets.trackTable = FC14_SONGTAB_OFFSET;
    }

#if defined(DEBUG)
    cout << "Track table (sequencer): " << endl;
    dumpLines(fcBuf,_admin.offsets.trackTable,
              readEndian(fcBuf[4],fcBuf[5],fcBuf[6],fcBuf[7]),
              TRACKTAB_ENTRY_LENGTH);
    cout << endl;
#endif

    // At +8 is offset to pattern data.
    _admin.offsets.patterns = readEndian(fcBuf[8],fcBuf[9],
                                           fcBuf[10],fcBuf[11]);
    // At +12 is length of patterns.
    // Divide by pattern length to get number of used patterns.
    // The editor is limited to 128 patterns.
    _admin.usedPatterns = readEndian(fcBuf[12],fcBuf[13],
                                      fcBuf[14],fcBuf[15])/PATTERN_LENGTH;
#if defined(DEBUG)
    cout << "Patterns: " << hex << _admin.usedPatterns << endl;
    dumpBlocks(fcBuf,_admin.offsets.patterns,
               _admin.usedPatterns*PATTERN_LENGTH,PATTERN_LENGTH);
#endif

    // At +16 is offset to first sound modulation sequence.
    _admin.offsets.sndModSeqs = readEndian(fcBuf[16],fcBuf[17],
                                            fcBuf[18],fcBuf[19]);
    // At +20 is total length of sound modulation sequences.
    // Divide by sequence length to get number of used sequences.
    // Each sequence is 64 bytes long.
    _admin.usedSndModSeqs = readEndian(fcBuf[20],fcBuf[21],
                                        fcBuf[22],fcBuf[23])/64;
#if defined(DEBUG)
    cout << "Sound modulation sequences: "
        << hex << _admin.usedSndModSeqs << endl;
    dumpBlocks(fcBuf,_admin.offsets.sndModSeqs,
               _admin.usedSndModSeqs*64,64);
#endif
    
    // At +24 is offset to first volume modulation sequence.
    _admin.offsets.volModSeqs = readEndian(fcBuf[24],fcBuf[25],
                                            fcBuf[26],fcBuf[27]);
    // At +28 is total length of volume modulation sequences.
    // Divide by sequence length to get number of used sequences.
    // Each sequence is 64 bytes long.
    _admin.usedVolModSeqs = readEndian(fcBuf[28],fcBuf[29],
                                        fcBuf[30],fcBuf[31])/64;
#if defined(DEBUG)
    cout << "Volume modulation sequences: "
        << hex << _admin.usedVolModSeqs << endl;
    dumpBlocks(fcBuf,_admin.offsets.volModSeqs,
               _admin.usedVolModSeqs*64,64);
#endif

#if defined(DEBUG)
    cout << "Samples:" << endl;
#endif
    // At +32 is module offset to start of samples.
    udword sampleOffset = readEndian(fcBuf[32],fcBuf[33],
                                     fcBuf[34],fcBuf[35]);
    udword sampleHeader = FC14_SMPHEADERS_OFFSET;
    // Max. 10 samples ($0-$9) or 10 sample-packs of 10 samples each.
    // Maximum sample length = 50000.
    // Previously: 32KB.
    // Total: 100000 (gee, old times).
    //
    // Sample length in words, repeat offset in bytes (but even),
    // Repeat length (in words, min=1). But in editor: all in bytes!
    //
    // One-shot sample (*recommended* method):
    // repeat offset = length*2 (to zero word at end of sample) and
    // replength = 1
    for (int sam = 0; sam < 10; sam++) {
        _sounds[sam].start = sampleOffset+fcBuf.tellBegin();
        // Sample length in words.
        uword sampleLength = readEndian(fcBuf[sampleHeader],
                                        fcBuf[sampleHeader+1]);
        _sounds[sam].len =  sampleLength;
        _sounds[sam].repOffs = readEndian(fcBuf[sampleHeader+2],
                                                   fcBuf[sampleHeader+3]);
        _sounds[sam].repLen = readEndian(fcBuf[sampleHeader+4],
                                                  fcBuf[sampleHeader+5]);

        // Safety treatment of "one-shot" samples.
        // 
        // We erase a word (two sample bytes) in the right place to
        // ensure that one-shot samples do not beep at their end
        // when looping on that part of the sample.
        //
        // It might not be strictly necessary to do this as it is
        // documented that FC is supposed to do this. But better be
        // safe than sorry.
        //
        // It is done because a cheap mixer is implemented which can
        // be used in the same way AMIGA custom chip Paula is used to
        // play quick-and-dirty one-shot samples.
        //
        // (NOTE) There is a difference in how one-shot samples are treated
        // in Future Composer 1.4 in comparison with older versions.
        if (isSMOD) {
            // Check whether this is a one-shot sample.
            if (_sounds[sam].repLen==1) {
                fcBuf[sampleOffset] = fcBuf[sampleOffset+1] = 0;
            }
        }

        // Skip to start of next sample data.
        sampleOffset += sampleLength;
        sampleOffset += sampleLength;
        
        if (isFC14) {
            udword pos = sampleOffset;
            // Make sure we do not erase the sample-pack ID "SSMP"
            // and check whether this is a one-shot sample.
            if (fcBuf[pos]!=0x53 && fcBuf[pos+1]!=0x53 &&
                fcBuf[pos+2]!=0x4D && fcBuf[pos+3]!=0x50 &&
                _sounds[sam].repLen==1) {
                fcBuf[pos] = fcBuf[pos+1] = 0;
            }
            // FC 1.4 keeps silent word behind sample.
            // Now skip that one to the next sample.
            //
            // (BUG-FIX) Add +2 to sample address is incorrect
            // for unused (i.e. empty) samples.
            //
            if (sampleLength != 0)
                sampleOffset += 2;
        }
        
        sampleHeader += 6;  // skip unused rest of header
        
#if defined(DEBUG)
        cout
            << dec << setw(6) << (long)_sounds[sam].start-(long)fcBuf.tellBegin() << " "
            << dec << setw(4) << (int)_sounds[sam].len*2L << " "
            << dec << setw(6) << (int)_sounds[sam].repOffs << " "
            << dec << setw(6) << (int)_sounds[sam].repLen*2L << " "
            << endl;
        dumpLines(fcBuf,(long)_sounds[sam].start-(long)fcBuf.tellBegin(),
                  16,16);
#endif
    }

#if defined(DEBUG)
    cout << "Waveforms:" << endl;
#endif
    // 80 waveforms ($0a-$59), max $100 bytes length each.
    if (isSMOD) {
        // Old FC has built-in waveforms.
        const ubyte* wavePtr = SMOD_waveforms;
        int infoIndex = 0;
        for (int wave = 0; wave < 47; wave++)
        {
            int sam = 10+wave;
            _sounds[sam].start = wavePtr+SMOD_waveInfo[infoIndex++];
            _sounds[sam].len = SMOD_waveInfo[infoIndex++];
            _sounds[sam].repOffs = SMOD_waveInfo[infoIndex++];
            _sounds[sam].repLen = SMOD_waveInfo[infoIndex++];
#if defined(DEBUG)
            cout
                << dec << setw(6) << (long)_sounds[sam].start-(long)wavePtr << " "
                << dec << setw(4) << (int)_sounds[sam].len*2L << " "
                << dec << setw(6) << (int)_sounds[sam].repOffs << " "
                << dec << setw(6) << (int)_sounds[sam].repLen*2L << " "
                << endl;
#endif
        }
    }
    else {  //if (isFC14)
        // At +36 is module offset to start of waveforms.
        udword waveOffset = readEndian(fcBuf[36],fcBuf[37],
                                       fcBuf[38],fcBuf[39]);
        // Module offset to array of waveform lengths.
        udword waveHeader = FC14_WAVEHEADERS_OFFSET;
        for (int wave = 0; wave < 80; wave++) {
            int sam = 10+wave;
            _sounds[sam].start = waveOffset+fcBuf.tellBegin();
            ubyte waveLength = fcBuf[waveHeader++];
            waveOffset += waveLength;
            waveOffset += waveLength;
            _sounds[sam].len = waveLength;
            _sounds[sam].repOffs = 0;
            _sounds[sam].repLen = waveLength;
#if defined(DEBUG)
            cout
                << dec << setw(6) << (long)_sounds[sam].start-(long)fcBuf.tellBegin() << " "
                << dec << setw(4) << (int)_sounds[sam].len*2L << " "
                << dec << setw(6) << (int)_sounds[sam].repOffs << " "
                << dec << setw(6) << (int)_sounds[sam].repLen*2L << " "
                << endl;
#endif
        }
    }

    _admin.initialized = true;
    restart(startStep,endStep);

    // (NOTE) The lowest octave in the period table is unreachable
    // due to a hardcoded range-check (see bottom).
    
    return true;
}


void FC::restart(int startStep, int endStep) {
    if (!_admin.initialized) {
        return;
    }
    // This one later on gets incremented prior to first comparison
    // (4+1=5 => get speed at first step).
    _admin.RScount = 4;
    // (NOTE) Some FC implementations instead count from 0 to 4.

    // At +4 is length of track table.
    udword trackTabLen = readEndian(fcBuf[4],fcBuf[5],fcBuf[6],fcBuf[7]);
#if defined(DEBUG)
    cout << "trackTabLen = " << hex << setw(8) << setfill('0') << trackTabLen << endl;
#endif

    off();
    for (ubyte c=0; c<channels; c++) {
        _CHdata[c].dmaMask = (1<<c);
        _CHdata[c].trackPos =
            _CHdata[c].pattPos = 0;
        _CHdata[c].volSlideSpeed =
            _CHdata[c].volSlideTime =
            _CHdata[c].volSlideDelayFlag =
            _CHdata[c].volSustainTime = 0;
        _CHdata[c].envelopeCount =
            _CHdata[c].envelopeSpeed = 1;
        _CHdata[c].vibSpeed =
            _CHdata[c].vibDelay =
            _CHdata[c].vibAmpl =
            _CHdata[c].vibCurOffs =
            _CHdata[c].vibFlag = 0;
        _CHdata[c].pitchBendSpeed = 
            _CHdata[c].pitchBendTime =
            _CHdata[c].pitchBendDelayFlag = 0;
        _CHdata[c].transpose =
            _CHdata[c].seqTranspose = 0;
        _CHdata[c].portaInfo =
            _CHdata[c].portaOffs =
            _CHdata[c].portDelayFlag = 0;
        _CHdata[c].volSeq = _admin.offsets.silence;
        _CHdata[c].sndSeq = _admin.offsets.silence+1;
        _CHdata[c].volSeqPos =
            _CHdata[c].sndSeqPos = 0;
        _CHdata[c].sndModSustainTime = 0;
        _CHdata[c].noteValue = 0;
        _CHdata[c].period = 0;
        _CHdata[c].volume = 0;

        killChannel(_CHdata[c]);

        // Track table start and end.
        _CHdata[c].trackStart = _admin.offsets.trackTable+c*3;
        _CHdata[c].trackEnd = _CHdata[c].trackStart+trackTabLen;
        _CHdata[c].trackPos = TRACKTAB_ENTRY_LENGTH*startStep;

        // 4*
        // PT = PATTERN
        // TR = TRANSPOSE
        // ST = SOUND TRANSPOSE
        //
        // 1*
        // RS = REPLAY SPEED
        
        // Read PT/TR/ST from track table.
        udword trackTabPos = _CHdata[c].trackStart+_CHdata[c].trackPos;
        uword pattern = fcBuf[trackTabPos++];  // PT
        _CHdata[c].pattStart = _admin.offsets.patterns+(pattern<<6);
        _CHdata[c].transpose = fcBuf[trackTabPos++];  // TR
        _CHdata[c].soundTranspose = fcBufS[trackTabPos];   // ST
    }

    // Get and set song speed.
    //
    // (BUG-FIX) Some FC players here read the speed from the first step.
    // This is the wrong speed if a sub-song is selected by skipping steps.
    // 
    // (NOTE) If it is skipped to a step where no replay step is specified,
    // the default speed is taken. This can be wrong. The only solution
    // would be to fast-forward the song, i.e. read the speed from all
    // steps up to the starting step.
    //
    _admin.speed = fcBuf[_CHdata[0].trackStart+_CHdata[0].trackPos+12];
    if (_admin.speed == 0) {
        _admin.speed = 3;  // documented default
    }
    _admin.count = _admin.speed;
    _admin.isEnabled = true;
    songEnd = false;
}
    
// --------------------------------------------------------------------------

void FC::run()
{
    if (!_admin.isEnabled)  // on/off flag
        return;
    
    if (--_admin.count == 0) {
        _admin.count = _admin.speed;  // reload

        // Prepare next note for each voice.
        nextNote(_CHdata[0]);
        nextNote(_CHdata[1]);
        nextNote(_CHdata[2]);
        nextNote(_CHdata[3]);
#if defined(DEBUG2)
        cout << endl << flush;
#endif
    }
    
    // Procedure calls in next loop will decide which
    // audio channel to turn on.
    _admin.dmaFlags = 0;

    for (ubyte c=0; c<channels; c++) {
        // Start or update instrument.
        processModulation(_CHdata[c]);
        
        _CHdata[c].ch->paula.period = _CHdata[c].period;
        _CHdata[c].ch->paula.volume = _CHdata[c].volume;
        
        if (_CHdata[c].repeatDelay != 0) {
            if (--_CHdata[c].repeatDelay == 1) {
                _CHdata[c].repeatDelay = 0;
                _CHdata[c].ch->paula.start = _CHdata[c].pSampleStart +
                    _CHdata[c].repeatOffset;
                _CHdata[c].ch->paula.length = _CHdata[c].repeatLength;
                _CHdata[c].ch->takeNextBuf();
            }
        }
    }
 
    // Finally decide which audio channels to start.
    // This could be moved into previous loop.
    for (ubyte c=0; c<channels; c++) {
        // Enable channel? Else, do not touch it.
        if ((_admin.dmaFlags&(1<<c)) != 0) {
            _CHdata[c].ch->on();
        }
    }
}

// --------------------------------------------------------------------------

void FC::killChannel(CHdata& CHXdata) {
    // The interface to a cheap Paula simulator/mixer.
    CHXdata.ch->off();
    CHXdata.ch->paula.start = fcBuf.tellBegin()+_admin.offsets.silence+1;
    // (NOTE) Some implementations set this to 0x0100.
    CHXdata.ch->paula.length = 1;
    CHXdata.ch->takeNextBuf();
}

void FC::nextNote(CHdata& CHXdata)
{
    // Get offset to (or address of) current pattern position.
    udword pattOffs = CHXdata.pattStart+CHXdata.pattPos;

    // Check for pattern end or whether pattern BREAK
    // command is set.
    if (CHXdata.pattPos==PATTERN_LENGTH
        || (isFC14 && fcBuf[pattOffs]==PATTERN_BREAK))
    {
        // End pattern.
        
#if defined(DEBUG3)
        if (fcBuf[pattOffs] == PATTERN_BREAK)
            cout << "--- PATTERN BREAK ---" << endl;
#endif
        
        // (NOTE) In order for pattern break to work correctly, the
        // pattern break value 0x49 must be at the same position in
        // each of the four patterns which are currently activated
        // for the four voices.
        //
        // Alternatively, one could advance all voices to the next
        // track step here in a 4-voice loop to make sure voices
        // stay in sync.
        
        CHXdata.pattPos = 0;
        
        // Advance one step in track table.
        CHXdata.trackPos += TRACKTAB_ENTRY_LENGTH;  // 0x000d
        udword trackOffs = CHXdata.trackStart+CHXdata.trackPos;
        
        // (BUG-FIX) Some FC players here apply a normal
        // compare-if-equal which is not accurate enough and
        // can cause the player to step beyond the song end.
        //
        // (BUG-FIX) Some FC14 modules have a pattern table length
        // which is not a multiple of 13. Hence we check whether
        // the currently activated table line would fit.
        
        if ((trackOffs+12) >= CHXdata.trackEnd)  // pattern table end?
        {
            CHXdata.trackPos = 0;     // restart by default
            trackOffs = CHXdata.trackStart;

            songEnd = true;
            
            // (NOTE) Some songs better stop here or reset all
            // channels to cut off any pending sounds.
        }

        // Step   Voice 1    Voice 2    Voice 3    Voice 4    Speed
        // SP     PT TR ST   PT TR ST   PT TR ST   PT TR ST   RS
        //
        // SP = STEP
        // PT = PATTERN
        // TR = TRANSPOSE
        // ST = SOUND TRANSPOSE
        // RS = REPLAY SPEED

        // Decide whether to read new song speed.
        if (++_admin.RScount == 5)
        {
            _admin.RScount = 1;
            ubyte newSpeed = fcBuf[trackOffs+12];  // RS (replay speed)
            if (newSpeed != 0)  // 0 would be underflow
            {
                _admin.count = _admin.speed = newSpeed;
            }
        }

        uword pattern = fcBuf[trackOffs++];  // PT
        CHXdata.transpose = fcBufS[trackOffs++];
        CHXdata.soundTranspose = fcBufS[trackOffs++];
        
        CHXdata.pattStart = _admin.offsets.patterns+(pattern<<6);
        // Get new pattern pointer (pattPos is 0 already, see above).
        pattOffs = CHXdata.pattStart;
    }

#if defined(DEBUG2)
    if (CHXdata.dmaMask==1
        && CHXdata.pattPos==0)
    {
        cout << endl;
        cout << "Step = " << hex << setw(4) << setfill('0') << CHXdata.trackPos/ TRACKTAB_ENTRY_LENGTH;
        cout << " | " << hex << setw(5) << setfill('0') << (int)CHXdata.trackStart << ", " << (int)(CHXdata.trackStart+CHXdata.trackPos) << ", " << (int)CHXdata.trackEnd << endl;
        udword tmp = CHXdata.trackStart+CHXdata.trackPos;
        for (int t = 0; t < 13; ++t)
            cout << hex << setw(2) << setfill('0') << (int)fcBuf[tmp++] << ' ';
        cout << endl;
        cout << endl;
    }
    
    cout << hex << setw(2) << setfill('0') << (int)fcBuf[pattOffs] << ' ' 
        << setw(2) << (int)fcBuf[pattOffs+1];
    if (CHXdata.dmaMask != 8)
        cout << " | ";
#endif

    // Process pattern entry.
    
    ubyte note = fcBuf[pattOffs++];
    ubyte info1 = fcBuf[pattOffs];  // info byte #1
    
    if (note != 0)
    {
        CHXdata.portaOffs = 0;   // reset portamento offset
        CHXdata.portaInfo = 0;   // stop port., erase old parameter
        
        // (BUG-FIX) Disallow signed underflow here.
        CHXdata.noteValue = note&0x7f;
        
        // (NOTE) Since first note is 0x01, first period at array
        // offset 0 cannot be accessed directly (i.e. without adding
        // transpose values from track table or modulation sequence).
        
        // Disable channel right now.
        CHXdata.ch->off();
        // Later enable channel.
        _admin.dmaFlags |= CHXdata.dmaMask;

        // Pattern offset stills points to info byte #1.
        // Get instrument/volModSeq number from info byte #1
        // and add sound transpose value from track table.
        uword sound = (fcBuf[pattOffs]&0x3f)+CHXdata.soundTranspose;
        //
        // (FC14 BUG-FIX) Better mask here to take care of overflow.
        //
        sound &= 0x3f;

        // (NOTE) Some FC players here put pattern info byte #1
        // into an unused byte variable at structure offset 9.
        
        udword seqOffs;  // the modulation sequence for this sound
        
        if (sound > (_admin.usedVolModSeqs-1))
        {
            seqOffs = _admin.offsets.silence;
        }
        else
        {
            seqOffs = _admin.offsets.volModSeqs+(sound<<6);
        }
        CHXdata.envelopeSpeed = CHXdata.envelopeCount = fcBuf[seqOffs++];
        // Get sound modulation sequence number.
        sound = fcBuf[seqOffs++];
        CHXdata.vibSpeed = fcBuf[seqOffs++];
        CHXdata.vibFlag = 0x40;  // vibrato UP at start
        CHXdata.vibAmpl = CHXdata.vibCurOffs = fcBuf[seqOffs++];
        CHXdata.vibDelay = fcBuf[seqOffs++];
        CHXdata.volSeq = seqOffs;
        CHXdata.volSeqPos = 0;
        CHXdata.volSustainTime = 0;
        
        if (sound > (_admin.usedSndModSeqs-1))
        {
            // (NOTE) Silent sound modulation sequence is different
            // from silent instrument definition sequence.
            seqOffs = _admin.offsets.silence+1;
        }
        else
        {
            seqOffs = _admin.offsets.sndModSeqs+(sound<<6);
        }
        CHXdata.sndSeq = seqOffs;
        CHXdata.sndSeqPos = 0;
        CHXdata.sndModSustainTime = 0;
    }

    // Portamento: bit 7 set = ON, bit 6 set = OFF, bits 5-0 = speed
    // New note resets and clears portamento working values.

    if ((info1&0x40) != 0)          // portamento OFF?
    {
        CHXdata.portaInfo = 0;   // stop port., erase old parameter
    }
    
    if ((info1&0x80) != 0)          // portamento ON?
    {
        //
        // (FC14 BUG-FIX) Kill portamento ON/OFF bits.
        //
        // Get portamento speed from info byte #2.
        // Info byte #2 is info byte #1 in next line of pattern,
        // Therefore the +2 offset.
        CHXdata.portaInfo = fcBuf[pattOffs+2]&0x3f;
    }

    // Advance to next pattern entry.
    CHXdata.pattPos += 2;
}

// --------------------------------------------------------------------------
// The order of func/proc calls might be confusing, but is necessary
// to simulate JMP instructions in the original player code without
// making use of ``goto''.

inline void FC::setWave(CHdata& CHXdata, ubyte num)
{
    CHXdata.pSampleStart = _sounds[num].start;
    CHXdata.ch->paula.start = _sounds[num].start;
    CHXdata.ch->paula.length = _sounds[num].len;
    CHXdata.ch->takeNextBuf();
    CHXdata.repeatOffset = _sounds[num].repOffs;
    CHXdata.repeatLength = _sounds[num].repLen;
    CHXdata.repeatDelay = 3;
}

inline void FC::readSeqTranspose(CHdata& CHXdata)
{
    CHXdata.seqTranspose = fcBufS[CHXdata.sndSeq+CHXdata.sndSeqPos];
    ++CHXdata.sndSeqPos;
}

void FC::processModulation(CHdata& CHXdata)
{
    if (CHXdata.sndModSustainTime != 0)
    {
        --CHXdata.sndModSustainTime;
        processPerVol(CHXdata);
        return;
    }
    readModCommand(CHXdata);
}

void FC::readModCommand(CHdata& CHXdata)
{
    udword seqOffs = CHXdata.sndSeq+CHXdata.sndSeqPos;

    // (NOTE) After each command (except LOOP, END, SUSTAIN,
    // and NEWVIB) follows a transpose value.
    
    if (fcBuf[seqOffs] == SNDMOD_LOOP)
    {
        CHXdata.sndSeqPos = fcBuf[seqOffs+1]&0x3f;
        // Calc new sequence address.
        seqOffs = CHXdata.sndSeq+CHXdata.sndSeqPos;
    }

    if (fcBuf[seqOffs] == SNDMOD_END)
    {
        processPerVol(CHXdata);
        return;
    }
    
    else if (fcBuf[seqOffs] == SNDMOD_SETWAVE)
    {
        // Disable channel right now.
        CHXdata.ch->off();
        // Enable channel later.
        _admin.dmaFlags |= CHXdata.dmaMask;
        // Restart envelope.
        CHXdata.volSeqPos = 0;
        CHXdata.envelopeCount = 1;
        
        setWave(CHXdata,fcBuf[seqOffs+1]);
        CHXdata.sndSeqPos += 2;
        
        readSeqTranspose(CHXdata);
        
        processPerVol(CHXdata);
        return;
    }

    else if (fcBuf[seqOffs] == SNDMOD_CHANGEWAVE)
    {
        setWave(CHXdata,fcBuf[seqOffs+1]);
        CHXdata.sndSeqPos += 2;
    
        readSeqTranspose(CHXdata);
        
        processPerVol(CHXdata);
        return;
    }

    else if (fcBuf[seqOffs] == SNDMOD_SETPACKWAVE)
    {
        // Disable channel right now.
        CHXdata.ch->off();
        // Enable channel later.
        _admin.dmaFlags |= CHXdata.dmaMask;
        
        uword i = fcBuf[seqOffs+1];  // sample/pack nr.
        if (i < 10)  // sample or waveform?
        {
            udword sndOffs = _sounds[i].start - fcBuf.tellBegin();
            // "SSMP"? sample-pack?
            if (fcBuf[sndOffs]==0x53 && fcBuf[sndOffs+1]==0x53 &&
                fcBuf[sndOffs+2]==0x4D && fcBuf[sndOffs+3]==0x50)
            {
                sndOffs += 4;
                // Skip header and 10*2 info blocks of size 16.
                udword smpStart = sndOffs+320;
                i = fcBuf[seqOffs+2];  // sample nr.
                i <<= 4;  // *16 (block size)
                sndOffs += i;
                smpStart += readEndian(fcBuf[sndOffs],fcBuf[sndOffs+1],
                                 fcBuf[sndOffs+2],fcBuf[sndOffs+3]);
                CHXdata.pSampleStart = smpStart+fcBuf.tellBegin();
                CHXdata.ch->paula.start = CHXdata.pSampleStart;
                CHXdata.ch->paula.length = readEndian(fcBuf[sndOffs+4],
                                                         fcBuf[sndOffs+5]);
                CHXdata.ch->takeNextBuf();
                
                // (FC14 BUG-FIX): Players set period here by accident.
                // m68k code move.l 4(a2),4(a3), but 6(a3) is period.
                
                CHXdata.repeatOffset = readEndian(fcBuf[sndOffs+6],
                                                     fcBuf[sndOffs+7]);
                CHXdata.repeatLength = readEndian(fcBuf[sndOffs+8],
                                                     fcBuf[sndOffs+9]);
                if (CHXdata.repeatLength == 1)
                {
                    // Erase first word behind sample to avoid beeping
                    // one-shot mode upon true emulation of Paula.
                    fcBuf[smpStart+CHXdata.repeatOffset] = 0;
                    fcBuf[smpStart+CHXdata.repeatOffset+1] = 0;
                }
                // Restart envelope.
                CHXdata.volSeqPos = 0;
                CHXdata.envelopeCount = 1;
                //
                CHXdata.repeatDelay = 3;
            }
        }
        CHXdata.sndSeqPos += 3;
        
        readSeqTranspose(CHXdata);
        
        processPerVol(CHXdata);
        return;
    }

    else if (fcBuf[seqOffs] == SNDMOD_NEWSEQ)
    {
        uword seq = fcBuf[seqOffs+1];
        CHXdata.sndSeq = _admin.offsets.sndModSeqs+(seq<<6);
        CHXdata.sndSeqPos = 0;
        // Recursive call (ought to be protected via a counter).
        readModCommand(CHXdata);
        return;
    }

    else if (fcBuf[seqOffs] == SNDMOD_SUSTAIN)
    {
        CHXdata.sndModSustainTime = fcBuf[seqOffs+1];
        CHXdata.sndSeqPos += 2;

        // Decrease sustain counter and decide whether to continue
        // to envelope modulation.
        // Recursive call (ought to be protected via a counter).
        processModulation(CHXdata);
        return;
    }

    else if (fcBuf[seqOffs] == SNDMOD_NEWVIB)
    {
        CHXdata.vibSpeed = fcBuf[seqOffs+1];
        CHXdata.vibAmpl = fcBuf[seqOffs+2];
        CHXdata.sndSeqPos += 3;
        
        processPerVol(CHXdata);
        return;
    }

    else if (fcBuf[seqOffs] == SNDMOD_PITCHBEND)
    {
        CHXdata.pitchBendSpeed = fcBufS[seqOffs+1];
        CHXdata.pitchBendTime = fcBuf[seqOffs+2];
        CHXdata.sndSeqPos += 3;
        
        readSeqTranspose(CHXdata);
    
        processPerVol(CHXdata);
        return;
    }

    else // Not a command, but a transpose value.
    {
        readSeqTranspose(CHXdata);
    
        processPerVol(CHXdata);
    }
}

// --------------------------------------------------------------------------
// (NOTE) This part of the code is not protected against a deadlock
// caused by damaged music module data.

void FC::volSlide(CHdata& CHXdata)
{
    // Following flag divides the volume sliding speed by two.
    CHXdata.volSlideDelayFlag ^= 0xff;  // = NOT
    if (CHXdata.volSlideDelayFlag != 0)
    {
        --CHXdata.volSlideTime;
        CHXdata.volume += CHXdata.volSlideSpeed;
        if (CHXdata.volume < 0)
        {
            CHXdata.volume = CHXdata.volSlideTime = 0;
        }
        
        // (NOTE) Most FC players do not check whether Paula's
        // maximum volume level is exceeded.
        
        if (CHXdata.volume > 64)
        {
            CHXdata.volume = 64;
            CHXdata.volSlideTime = 0;
        }
    }
}

void FC::processPerVol(CHdata& CHXdata)
{
    bool repeatVolSeq;  // JUMP/GOTO - WHILE conversion
    do
    {
        repeatVolSeq = false;
        
        // Sustain current volume level? NE => yes, EQ => no.
        if (CHXdata.volSustainTime != 0)
        {
            --CHXdata.volSustainTime;
        }

        // Slide volume? NE => yes, EQ => no.
        else if (CHXdata.volSlideTime != 0)
        {
            volSlide(CHXdata);
        }

        // Time to set next volume level? NE => no, EQ => yes.
        else if (--CHXdata.envelopeCount == 0)
        {
            CHXdata.envelopeCount = CHXdata.envelopeSpeed;

            bool readNextVal;  // JUMP/GOTO - WHILE conversion
            do
            {
                readNextVal = false;
                    
                udword seqOffs = CHXdata.volSeq+CHXdata.volSeqPos;
                ubyte command = fcBuf[seqOffs];

                switch (command)
                {
                 case ENVELOPE_SUSTAIN:
                    {
                        CHXdata.volSustainTime = fcBuf[seqOffs+1];
                        CHXdata.volSeqPos += 2;
                        // This shall loop to beginning of proc.
                        repeatVolSeq = true;
                        break;
                    }
                 case ENVELOPE_SLIDE:
                    {
                        CHXdata.volSlideSpeed = fcBuf[seqOffs+1];
                        CHXdata.volSlideTime = fcBuf[seqOffs+2];
                        CHXdata.volSeqPos += 3;
                        volSlide(CHXdata);
                        break;
                    }
                 case ENVELOPE_LOOP:
                    {
                        // Range check should be done here.
                        CHXdata.volSeqPos = (fcBuf[seqOffs+1]-5)&0x3f;
                        // (FC14 BUG) Some FC players here do not read a
                        // parameter at the new sequence position. They
                        // leave the pos value in d0, which then passes
                        // as parameter through all the command comparisons
                        // (this switch statement) in FC_effa() up to
                        // FC_effno().
                        readNextVal = true;
                        break;
                    }
                 case ENVELOPE_END:
                    {
                        break;
                    }
                 default:
                    {
                        // Read volume value and advance.
                        CHXdata.volume = fcBuf[seqOffs];
                        if (++CHXdata.volSeqPos > 0x3f) {
                            CHXdata.volSeqPos = 0x3f;
                        }
                        // Full range check for volume 0-64.
                        if (CHXdata.volume > 64) {
                            CHXdata.volume = 64;
                        }
                        else if (CHXdata.volume < 0) {
                            CHXdata.volume = 0;
                        }
                        break;
                    }
                }
            }
            while (readNextVal);
        }
    }
    while (repeatVolSeq);
    
    // Now determine note and period value to play.
    
    sdword tmp0, tmp1;

    tmp0 = CHXdata.seqTranspose;
    if (tmp0 >= 0)
    {
        tmp0 += CHXdata.noteValue;
        tmp0 += CHXdata.transpose;
        // (NOTE) Permit underflow at this point. Some modules
        // need it because--for some unknown reason--they work
        // with huge values such as transpose = 0x8c.
    }
    // else: lock note (i.e. transpose value from sequence is note to play)
    
#if defined(DEBUG2)
    if ((tmp0&0x7f)>0x53)
    {
        cout << "X ";
#if defined(DEBUG3)
        cout << "=== NOTE > 0x53 ===" << endl;
#endif
    }
#endif

    tmp0 &= 0x7f;
    tmp1 = tmp0<<1;           // *2 (later used to find octave)
    tmp0 = periods[tmp0];

    // Vibrato.
    //
    // Vibrato offset changes between [0,1,...,2*vibAmpl]
    // Offset minus vibAmpl is value to apply.
    
    if (CHXdata.vibDelay == 0)
    {
        uword noteTableOffset = tmp1;  // tmp1 is note*2;
    
        sword vibDelta = CHXdata.vibAmpl;
        vibDelta <<= 1;  // pos/neg amplitude delta
    
        // vibFlag bit 5: 0 => vibrato down, 1 => vibrato up
        //
        // (NOTE) In the original player code the vibrato half speed delay
        // flag (D6) in bit 0 is toggled but never checked, because the
        // vibrato flag byte will never get negative.
    
        tmp1 = CHXdata.vibCurOffs;

        if ((CHXdata.vibFlag&(1<<5))==0)
        {
            tmp1 -= CHXdata.vibSpeed;
            // Lowest value reached?
            if (tmp1 < 0)
            {
                tmp1 = 0;
                CHXdata.vibFlag |= (1<<5);   // switch to vibrato up
            }
        }
        else
        {
            tmp1 += CHXdata.vibSpeed;
            // Amplitude reached?
            if (tmp1 > vibDelta)
            {
                tmp1 = vibDelta;
                CHXdata.vibFlag &= ~(1<<5);  // switch to vibrato down
            }
        }

        CHXdata.vibCurOffs = tmp1;
    
        // noteTableOffset is note*2;
    
        tmp1 -= CHXdata.vibAmpl;

        // Octave 5 at period table byte-offset 96 contains the highest
        // period only. 96+160 = 256. This next bit ensures that vibrato
        // does not exceed the five octaves in the period table.
        // Octave 6 (but lowest!) is FC14 only.
        noteTableOffset += 160;  // + $a0
        while (noteTableOffset < 256)
        {
            tmp1 <<= 1;    // double vibrato value for each octave
            noteTableOffset += 2*12;  // advance octave index
        };
        tmp0 += tmp1;  // apply vibrato to period
    
        // (NOTE) Questionable code here in the original player sources.
        // Although bit 0 of D6 is toggled, the code (see above) that
        // checks it is unreachable.
    }
    else
    {
        --CHXdata.vibDelay;
    
        // (NOTE) Questionable code here in existing FC players. Although
        // bit 0 of D6 is toggled, the code that checks it is unreachable.
        // That bad code has not been converted.
    }

    // Portamento.

    // (NOTE) As of FC 1.4 portamento plays at half speed compared to
    // old versions.
    
    // Following flag divides the portamento speed by two
    // for FC14 modules.
    CHXdata.portDelayFlag ^= 0xff;  // = NOT
    if (isSMOD || CHXdata.portDelayFlag!=0)
    {
        sbyte param = CHXdata.portaInfo;
        if (param != 0)
        {
            if (param > 0x1f)  // > 0x20 = portamento down
            {
                param &= 0x1f;
                param = (-param);
            }
            CHXdata.portaOffs -= param;
        }
    }

    // Pitchbend.
    
    // Following flag divides the pitch bending speed by two.
    CHXdata.pitchBendDelayFlag ^= 0xff;  // not
    if (CHXdata.pitchBendDelayFlag != 0)
    {
        if (CHXdata.pitchBendTime != 0)
        {
            --CHXdata.pitchBendTime;
            sbyte speed = CHXdata.pitchBendSpeed;
            if (speed != 0)
            {
                CHXdata.portaOffs -= speed;
            }
        }
    }
    
    tmp0 += CHXdata.portaOffs;
    if (tmp0 <= 0x0070)
    {
        tmp0 = 0x0071;
    }
    // (NOTE) This should be 0x1ac0, but the extra low octave has
    // been added in FC 1.4 and is a non-working hack due to this
    // range-check (see header file).
    if (tmp0 > 0x0d60)
    {
        tmp0 = 0x0d60;
    }
    CHXdata.period = tmp0;
}
