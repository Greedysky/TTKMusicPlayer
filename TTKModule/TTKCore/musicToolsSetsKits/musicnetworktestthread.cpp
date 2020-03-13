#include "musicnetworktestthread.h"
#include "musicobject.h"
#include "musictime.h"
#include "musiccoreutils.h"

#include <QHostInfo>

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : MusicAbstractThread(parent)
{
    MusicTime::InitSRand();
}

void MusicNetworkTestThread::setUrl(const QString &url)
{
    m_currentUrl = url;
}

void MusicNetworkTestThread::run()
{
    MusicAbstractThread::run();

    const QHostInfo &info = QHostInfo::fromName(m_currentUrl);
    const int rand = qrand()%8 + 1;

    MusicUtils::Core::sleep(rand * MT_S2MS);

    if(m_running)
    {
        Q_EMIT networkConnectionTestChanged( !info.addresses().isEmpty() );
    }
}
