#ifndef MUSICFORMATS_H
#define MUSICFORMATS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music formats.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicFormats
{
    TTK_DECLARE_MODULE(MusicFormats)
public:
    /*!
     * Get player supported formats.
     */
    static QStringList supportFormatsString();
    /*!
     * Get player supported formats.
     */
    static MStringListMap supportFormatsStringMap();
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
