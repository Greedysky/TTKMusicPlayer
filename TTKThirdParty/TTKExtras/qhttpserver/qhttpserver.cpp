#include "qhttpserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QVariant>

#include "qhttpconnection.h"

QHash<int, QString> STATUS_CODES;

/*! @brief The class of the http server private.
 * @author Greedysky <greedysky@163.com>
 */
class QHttpServerPrivate : public TTKPrivate<QHttpServer>
{
public:
    QHttpServerPrivate();

    bool listen(const QHostAddress &address, quint16 port);
    void create();
    void close();
    void free();

    QTcpServer *m_tcpServer;

};

QHttpServerPrivate::QHttpServerPrivate()
    : m_tcpServer(nullptr)
{

}

bool QHttpServerPrivate::listen(const QHostAddress &address, quint16 port)
{
    m_tcpServer = new QTcpServer(ttk_q());
    bool couldBindToPort = m_tcpServer->listen(address, port);
    if(couldBindToPort)
    {
        QObject::connect(m_tcpServer, SIGNAL(newConnection()), ttk_q(), SLOT(newConnection()));
    }
    else
    {
        free();
    }
    return couldBindToPort;
}

void QHttpServerPrivate::create()
{
    while(m_tcpServer->hasPendingConnections())
    {
        QHttpConnection *connection = new QHttpConnection(m_tcpServer->nextPendingConnection(), ttk_q());
        QObject::connect(connection, SIGNAL(newRequest(QHttpRequest *, QHttpResponse *)), ttk_q(),
                         SIGNAL(newRequest(QHttpRequest *, QHttpResponse *)));
    }
}

void QHttpServerPrivate::close()
{
    if(m_tcpServer)
    {
        m_tcpServer->close();
    }
}

void QHttpServerPrivate::free()
{
    delete m_tcpServer;
    m_tcpServer = nullptr;
}



QHttpServer::QHttpServer(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(QHttpServer);
#define STATUS_CODE(num, reason) STATUS_CODES.insert(num, reason);
    STATUS_CODE(100, "Continue")
    STATUS_CODE(101, "Switching Protocols")
    STATUS_CODE(102, "Processing") // RFC 2518) obsoleted by RFC 4918
    STATUS_CODE(200, "OK")
    STATUS_CODE(201, "Created")
    STATUS_CODE(202, "Accepted")
    STATUS_CODE(203, "Non-Authoritative Information")
    STATUS_CODE(204, "No Content")
    STATUS_CODE(205, "Reset Content")
    STATUS_CODE(206, "Partial Content")
    STATUS_CODE(207, "Multi-Status") // RFC 4918
    STATUS_CODE(300, "Multiple Choices")
    STATUS_CODE(301, "Moved Permanently")
    STATUS_CODE(302, "Moved Temporarily")
    STATUS_CODE(303, "See Other")
    STATUS_CODE(304, "Not Modified")
    STATUS_CODE(305, "Use Proxy")
    STATUS_CODE(307, "Temporary Redirect")
    STATUS_CODE(400, "Bad Request")
    STATUS_CODE(401, "Unauthorized")
    STATUS_CODE(402, "Payment Required")
    STATUS_CODE(403, "Forbidden")
    STATUS_CODE(404, "Not Found")
    STATUS_CODE(405, "Method Not Allowed")
    STATUS_CODE(406, "Not Acceptable")
    STATUS_CODE(407, "Proxy Authentication Required")
    STATUS_CODE(408, "Request Time-out")
    STATUS_CODE(409, "Conflict")
    STATUS_CODE(410, "Gone")
    STATUS_CODE(411, "Length Required")
    STATUS_CODE(412, "Precondition Failed")
    STATUS_CODE(413, "Request Entity Too Large")
    STATUS_CODE(414, "Request-URI Too Large")
    STATUS_CODE(415, "Unsupported Media Type")
    STATUS_CODE(416, "Requested Range Not Satisfiable")
    STATUS_CODE(417, "Expectation Failed")
    STATUS_CODE(418, "I\"m a teapot")        // RFC 2324
    STATUS_CODE(422, "Unprocessable Entity") // RFC 4918
    STATUS_CODE(423, "Locked")               // RFC 4918
    STATUS_CODE(424, "Failed Dependency")    // RFC 4918
    STATUS_CODE(425, "Unordered Collection") // RFC 4918
    STATUS_CODE(426, "Upgrade Required")     // RFC 2817
    STATUS_CODE(500, "Internal Server Error")
    STATUS_CODE(501, "Not Implemented")
    STATUS_CODE(502, "Bad Gateway")
    STATUS_CODE(503, "Service Unavailable")
    STATUS_CODE(504, "Gateway Time-out")
    STATUS_CODE(505, "HTTP Version not supported")
    STATUS_CODE(506, "Variant Also Negotiates") // RFC 2295
    STATUS_CODE(507, "Insufficient Storage")    // RFC 4918
    STATUS_CODE(509, "Bandwidth Limit Exceeded")
    STATUS_CODE(510, "Not Extended") // RFC 2774
}

void QHttpServer::newConnection()
{
    TTK_D(QHttpServer);
    d->create();
}

bool QHttpServer::listen(const QHostAddress &address, quint16 port)
{
    TTK_D(QHttpServer);
    return d->listen(address, port);
}

bool QHttpServer::listen(quint16 port)
{
    return listen(QHostAddress::Any, port);
}

void QHttpServer::close()
{
    TTK_D(QHttpServer);
    d->close();
}
