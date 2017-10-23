#include "musicdownloadquerytoplistthread.h"

MusicDownLoadQueryToplistThread::MusicDownLoadQueryToplistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryToplistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryToplistThread::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}
