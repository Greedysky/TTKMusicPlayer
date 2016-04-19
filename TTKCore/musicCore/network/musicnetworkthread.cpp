#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QHostInfo>

MusicNetworkThread::MusicNetworkThread()
    : QObject(0), m_networkState(true)
{
    M_CONNECTION->setValue("MusicNetworkThread", this);
    connect(&m_timer, SIGNAL(timeout()), SLOT(networkStateChanged()));
    m_timer.start(NETWORK_DETECT_INTERVAL);
}

MusicNetworkThread::~MusicNetworkThread()
{
    m_timer.stop();
}

void MusicNetworkThread::start()
{
    M_LOGGER_INFO("Load NetworkThread");
}

void MusicNetworkThread::setBlockNetWork(int block)
{
    M_SETTING->setValue(MusicSettingManager::CloseNetWorkChoiced, block);
}

void MusicNetworkThread::networkStateChanged()
{
    bool block = M_SETTING->value(MusicSettingManager::CloseNetWorkChoiced).toBool();
    QHostInfo info = QHostInfo::fromName(NETWORK_REQUEST_ADDRESS);
    m_networkState = !info.addresses().isEmpty();
    m_networkState = block ? false : m_networkState;
    emit networkConnectionStateChanged(m_networkState);
}
