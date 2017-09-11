#include "musicnetworktestthread.h"
#include "musicnumberdefine.h"
#include "musicobject.h"
#include "musictime.h"

#include <QHostInfo>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

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

QString MusicNetworkTestThread::getClassName()
{
    return staticMetaObject.className();
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
    QHostInfo info = QHostInfo::fromName(m_currentUrl);
    int rand = qrand()%8 + 1;
#if defined Q_OS_WIN && defined MUSIC_GREATER_NEW
    QThread::msleep(rand*MT_S2MS);
#else
    usleep(rand*MT_S2US);
#endif
    if(m_run)
    {
        emit networkConnectionTestChanged( !info.addresses().isEmpty() );
    }
}
