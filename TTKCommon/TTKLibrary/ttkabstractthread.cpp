#include "ttkabstractthread.h"

TTKAbstractThread::TTKAbstractThread(QObject *parent)
    : QThread(parent),
      m_running(true)
{

}

void TTKAbstractThread::stop()
{
    m_running = false;
    if(isRunning())
    {
        quit();
        wait();
    }
}

void TTKAbstractThread::start()
{
    m_running = true;
    QThread::start();
}

void TTKAbstractThread::run()
{

}
