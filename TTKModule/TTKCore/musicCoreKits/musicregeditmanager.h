#ifndef MUSICREGEDITMANAGER_H
#define MUSICREGEDITMANAGER_H

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

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of the windows regedit manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicRegeditManager
{
    TTK_DECLARE_MODULE(MusicRegeditManager)
public:
    /*!
     * Check current is file associate.
     */
    bool isFileAssociate();
    /*!
     * Set music regedit associate file icon.
     */
    void setMusicRegeditAssociateFileIcon();

    /*!
     * Set desktop wall auto start.
     */
    void setDesktopWallAutoStart(bool state);
    /*!
     * Get desktop wall control panel.
     */
    void getDesktopWallControlPanel(QString &originPath, int &originType);
    /*!
     * Set desktop wall control panel.
     */
    void setDesktopWallControlPanel(const QString &originPath, int originType);

    /*!
     * Set left win key enable.
     */
    void setLeftWinEnable();
    /*!
     * Get local IE version.
     */
    int getLocalIEVersion() const;
    /*!
     * Set file link.
     */
    void setFileLink(const QString &src, const QString &des, const QString &ico, const QString &args, const QString &description);

protected:
    /*!
     * Check current regedit item has exist.
     */
    bool currentNodeHasExist(const QString &key);
    /*!
     * Create music regedit item.
     */
    void createMusicRegedit(const QString &key);

};

#endif // MUSICREGEDITMANAGER_H
