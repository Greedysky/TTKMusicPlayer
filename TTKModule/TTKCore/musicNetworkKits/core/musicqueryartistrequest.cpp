#include "musicqueryartistrequest.h"

MusicQueryArtistRequest::MusicQueryArtistRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryArtistRequest::startToSearch(QueryType type, const QString &value)
{
    Q_UNUSED(type);
    startToSearch(value);
}
