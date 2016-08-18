#ifndef MUSICRUNAPPLICATION_H
#define MUSICRUNAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QApplication>
#include "musicprivate.h"
#include "musicrunglobaldefine.h"

class MusicRunApplicationPrivate;

/*! @brief The class of the music run application.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicRunApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MusicRunApplication(int argc, char **argv);
    /*!
     * Object contsructor.
     */

    bool isRunning();
    /*!
     * Check current main window is running or not.
     */

private slots:
    void newLocalConnection();
    /*!
     * The new one main window comes.
     */

private:
    TTK_DECLARE_PRIVATE(MusicRunApplication)

};

#endif // MUSICRUNAPPLICATION_H
