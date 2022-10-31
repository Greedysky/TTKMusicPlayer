#ifndef MINIPROCESS_H
#define MINIPROCESS_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "ttkglobaldefine.h"

namespace TTKObject
{
    /*!
     * Kill process by name.
     */
    TTK_MODULE_EXPORT void killProcessByName(const QString &process);
    /*!
     * Kill process by name list.
     */
    TTK_MODULE_EXPORT void killProcessByName(const QStringList &processes);

}

#endif
