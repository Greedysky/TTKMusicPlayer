#ifndef MUSICLOCALPEER_H
#define MUSICLOCALPEER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicprivate.h"
#include "musicrunglobaldefine.h"

class MusicLocalPeerPrivate;

/*! @brief The class of the music local peer.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicLocalPeer : public QObject
{
    Q_OBJECT
public:
    explicit MusicLocalPeer(QObject *parent = 0, const QString &appId = QString());
    /*!
     * Object contsructor.
    */

    bool isClient();
    /*!
     * Current client is running or not.
    */
    bool sendMessage(const QString &message, int timeout);
    /*!
     * Send current message when the client in.
    */
    QString applicationId() const;
    /*!
     * Get current server id.
    */

Q_SIGNALS:
    void messageReceived(const QString &message);
    /*!
     * Emit when the current message received.
    */

protected Q_SLOTS:
    void receiveConnection();
    /*!
     * Current message received.
    */

private:
    TTK_DECLARE_PRIVATE(MusicLocalPeer)

};

#endif // MUSICLOCALPEER_H
