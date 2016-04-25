#include "musicsemaphoreeventloop.h"

#include <QThread>
#include <QApplication>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

MusicSemaphoreEventLoop::MusicSemaphoreEventLoop(QObject* parent)
    : QObject(parent), m_semaphore(0)
{

}

MusicSemaphoreEventLoop::MusicSemaphoreEventLoop(int sem, QObject* parent)
    : MusicSemaphoreEventLoop(parent)
{
    m_semaphore = sem;
}

void MusicSemaphoreEventLoop::exec()
{
    QMutexLocker locker(&m_mutex);
    --m_semaphore;
    while(m_semaphore < 0)
    {
        QApplication::processEvents();
#if defined Q_OS_WIN && defined MUSIC_QT_5
        QThread::msleep(100);
#else
        usleep(100*1000);
#endif
    }
}

void MusicSemaphoreEventLoop::quit()
{
    ++m_semaphore;
}
