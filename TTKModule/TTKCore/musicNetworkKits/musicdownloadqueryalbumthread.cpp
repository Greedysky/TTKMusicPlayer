#include "musicdownloadqueryalbumthread.h"

MusicDownLoadQueryAlbumThread::MusicDownLoadQueryAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryAlbumThread::startToSearch(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startToSearch(album);
}
