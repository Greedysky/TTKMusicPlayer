#include "musicdownloadsongsuggestthread.h"

MusicDownLoadSongSuggestThread::MusicDownLoadSongSuggestThread(QObject *parent)
    : MusicDownLoadPagingThread(parent)
{

}

QString MusicDownLoadSongSuggestThread::getClassName()
{
    return staticMetaObject.className();
}
