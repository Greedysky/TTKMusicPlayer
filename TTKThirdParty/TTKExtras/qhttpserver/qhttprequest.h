#ifndef QHTTPREQUEST_H
#define QHTTPREQUEST_H

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

#include <QUrl>
#include <QMetaEnum>
#include <QMetaType>
#include "qhttpserverfwd.h"

class QHttpRequestPrivate;

/*! @brief The class of the http request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QHttpRequest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HeaderHash headers READ headers)
    Q_PROPERTY(QString remoteAddress READ remoteAddress)
    Q_PROPERTY(quint16 remotePort READ remotePort)
    Q_PROPERTY(HttpMethod method READ method)
    Q_PROPERTY(QUrl url READ url)
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QString httpVersion READ httpVersion)

    Q_ENUMS(HttpMethod)

    /// @cond nodoc
    friend class QHttpConnection;
    friend class QHttpConnectionPrivate;
    /// @endcond

public:
    /// Request method enumeration.
    /** @note Taken from http_parser.h -- make sure to keep synced */
    enum HttpMethod {
        HTTP_DELETE = 0,
        HTTP_GET,
        HTTP_HEAD,
        HTTP_POST,
        HTTP_PUT,
        // pathological
        HTTP_CONNECT,
        HTTP_OPTIONS,
        HTTP_TRACE,
        // webdav
        HTTP_COPY,
        HTTP_LOCK,
        HTTP_MKCOL,
        HTTP_MOVE,
        HTTP_PROPFIND,
        HTTP_PROPPATCH,
        HTTP_SEARCH,
        HTTP_UNLOCK,
        // subversion
        HTTP_REPORT,
        HTTP_MKACTIVITY,
        HTTP_CHECKOUT,
        HTTP_MERGE,
        // upnp
        HTTP_MSEARCH,
        HTTP_NOTIFY,
        HTTP_SUBSCRIBE,
        HTTP_UNSUBSCRIBE,
        // RFC-5789
        HTTP_PATCH,
        HTTP_PURGE
    };

    /// The method used for the request.
    HttpMethod method() const;

    /// Returns the method string for the request.
    /** @note This will plainly transform the enum into a string HTTP_GET -> "HTTP_GET". */
    const QString methodString() const;

    /// The complete URL for the request.
    /** This includes the path and query string.
        @sa path() */
    const QUrl &url() const;

    /// The path portion of the query URL.
    /** @sa url() */
    const QString path() const;

    /// The HTTP version of the request.
    /** @return A string in the form of "x.x" */
    const QString &httpVersion() const;

    /// Return all the headers sent by the client.
    /** This returns a reference. If you want to store headers
        somewhere else, where the request may be deleted,
        make sure you store them as a copy.
        @note All header names are <b>lowercase</b>
        so that Content-Length becomes content-length etc. */
    const HeaderHash &headers() const;

    /// Get the value of a header.
    /** Headers are stored as lowercase so the input @c field will be lowercased.
        @param field Name of the header field
        @return Value of the header or empty string if not found. */
    QString header(const QString &field);

    /// IP Address of the client in dotted decimal format.
    const QString &remoteAddress() const;

    /// Outbound connection port for the client.
    quint16 remotePort() const;

    /// Request body data, empty for non POST/PUT requests.
    /** @sa storeBody() */
    const QByteArray &body() const;

    /// If this request was successfully received.
    /** Set before end() has been emitted, stating whether
        the message was properly received. This is false
        until the receiving the full request has completed. */
    bool successful() const;

    /// Utility function to make this request store all body data internally.
    /** If you call this when the request is received via QHttpServer::newRequest()
        the request will take care of storing the body data for you.
        Once the end() signal is emitted you can access the body data with
        the body() function.

        If you wish to handle incoming data yourself don't call this function
        and see the data() signal.
        @sa data() body() */
    void storeBody();

Q_SIGNALS:
    /// Emitted when new body data has been received.
    /** @note This may be emitted zero or more times
        depending on the request type.
        @param data Received data. */
    void data(const QByteArray &data);

    /// Emitted when the request has been fully received.
    /** @note The no more data() signals will be emitted after this. */
    void end();

private Q_SLOTS:
    void appendBody(const QByteArray &body);

private:
    explicit QHttpRequest(QHttpConnection *connection, QObject *parent = nullptr);

    static QString MethodToString(HttpMethod method);
    void setMethod(HttpMethod method);
    void setVersion(const QString &version);
    void setUrl(const QUrl &url);
    void setHeaders(const HeaderHash headers);
    void setSuccessful(bool success);
    void setRemoteAddress(const QString &address);
    void setRemotePort(quint16 port);

private:
    TTK_DECLARE_PRIVATE(QHttpRequest)

};

#endif
