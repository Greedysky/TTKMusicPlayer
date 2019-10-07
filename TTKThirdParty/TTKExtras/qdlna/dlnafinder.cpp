#include "dlnafinder.h"
#include "dlnaclient.h"

#define DEFAULT_ROUTER_IP   "192.168.0.1"

DlnaFinder::DlnaFinder(QObject *parent)
    : QObject(parent)
{
    m_udpSock = new QUdpSocket(this);
    m_udpSock->bind(QHostAddress(QHostAddress::Any), 6000);

    connect(m_udpSock, SIGNAL(readyRead()), SLOT(readResponse()));
}

DlnaFinder::~DlnaFinder()
{
    delete m_udpSock;
    removeClients();
}

void DlnaFinder::find()
{
    removeClients();
    const QByteArray& data = "M-SEARCH * HTTP/1.1\r\n"
                             "HOST:239.255.255.250:1900\r\n"
                             "MAN:\"ssdp:discover\"\r\n"
                             "ST:ssdp:all\r\n"
                             "MX:3\r\n\r\n";
    m_udpSock->writeDatagram(data, QHostAddress("239.255.255.250"), 1900);
}

DlnaClient* DlnaFinder::client(int index) const
{
    if(index < 0 || index >= m_clients.size())
    {
        return nullptr;
    }

    return m_clients[index];
}

QList<DlnaClient*> DlnaFinder::clients() const
{
    return m_clients;
}

QStringList DlnaFinder::clientNames() const
{
    QStringList names;
    foreach(DlnaClient *client, m_clients)
    {
        names.push_back(client->serverName());
    }
    return names;
}

void DlnaFinder::readResponse()
{
    while(m_udpSock->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSock->pendingDatagramSize());
        m_udpSock->readDatagram(datagram.data(), datagram.size());

        DlnaClient *client = new DlnaClient(QString::fromUtf8(datagram.data()));
        qDebug() << client->server();
        if(client->server() == DEFAULT_ROUTER_IP || findClient(client->server()))
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

        m_clients.push_back(client);
        emit finished();
    }
}

void DlnaFinder::removeClients()
{
    while(!m_clients.isEmpty())
    {
        delete m_clients.takeLast();
    }
}

bool DlnaFinder::findClient(const QString &server)
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
