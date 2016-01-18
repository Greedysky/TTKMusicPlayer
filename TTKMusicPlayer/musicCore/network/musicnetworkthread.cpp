#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QHostInfo>

MusicNetworkThread::MusicNetworkThread()
    : QObject(0), m_networkState(true)
{
    M_CONNECTION->setValue("MusicNetworkThread", this);
    connect(&m_timer, SIGNAL(timeout()), SLOT(timerOut()));
    m_timer.start(NETWORK_DETECT_INTERVAL);
}

MusicNetworkThread::~MusicNetworkThread()
{
    m_timer.stop();
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

void MusicNetworkThread::timerOut()
{
    QHostInfo info = QHostInfo::fromName(NETWORK_REQUEST_ADDRESS);
    m_networkState = !info.addresses().isEmpty();

    emit networkConnectionStateChanged(m_blockNetWork ? false : m_networkState);
}
