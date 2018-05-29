#include "musicdownloadquerytoplistthread.h"

MusicDownLoadQueryToplistThread::MusicDownLoadQueryToplistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

void MusicDownLoadQueryToplistThread::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}
