#include "musicsemaphoreloop.h"

MusicSemaphoreLoop::MusicSemaphoreLoop(QObject *parent)
    : QEventLoop(parent)
{
    m_timer.setInterval(10*1000);
    connect(&m_timer, SIGNAL(timeout()), SLOT(quit()));
}

MusicSemaphoreLoop::~MusicSemaphoreLoop()
{
    m_timer.stop();
    quit();
}

QString MusicSemaphoreLoop::getClassName()
{
    return staticMetaObject.className();
}

int MusicSemaphoreLoop::exec(ProcessEventsFlags flags)
{
    m_timer.start();
    return QEventLoop::exec(flags);
}
