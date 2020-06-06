#include "musicdownloaddiscoverlistthread.h"

MusicDownLoadDiscoverListThread::MusicDownLoadDiscoverListThread(QObject *parent)
    : MusicDownLoadPagingThread(parent)
{
    MusicTime::initRandom();
}
