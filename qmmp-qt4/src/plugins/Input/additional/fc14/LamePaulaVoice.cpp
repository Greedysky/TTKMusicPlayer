// Simple AMIGA Paula Audio channel mixer -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#include "LamePaula.h"

LamePaulaVoice::LamePaulaVoice() {
    looping = true;
    off();
}


LamePaulaVoice::~LamePaulaVoice() {
    off();
}


void LamePaulaVoice::off() {
    isOn = false;
    paula.period = 0;
    paula.volume = 0;
}


void LamePaulaVoice::on() {
    takeNextBuf();
    isOn = true;
}


void LamePaulaVoice::takeNextBuf() {
    if (!isOn) {
        // If channel is off, take sample START parameters.
        start = paula.start;
        length = paula.length;
        length <<= 1;
        if (length == 0) {  // Paula would play $FFFF words (!)
            length = 1;
        }
        end = start+length;
    }
    repeatStart = paula.start;
    repeatLength = paula.length;
    repeatLength <<= 1;
    if (repeatLength == 0) {  // Paula would play $FFFF words (!)
        repeatLength = 1;
    }
    repeatEnd = repeatStart+repeatLength;
}
