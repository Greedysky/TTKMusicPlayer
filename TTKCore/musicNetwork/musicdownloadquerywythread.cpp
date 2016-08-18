#include "musicdownloadquerywythread.h"

MusicDownLoadQueryWYThread::MusicDownLoadQueryWYThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryWYThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYThread::startSearchSong(QueryType type, const QString &text)
{
    Q_UNUSED(type);
    Q_UNUSED(text);
}

void MusicDownLoadQueryWYThread::downLoadFinished()
{

}
