#include "musicquerymovierequest.h"

MusicQueryMovieRequest::MusicQueryMovieRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryMovieRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    m_queryValue = value;
    startToPage(0);
}
