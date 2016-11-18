#include "musicnetworktestthread.h"
#include "musicobject.h"

#include <QHostInfo>
#ifdef MUSIC_GREATER_NEW
#  include <QtConcurrent/QtConcurrent>
#else
#  include <QtConcurrentRun>
#endif

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : QObject(parent)
{

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
        emit networkConnectionTestChanged( !info.addresses().isEmpty() );
    });
}
