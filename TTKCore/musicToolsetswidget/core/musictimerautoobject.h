#ifndef MUSICTIMERAUTOOBJECT_H
#define MUSICTIMERAUTOOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include <QTimer>

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the time auto run.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicTimerAutoObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicTimerAutoObject(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicTimerAutoObject();

    void runTimerAutoConfig();
    /*!
     * Run time auto from local file config.
     */

Q_SIGNALS:
    void setPlaySong(int index);
    /*!
     * Set current song to play in play list.
     */
    void setStopSong();
    /*!
     * Set current song to stop in play list.
     */

public Q_SLOTS:
    void timeout();
    /*!
     * Check current state by one minute.
     */

protected:
    bool systemIs64bit() const;
    /*!
     * Check current system is 64 bit or not.
     */
    void setShutdown();
    /*!
     * Set current system to shutdown.
     */

    QTimer m_timer;
    int m_hour;
    int m_second;
    int m_funcIndex;

};

#endif // MUSICTIMERAUTOOBJECT_H
