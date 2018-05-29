#include "musicdownloadqueryrecommendthread.h"

MusicDownLoadQueryRecommendThread::MusicDownLoadQueryRecommendThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

void MusicDownLoadQueryRecommendThread::startToSearch(QueryType type, const QString &id)
{
    Q_UNUSED(type);
    startToSearch(id);
}
