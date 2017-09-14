#ifndef MUSICREGEDITMANAGER_H
#define MUSICREGEDITMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of the windows regedit manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicRegeditManager
{
public:
    /*!
     * Get class object name.
     */
    static QString getClassName();

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
