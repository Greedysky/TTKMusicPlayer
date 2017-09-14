#ifndef MUSICRUNTIMEMANAGER_H
#define MUSICRUNTIMEMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

/*! @brief The class of the app run time needed.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicRunTimeManager
{
public:
    /*!
     * Object contsructor.
     */
    MusicRunTimeManager();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Run.
     */
    void run() const;

    /*!
     * Get translator path.
     */
    QString translator() const;

};

#endif // MUSICRUNTIMEMANAGER_H
