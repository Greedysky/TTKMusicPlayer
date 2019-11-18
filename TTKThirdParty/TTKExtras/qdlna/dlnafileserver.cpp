#include "dlnafileserver.h"

#include "qhttpserver/qhttpserver.h"
#include "qhttpserver/qhttprequest.h"
#include "qhttpserver/qhttpresponse.h"

#include <QFile>
#include <QRegExp>
#include <QNetworkInterface>

class DlnaFileServerPrivate : public TTKPrivate<DlnaFileServer>
{
public:
    DlnaFileServerPrivate();
    ~DlnaFileServerPrivate();

    QString m_prefix;
    QHttpServer *m_server;

};

DlnaFileServerPrivate::DlnaFileServerPrivate()
{
    m_server = new QHttpServer(ttk_q());
}

DlnaFileServerPrivate::~DlnaFileServerPrivate()
{
    m_server->close();
    delete m_server;
}



DlnaFileServer::DlnaFileServer(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(DlnaFileServer);
    connect(d->m_server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)), SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
}

void DlnaFileServer::start()
{    
    TTK_D(DlnaFileServer);
    d->m_server->listen(QHostAddress::Any, 11111);
}

void DlnaFileServer::setPrefixPath(const QString &path)
{
    TTK_D(DlnaFileServer);
    d->m_prefix = path;
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
    TTK_D(DlnaFileServer);
    QRegExp regx("^/music/(.*)$");
    if(regx.indexIn(request->path()) != -1)
    {
        response->setHeader("Content-Type", "audio/mp3");

        const QString &name = regx.cap(1);
        QFile file(d->m_prefix + "/" + name);
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
