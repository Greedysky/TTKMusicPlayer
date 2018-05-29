#include "musicnetworkproxy.h"
#include "musicobject.h"

#include <QNetworkProxy>

MusicNetworkProxy::MusicNetworkProxy(QObject *parent)
    : QObject(parent), m_testSocket(nullptr)
{
    m_port = -1;
    m_type = 2; ///no proxy
}

MusicNetworkProxy::~MusicNetworkProxy()
{
    delete m_testSocket;
}

void MusicNetworkProxy::setHostName(const QString &name)
{
    m_hostName = name;
}

QString MusicNetworkProxy::hostName() const
{
    return m_hostName;
}

void MusicNetworkProxy::setPort(int port)
{
    m_port = port;
}

void MusicNetworkProxy::setType(int type)
{
    m_type = type;
}

int MusicNetworkProxy::type() const
{
    return m_type;
}

int MusicNetworkProxy::port() const
{
    return m_port;
}

void MusicNetworkProxy::setUser(const QString &user)
{
    m_user = user;
}

QString MusicNetworkProxy::user() const
{
    return m_user;
}

void MusicNetworkProxy::setPassword(const QString &pwd)
{
    m_pwd = pwd;
}

QString MusicNetworkProxy::password() const
{
    return m_pwd;
}

void MusicNetworkProxy::testProxy()
{
    if(!m_testSocket)
    {
        m_testSocket = new QTcpSocket(this);
        connect(m_testSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                              SLOT(testProxyChanged(QAbstractSocket::SocketState)));
    }
    m_testSocket->abort();
    m_testSocket->connectToHost(m_hostName, m_port);
}

void MusicNetworkProxy::applyProxy()
{
    QNetworkProxy proxy;
    proxy.setType(MStatic_cast(QNetworkProxy::ProxyType, m_type));
    if(!m_hostName.isEmpty())
    {
        proxy.setHostName(m_hostName);
    }

    if(m_port != -1)
    {
        proxy.setPort(m_port);
    }

    if(!m_user.isEmpty())
    {
        proxy.setUser(m_user);
    }

    if(!m_pwd.isEmpty())
    {
        proxy.setPassword(m_pwd);
    }
    QNetworkProxy::setApplicationProxy(proxy);
}

void MusicNetworkProxy::testProxyChanged(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
    {
        emit testProxyStateChanged(false);
    }
    if(state == QAbstractSocket::ConnectedState)
    {
        emit testProxyStateChanged(true);
    }
}
