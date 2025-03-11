#ifndef MUSICFORMATS_H
#define MUSICFORMATS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
     * Check song is track valid or not.
     */
    static bool isTrack(const QString &url) noexcept;
    /*!
     * Check song is dedirection valid or not.
     */
    static bool isRedirection(const QString &url) noexcept;

public:
    /*!
     * Get player supported formats.
     */
    static QStringList supportMusicFormats() noexcept;
    /*!
     * Get player supported input filter formats.
     */
    static QStringList supportMusicInputFilterFormats() noexcept;
    /*!
     * Get player supported input format .
     */
    static QString supportMusicInputFormats() noexcept;

    /*!
     * Get spek supported input filter formats.
     */
    static QStringList supportSpekInputFilterFormats() noexcept;
    /*!
     * Get spek supported input formats.
     */
    static QString supportSpekInputFormats() noexcept;

    /*!
     * Get playlist supported input formats.
     */
    static QString supportPlaylistInputFormats() noexcept;
    /*!
     * Get playlist supported output formats.
     */
    static QString supportPlaylistOutputFormats() noexcept;

};

#endif // MUSICFORMATS_H
