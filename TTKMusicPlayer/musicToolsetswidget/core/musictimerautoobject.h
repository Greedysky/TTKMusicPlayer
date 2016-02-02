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

class MUSIC_CORE_EXPORT MusicTimerAutoObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicTimerAutoObject(QObject *parent = 0);
    ~MusicTimerAutoObject();

    void runTimerAutoConfig();

Q_SIGNALS:
    void setPlaySong(int index);
    void setStopSong();

public Q_SLOTS:
    void timeout();

protected:
    bool systemIs64bit() const;
    void setShutdown();

    QTimer m_timer;
    int m_hour;
    int m_second;
    int m_funcIndex;

};

#endif // MUSICTIMERAUTOOBJECT_H
