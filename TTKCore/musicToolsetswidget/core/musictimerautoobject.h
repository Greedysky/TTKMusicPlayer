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

typedef struct MUSIC_TOOLSET_EXPORT TimeObjectPair
{
    int m_index;
    int m_hour;
    int m_minute;
    bool m_state;

    TimeObjectPair()
    {
        m_index = -1;
        m_hour = -1;
        m_minute = -1;
        m_state = false;
    }
}TimeObjectPair;
TTK_DECLARE_LISTS(TimeObjectPair)

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

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void runTimerAutoConfig();
    /*!
     * Run time auto from local file config.
     */

public Q_SLOTS:
    void timeout();
    /*!
     * Check current state by one minute.
     */

protected:
    void setShutdown();
    /*!
     * Set current system to shutdown.
     */

    QTimer m_timer;
    TimeObjectPairs m_timeObjects;

};

#endif // MUSICTIMERAUTOOBJECT_H
