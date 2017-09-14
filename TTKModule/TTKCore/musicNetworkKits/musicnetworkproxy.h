#ifndef MUSICNETWORKPROXY_H
#define MUSICNETWORKPROXY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicNetworkProxy(QObject *parent = 0);

    ~MusicNetworkProxy();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set proxy hostName.
     */
    void setHostName(const QString &name);
    /*!
     * Get proxy hostName.
     */
    QString hostName() const;

    /*!
     * Set proxy port.
     */
    void setPort(int port);
    /*!
     * Get proxy port.
     */
    int port() const;

    /*!
     * Set proxy type.
     */
    void setType(int type);
    /*!
     * Get proxy type.
     */
    int type() const;

    /*!
     * Set proxy username.
     */
    void setUser(const QString &user);
    /*!
     * Get proxy username.
     */
    QString user() const;

    /*!
     * Set proxy password.
     */
    void setPassword(const QString &pwd);
    /*!
     * Get proxy password.
     */
    QString password() const;

    /*!
     * Test current proxy available.
     */
    void testProxy();
    /*!
     * Apply current proxy.
     */
    void applyProxy();

Q_SIGNALS:
    /*!
     * Test current proxy available changed.
     */
    void testProxyStateChanged(bool state);

private Q_SLOTS:
    /*!
     * Test current proxy available changed.
     */
    void testProxyChanged(QAbstractSocket::SocketState state);

protected:
    int m_port, m_type;
    QString m_hostName, m_user, m_pwd;
    QTcpSocket *m_testSocket;

};

#endif // MUSICNETWORKPROXY_H
