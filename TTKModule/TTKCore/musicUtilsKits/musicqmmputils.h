#ifndef MUSICQMMPUTILS_H
#define MUSICQMMPUTILS_H

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

#include "musicglobaldefine.h"

/*! @brief The namespace of the utils qmmp.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace QMMP
    {
        /*!
         * Get qmmp plugin dir path.
         */
        MUSIC_UTILS_EXPORT QString pluginPath(const QString &module, const QString &format);
        /*!
         * Update mid config file transfer.
         */
        MUSIC_UTILS_EXPORT void updateMidConfigFile();
        /*!
         * Enable visual wave control.
         */
        MUSIC_UTILS_EXPORT void enableVisualPlugin(const QString &name, bool enable);
        /*!
         * Update ripple spectrum config file transfer.
         */
        MUSIC_UTILS_EXPORT void updateRippleSpectrumConfigFile();
    }
}

#endif // MUSICQMMPUTILS_H
