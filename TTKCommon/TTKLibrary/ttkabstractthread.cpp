#include "ttkabstractthread.h"

TTKAbstractThread::TTKAbstractThread(QObject *parent)
    : QThread(parent),
      m_running(true)
{

}

TTKAbstractThread::~TTKAbstractThread()
{

}

void TTKAbstractThread::stop()
{
    if(isRunning())
    {
        quit();
        wait();
    }
    m_running = false;
}

void TTKAbstractThread::start()
{
    m_running = true;
    QThread::start();
}

void TTKAbstractThread::run()
{

}
