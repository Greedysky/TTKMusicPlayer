#include "qhttpresponse.h"

#include <QDateTime>
#include <QLocale>

#include "qhttpserver.h"
#include "qhttpconnection.h"

/*! @brief The class of the http response private.
 * @author Greedysky <greedysky@163.com>
 */
class QHttpResponsePrivate : public TTKPrivate<QHttpResponse>
{
public:
    QHttpResponsePrivate();

    void writeHeaders();
    void writeHead(int status);
    void writeHeader(const char *field, const QString &value);
    void write(const QByteArray &data);

    void setHeader(const QString &field, const QString &value);

    QHttpConnection *m_connection;

    HeaderHash m_headers;

    bool m_headerWritten;
    bool m_sentConnectionHeader;
    bool m_sentContentLengthHeader;
    bool m_sentTransferEncodingHeader;
    bool m_sentDate;
    bool m_keepAlive;
    bool m_last;
    bool m_useChunkedEncoding;
    bool m_finished;

};

QHttpResponsePrivate::QHttpResponsePrivate()
    : m_connection(nullptr),
      m_headerWritten(false),
      m_sentConnectionHeader(false),
      m_sentContentLengthHeader(false),
      m_sentTransferEncodingHeader(false),
      m_sentDate(false),
      m_keepAlive(true),
      m_last(false),
      m_useChunkedEncoding(false),
      m_finished(false)
{

}

void QHttpResponsePrivate::writeHeaders()
{
    if(m_finished)
    {
        return;
    }

    for(const QString &name : m_headers.keys())
    {
        QString value = m_headers[name];
        if(name.compare("connection", Qt::CaseInsensitive) == 0)
        {
            m_sentConnectionHeader = true;
            if(value.compare("close", Qt::CaseInsensitive) == 0)
            {
                m_last = true;
            }
            else
            {
                m_keepAlive = true;
            }
        }
        else if(name.compare("transfer-encoding", Qt::CaseInsensitive) == 0)
        {
            m_sentTransferEncodingHeader = true;
            if(value.compare("chunked", Qt::CaseInsensitive) == 0)
            {
                m_useChunkedEncoding = true;
            }
        }
        else if(name.compare("content-length", Qt::CaseInsensitive) == 0)
        {
            m_sentContentLengthHeader = true;
        }
        else if(name.compare("date", Qt::CaseInsensitive) == 0)
        {
            m_sentDate = true;
        }

        writeHeader(name.toLatin1(), value.toLatin1());
    }

    if(!m_sentConnectionHeader)
    {
        if(m_keepAlive && (m_sentContentLengthHeader || m_useChunkedEncoding))
        {
            writeHeader("Connection", "keep-alive");
        }
        else
        {
            m_last = true;
            writeHeader("Connection", "close");
        }
    }

    if(!m_sentContentLengthHeader && !m_sentTransferEncodingHeader)
    {
        if(m_useChunkedEncoding)
        {
            writeHeader("Transfer-Encoding", "chunked");
        }
        else
        {
            m_last = true;
        }
    }

    // Sun, 06 Nov 1994 08:49:37 GMT - RFC 822. Use QLocale::c() so english is used for month and
    // day.
    if(!m_sentDate)
    {
        writeHeader("Date",
                    QLocale::c().toString(QDateTime::currentDateTimeUtc(),
                                          "ddd, dd MMM yyyy hh:mm:ss") + " GMT");
    }
}

void QHttpResponsePrivate::writeHead(int status)
{
    if(m_finished)
    {
        qWarning() << "QHttpResponse::writeHead() Cannot write headers after response has finished.";
        return;
    }

    if(m_headerWritten){
        qWarning() << "QHttpResponse::writeHead() Already called once for this response.";
        return;
    }

    m_connection->write(QString("HTTP/1.1 %1 %2\r\n").arg(status).arg(STATUS_CODES[status]).toLatin1());
    writeHeaders();
    m_connection->write("\r\n");

    m_headerWritten = true;
}

void QHttpResponsePrivate::writeHeader(const char *field, const QString &value)
{
    if(!m_finished)
    {
        m_connection->write(field);
        m_connection->write(": ");
        m_connection->write(value.toUtf8());
        m_connection->write("\r\n");
    }
    else
    {
        qWarning() << "QHttpResponse::writeHeader() Cannot write headers after response has finished.";
    }
}

void QHttpResponsePrivate::write(const QByteArray &data)
{
    if(m_finished)
    {
        qWarning() << "QHttpResponse::write() Cannot write body after response has finished.";
        return;
    }

    if(!m_headerWritten)
    {
        qWarning() << "QHttpResponse::write() You must call writeHead() before writing body data.";
        return;
    }

    m_connection->write(data);
}

void QHttpResponsePrivate::setHeader(const QString &field, const QString &value)
{
    if(!m_finished)
    {
        m_headers[field] = value;
    }
    else
    {
        qWarning() << "QHttpResponse::setHeader() Cannot set headers after response has finished.";
    }
}



QHttpResponse::QHttpResponse(QHttpConnection *connection)
    : QObject(0)
{
    TTK_INIT_PRIVATE(QHttpResponse);
    TTK_D(QHttpResponse);
    d->m_connection = connection;
    connect(connection, SIGNAL(allBytesWritten()), this, SIGNAL(allBytesWritten()));
}

void QHttpResponse::setHeader(const QString &field, const QString &value)
{
    TTK_D(QHttpResponse);
    d->setHeader(field, value);
}

void QHttpResponse::writeHead(int status)
{
    TTK_D(QHttpResponse);
    d->writeHead(status);
}

void QHttpResponse::writeHead(StatusCode statusCode)
{
    TTK_D(QHttpResponse);
    d->writeHead(TTKStaticCast(int, statusCode));
}

void QHttpResponse::write(const QByteArray &data)
{
    TTK_D(QHttpResponse);
    d->write(data);
}

void QHttpResponse::flush()
{
    TTK_D(QHttpResponse);
    d->m_connection->flush();
}

void QHttpResponse::waitForBytesWritten()
{
    TTK_D(QHttpResponse);
    d->m_connection->waitForBytesWritten();
}

void QHttpResponse::end(const QByteArray &data)
{
    TTK_D(QHttpResponse);
    if(d->m_finished)
    {
        qWarning() << "QHttpResponse::end() Cannot write end after response has finished.";
        return;
    }

    if(data.length() > 0)
    {
        write(data);
    }
    d->m_finished = true;

    Q_EMIT done();

    /// @todo End connection and delete ourselves. Is this a still valid note?
    deleteLater();
}

void QHttpResponse::connectionClosed()
{
    TTK_D(QHttpResponse);
    d->m_finished = true;
    Q_EMIT done();
    deleteLater();
}

void QHttpResponse::setKeepAlive(bool alive)
{
    TTK_D(QHttpResponse);
    d->m_keepAlive = alive;
}

bool QHttpResponse::isLast() const
{
    TTK_D(QHttpResponse);
    return d->m_last;
}
