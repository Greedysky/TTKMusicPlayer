#include "musicdownloadqueryartistlistthread.h"

MusicDownLoadQueryArtistListThread::MusicDownLoadQueryArtistListThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryArtistListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryArtistListThread::startToSearch(QueryType type, const QString &artistlist)
{
    Q_UNUSED(type);
    startToSearch(artistlist);
}
