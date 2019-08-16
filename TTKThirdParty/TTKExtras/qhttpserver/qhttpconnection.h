#ifndef QHTTPCONNECTION_H
#define QHTTPCONNECTION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "qhttpserverfwd.h"

/*! @brief The class of the http connection.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QHttpConnection : public QObject
{
    Q_OBJECT
public:
    explicit QHttpConnection(QTcpSocket *socket, QObject *parent = nullptr);
    virtual ~QHttpConnection();

    void write(const QByteArray &data);
    void flush();
    void waitForBytesWritten();

Q_SIGNALS:
    void newRequest(QHttpRequest *, QHttpResponse *);
    void allBytesWritten();

private Q_SLOTS:
    void parseRequest();
    void responseDone();
    void socketDisconnected();
    void invalidateRequest();
    void updateWriteCount(qint64);

private:
    static int MessageBegin(http_parser *parser);
    static int Url(http_parser *parser, const char *at, size_t length);
    static int HeaderField(http_parser *parser, const char *at, size_t length);
    static int HeaderValue(http_parser *parser, const char *at, size_t length);
    static int HeadersComplete(http_parser *parser);
    static int Body(http_parser *parser, const char *at, size_t length);
    static int MessageComplete(http_parser *parser);

private:
    QTcpSocket *m_socket;
    http_parser *m_parser;
    http_parser_settings *m_parserSettings;

    // Since there can only be one request at any time even with pipelining.
    QHttpRequest *m_request;

    QByteArray m_currentUrl;
    // The ones we are reading in from the parser
    HeaderHash m_currentHeaders;
    QString m_currentHeaderField;
    QString m_currentHeaderValue;

    // Keep track of transmit buffer status
    qint64 m_transmitLen;
    qint64 m_transmitPos;
};

#endif
