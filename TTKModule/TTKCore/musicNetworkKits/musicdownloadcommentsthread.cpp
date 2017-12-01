#include "musicdownloadcommentsthread.h"

MusicDownLoadCommentsThread::MusicDownLoadCommentsThread(QObject *parent)
    : MusicDownLoadPagingThread(parent)
{

}

QString MusicDownLoadCommentsThread::getClassName()
{
    return staticMetaObject.className();
}
