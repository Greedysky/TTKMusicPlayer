#ifndef MUSICSTRINGUTILS_H
#define MUSICSTRINGUTILS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
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

#include "musicglobaldefine.h"

/*! @brief The namespace of the utils string.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace String
    {
        /*!
         * Get lrc dir prefix path.
         */
        TTK_MODULE_EXPORT QString lrcPrefix();
        /*!
         * Get music dir prefix path.
         */
        TTK_MODULE_EXPORT QString musicPrefix();

        /*!
         * Get string prefix path.
         */
        TTK_MODULE_EXPORT QString stringPrefix(const QString &name);
        /*!
         * Get string prefix path.
         */
        TTK_MODULE_EXPORT QString stringPrefix(const QString &name, const QString &prefix);
        /*!
         * Get string suffix path.
         */
        TTK_MODULE_EXPORT QString stringSuffix(const QString &name);
        /*!
         * Get string suffix path.
         */
        TTK_MODULE_EXPORT QString stringSuffix(const QString &name, const QString &suffix);
        /*!
         * Get string splite path.
         */
        TTK_MODULE_EXPORT QString stringSplitToken(const QString &name);
        /*!
         * Get string splite path.
         */
        TTK_MODULE_EXPORT QString stringSplitToken(const QString &name, const QString &prefix, const QString &suffix, bool revert = true);
        /*!
         * Get split string value.
         */
        TTK_MODULE_EXPORT QStringList stringSplit(const QString &value, const QString &key = TTK_DEFAULT_STR);
        /*!
         * Remove given key in string, default is space.
         */
        TTK_MODULE_EXPORT QString removeStringToken(const QString &value, const QString &key = " ");

        /*!
         * Get new \r \n string key.
         */
        TTK_MODULE_EXPORT QString lineWrap();

        /*!
         * Check the given char is or not chinese character.
         */
        TTK_MODULE_EXPORT bool isChinese(const QChar &c);

        /*!
         * Get song artist name.
         */
        TTK_MODULE_EXPORT QString artistName(const QString &value, const QString &key = TTK_DEFAULT_STR);
        /*!
         * Get song name.
         */
        TTK_MODULE_EXPORT QString songName(const QString &value, const QString &key = TTK_DEFAULT_STR);

        /*!
         * Html string to plain.
         */
        TTK_MODULE_EXPORT QString convertHtmlToPlain(const QString &value);
        /*!
         * Illegal characters.
         */
        TTK_MODULE_EXPORT QStringList illegalCharacters();
        /*!
         * Illegal characters check.
         */
        TTK_MODULE_EXPORT bool isCharacterValid(const QString &value);
        /*!
         * Illegal characters replaced.
         */
        TTK_MODULE_EXPORT QString charactersReplaced(const QString &value);

    }
}

#endif // MUSICSTRINGUTILS_H
