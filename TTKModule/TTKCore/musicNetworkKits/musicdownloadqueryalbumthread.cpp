#include "musicdownloadqueryalbumthread.h"

MusicDownLoadQueryAlbumThread::MusicDownLoadQueryAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

void MusicDownLoadQueryAlbumThread::startToSearch(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startToSearch(album);
}
