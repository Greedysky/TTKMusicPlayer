#include "musicnetworktestthread.h"
#include "ttktime.h"
#include "musiccoreutils.h"

#include <QHostInfo>

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : TTKAbstractThread(parent)
{
    TTK::initRandom();
}

void MusicNetworkTestThread::setUrl(const QString &url) noexcept
{
    m_currentUrl = url;
}

void MusicNetworkTestThread::run()
{
    const QHostInfo &info = QHostInfo::fromName(m_currentUrl);
    const int rand = TTK::random(5) + 1;

    TTK::Core::sleep(rand * TTK_DN_S2MS);

    Q_EMIT networkConnectionTestChanged(!info.addresses().isEmpty());
}
