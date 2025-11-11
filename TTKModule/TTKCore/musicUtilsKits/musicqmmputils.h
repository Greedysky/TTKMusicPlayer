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
         * Get the visual plugin is enabled or not.
         */
        TTK_MODULE_EXPORT bool isVisualEnabled(const QString &name);
        /*!
         * Set whether the visual plugin is enabled.
         */
        TTK_MODULE_EXPORT void setVisualEnabled(const QString &name, bool enabled);

        /*!
         * Get the effect plugin is enabled or not.
         */
        TTK_MODULE_EXPORT bool isEffectEnabled(const QString &name);
        /*!
         * Set whether the effect plugin is enabled.
         */
        TTK_MODULE_EXPORT void setEffectEnabled(const QString &name, bool enabled);
        /*!
         * Set whether the all effect plugins is enabled.
         */
        TTK_MODULE_EXPORT void setEffectsEnabled(bool enabled);
        /*!
         * Get all effect plugin's property.
         */
        TTK_MODULE_EXPORT MusicPluginPropertyList effectModules();
        /*!
         * Show effect setting control widget.
         */
        TTK_MODULE_EXPORT void showEffectSetting(const QString &name);

        /*!
         * Update base config.
         */
        TTK_MODULE_EXPORT void updateBaseConfig();
        /*!
         * Update blur spectrum config.
         */
        TTK_MODULE_EXPORT void updateBlurConfig();
        /*!
         * Update voice effect config.
         */
        TTK_MODULE_EXPORT void updateVoiceConfig(bool update, int &value);


    }
}

#endif // MUSICQMMPUTILS_H
