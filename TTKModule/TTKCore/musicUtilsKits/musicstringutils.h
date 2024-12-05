#ifndef MUSICSTRINGUTILS_H
#define MUSICSTRINGUTILS_H

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

/*! @brief The namespace of the utils string.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace String
    {
        /*!
         * Get lrc dir prefix path.
         */
        TTK_MODULE_EXPORT QString lrcDirPrefix();
        /*!
         * Get music dir prefix path.
         */
        TTK_MODULE_EXPORT QString musicDirPrefix();

        /*!
         * Get string prefix path.
         */
        TTK_MODULE_EXPORT QString pefix(const QString &name);
        /*!
         * Get string prefix path.
         */
        TTK_MODULE_EXPORT QString pefix(const QString &name, const QString &prefix);
        /*!
         * Get string suffix path.
         */
        TTK_MODULE_EXPORT QString suffix(const QString &name);
        /*!
         * Get string suffix path.
         */
        TTK_MODULE_EXPORT QString suffix(const QString &name, const QString &suffix);

        /*!
         * Get string split path.
         */
        TTK_MODULE_EXPORT QString splitToken(const QString &name);
        /*!
         * Get string split path.
         */
        TTK_MODULE_EXPORT QString splitToken(const QString &name, const QString &prefix, const QString &suffix, bool revert = true);
        /*!
         * Get split string value.
         */
        TTK_MODULE_EXPORT QStringList split(const QString &value, const QString &key = TTK_DEFAULT_STR);
        /*!
         * Remove given key in string, default is space.
         */
        TTK_MODULE_EXPORT QString removeToken(const QString &value, const QString &key = TTK_SPACE);

        /*!
         * Check the given char is or not chinese character.
         */
        TTK_MODULE_EXPORT bool isChinese(const QChar &c);
        /*!
         * Check input url path is network path or not.
         */
        TTK_MODULE_EXPORT bool isNetworkUrl(const QString &path);

        /*!
         * Html string to plain.
         */
        TTK_MODULE_EXPORT QString convertHtmlToPlain(const QString &value);
        /*!
         * Illegal characters.
         */
        TTK_MODULE_EXPORT QStringList characters();
        /*!
         * Illegal characters check.
         */
        TTK_MODULE_EXPORT bool isCharValid(const QString &value);
        /*!
         * Illegal characters replaced.
         */
        TTK_MODULE_EXPORT QString charactersReplace(const QString &value);

    }
}

#endif // MUSICSTRINGUTILS_H
