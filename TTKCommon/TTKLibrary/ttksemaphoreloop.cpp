#include "ttksemaphoreloop.h"
#include "ttknumberdefine.h"

TTKSemaphoreLoop::TTKSemaphoreLoop(QObject *parent)
    : QEventLoop(parent)
{
    m_timer.setInterval(10 * MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(quit()));
}

TTKSemaphoreLoop::~TTKSemaphoreLoop()
{
    quit();
}

void TTKSemaphoreLoop::quit()
{
    m_timer.stop();
    return QEventLoop::quit();
}

int TTKSemaphoreLoop::exec(ProcessEventsFlags flags)
{
    m_timer.start();
    return QEventLoop::exec(flags);
}
