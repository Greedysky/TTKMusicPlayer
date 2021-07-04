#include "qhttprequest.h"
#include "qhttpconnection.h"

/*! @brief The class of the http request private.
 * @author Greedysky <greedysky@163.com>
 */
class QHttpRequestPrivate : public TTKPrivate<QHttpRequest>
{
public:
    QHttpRequestPrivate();

    QHttpConnection *m_connection;
    HeaderHash m_headers;
    QHttpRequest::HttpMethod m_method;
    QUrl m_url;
    QString m_version;
    QString m_remoteAddress;
    quint16 m_remotePort;
    QByteArray m_body;
    bool m_success;

};

QHttpRequestPrivate::QHttpRequestPrivate()
    : m_connection(nullptr),
      m_url("http://localhost/"),
      m_success(false)
{

}



QHttpRequest::QHttpRequest(QHttpConnection *connection, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(QHttpRequest);
    TTK_D(QHttpRequest);
    d->m_connection = connection;
}

QString QHttpRequest::header(const QString &field)
{
    TTK_D(QHttpRequest);
    return d->m_headers.value(field.toLower(), "");
}

const HeaderHash &QHttpRequest::headers() const
{
    TTK_D(QHttpRequest);
    return d->m_headers;
}

const QString &QHttpRequest::httpVersion() const
{
    TTK_D(QHttpRequest);
    return d->m_version;
}

const QUrl &QHttpRequest::url() const
{
    TTK_D(QHttpRequest);
    return d->m_url;
}

const QString QHttpRequest::path() const
{
    TTK_D(QHttpRequest);
    return d->m_url.path();
}

const QString QHttpRequest::methodString() const
{
    return MethodToString(method());
}

QHttpRequest::HttpMethod QHttpRequest::method() const
{
    TTK_D(QHttpRequest);
    return d->m_method;
}

const QString &QHttpRequest::remoteAddress() const
{
    TTK_D(QHttpRequest);
    return d->m_remoteAddress;
}

quint16 QHttpRequest::remotePort() const
{
    TTK_D(QHttpRequest);
    return d->m_remotePort;
}

const QByteArray &QHttpRequest::body() const
{
    TTK_D(QHttpRequest);
    return d->m_body;
}

bool QHttpRequest::successful() const
{
    TTK_D(QHttpRequest);
    return d->m_success;
}

void QHttpRequest::storeBody()
{
    connect(this, SIGNAL(data(const QByteArray &)), this, SLOT(appendBody(const QByteArray &)), Qt::UniqueConnection);
}

QString QHttpRequest::MethodToString(HttpMethod method)
{
    const int index = staticMetaObject.indexOfEnumerator("HttpMethod");
    return staticMetaObject.enumerator(index).valueToKey(method);
}

void QHttpRequest::appendBody(const QByteArray &body)
{
    TTK_D(QHttpRequest);
    d->m_body.append(body);
}

void QHttpRequest::setMethod(HttpMethod method)
{
    TTK_D(QHttpRequest);
    d->m_method = method;
}

void QHttpRequest::setVersion(const QString &version)
{
    TTK_D(QHttpRequest);
    d->m_version = version;
}

void QHttpRequest::setUrl(const QUrl &url)
{
    TTK_D(QHttpRequest);
    d->m_url = url;
}

void QHttpRequest::setHeaders(const HeaderHash headers)
{
    TTK_D(QHttpRequest);
    d->m_headers = headers;
}

void QHttpRequest::setSuccessful(bool success)
{
    TTK_D(QHttpRequest);
    d->m_success = success;
}

void QHttpRequest::setRemoteAddress(const QString &address)
{
    TTK_D(QHttpRequest);
    d->m_remoteAddress = address;
}

void QHttpRequest::setRemotePort(quint16 port)
{
    TTK_D(QHttpRequest);
    d->m_remotePort = port;
}
