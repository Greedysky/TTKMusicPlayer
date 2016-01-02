#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

MusicNetworkThread::MusicNetworkThread()
    : QObject(0), m_networkState(true)
{
    M_CONNECTION->setValue("MusicNetworkThread", this);
    m_client = nullptr;
    connect(&m_timer, SIGNAL(timeout()), SLOT(slTimerOut()));
    m_timer.start(NETWORK_DETECT_INTERVAL);
}

MusicNetworkThread::~MusicNetworkThread()
{
    m_timer.stop();
    delete m_client;
}

void MusicNetworkThread::start()
{
    m_blockNetWork = false;
    M_LOGGERS("Load NetworkThread");
}

void MusicNetworkThread::setBlockNetWork(int block)
{
    m_blockNetWork = block;
    M_SETTING->setValue(MusicSettingManager::CloseNetWorkChoiced, block);
}

void MusicNetworkThread::slSocketStateChanged(QAbstractSocket::SocketState socketState)
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
        emit sgNetworkConnectionStateChanged(m_networkState = m_blockNetWork ? false : state);
    }
}

void MusicNetworkThread::slTimerOut()
{
    delete m_client;
    m_client = new QTcpSocket;
    connect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                      SLOT(slSocketStateChanged(QAbstractSocket::SocketState)));
    m_client->connectToHost(NETWORK_REQUEST_ADDRESS, 443, QIODevice::ReadOnly);
    M_LOGGER << "Connect state: " << !m_networkState << LOG_END;
}
