#ifndef MUSICRULESANALYSIS_H
#define MUSICRULESANALYSIS_H

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

#include "musicobject.h"
#include "ttkmoduleexport.h"

/*! @brief The class of the file name rules analysis.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRulesAnalysis
{
public:
    /*!
     * Get all rule filters.
     */
    static QStringList rules() noexcept;
    /*!
     * Get rule filter by index.
     */
    static QString rule(int index) noexcept;
    /*!
     * Parse rule value string to normal.
     */
    static QString parse(const TTK::MusicSongInformation &info, const QString &value);

};

#endif // MUSICRULESANALYSIS_H
