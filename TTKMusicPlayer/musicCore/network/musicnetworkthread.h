#ifndef MUSICNETWORKTHREAD_H
#define MUSICNETWORKTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingleton.h"

#include <QTimer>

#define M_NETWORK (MusicSingleton<MusicNetworkThread>::createInstance())

#define NETWORK_DETECT_INTERVAL 5000             // second
#define NETWORK_REQUEST_ADDRESS "www.baidu.com"  // ip

class MUSIC_NETWORK_EXPORT MusicNetworkThread : public QObject
{
    Q_OBJECT
public:
    void start();
    void setBlockNetWork(int block);
    inline bool isOnline() const {return m_networkState;}

Q_SIGNALS:
    void networkConnectionStateChanged(bool state);
    //default status is true, means connected network

public Q_SLOTS:
    void networkStateChanged();

private:
    QTimer m_timer;
    bool m_networkState;

    MusicNetworkThread();
    ~MusicNetworkThread();

    DECLARE_SINGLETON_CLASS(MusicNetworkThread)
};

#endif // MUSICNETWORKTHREAD_H

