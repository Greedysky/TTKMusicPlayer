#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QHostInfo>
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QtConcurrent/QtConcurrent>
#else
#  include <QtConcurrentRun>
#endif

#define NETWORK_DETECT_INTERVAL     5000             // second
#define NETWORK_REQUEST_ADDRESS     "www.baidu.com"  // ip

MusicNetworkThread::MusicNetworkThread()
    : QObject(nullptr), m_networkState(true)
{
    G_CONNECTION_PTR->setValue(className(), this);
    connect(&m_timer, SIGNAL(timeout()), SLOT(networkStateChanged()));
}

MusicNetworkThread::~MusicNetworkThread()
{
    m_timer.stop();
}

void MusicNetworkThread::start()
{
    TTK_LOGGER_INFO("Load NetworkThread");
    m_timer.start(NETWORK_DETECT_INTERVAL);
    networkStateChanged();
}

void MusicNetworkThread::setBlockNetWork(int block)
{
    G_SETTING_PTR->setValue(MusicSettingManager::CloseNetWorkMode, block);
}

void MusicNetworkThread::networkStateChanged()
{
    const auto status = QtConcurrent::run([&]()
    {
        const bool block = G_SETTING_PTR->value(MusicSettingManager::CloseNetWorkMode).toBool();
        const QHostInfo &info = QHostInfo::fromName(NETWORK_REQUEST_ADDRESS);
        m_networkState = !info.addresses().isEmpty();
        m_networkState = block ? false : m_networkState;
        Q_EMIT networkConnectionStateChanged(m_networkState);
    });
    Q_UNUSED(status);
}
