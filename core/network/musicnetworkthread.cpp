#include "musicnetworkthread.h"

#include <QDebug>

MusicNetworkThread::MusicNetworkThread(QObject *parent)
    :QObject(parent), m_networkState(true)
{
    m_client = NULL;
    connect(&m_timer, SIGNAL(timeout()), SLOT(timerOut()));
    m_timer.start(NETWORK_DETECT_INTERVAL);
}

MusicNetworkThread::~MusicNetworkThread()
{
    delete m_client;
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
        qDebug()<<"Connect state: " << m_networkState;
    }
}

void MusicNetworkThread::timerOut()
{
    disconnect(m_client);
    m_client->deleteLater();
    m_client = new QTcpSocket;
    connect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                      SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    m_client->connectToHost(NETWORK_REQUEST_ADDRESS, 443, QIODevice::ReadOnly);
}
