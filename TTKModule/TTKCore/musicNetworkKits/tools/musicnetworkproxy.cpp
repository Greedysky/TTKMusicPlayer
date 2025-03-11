#include "musicnetworkproxy.h"

#include <QNetworkProxy>

MusicNetworkProxy::MusicNetworkProxy(QObject *parent)
    : QObject(parent),
      m_port(-1),
      m_type(2), ///no proxy
      m_socket(nullptr)
{

}

MusicNetworkProxy::~MusicNetworkProxy()
{
    delete m_socket;
}

void MusicNetworkProxy::setHostName(const QString &name) noexcept
{
    m_hostName = name;
}

QString MusicNetworkProxy::hostName() const noexcept
{
    return m_hostName;
}

void MusicNetworkProxy::setPort(int port) noexcept
{
    m_port = port;
}

void MusicNetworkProxy::setType(int type) noexcept
{
    m_type = type;
}

int MusicNetworkProxy::type() const noexcept
{
    return m_type;
}

int MusicNetworkProxy::port() const noexcept
{
    return m_port;
}

void MusicNetworkProxy::setUser(const QString &user) noexcept
{
    m_user = user;
}

QString MusicNetworkProxy::user() const noexcept
{
    return m_user;
}

void MusicNetworkProxy::setPassword(const QString &pwd) noexcept
{
    m_pwd = pwd;
}

QString MusicNetworkProxy::password() const noexcept
{
    return m_pwd;
}

void MusicNetworkProxy::testProxy()
{
    if(!m_socket)
    {
        m_socket = new QTcpSocket(this);
        connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(testProxyChanged(QAbstractSocket::SocketState)));
    }

    m_socket->abort();
    m_socket->connectToHost(m_hostName, m_port);
}

void MusicNetworkProxy::applyProxy()
{
    QNetworkProxy proxy;
    proxy.setType(TTKStaticCast(QNetworkProxy::ProxyType, m_type));
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
        Q_EMIT testProxyStateChanged(false);
    }

    if(state == QAbstractSocket::ConnectedState)
    {
        Q_EMIT testProxyStateChanged(true);
    }
}
