#include "musicdownloadqueryartistthread.h"

MusicDownLoadQueryArtistThread::MusicDownLoadQueryArtistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

void MusicDownLoadQueryArtistThread::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}
