#ifndef MUSICLOCALPEER_H
#define MUSICLOCALPEER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicprivate.h"
#include "musicrunglobaldefine.h"

class MusicLocalPeerPrivate;

class MUSIC_RUN_EXPORT MusicLocalPeer : public QObject
{
    Q_OBJECT
public:
    explicit MusicLocalPeer(QObject *parent = 0, const QString &appId = QString());

    bool isClient();
    bool sendMessage(const QString &message, int timeout);
    QString applicationId() const;

Q_SIGNALS:
    void messageReceived(const QString &message);

protected Q_SLOTS:
    void receiveConnection();

private:
    TTK_DECLARE_PRIVATE(MusicLocalPeer)

};

#endif // MUSICLOCALPEER_H
