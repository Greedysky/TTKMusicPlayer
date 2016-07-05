#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QHostInfo>

MusicNetworkThread::MusicNetworkThread()
    : QObject(nullptr), m_networkState(true)
{
    M_CONNECTION_PTR->setValue(getClassName(), this);
    connect(&m_timer, SIGNAL(timeout()), SLOT(networkStateChanged()));
}

MusicNetworkThread::~MusicNetworkThread()
{
    m_timer.stop();
}

QString MusicNetworkThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkThread::start()
{
    M_LOGGER_INFO("Load NetworkThread");
    m_timer.start(NETWORK_DETECT_INTERVAL);
}

void MusicNetworkThread::setBlockNetWork(int block)
{
    M_SETTING_PTR->setValue(MusicSettingManager::CloseNetWorkChoiced, block);
}

void MusicNetworkThread::networkStateChanged()
{
    bool block = M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toBool();
    QHostInfo info = QHostInfo::fromName(NETWORK_REQUEST_ADDRESS);
    m_networkState = !info.addresses().isEmpty();
    m_networkState = block ? false : m_networkState;
    emit networkConnectionStateChanged(m_networkState);
}
