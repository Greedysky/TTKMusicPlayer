#include "qdlnafileserver.h"

#include "qhttpserver/qhttpserver.h"
#include "qhttpserver/qhttprequest.h"
#include "qhttpserver/qhttpresponse.h"

#include <QFile>
#include <QRegExp>
#include <QNetworkInterface>

/*! @brief The class of the dlna file server private.
 * @author Greedysky <greedysky@163.com>
 */
class QDlnaFileServerPrivate : public TTKPrivate<QDlnaFileServer>
{
public:
    QDlnaFileServerPrivate();
    ~QDlnaFileServerPrivate();

    QString m_prefix;
    QHttpServer *m_server;

};

QDlnaFileServerPrivate::QDlnaFileServerPrivate()
    : m_server(new QHttpServer)
{

}

QDlnaFileServerPrivate::~QDlnaFileServerPrivate()
{
    m_server->close();
    delete m_server;
}



QDlnaFileServer::QDlnaFileServer(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(QDlnaFileServer);
    TTK_D(QDlnaFileServer);
    connect(d->m_server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)), SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
}

void QDlnaFileServer::start()
{    
    TTK_D(QDlnaFileServer);
    d->m_server->listen(QHostAddress::Any, 11111);
}

void QDlnaFileServer::setPrefixPath(const QString &path)
{
    TTK_D(QDlnaFileServer);
    d->m_prefix = path;
}

QString QDlnaFileServer::localAddress(const QString &prefix) const
{
    const QString &url = "http://%1:11111/music/";
    const QString &value = prefix.left(prefix.lastIndexOf(TTK_DOT));

    for(const QHostAddress &address : QNetworkInterface::allAddresses())
    {
      if(address.toString().contains(value))
      {
          return url.arg(address.toString());
      }
    }
    return url.arg("0.0.0.0");
}

void QDlnaFileServer::handleRequest(QHttpRequest *request, QHttpResponse *response)
{
    TTK_D(QDlnaFileServer);
    if(d->m_prefix.isEmpty())
    {
        response->writeHead(500);
        response->end("Music path prefix is empty");
        return;
    }

    const QRegExp regx("^/music/(.*)$");
    if(regx.indexIn(request->path()) != -1)
    {
        response->setHeader("Content-Type", "audio/mp3");

        const QString &name = regx.cap(1);
        QFile file(d->m_prefix + TTK_SEPARATOR + name);
        if(file.open(QIODevice::ReadOnly))
        {
            response->writeHead(200);
            response->end(file.readAll());
            file.close();
        }
        else
        {
            response->writeHead(404);
            response->end("Resource not found");
        }
    }
    else
    {
        response->writeHead(403);
        response->end("You aren't allowed here");
    }
}
