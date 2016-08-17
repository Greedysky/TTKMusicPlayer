#ifndef MUSICSERVICEAPPLICATION_H
#define MUSICSERVICEAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QApplication>
#include "musicprivate.h"
#include "musicserviceglobaldefine.h"

class MusicServiceApplicationPrivate;

/*! @brief The class of the music service application.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SERVICE_EXPORT MusicServiceApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MusicServiceApplication(int argc, char **argv);
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
    TTK_DECLARE_PRIVATE(MusicServiceApplication)

};

#endif // MUSICSERVICEAPPLICATION_H
