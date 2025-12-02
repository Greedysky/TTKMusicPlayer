#ifndef TTKPROCESS_H
#define TTKPROCESS_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkmoduleexport.h"

/*! @brief The namespace of the process utils.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Kill process by name.
     */
    TTK_MODULE_EXPORT void killProcessByName(const QString &process, bool more = false);
    /*!
     * Kill process by name list.
     */
    TTK_MODULE_EXPORT void killProcessByName(const QStringList &processes, bool more = false);

}

#endif // TTKPROCESS_H
