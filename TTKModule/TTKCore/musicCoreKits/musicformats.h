#ifndef MUSICFORMATS_H
#define MUSICFORMATS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include "musicglobaldefine.h"

/*! @brief The class of the music formats.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFormats
{
    TTK_DECLARE_MODULE(MusicFormats)
public:
    /*!
     * Check song track is valid or not.
     */
    static bool songTrackValid(const QString &url);

public:
    /*!
     * Get player supported formats.
     */
    static QStringList supportMusicFormats();
    /*!
     * Get player supported input filter formats.
     */
    static QStringList supportMusicInputFilterFormats();
    /*!
     * Get player supported input format .
     */
    static QString supportMusicInputFormats();

    /*!
     * Get spek supported input filter formats.
     */
    static QStringList supportSpekInputFilterFormats();
    /*!
     * Get spek supported input formats.
     */
    static QString supportSpekInputFormats();

    /*!
     * Get playlist supported input formats.
     */
    static QString supportPlaylistInputFormats();
    /*!
     * Get playlist supported output formats.
     */
    static QString supportPlaylistOutputFormats();

};

#endif // MUSICFORMATS_H
