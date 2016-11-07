#ifndef MUSICNETWORKTHREAD_H
#define MUSICNETWORKTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicsingleton.h"

#define M_NETWORK_PTR (MusicSingleton<MusicNetworkThread>::createInstance())
#define NETWORK_DETECT_INTERVAL     5000             // second
#define NETWORK_REQUEST_ADDRESS     "www.baidu.com"  // ip

/*! @brief The class of thread to check current network.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkThread : public QObject
{
    Q_OBJECT
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */
    void start();
    /*!
     * Strat thread now.
     */
    void setBlockNetWork(int block);
    /*!
     * Set flag wheather to block current network.
     */
    inline void setNetWorkState(bool state) { m_networkState = state; }
    /*!
     * Set current network state.
     */
    inline bool isOnline() const { return m_networkState; }
    /*!
     * Check current network is on.
     */

Q_SIGNALS:
    void networkConnectionStateChanged(bool state);
    /*!
     * Network connection state changed.
     * default status is true, means connected network.
     */

public Q_SLOTS:
    void networkStateChanged();
    /*!
     * Network connection state changed.
     */

private:
    MusicNetworkThread();
    /*!
     * Object contsructor.
     */
    ~MusicNetworkThread();

    QTimer m_timer;
    bool m_networkState;

    DECLARE_SINGLETON_CLASS(MusicNetworkThread)
};

#endif // MUSICNETWORKTHREAD_H

