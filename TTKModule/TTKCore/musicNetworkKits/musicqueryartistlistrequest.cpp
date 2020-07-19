#include "musicqueryartistlistrequest.h"

MusicQueryArtistListRequest::MusicQueryArtistListRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryArtistListRequest::startToSearch(QueryType type, const QString &artistlist)
{
    Q_UNUSED(type);
    startToSearch(artistlist);
}
