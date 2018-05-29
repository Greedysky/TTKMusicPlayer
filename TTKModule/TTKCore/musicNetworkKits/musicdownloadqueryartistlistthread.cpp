#include "musicdownloadqueryartistlistthread.h"

MusicDownLoadQueryArtistListThread::MusicDownLoadQueryArtistListThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

void MusicDownLoadQueryArtistListThread::startToSearch(QueryType type, const QString &artistlist)
{
    Q_UNUSED(type);
    startToSearch(artistlist);
}
