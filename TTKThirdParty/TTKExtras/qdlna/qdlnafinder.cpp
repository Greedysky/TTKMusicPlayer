#include "qdlnafinder.h"
#include "qdlnaclient.h"

#include <QUdpSocket>

#define DEFAULT_ROUTER_IP   "192.168.0.1"

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
{
    m_udpSock = new QUdpSocket;
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
    while(!m_clients.isEmpty())
    {
        delete m_clients.takeLast();
    }
}

bool QDlnaFinderPrivate::findClient(const QString &server)
{
    foreach(QDlnaClient *client, m_clients)
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
    TTK_INIT_PRIVATE;
    TTK_D(QDlnaFinder);
    connect(d->m_udpSock, SIGNAL(readyRead()), SLOT(readResponse()));
}

void QDlnaFinder::find()
{
    TTK_D(QDlnaFinder);
    d->find();
}

QDlnaClient* QDlnaFinder::client(int index) const
{
    TTK_D(QDlnaFinder);
    if(index < 0 || index >= d->m_clients.size())
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
    foreach(QDlnaClient *client, d->m_clients)
    {
        names.push_back(client->serverName());
    }
    return names;
}

void QDlnaFinder::readResponse()
{
    TTK_D(QDlnaFinder);
    while(d->m_udpSock->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(d->m_udpSock->pendingDatagramSize());
        d->m_udpSock->readDatagram(datagram.data(), datagram.size());

        QDlnaClient *client = new QDlnaClient(QString::fromUtf8(datagram.data()));
        if(client->server() == DEFAULT_ROUTER_IP || d->findClient(client->server()))
        {
            delete client;
            continue;
        }

        int tryTimes = 5;
        do
        {
            --tryTimes;
            client->connect();
        }while(!client->isConnected() && tryTimes > 0);

        d->m_clients.push_back(client);
        Q_EMIT finished();
    }
}
