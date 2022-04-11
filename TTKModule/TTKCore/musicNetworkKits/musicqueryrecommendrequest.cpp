#include "musicqueryrecommendrequest.h"

MusicQueryRecommendRequest::MusicQueryRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryRecommendRequest::startToSearch(QueryType type, const QString &value)
{
    Q_UNUSED(type);
    Q_UNUSED(value);
    startToSearch();
}
