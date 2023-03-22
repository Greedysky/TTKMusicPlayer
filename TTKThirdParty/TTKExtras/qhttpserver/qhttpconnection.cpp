#include "qhttpconnection.h"

#include <QTcpSocket>
#include <QHostAddress>

#include "http_parser.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"

/*! @brief The class of the http connection private.
 * @author Greedysky <greedysky@163.com>
 */
class QHttpConnectionPrivate : public TTKPrivate<QHttpConnection>
{
public:
    QHttpConnectionPrivate();
    ~QHttpConnectionPrivate();

    void invalidateRequest();
    void writeCount(qint64 count);
    void write(const QByteArray &data);
    void parseRequest();

    static int MessageBegin(http_parser *parser);
    static int Url(http_parser *parser, const char *at, size_t length);
    static int HeaderField(http_parser *parser, const char *at, size_t length);
    static int HeaderValue(http_parser *parser, const char *at, size_t length);
    static int HeadersComplete(http_parser *parser);
    static int Body(http_parser *parser, const char *at, size_t length);
    static int MessageComplete(http_parser *parser);

    QHttpConnection *m_parent;
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

QHttpConnectionPrivate::QHttpConnectionPrivate()
    : m_socket(nullptr),
      m_parser(nullptr),
      m_parserSettings(nullptr),
      m_request(nullptr),
      m_transmitLen(0),
      m_transmitPos(0)
{
    m_parser = (http_parser *)malloc(sizeof(http_parser));
    http_parser_init(m_parser, HTTP_REQUEST);

    m_parserSettings = new http_parser_settings();
    m_parserSettings->on_message_begin = MessageBegin;
    m_parserSettings->on_url = Url;
    m_parserSettings->on_header_field = HeaderField;
    m_parserSettings->on_header_value = HeaderValue;
    m_parserSettings->on_headers_complete = HeadersComplete;
    m_parserSettings->on_body = Body;
    m_parserSettings->on_message_complete = MessageComplete;

    m_parser->data = this;
}

QHttpConnectionPrivate::~QHttpConnectionPrivate()
{
    delete m_socket;
    m_socket = nullptr;

    free(m_parser);
    m_parser = nullptr;

    delete m_parserSettings;
    m_parserSettings = nullptr;
}

void QHttpConnectionPrivate::invalidateRequest()
{
    if(m_request && !m_request->successful())
    {
        Q_EMIT m_request->end();
    }

    m_request = nullptr;
}

void QHttpConnectionPrivate::writeCount(qint64 count)
{
    Q_ASSERT(m_transmitPos + count <= m_transmitLen);

    m_transmitPos += count;

    if(m_transmitPos == m_transmitLen)
    {
        m_transmitLen = 0;
        m_transmitPos = 0;
        Q_EMIT m_parent->allBytesWritten();
    }
}

void QHttpConnectionPrivate::parseRequest()
{
    Q_ASSERT(m_parser);

    while(m_socket->bytesAvailable())
    {
        QByteArray arr = m_socket->readAll();
        http_parser_execute(m_parser, m_parserSettings, arr.constData(), arr.length());
    }
}

void QHttpConnectionPrivate::write(const QByteArray &data)
{
    m_socket->write(data);
    m_transmitLen += data.length();
}



QHttpConnection::QHttpConnection(QTcpSocket *socket, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(QHttpConnection);
    TTK_D(QHttpConnection);
    d->m_socket = socket;
    d->m_parent = this;

    connect(socket, SIGNAL(readyRead()), this, SLOT(parseRequest()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateWriteCount(qint64)));
}

void QHttpConnection::socketDisconnected()
{
    deleteLater();
    invalidateRequest();
}

void QHttpConnection::invalidateRequest()
{
    TTK_D(QHttpConnection);
    d->invalidateRequest();
}

void QHttpConnection::updateWriteCount(qint64 count)
{
    TTK_D(QHttpConnection);
    d->writeCount(count);
}

void QHttpConnection::parseRequest()
{
    TTK_D(QHttpConnection);
    d->parseRequest();
}

void QHttpConnection::write(const QByteArray &data)
{
    TTK_D(QHttpConnection);
    d->write(data);
}

void QHttpConnection::flush()
{
    TTK_D(QHttpConnection);
    d->m_socket->flush();
}

void QHttpConnection::waitForBytesWritten()
{
    TTK_D(QHttpConnection);
    d->m_socket->waitForBytesWritten();
}

void QHttpConnection::responseDone()
{
    TTK_D(QHttpConnection);
    QHttpResponse *response = TTKObjectCast(QHttpResponse*, sender());
    if(response->isLast())
    {
        d->m_socket->disconnectFromHost();
    }
}

/* URL Utilities */
#define HAS_URL_FIELD(info, field) (info.field_set &(1 << (field)))

#define GET_FIELD(data, info, field)                                                               \
    QString::fromLatin1(data + info.field_data[field].off, info.field_data[field].len)

#define CHECK_AND_GET_FIELD(data, info, field)                                                     \
    (HAS_URL_FIELD(info, field) ? GET_FIELD(data, info, field) : QString())

QUrl createUrl(const char *urlData, const http_parser_url &urlInfo)
{
    QUrl url;
    url.setScheme(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_SCHEMA));
    url.setHost(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_HOST));
    // Port is dealt with separately since it is available as an integer.
#if TTK_QT_VERSION_CHECK(5,0,0)
    url.setPath(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_PATH), QUrl::TolerantMode);
    url.setQuery(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_QUERY));
#else
    url.setPath(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_PATH));
    if(HAS_URL_FIELD(urlInfo, UF_QUERY))
    {
        url.setEncodedQuery(QByteArray(urlData + urlInfo.field_data[UF_QUERY].off, urlInfo.field_data[UF_QUERY].len));
    }
#endif
    url.setFragment(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_FRAGMENT));
    url.setUserInfo(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_USERINFO));

    if(HAS_URL_FIELD(urlInfo, UF_PORT))
    {
        url.setPort(urlInfo.port);
    }

    return url;
}

#undef CHECK_AND_SET_FIELD
#undef GET_FIELD
#undef HAS_URL_FIELD

/********************
 * Static Callbacks *
 *******************/

int QHttpConnectionPrivate::MessageBegin(http_parser *parser)
{
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    theConnection->m_currentHeaders.clear();
    theConnection->m_currentUrl.clear();
    theConnection->m_currentUrl.reserve(128);

    // The QHttpRequest should not be parented to this, since it's memory
    // management is the responsibility of the user of the library.
    theConnection->m_request = new QHttpRequest(theConnection->m_parent);

    // Invalidate the request when it is deleted to prevent keep-alive requests
    // from calling a signal on a deleted object.
    QObject::connect(theConnection->m_request, SIGNAL(destroyed(QObject*)), theConnection->m_parent, SLOT(invalidateRequest()));

    return 0;
}

int QHttpConnectionPrivate::HeadersComplete(http_parser *parser)
{
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    Q_ASSERT(theConnection->m_request);

    /** set method **/
    theConnection->m_request->setMethod(TTKStaticCast(QHttpRequest::HttpMethod, parser->method));

    /** set version **/
    theConnection->m_request->setVersion(QString("%1.%2").arg(parser->http_major).arg(parser->http_minor));

    /** get parsed url **/
    struct http_parser_url urlInfo;
    int r = http_parser_parse_url(theConnection->m_currentUrl.constData(),
                                  theConnection->m_currentUrl.length(),
                                  parser->method == HTTP_CONNECT, &urlInfo);
    Q_ASSERT(r == 0);
    Q_UNUSED(r);

    theConnection->m_request->setUrl(createUrl(theConnection->m_currentUrl.constData(), urlInfo));

    // Insert last remaining header
    theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] = theConnection->m_currentHeaderValue;
    theConnection->m_request->setHeaders(theConnection->m_currentHeaders);

    /** set client information **/
    theConnection->m_request->setRemoteAddress(theConnection->m_socket->peerAddress().toString());
    theConnection->m_request->setRemotePort(theConnection->m_socket->peerPort());

    QHttpResponse *response = new QHttpResponse(theConnection->m_parent);
    if(parser->http_major < 1 || parser->http_minor < 1)
    {
        response->setKeepAlive(false);
    }

    QObject::connect(theConnection->m_parent, SIGNAL(destroyed()), response, SLOT(connectionClosed()));
    QObject::connect(response, SIGNAL(done()), theConnection->m_parent, SLOT(responseDone()));

    // we are good to go!
    Q_EMIT theConnection->m_parent->newRequest(theConnection->m_request, response);
    return 0;
}

int QHttpConnectionPrivate::MessageComplete(http_parser *parser)
{
    // TODO: do cleanup and prepare for next request
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    Q_ASSERT(theConnection->m_request);

    theConnection->m_request->setSuccessful(true);
    Q_EMIT theConnection->m_request->end();
    return 0;
}

int QHttpConnectionPrivate::Url(http_parser *parser, const char *at, size_t length)
{
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    Q_ASSERT(theConnection->m_request);

    theConnection->m_currentUrl.append(at, length);
    return 0;
}

int QHttpConnectionPrivate::HeaderField(http_parser *parser, const char *at, size_t length)
{
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    Q_ASSERT(theConnection->m_request);

    // insert the header we parsed previously
    // into the header map
    if(!theConnection->m_currentHeaderField.isEmpty() && !theConnection->m_currentHeaderValue.isEmpty())
    {
        // header names are always lower-cased
        theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] = theConnection->m_currentHeaderValue;
        // clear header value. this sets up a nice
        // feedback loop where the next time
        // HeaderValue is called, it can simply append
        theConnection->m_currentHeaderField = QString();
        theConnection->m_currentHeaderValue = QString();
    }

    QString fieldSuffix = QString::fromLatin1(at, length);
    theConnection->m_currentHeaderField += fieldSuffix;
    return 0;
}

int QHttpConnectionPrivate::HeaderValue(http_parser *parser, const char *at, size_t length)
{
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    Q_ASSERT(theConnection->m_request);

    QString valueSuffix = QString::fromLatin1(at, length);
    theConnection->m_currentHeaderValue += valueSuffix;
    return 0;
}

int QHttpConnectionPrivate::Body(http_parser *parser, const char *at, size_t length)
{
    QHttpConnectionPrivate *theConnection = TTKStaticCast(QHttpConnectionPrivate*, parser->data);
    Q_ASSERT(theConnection->m_request);

    Q_EMIT theConnection->m_request->data(QByteArray(at, length));
    return 0;
}
