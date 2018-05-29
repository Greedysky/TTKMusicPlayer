#include "musicdownloaddiscoverlistthread.h"
#include "musictime.h"

MusicDownLoadDiscoverListThread::MusicDownLoadDiscoverListThread(QObject *parent)
    : MusicDownLoadPagingThread(parent)
{
    MusicTime::timeSRand();
}
