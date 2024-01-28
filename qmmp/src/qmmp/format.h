/***************************************************************************
 * This file is part of the TTK qmmp plugin project
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

#ifndef FORMAT_H
#define FORMAT_H

#include <QMap>
#include <QStringList>
#include "qmmp_export.h"

/*! @brief The input file format support
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT Format
{
public:
    /*!
     * Check song is track valid or not.
     */
    static bool isTrack(const QString &url);
    /*!
     * Check song is dedirection valid or not.
     */
    static bool isRedirection(const QString &url);

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
    static QStringList supportMusicInputFormats();

};

#endif
