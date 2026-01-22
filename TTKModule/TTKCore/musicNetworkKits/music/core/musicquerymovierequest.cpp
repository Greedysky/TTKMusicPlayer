#include "musicquerymovierequest.h"

MusicQueryMovieRequest::MusicQueryMovieRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryType = MusicAbstractQueryRequest::QueryType::Movie;
}

void MusicQueryMovieRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    MusicAbstractQueryRequest::downloadFinished();
    m_queryValue = value;
    startToPage(0);
}
