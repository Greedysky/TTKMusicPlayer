#include "musicdownloadqueryartistthread.h"

MusicDownLoadQueryArtistThread::MusicDownLoadQueryArtistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryArtistThread::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}
