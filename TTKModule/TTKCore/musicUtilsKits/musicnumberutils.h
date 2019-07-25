#ifndef MUSICNUMBERUTILS_H
#define MUSICNUMBERUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The namespace of the utils number.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Number
    {
        /*!
         * Transform value to cp float value.
         */
        MUSIC_UTILS_EXPORT QString size2Number(qint64 size);
        /*!
         * Transform value to cp int value.
         */
        MUSIC_UTILS_EXPORT QString size2NumberInt(qint64 size);
        /*!
         * Transform value to cp value.
         */
        MUSIC_UTILS_EXPORT QString size2Label(qint64 size);
        /*!
         * Transform value to speed float value.
         */
        MUSIC_UTILS_EXPORT QString speed2Label(qint64 size);
        /*!
         * Transform value to speed int value.
         */
        MUSIC_UTILS_EXPORT QString speed2LabelInt(qint64 size);

        /*!
         * Transform value to kb entity.
         */
        MUSIC_UTILS_EXPORT qreal sizeByte2KByte(qint64 size);
        /*!
         * Transform value to mb entity.
         */
        MUSIC_UTILS_EXPORT qreal sizeByte2MByte(qint64 size);
        /*!
         * Transform value to tb entity.
         */
        MUSIC_UTILS_EXPORT qreal sizeByte2TByte(qint64 size);

        /*!
         * Transform value to speed value base.
         */
        MUSIC_UTILS_EXPORT QString speed2LabelFromLabel(qint64 size, const QString &label);

        /*!
         * Transfrom bitrate to quality.
         */
        MUSIC_UTILS_EXPORT void transfromBitrateToQuality(int level, QString &bitString, QColor &color);
        /*!
         * Transfrom bitrate to quality.
         */
        MUSIC_UTILS_EXPORT void transfromBitrateToQuality(int level, QString &bitString);
        /*!
         * Transfrom bitrate to quality.
         */
        MUSIC_UTILS_EXPORT QString transfromBitrateToQuality(int level);
        /*!
         * Transfrom bitrate to level.
         */
        MUSIC_UTILS_EXPORT int transfromBitrateToLevel(const QString &bitrate);
        /*!
         * Transfrom bitrate to normal.
         */
        MUSIC_UTILS_EXPORT int transfromBitrateToNormal(int bitrate);
        /*!
         * Transfrom bitrate to normal.
         */
        MUSIC_UTILS_EXPORT int transfromBitrateToNormal(const QString &bitrate);

    }
}

#endif // MUSICNUMBERUTILS_H
