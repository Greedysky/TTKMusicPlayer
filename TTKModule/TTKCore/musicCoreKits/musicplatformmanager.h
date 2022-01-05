#ifndef MUSICPLATFORMMANAGER_H
#define MUSICPLATFORMMANAGER_H

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

/*! @brief The class of the platform system manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlatformManager
{
    TTK_DECLARE_MODULE(MusicPlatformManager)
public:
    enum SystemType
    {
        System_Win10,
        System_Win8_1,
        System_Win8,
        System_Win7,
        System_WinVista,
        System_WinXP,
        System_WinXP_Professional_x64_Edition,
        System_Win2000,
        System_WinNT_4_0,
        System_Win95,
        System_Win98,
        System_WinMe,
        System_WinServer_2003,
        System_WinServer_2003_R2,
        System_WinServer_2008,
        System_WinServer_2008_R2,
        System_WinServer_2012,
        System_Linux,
        System_Linux_Ubuntu,
        System_Linux_Debian,
        System_Linux_Arch,
        System_Linux_CentOS,
        System_Mac,
        System_Unkown
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
    SystemType systemName() const;
    /*!
     * Set file link.
     */
    void setFileLink(const QString &src, const QString &des, const QString &ico, const QString &args, const QString &description);

protected:
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
