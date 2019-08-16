#ifndef MUSICLRCDEFINES_H
#define MUSICLRCDEFINES_H

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

#include <QStringList>
#include "musicglobaldefine.h"

/*! @brief The class of the lrc defines.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcDefines
{
    TTK_DECLARE_MODULE(MusicLrcDefines)
public:
    /*!
     * Object contsructor.
     */
    MusicLrcDefines();

    /*!
     * Get inline lrc size.
     */
    QStringList getInlineLrcSize() const;
    /*!
     * Get desktop lrc size.
     */
    QStringList getDesktopLrcSize() const;

    /*!
     * Find inline lrc index.
     */
    int findInlineLrcIndex(int size) const;
    /*!
     * Find inline pre size.
     */
    int findInlinePreSize(int index) const;
    /*!
     * Find inline next size.
     */
    int findInlineNextSize(int index) const;
    /*!
     * Find inline first size.
     */
    int findInlineFirstSize() const;
    /*!
     * Find inline last size.
     */
    int findInlineLastSize() const;

    /*!
     * Find desktop lrc index.
     */
    int findDesktopLrcIndex(int size) const;
    /*!
     * Find desktop pre size.
     */
    int findDesktopPreSize(int index) const;
    /*!
     * Find desktop next size.
     */
    int findDesktopNextSize(int index) const;
    /*!
     * Find desktop first size.
     */
    int findDesktopFirstSize() const;
    /*!
     * Find desktop last size.
     */
    int findDesktopLastSize() const;

protected:
    QStringList m_inlineLrcSize, m_desktopLrcSize;

};

#endif // MUSICLRCDEFINES_H
