#ifndef MUSICCODECUTILS_H
#define MUSICCODECUTILS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
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

#include "musicglobaldefine.h"

/*! @brief The namespace of the utils codec.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace Codec
    {
        /*!
         * Set string to unicode string by format.
         */
        TTK_MODULE_EXPORT QString toUnicode(const char *chars, const char *format = "GBK");
        /*!
         * Set string to unicode string by format.
         */
        TTK_MODULE_EXPORT QString toUnicode(const QByteArray &chars, const char *format = "GBK");
        /*!
         * Set string from unicode string by format.
         */
        TTK_MODULE_EXPORT QByteArray fromUnicode(const QString &chars, const char *format = "GBK");
        /*!
         * Set local codec by format.
         */
        TTK_MODULE_EXPORT void setLocalCodec(const char *format = "UTF-8");

    }
}

#endif // MUSICCODECUTILS_H
