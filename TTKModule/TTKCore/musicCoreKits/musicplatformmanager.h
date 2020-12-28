#ifndef MUSICPLATFORMMANAGER_H
#define MUSICPLATFORMMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of the platform system manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlatformManager
{
    TTK_DECLARE_MODULE(MusicPlatformManager)
public:
    enum SystemType
    {
        Windows_10,
        Windows_8_1,
        Windows_8,
        Windows_7,
        Windows_Vista,
        Windows_XP,
        Windows_XP_Professional_x64_Edition,
        Windows_2000,
        Windows_NT_4_0,
        Windows_95,
        Windows_98,
        Windows_Me,
        Windows_Server_2003,
        Windows_Server_2003_R2,
        Windows_Server_2008,
        Windows_Server_2008_R2,
        Windows_Server_2012,
        Windows_Unix,
        Windows_Mac,
        Windows_Unkown
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
    void setLeftWinEnabled();
    /*!
     * Get local IE version.
     */
    int getLocalIEVersion() const;
#endif

    /*!
     * Get local DPI x.
     */
    int getLogicalDotsPerInchX() const;
    /*!
     * Get local DPI y.
     */
    int getLogicalDotsPerInchY() const;
    /*!
     * Get local DPI.
     */
    int getLogicalDotsPerInch() const;

    /*!
     * Get local system name.
     */
    SystemType getWindowSystemName() const;
    /*!
     * Set file link.
     */
    void setFileLink(const QString &src, const QString &des, const QString &ico, const QString &args, const QString &description);

protected:
#ifdef Q_OS_WIN
    /*!
     * Check current regedit item has exist.
     */
    bool currentNodeHasExist(const QString &key);
    /*!
     * Create music regedit item.
     */
    void createMusicRegedit(const QString &key);
#endif

};

#endif // MUSICPLATFORMMANAGER_H
