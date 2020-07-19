#include "musicqueryrecommendrequest.h"

MusicQueryRecommendRequest::MusicQueryRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryRecommendRequest::startToSearch(QueryType type, const QString &id)
{
    Q_UNUSED(type);
    startToSearch(id);
}
