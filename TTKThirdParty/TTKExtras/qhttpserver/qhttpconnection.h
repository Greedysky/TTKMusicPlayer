#ifndef QHTTPCONNECTION_H
#define QHTTPCONNECTION_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "qhttpserverfwd.h"

class QHttpConnectionPrivate;

/*! @brief The class of the http connection.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QHttpConnection : public QObject
{
    Q_OBJECT
public:
    explicit QHttpConnection(QTcpSocket *socket, QObject *parent = nullptr);

    void write(const QByteArray &data);
    void flush();
    void waitForBytesWritten();

Q_SIGNALS:
    void newRequest(QHttpRequest *request, QHttpResponse *response);
    void allBytesWritten();

private Q_SLOTS:
    void parseRequest();
    void responseDone();
    void socketDisconnected();
    void invalidateRequest();
    void updateWriteCount(qint64);

private:
    TTK_DECLARE_PRIVATE(QHttpConnection)

};

#endif
