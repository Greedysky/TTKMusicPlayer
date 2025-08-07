#ifndef MUSICSONGHELPER_H
#define MUSICSONGHELPER_H

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

#include "musicstringutils.h"

/*! @brief The namespace of the song helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Check current song playlist row is valid or not.
     */
    TTK_MODULE_EXPORT bool playlistRowValid(int index) noexcept;
    /*!
     * Remove track info in path.
     */
    TTK_MODULE_EXPORT QString trackRelatedPath(const QString &path);

    /*!
     * Generate song name.
     */
    TTK_MODULE_EXPORT QString generateSongName(const QString &title, const QString &artist) noexcept;
    /*!
     * Get song title name.
     */
    TTK_MODULE_EXPORT QString generateSongTitle(const QString &name, const QString &key = TTK_DEFAULT_STR);
    /*!
     * Get song artist name.
     */
    TTK_MODULE_EXPORT QString generateSongArtist(const QString &name, const QString &key = TTK_DEFAULT_STR);

    /*!
     * Generate network song play time.
    */
    TTK_MODULE_EXPORT QString generateNetworkSongTime(const QString &path);
    /*!
     * Generate network song path.
    */
    TTK_MODULE_EXPORT QString generateNetworkSongPath(const QString &path);

}

#endif // MUSICSONGHELPER_H
