#ifndef TTKDUMPER_H
#define TTKDUMPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicextrasglobaldefine.h"

class MiniDumper;

class MUSIC_EXTRAS_EXPORT TTKDumper
{
public:
    TTKDumper();
    ~TTKDumper();

    void run();

protected:
    MiniDumper *m_winDump;

};

#endif // TTKDUMPER_H
