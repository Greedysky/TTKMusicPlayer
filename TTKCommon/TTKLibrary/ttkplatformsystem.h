#ifndef TTKPLATFORMSYSTEM_H
#define TTKPLATFORMSYSTEM_H

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

/*! @brief The class of the platform system.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKPlatformSystem
{
public:
    enum class System
    {
        Win11,
        Win10,
        Win81,
        Win8,
        Win7,
        WinVista,
        WinXP,
        WinXPProfessionalEdition,
        Win2000,
        WinNT40,
        Win95,
        Win98,
        WinMe,
        WinServer2003,
        WinServer2003R2,
        WinServer2008,
        WinServer2008R2,
        WinServer2012,
        Linux,
        LinuxUbuntu,
        LinuxDebian,
        LinuxArch,
        LinuxCentOS,
        Mac,
        Unkown
    };

    /*!
     * Get system name.
     */
    static System systemName();

};

#endif // TTKPLATFORMSYSTEM_H
