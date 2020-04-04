// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#ifndef PAULA_H
#define PAULA_H

#include "MyTypes.h"

class PaulaVoice {
 public:
    // Paula
    struct _paula {
        const ubyte* start;  // start address
        uword length;        // length in 16-bit words
        uword period;
        uword volume;        // 0-64
    } paula;

    virtual void on();
    virtual void off();
    virtual void takeNextBuf();   // take parameters from paula.* (or just to repeat.*)
};

class PaulaMixer {
 public:
    virtual void init(ubyte voices) = 0;
    virtual PaulaVoice* getVoice(ubyte) = 0;
};

class PaulaPlayer {
 public:
    virtual void run() = 0;
};

#endif  // PAULA_H
