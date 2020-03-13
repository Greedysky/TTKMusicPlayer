#include "musicabstractthread.h"

MusicAbstractThread::MusicAbstractThread(QObject *parent)
    : QThread(parent)
{
    m_running = true;
}

MusicAbstractThread::~MusicAbstractThread()
{

}

void MusicAbstractThread::stopAndQuitThread()
{
    if(isRunning())
    {
        quit();
        wait();
    }
    m_running = false;
}

void MusicAbstractThread::start()
{
    m_running = true;
    QThread::start();
}

void MusicAbstractThread::run()
{

}
