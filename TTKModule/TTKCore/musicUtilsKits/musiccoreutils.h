#ifndef MUSICCOREUTILS_H
#define MUSICCOREUTILS_H

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

/*! @brief The namespace of the utils core.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace Core
    {
        /*!
         * Sleep by millisecond.
         */
        TTK_MODULE_EXPORT void sleep(int ms);

        /*!
         * App version check.
         */
        TTK_MODULE_EXPORT bool appVersionCheck(const QString &o, const QString &d);

        /*!
         * Reset break point.
         */
        TTK_MODULE_EXPORT void resetBreakPoint() noexcept;
        /*!
         * Enable break point.
         */
        TTK_MODULE_EXPORT void enableBreakPoint(bool enable) noexcept;
        /*!
         * Break point enabled or not.
         */
        TTK_MODULE_EXPORT bool isBreakPointEnabled() noexcept;

    }
}

#endif // MUSICCOREUTILS_H
