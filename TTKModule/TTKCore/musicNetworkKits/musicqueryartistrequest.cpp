#include "musicqueryartistrequest.h"

MusicQueryArtistRequest::MusicQueryArtistRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryArtistRequest::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}
