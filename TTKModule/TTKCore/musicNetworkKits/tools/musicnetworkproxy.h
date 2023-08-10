#ifndef MUSICNETWORKPROXY_H
#define MUSICNETWORKPROXY_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QTcpSocket>
#include "musicglobaldefine.h"

/*! @brief The class of the music network proxy.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNetworkProxy : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicNetworkProxy)
public:
    /*!
     * Object constructor.
     */
    explicit MusicNetworkProxy(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicNetworkProxy();

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

private:
    int m_port, m_type;
    QString m_hostName, m_user, m_pwd;
    QTcpSocket *m_socket;

};

#endif // MUSICNETWORKPROXY_H
