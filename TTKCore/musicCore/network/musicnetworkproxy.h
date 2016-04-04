#ifndef MUSICNETWORKPROXY_H
#define MUSICNETWORKPROXY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTcpSocket>
#include "musicglobaldefine.h"

/*! @brief The class of set music network proxy.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkProxy : public QObject
{
    Q_OBJECT
public:
    explicit MusicNetworkProxy(QObject *parent = 0);
    ~MusicNetworkProxy();

    void setHostName(const QString &name);
    QString hostName() const;

    void setPort(int port);
    int port() const;

    void setType(int type);
    int type() const;

    void setUser(const QString &user);
    QString user() const;

    void setPassword(const QString &pwd);
    QString password() const;

    void testProxy();
    void applyProxy();

Q_SIGNALS:
    void testProxyStateChanged(bool state);

private Q_SLOTS:
    void testProxyChanged(QAbstractSocket::SocketState state);

protected:
    int m_port, m_type;
    QString m_hostName, m_user, m_pwd;
    QTcpSocket *m_testSocket;

};

#endif // MUSICNETWORKPROXY_H
