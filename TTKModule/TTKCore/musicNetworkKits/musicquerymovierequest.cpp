#include "musicquerymovierequest.h"

MusicQueryMovieRequest::MusicQueryMovieRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryMovieRequest::startToSearch(const QString &value)
{
    m_queryValue = value;
    startToPage(0);
}
