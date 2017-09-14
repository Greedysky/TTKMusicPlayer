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
    /*!
     * Object contsructor.
    */
    explicit MusicLocalPeer(QObject *parent = 0, const QString &appId = QString());

    /*!
     * Current client is running or not.
    */
    bool isClient();

    /*!
     * Send current message when the client in.
    */
    bool sendMessage(const QString &message, int timeout);

    /*!
     * Get current server id.
    */
    QString applicationId() const;

Q_SIGNALS:
    /*!
     * Emit when the current message received.
    */
    void messageReceived(const QString &message);

protected Q_SLOTS:
    /*!
     * Current message received.
    */
    void receiveConnection();

private:
    TTK_DECLARE_PRIVATE(MusicLocalPeer)

};

#endif // MUSICLOCALPEER_H
