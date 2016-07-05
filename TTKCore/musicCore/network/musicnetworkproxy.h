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
    /*!
     * Object contsructor.
     */
    ~MusicNetworkProxy();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setHostName(const QString &name);
    /*!
     * Set proxy hostName.
     */
    QString hostName() const;
    /*!
     * Get proxy hostName.
     */

    void setPort(int port);
    /*!
     * Set proxy port.
     */
    int port() const;
    /*!
     * Get proxy port.
     */

    void setType(int type);
    /*!
     * Set proxy type.
     */
    int type() const;
    /*!
     * Get proxy type.
     */

    void setUser(const QString &user);
    /*!
     * Set proxy username.
     */
    QString user() const;
    /*!
     * Get proxy username.
     */

    void setPassword(const QString &pwd);
    /*!
     * Set proxy password.
     */
    QString password() const;
    /*!
     * Get proxy password.
     */

    void testProxy();
    /*!
     * Test current proxy available.
     */
    void applyProxy();
    /*!
     * Apply current proxy.
     */

Q_SIGNALS:
    void testProxyStateChanged(bool state);
    /*!
     * Test current proxy available changed.
     */

private Q_SLOTS:
    void testProxyChanged(QAbstractSocket::SocketState state);
    /*!
     * Test current proxy available changed.
     */

protected:
    int m_port, m_type;
    QString m_hostName, m_user, m_pwd;
    QTcpSocket *m_testSocket;

};

#endif // MUSICNETWORKPROXY_H
