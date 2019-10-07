#include "dlnafileserver.h"

#include "qhttpserver/qhttpserver.h"
#include "qhttpserver/qhttprequest.h"
#include "qhttpserver/qhttpresponse.h"

#include <QFile>
#include <QRegExp>
#include <QNetworkInterface>

DlnaFileServer::DlnaFileServer(QObject *parent)
    : QObject(parent)
{
    m_server = new QHttpServer(this);
    connect(m_server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)), SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
}

DlnaFileServer::~DlnaFileServer()
{
    m_server->close();
    delete m_server;
}

void DlnaFileServer::start()
{    
    m_server->listen(QHostAddress::Any, 11111);
}

void DlnaFileServer::setPrefixPath(const QString &path)
{
    m_prefix = path;
}

QString DlnaFileServer::getLocalAddress() const
{
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return QString("http://%1:11111/").arg(address.toString());
        }
    }
    return "http://0.0.0.0:11111/";
}

void DlnaFileServer::handleRequest(QHttpRequest *request, QHttpResponse *response)
{
    QRegExp regx("^/music/(.*)$");
    if(regx.indexIn(request->path()) != -1)
    {
        response->setHeader("Content-Type", "audio/mp3");

        const QString &name = regx.cap(1);
        QFile file(m_prefix + "/" + name);
        if(file.open(QFile::ReadOnly))
        {
            response->writeHead(200);
            response->end(file.readAll());
            file.close();
        }
        else
        {
            response->writeHead(404);
            response->end("Resource not found!");
        }
    }
    else
    {
        response->writeHead(403);
        response->end("You aren't allowed here!");
    }
}
