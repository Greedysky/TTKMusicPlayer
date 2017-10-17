#include "musicdownloadqueryplaylistthread.h"

MusicDownLoadQueryPlaylistThread::MusicDownLoadQueryPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}
