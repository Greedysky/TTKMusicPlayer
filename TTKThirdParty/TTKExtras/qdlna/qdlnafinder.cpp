#include "qdlnafinder.h"
#include "qdlnaclient.h"

#include <QUdpSocket>
#include <QApplication>

static constexpr const char *DEFAULT_ROUTER_IP = "192.168.0.1";

/*! @brief The class of the dlna finder private.
 * @author Greedysky <greedysky@163.com>
 */
class QDlnaFinderPrivate : public TTKPrivate<QDlnaFinder>
{
public:
    QDlnaFinderPrivate();
    ~QDlnaFinderPrivate();

    void find();
    void removeClients();
    bool findClient(const QString &server);

    QUdpSocket *m_udpSock;
    QList<QDlnaClient*> m_clients;

};

QDlnaFinderPrivate::QDlnaFinderPrivate()
    : m_udpSock(new QUdpSocket)
{
    m_udpSock->bind(QHostAddress(QHostAddress::Any), 6000);
}

QDlnaFinderPrivate::~QDlnaFinderPrivate()
{
    delete m_udpSock;
    removeClients();
}

void QDlnaFinderPrivate::find()
{
    removeClients();
    const QByteArray& data = "M-SEARCH * HTTP/1.1\r\n"
                             "HOST:239.255.255.250:1900\r\n"
                             "MAN:\"ssdp:discover\"\r\n"
                             "ST:ssdp:all\r\n"
                             "MX:3\r\n\r\n";
    m_udpSock->writeDatagram(data, QHostAddress("239.255.255.250"), 1900);
}

void QDlnaFinderPrivate::removeClients()
{
    qDeleteAll(m_clients);
}

bool QDlnaFinderPrivate::findClient(const QString &server)
{
    for(QDlnaClient *client : qAsConst(m_clients))
    {
        if(client->server() == server)
        {
            return true;
        }
    }
    return false;
}



QDlnaFinder::QDlnaFinder(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(QDlnaFinder);
    TTK_D(QDlnaFinder);
    connect(d->m_udpSock, SIGNAL(readyRead()), SLOT(handleReadyRead()));
}

void QDlnaFinder::find()
{
    TTK_D(QDlnaFinder);
    d->find();
}

QDlnaClient* QDlnaFinder::client(int index) const
{
    TTK_D(QDlnaFinder);
    if(index < 0 || index >= d->m_clients.count())
    {
        return nullptr;
    }

    return d->m_clients[index];
}

QList<QDlnaClient*> QDlnaFinder::clients() const
{
    TTK_D(QDlnaFinder);
    return d->m_clients;
}

QStringList QDlnaFinder::clientNames() const
{
    TTK_D(QDlnaFinder);
    QStringList names;
    for(QDlnaClient *client : qAsConst(d->m_clients))
    {
        names.push_back(client->serverName());
    }
    return names;
}

void QDlnaFinder::handleReadyRead()
{
    TTK_D(QDlnaFinder);
    while(d->m_udpSock->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(d->m_udpSock->pendingDatagramSize());
        d->m_udpSock->readDatagram(datagram.data(), datagram.length());

        QDlnaClient *client = new QDlnaClient(QString::fromUtf8(datagram.data()));
        if(client->server() == DEFAULT_ROUTER_IP || d->findClient(client->server()))
        {
            delete client;
            continue;
        }

        int tryTimes = 3;
        do
        {
            --tryTimes;
            client->connect();
            qApp->processEvents();
        } while(!client->isConnected() && tryTimes > 0);

        d->m_clients.push_back(client);
        Q_EMIT finished();
    }
}
