#ifndef MUSICREGEDITMANAGER_H
#define MUSICREGEDITMANAGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicRegeditManager
{
public:
    explicit MusicRegeditManager();

    void setDesktopWallAutoStart(bool state);
    void getDesktopWallControlPanel(QString &originPath,
                                    int &originType);
    void setDesktopWallControlPanel(const QString &originPath,
                                    int originType);


};

#endif // MUSICREGEDITMANAGER_H
