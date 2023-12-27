#ifndef QHTTPRESPONSE_H
#define QHTTPRESPONSE_H

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

#include "qhttpserverfwd.h"

class QHttpResponsePrivate;

/*! @brief The class of the http response.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QHttpResponse : public QObject
{
    Q_OBJECT
public:
    /// HTTP status code.
    enum StatusCode {
        STATUS_CONTINUE = 100,
        STATUS_SWITCH_PROTOCOLS = 101,
        STATUS_OK = 200,
        STATUS_CREATED = 201,
        STATUS_ACCEPTED = 202,
        STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
        STATUS_NO_CONTENT = 204,
        STATUS_RESET_CONTENT = 205,
        STATUS_PARTIAL_CONTENT = 206,
        STATUS_MULTIPLE_CHOICES = 300,
        STATUS_MOVED_PERMANENTLY = 301,
        STATUS_FOUND = 302,
        STATUS_SEE_OTHER = 303,
        STATUS_NOT_MODIFIED = 304,
        STATUS_USE_PROXY = 305,
        STATUS_TEMPORARY_REDIRECT = 307,
        STATUS_BAD_REQUEST = 400,
        STATUS_UNAUTHORIZED = 401,
        STATUS_PAYMENT_REQUIRED = 402,
        STATUS_FORBIDDEN = 403,
        STATUS_NOT_FOUND = 404,
        STATUS_METHOD_NOT_ALLOWED = 405,
        STATUS_NOT_ACCEPTABLE = 406,
        STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
        STATUS_REQUEST_TIMEOUT = 408,
        STATUS_CONFLICT = 409,
        STATUS_GONE = 410,
        STATUS_LENGTH_REQUIRED = 411,
        STATUS_PRECONDITION_FAILED = 412,
        STATUS_REQUEST_ENTITY_TOO_LARGE = 413,
        STATUS_REQUEST_URI_TOO_LONG = 414,
        STATUS_REQUEST_UNSUPPORTED_MEDIA_TYPE = 415,
        STATUS_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        STATUS_EXPECTATION_FAILED = 417,
        STATUS_INTERNAL_SERVER_ERROR = 500,
        STATUS_NOT_IMPLEMENTED = 501,
        STATUS_BAD_GATEWAY = 502,
        STATUS_SERVICE_UNAVAILABLE = 503,
        STATUS_GATEWAY_TIMEOUT = 504,
        STATUS_HTTP_VERSION_NOT_SUPPORTED = 505
    };

    /// @cond nodoc
    friend class QHttpConnection;
    friend class QHttpConnectionPrivate;
    /// @endcond

public Q_SLOTS:
    /// Sets a response header @c field to @c value.
    /** @note You must call this with all your custom headers
        before calling writeHead(), write() or end().
        @param field Header field to be set.
        @param value Header value to be set. */
    void setHeader(const QString &field, const QString &value);

    /// Writes the header section of the response
    /// using @c status as the response status code.
    /** @param statusCode Status code for the response.
        @note Any headers should be set before
        invoking this function with setHeader(). */
    void writeHead(int statusCode);

    /** @overload */
    void writeHead(StatusCode statusCode);

    /// Writes a block of @c data to the client.
    /** @note writeHead() must be called before this function. */
    void write(const QByteArray &data);

    /// Flushes the written data to the client.
    /** @note writeHead() must be called before this function. */
    void flush();

    /// Waiting for bytes to be written. See QAbstractSocket::waitForBytesWritten in the Qt documentation
    /** @note writeHead() must be called before this function. */
    void waitForBytesWritten();

    /// End/finish the response.
    /** Data will be flushed to the underlying socket
        and the connection itself will be closed if
        this is the last response.

        This will emit done() and queue this object
        for deletion. For details see \ref memorymanagement.
        @param data Optional data to be written before finishing. */
    void end(const QByteArray &data = "");

Q_SIGNALS:
    /// Emitted when all the data has been sent
    /** This signal indicates that the underlaying socket has transmitted all
        of it's buffered data. It is possible to implement memory-efficient
        file transfers by calling \ref write() for a block of data only after
        receiving this signal. */
    void allBytesWritten();

    /// Emitted when the response is finished.
    /** You should <b>not</b> interact with this object
        after done() has been emitted as the object
        has already been scheduled for deletion. */
    void done();

private Q_SLOTS:
    void connectionClosed();

private:
    explicit QHttpResponse(QHttpConnection *connection);

    /** @overload */
    void setKeepAlive(bool alive);
    /** @overload */
    bool isLast() const;

private:
    TTK_DECLARE_PRIVATE(QHttpResponse)

};

#endif
