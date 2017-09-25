#ifndef MUSICTIMERAUTOOBJECT_H
#define MUSICTIMERAUTOOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include <QTimer>

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the time check object.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOLSET_EXPORT MusicTimeObject
{
    int m_index;
    int m_hour;
    int m_minute;
    bool m_state;

    MusicTimeObject()
    {
        m_index = -1;
        m_hour = -1;
        m_minute = -1;
        m_state = false;
    }
}MusicTimeObject;
MUSIC_DECLARE_LISTS(MusicTimeObject)


/*! @brief The class of the time auto run.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicTimerAutoObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTimerAutoObject(QObject *parent = 0);

    ~MusicTimerAutoObject();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Run time auto from local file config.
     */
    void runTimerAutoConfig();

public Q_SLOTS:
    /*!
     * Check current state by one minute.
     */
    void timeout();

protected:
    /*!
     * Set current system to shutdown.
     */
    void setShutdown();

    QTimer m_timer;
    MusicTimeObjects m_timeObjects;

};

#endif // MUSICTIMERAUTOOBJECT_H
