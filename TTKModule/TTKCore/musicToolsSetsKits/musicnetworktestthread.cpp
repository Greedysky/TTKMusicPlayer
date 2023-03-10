#include "musicnetworktestthread.h"
#include "ttktime.h"
#include "musiccoreutils.h"

#include <QHostInfo>

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : TTKAbstractThread(parent)
{
    TTK::initRandom();
}

void MusicNetworkTestThread::setUrl(const QString &url)
{
    m_currentUrl = url;
}

void MusicNetworkTestThread::run()
{
    TTKAbstractThread::run();

    const QHostInfo &info = QHostInfo::fromName(m_currentUrl);
    const int rand = TTK::random(8) + 1;

    TTK::Core::sleep(rand * MT_S2MS);

    if(m_running)
    {
        Q_EMIT networkConnectionTestChanged(!info.addresses().isEmpty());
    }
}
