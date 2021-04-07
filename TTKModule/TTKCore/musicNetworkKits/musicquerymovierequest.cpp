#include "musicquerymovierequest.h"

MusicQueryMovieRequest::MusicQueryMovieRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryMovieRequest::startToSearch(const QString &movie)
{
    m_queryText = movie;
    startToPage(0);
}
