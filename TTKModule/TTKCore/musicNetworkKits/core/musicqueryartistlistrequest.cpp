#include "musicqueryartistlistrequest.h"

MusicQueryArtistListRequest::MusicQueryArtistListRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryArtistListRequest::startToSearch(QueryType type, const QString &value)
{
    Q_UNUSED(type);
    startToSearch(value);
}
