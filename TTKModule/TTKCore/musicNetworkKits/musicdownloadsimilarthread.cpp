#include "musicdownloadsimilarthread.h"

MusicDownLoadSimilarThread::MusicDownLoadSimilarThread(QObject *parent)
    : MusicDownLoadPagingThread(parent)
{

}

QString MusicDownLoadSimilarThread::getClassName()
{
    return staticMetaObject.className();
}
