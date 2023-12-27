#ifndef QHTTPSERVER_H
#define QHTTPSERVER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#define QHTTPSERVER_VERSION_MAJOR 0
#define QHTTPSERVER_VERSION_MINOR 1
#define QHTTPSERVER_VERSION_PATCH 0

#include <QHostAddress>
#include "qhttpserverfwd.h"

class QHttpServerPrivate;

/// Maps status codes to string reason phrases
extern QHash<int, QString> STATUS_CODES;

/*! @brief The class of the http server.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QHttpServer : public QObject
{
    Q_OBJECT
public:
    /// Construct a new HTTP Server.
    /** @param parent Parent QObject for the server. */
    explicit QHttpServer(QObject *parent = nullptr);

    /// Start the server by bounding to the given @c address and @c port.
    /** @note This function returns immediately, it does not block.
        @param address Address on which to listen to. Default is to listen on
        all interfaces which means the server can be accessed from anywhere.
        @param port Port number on which the server should run.
        @return True if the server was started successfully, false otherwise.
        @sa listen(quint16) */
    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 80);

    /// Starts the server on @c port listening on all interfaces.
    /** @param port Port number on which the server should run.
        @return True if the server was started successfully, false otherwise.
        @sa listen(const QHostAddress&, quint16) */
    bool listen(quint16 port);

    /// Stop the server and listening for new connections.
    void close();

Q_SIGNALS:
    /// Emitted when a client makes a new request to the server.
    /** The slot should use the given @c request and @c response
        objects to communicate with the client.
        @param request New incoming request.
        @param response Response object to the request. */
    void newRequest(QHttpRequest *request, QHttpResponse *response);

private Q_SLOTS:
    void newConnection();

private:
    TTK_DECLARE_PRIVATE(QHttpServer)

};

#endif
