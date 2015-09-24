#include "musicnetworkthread.h"
#include "musicconnectionpool.h"

MusicNetworkThread::MusicNetworkThread(QObject *parent)
    :QObject(parent), m_networkState(true)
{
    M_Connection->setValue("MusicNetworkThread", this);
    m_client = NULL;
    connect(&m_timer, SIGNAL(timeout()), SLOT(timerOut()));
    m_timer.start(NETWORK_DETECT_INTERVAL);
}

MusicNetworkThread::~MusicNetworkThread()
{
    delete m_client;
}

void MusicNetworkThread::start()
{
    M_LOOGERS("Load NetworkThread");
}

void MusicNetworkThread::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    if(socketState == QAbstractSocket::UnconnectedState ||
       socketState == QAbstractSocket::ConnectedState)
    {
        bool state = (socketState != QAbstractSocket::UnconnectedState);
        m_networkState = state ? true : !m_networkState;
        if(!m_networkState)
        {
            return;
        }
        emit networkConnectionStateChanged(m_networkState = state);
        M_LOOGER << "Connect state: " << m_networkState;
    }
}

void MusicNetworkThread::timerOut()
{
    delete m_client;
    m_client = new QTcpSocket;
    connect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                      SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    m_client->connectToHost(NETWORK_REQUEST_ADDRESS, 443, QIODevice::ReadOnly);
}
