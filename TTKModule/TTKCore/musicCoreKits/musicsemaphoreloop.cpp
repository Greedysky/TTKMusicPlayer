#include "musicsemaphoreloop.h"
#include "musicnumberdefine.h"

MusicSemaphoreLoop::MusicSemaphoreLoop(QObject *parent)
    : QEventLoop(parent)
{
    m_timer.setInterval(10*MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(quit()));
}

MusicSemaphoreLoop::~MusicSemaphoreLoop()
{
    m_timer.stop();
    quit();
}

int MusicSemaphoreLoop::exec(ProcessEventsFlags flags)
{
    m_timer.start();
    return QEventLoop::exec(flags);
}
