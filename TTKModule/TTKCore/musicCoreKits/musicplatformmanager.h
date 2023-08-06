#ifndef MUSICPLATFORMMANAGER_H
#define MUSICPLATFORMMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

/*! @brief The class of the platform system manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlatformManager
{
    TTK_DECLARE_MODULE(MusicPlatformManager)
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

#ifdef Q_OS_WIN
    /*!
     * Check current is file associate.
     */
    bool isFileAssociate();
    /*!
     * Set music regedit associate file icon.
     */
    void setMusicRegeditAssociateFileIcon();

    /*!
     * Set left win key enable.
     */
    void enabledLeftWinMode();
    /*!
     * Get local IE version.
     */
    int windowsIEVersion() const;
    /*!
     * Windows start up.
     */
    void windowsStartUp(bool v) const;
#endif

    /*!
     * Get local DPI x.
     */
    int logicalDotsPerInchX() const;
    /*!
     * Get local DPI y.
     */
    int logicalDotsPerInchY() const;
    /*!
     * Get local DPI.
     */
    int logicalDotsPerInch() const;

    /*!
     * Get system name.
     */
    System systemName() const;

private:
#ifdef Q_OS_WIN
    /*!
     * Check current regedit item has exist.
     */
    bool currentNodeExist(const QString &key);
    /*!
     * Create music regedit item.
     */
    void createMusicRegedit(const QString &key);
#endif

};

#endif // MUSICPLATFORMMANAGER_H
