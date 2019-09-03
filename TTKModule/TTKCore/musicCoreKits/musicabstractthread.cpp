#include "musicabstractthread.h"

MusicAbstractThread::MusicAbstractThread(QObject *parent)
    : QThread(parent)
{
    m_run = true;
}

MusicAbstractThread::~MusicAbstractThread()
{

}

void MusicAbstractThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicAbstractThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicAbstractThread::run()
{

}
