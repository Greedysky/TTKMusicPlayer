#include "musicsemaphoreeventloop.h"

#include <QThread>
#include <QApplication>

MusicSemaphoreEventLoop::MusicSemaphoreEventLoop(QObject* parent)
    : QObject(parent), m_semaphore(0)
{

}

void MusicSemaphoreEventLoop::exec()
{
    QMutexLocker locker(&m_mutex);
    --m_semaphore;
    while(m_semaphore)
    {
        QApplication::processEvents();
        QThread::msleep(100);
    }
}

void MusicSemaphoreEventLoop::quit()
{
    ++m_semaphore;
}
