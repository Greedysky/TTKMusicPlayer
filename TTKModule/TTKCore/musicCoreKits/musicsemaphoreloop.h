#ifndef MUSICSEMAPHORELOOP_H
#define MUSICSEMAPHORELOOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include <QEventLoop>
#include "musicglobaldefine.h"

/*! @brief The class of the semaphore event loop.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSemaphoreLoop : public QEventLoop
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSemaphoreLoop(QObject *parent = 0);

    ~MusicSemaphoreLoop();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Event loop start.
     */
    int exec(ProcessEventsFlags flags = AllEvents);

protected:
    QTimer m_timer;

};

#endif // MUSICSEMAPHORELOOP_H
