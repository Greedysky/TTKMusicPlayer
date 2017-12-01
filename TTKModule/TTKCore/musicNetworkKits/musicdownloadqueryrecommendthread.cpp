#include "musicdownloadqueryrecommendthread.h"

MusicDownLoadQueryRecommendThread::MusicDownLoadQueryRecommendThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryRecommendThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryRecommendThread::startToSearch(QueryType type, const QString &id)
{
    Q_UNUSED(type);
    startToSearch(id);
}
