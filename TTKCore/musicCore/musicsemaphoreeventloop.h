#ifndef MUSICSEMAPHOREEVENTLOOP_H
#define MUSICSEMAPHOREEVENTLOOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMutexLocker>
#include "musicglobaldefine.h"

/*! @brief The class of the semaphore event loop.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSemaphoreEventLoop : public QObject
{
    Q_OBJECT
public:
    explicit MusicSemaphoreEventLoop(QObject* parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicSemaphoreEventLoop(int sem, QObject* parent = 0);
    /*!
     * Object contsructor.
     */

    inline void setSemaphore(int sem) { m_semaphore = sem;}
    /*!
     * Set semaphore.
     */
    inline int semaphore() const { return m_semaphore;}
    /*!
     * Get semaphore.
     */
     
    void exec();
    /*!
     * Start the thread to mutex locked.
     */
     
public slots:
    void quit();
    /*!
     * Stop the thread to mutex unlocked.
     */
     
private:
    QMutex m_mutex;
    volatile int m_semaphore;

};

#endif // MUSICSEMAPHOREEVENTLOOP_H
