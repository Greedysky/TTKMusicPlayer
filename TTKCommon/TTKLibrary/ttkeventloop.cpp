#include "ttkeventloop.h"

TTKEventLoop::TTKEventLoop(QObject *parent)
    : QEventLoop(parent)
{
    m_timer.setInterval(10 * TTK_DN_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(quit()));
}

TTKEventLoop::~TTKEventLoop()
{
    quit();
}

void TTKEventLoop::setTimeout(int msec)
{
    m_timer.setInterval(msec);
}

int TTKEventLoop::exec(ProcessEventsFlags flags)
{
    m_timer.start();
    return QEventLoop::exec(flags);
}

void TTKEventLoop::quit()
{
    m_timer.stop();
    return QEventLoop::quit();
}

void TTKEventLoop::exit()
{
    m_timer.stop();
    return QEventLoop::exit();
}
