#ifndef MUSICSTRINGUTILS_H
#define MUSICSTRINGUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
        MUSIC_UTILS_EXPORT QString lrcPrefix();
        /*!
         * Get music dir prefix path.
         */
        MUSIC_UTILS_EXPORT QString musicPrefix();

        /*!
         * Get string prefix path.
         */
        MUSIC_UTILS_EXPORT QString stringPrefix(const QString &name);
        /*!
         * Get string prefix path.
         */
        MUSIC_UTILS_EXPORT QString stringPrefix(const QString &name, const QString &prefix);
        /*!
         * Get string suffix path.
         */
        MUSIC_UTILS_EXPORT QString stringSuffix(const QString &name);
        /*!
         * Get string suffix path.
         */
        MUSIC_UTILS_EXPORT QString stringSuffix(const QString &name, const QString &suffix);
        /*!
         * Get string splite path.
         */
        MUSIC_UTILS_EXPORT QString stringSplitToken(const QString &name);
        /*!
         * Get string splite path.
         */
        MUSIC_UTILS_EXPORT QString stringSplitToken(const QString &name, const QString &prefix, const QString &suffix, bool revert = false);
        /*!
         * Get split string value.
         */
        MUSIC_UTILS_EXPORT QStringList stringSplit(const QString &value, const QString &key = "-");
        /*!
         * Remove given key in string, default is space.
         */
        MUSIC_UTILS_EXPORT QString removeStringToken(const QString &value, const QString &key = " ");

        /*!
         * Get new \r \n string key.
         */
        MUSIC_UTILS_EXPORT QString newlines();

        /*!
         * Check the given char is or not chinese character.
         */
        MUSIC_UTILS_EXPORT bool isChinese(const QChar &c);

        /*!
         * Get song artist name.
         */
        MUSIC_UTILS_EXPORT QString artistName(const QString &value, const QString &key = "-");
        /*!
         * Get song name.
         */
        MUSIC_UTILS_EXPORT QString songName(const QString &value, const QString &key = "-");

        /*!
         * Illegal characters check.
         */
        MUSIC_UTILS_EXPORT QStringList illegalCharacters();
        /*!
         * Illegal characters check.
         */
        MUSIC_UTILS_EXPORT bool illegalCharactersCheck(const QString &value);
        /*!
         * Illegal characters replaced.
         */
        MUSIC_UTILS_EXPORT QString illegalCharactersReplaced(const QString &value);

        /*!
         * Transform colors string to color list.
         */
        MUSIC_UTILS_EXPORT QList<QColor> readColorConfig(const QString &value);
        /*!
         * Transform color to color string.
         */
        MUSIC_UTILS_EXPORT QString writeColorConfig(const QColor &color);
        /*!
         * Transform color list to colors string.
         */
        MUSIC_UTILS_EXPORT QString writeColorConfig(const QList<QColor> &colors);

    }
}

#endif // MUSICSTRINGUTILS_H
