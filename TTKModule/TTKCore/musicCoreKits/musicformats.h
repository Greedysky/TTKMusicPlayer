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

/*! @brief The class of the music formats.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicFormats
{
public:
    /*!
     * Get player supported formats.
     */
    static QStringList supportFormatsString();
    /*!
     * Get player supported formats.
     */
    static MusicObject::MStringsListMap supportFormatsStringMap();
    /*!
     * Get player supported formats filter.
     */
    static QStringList supportFormatsFilterString();
    /*!
     * Get player supported formats filter dialog.
     */
    static QStringList supportFormatsFilterDialogString();
    /*!
     * Get spek supported formats filter dialog.
     */
    static QStringList supportFormatsSpekString();
    /*!
     * Get playlist supported formats filter dialog.
     */
    static QStringList supportFormatsPlaylistDialogString();
    /*!
     * Get playlist supported formats dialog.
     */
    static QString supportFormatsPlaylistString();

};

#endif // MUSICFORMATS_H
