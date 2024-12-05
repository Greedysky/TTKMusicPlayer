#ifndef MUSICQMMPUTILS_H
#define MUSICQMMPUTILS_H

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
#include "musicpluginproperty.h"

/*! @brief The namespace of the utils ttk qmmp.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace TTKQmmp
    {
        /*!
         * Get qmmp plugin dir path.
         */
        TTK_MODULE_EXPORT QString pluginPath(const QString &module, const QString &format);

        /*!
         * Enable effect module control.
         */
        TTK_MODULE_EXPORT void enabledEffectPlugin(bool enabled, const QString &name = {});
        /*!
         * Check effect is valid.
         */
        TTK_MODULE_EXPORT MusicPluginPropertyList effectPlugins();
        /*!
         * Show effect setting control.
         */
        TTK_MODULE_EXPORT void showEffectSetting(const QString &name);

        /*!
         * Enable visual module control.
         */
        TTK_MODULE_EXPORT void enabledVisualPlugin(const QString &name, bool enabled);

        /*!
         * Update base config.
         */
        TTK_MODULE_EXPORT void updateBaseConfig();
        /*!
         * Update blur spectrum config.
         */
        TTK_MODULE_EXPORT void updateBlurConfig();

    }
}

#endif // MUSICQMMPUTILS_H
