#ifndef TTKLOGOUTPUT_H
#define TTKLOGOUTPUT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

/*! @brief The namespace of the log output.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Initiailize log module.
     */
    TTK_MODULE_EXPORT void initiailizeLog(const QString &module);
    /*!
     * Install log output handler.
     */
    TTK_MODULE_EXPORT void installLogHandler();
    /*!
     * Remove log output handler.
     */
    TTK_MODULE_EXPORT void removeLogHandler();

    /*!
     * Set max size for log file.
     */
    TTK_MODULE_EXPORT void setLogMaxSize(size_t maxSize);
    /*!
     * Remove log file after max second.
     */
    TTK_MODULE_EXPORT void setExpireSecond(size_t maxSecond);
    /*!
     * Set log output dir path.
     */
    TTK_MODULE_EXPORT void setOutputPath(const QString &dir);

}

#endif // TTKLOGOUTPUT_H
