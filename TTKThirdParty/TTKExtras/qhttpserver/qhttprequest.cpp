#include "qhttprequest.h"
#include "qhttpconnection.h"

QHttpRequest::QHttpRequest(QHttpConnection *connection, QObject *parent)
    : QObject(parent),
      m_connection(connection),
      m_url("http://localhost/"),
      m_success(false)
{
}

QHttpRequest::~QHttpRequest()
{
}

QString QHttpRequest::header(const QString &field)
{
    return m_headers.value(field.toLower(), "");
}

const HeaderHash &QHttpRequest::headers() const
{
    return m_headers;
}

const QString &QHttpRequest::httpVersion() const
{
    return m_version;
}

const QUrl &QHttpRequest::url() const
{
    return m_url;
}

const QString QHttpRequest::path() const
{
    return m_url.path();
}

const QString QHttpRequest::methodString() const
{
    return MethodToString(method());
}

QHttpRequest::HttpMethod QHttpRequest::method() const
{
    return m_method;
}

const QString &QHttpRequest::remoteAddress() const
{
    return m_remoteAddress;
}

quint16 QHttpRequest::remotePort() const
{
    return m_remotePort;
}

void QHttpRequest::storeBody()
{
    connect(this, SIGNAL(data(const QByteArray &)), this, SLOT(appendBody(const QByteArray &)),
            Qt::UniqueConnection);
}

QString QHttpRequest::MethodToString(HttpMethod method)
{
    int index = staticMetaObject.indexOfEnumerator("HttpMethod");
    return staticMetaObject.enumerator(index).valueToKey(method);
}

void QHttpRequest::appendBody(const QByteArray &body)
{
    m_body.append(body);
}
