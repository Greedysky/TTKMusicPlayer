#ifndef MUSICFORMATS_H
#define MUSICFORMATS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

class MUSIC_CORE_EXPORT MusicFormats
{
public:
    static QStringList supportFormatsString();
    /*!
     * Get player supported formats.
     */
    static MusicObject::MStringsListMap supportFormatsStringMap();
    /*!
     * Get player supported formats.
     */
    static QStringList supportFormatsFilterString();
    /*!
     * Get player supported formats filter.
     */
    static QStringList supportFormatsFilterDialogString();
    /*!
     * Get player supported formats filter dialog.
     */
    static QStringList supportFormatsSpekString();
    /*!
     * Get spek supported formats filter dialog.
     */

};

#endif // MUSICFORMATS_H
