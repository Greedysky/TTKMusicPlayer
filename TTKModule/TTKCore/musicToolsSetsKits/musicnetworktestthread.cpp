#include "musicnetworktestthread.h"
#include "musicnumberdefine.h"
#include "musicobject.h"
#include "musictime.h"
#include "musiccoreutils.h"

#include <QHostInfo>

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : QThread(parent)
{
    MusicTime::timeSRand();
    m_run = false;
}

MusicNetworkTestThread::~MusicNetworkTestThread()
{
    stopAndQuitThread();
}

void MusicNetworkTestThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicNetworkTestThread::setUrl(const QString &url)
{
    m_currentUrl = url;
}

void MusicNetworkTestThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicNetworkTestThread::run()
{
    const QHostInfo &info = QHostInfo::fromName(m_currentUrl);
    const int rand = qrand()%8 + 1;

    MusicUtils::Core::sleep(rand * MT_S2MS);

    if(m_run)
    {
        emit networkConnectionTestChanged( !info.addresses().isEmpty() );
    }
}
