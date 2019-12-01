#include "dlnafinder.h"
#include "dlnaclient.h"

#include <QUdpSocket>

#define DEFAULT_ROUTER_IP   "192.168.0.1"

class DlnaFinderPrivate : public TTKPrivate<DlnaFinder>
{
public:
    DlnaFinderPrivate();
    ~DlnaFinderPrivate();

    void find();
    void removeClients();
    bool findClient(const QString &server);

    QUdpSocket *m_udpSock;
    QList<DlnaClient*> m_clients;

};

DlnaFinderPrivate::DlnaFinderPrivate()
{
    m_udpSock = new QUdpSocket;
    m_udpSock->bind(QHostAddress(QHostAddress::Any), 6000);
}

DlnaFinderPrivate::~DlnaFinderPrivate()
{
    delete m_udpSock;
    removeClients();
}

void DlnaFinderPrivate::find()
{
    removeClients();
    const QByteArray& data = "M-SEARCH * HTTP/1.1\r\n"
                             "HOST:239.255.255.250:1900\r\n"
                             "MAN:\"ssdp:discover\"\r\n"
                             "ST:ssdp:all\r\n"
                             "MX:3\r\n\r\n";
    m_udpSock->writeDatagram(data, QHostAddress("239.255.255.250"), 1900);
}

void DlnaFinderPrivate::removeClients()
{
    while(!m_clients.isEmpty())
    {
        delete m_clients.takeLast();
    }
}

bool DlnaFinderPrivate::findClient(const QString &server)
{
    foreach(DlnaClient *client, m_clients)
    {
        if(client->server() == server)
        {
            return true;
        }
    }
    return false;
}



DlnaFinder::DlnaFinder(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(DlnaFinder);
    connect(d->m_udpSock, SIGNAL(readyRead()), SLOT(readResponse()));
}

void DlnaFinder::find()
{
    TTK_D(DlnaFinder);
    d->find();
}

DlnaClient* DlnaFinder::client(int index) const
{
    TTK_D(DlnaFinder);
    if(index < 0 || index >= d->m_clients.size())
    {
        return nullptr;
    }

    return d->m_clients[index];
}

QList<DlnaClient*> DlnaFinder::clients() const
{
    TTK_D(DlnaFinder);
    return d->m_clients;
}

QStringList DlnaFinder::clientNames() const
{
    TTK_D(DlnaFinder);
    QStringList names;
    foreach(DlnaClient *client, d->m_clients)
    {
        names.push_back(client->serverName());
    }
    return names;
}

void DlnaFinder::readResponse()
{
    TTK_D(DlnaFinder);
    while(d->m_udpSock->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(d->m_udpSock->pendingDatagramSize());
        d->m_udpSock->readDatagram(datagram.data(), datagram.size());

        DlnaClient *client = new DlnaClient(QString::fromUtf8(datagram.data()));
        M_LOGGER_INFO(client->server());
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
        emit finished();
    }
}
