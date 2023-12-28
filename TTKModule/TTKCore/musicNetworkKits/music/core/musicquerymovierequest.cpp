#include "musicquerymovierequest.h"

MusicQueryMovieRequest::MusicQueryMovieRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryMovieRequest::startToQueryInfo(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToQueryInfo" << value);

    m_queryValue = value;
    startToPage(0);
}
