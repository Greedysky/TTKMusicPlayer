#ifndef QGRYWRAP_H
#define QGRYWRAP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the gry wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QGryWrap
{
public:
    /*!
     * Encrypt gry by input.
     */
    static QString encrypt(const QString &in);
    /*!
     * Decrypt gry by input.
     */
    static QString decrypt(const QString &in);

};

#endif // QGRYWRAP_H
