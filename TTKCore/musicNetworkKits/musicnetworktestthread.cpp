#include "musicnetworktestthread.h"
#include "musicnumberdefine.h"
#include "musicobject.h"
#include "musictime.h"

#include <QHostInfo>
#ifdef MUSIC_GREATER_NEW
#  include <QtConcurrent/QtConcurrent>
#else
#  include <QtConcurrentRun>
#endif
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : QObject(parent)
{
    MusicTime::timeSRand();
}

QString MusicNetworkTestThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkTestThread::setUrl(const QString &url)
{
    m_currentUrl = url;
}

void MusicNetworkTestThread::start()
{
    QtConcurrent::run([&]
    {
        QHostInfo info = QHostInfo::fromName(m_currentUrl);
        int rand = qrand()%8 + 1;
#if defined Q_OS_WIN && defined MUSIC_GREATER_NEW
        QThread::msleep(rand*MT_S2MS);
#else
        usleep(rand*MT_S2US);
#endif
        emit networkConnectionTestChanged( !info.addresses().isEmpty() );
    });
}
