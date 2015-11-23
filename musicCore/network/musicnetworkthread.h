#ifndef MUSICNETWORKTHREAD_H
#define MUSICNETWORKTHREAD_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingleton.h"

#include <QTimer>
#include <QTcpSocket>

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

signals:
    void networkConnectionStateChanged(bool state);
    //default status is true, means connected network

public slots:
    void socketStateChanged(QAbstractSocket::SocketState socketState);
    void timerOut();

private:
    QTimer m_timer;
    QTcpSocket *m_client;
    bool m_networkState;
    bool m_blockNetWork;

    MusicNetworkThread();
    ~MusicNetworkThread();

    DECLARE_SINGLETON_CLASS(MusicNetworkThread)
};

#endif // MUSICNETWORKTHREAD_H

