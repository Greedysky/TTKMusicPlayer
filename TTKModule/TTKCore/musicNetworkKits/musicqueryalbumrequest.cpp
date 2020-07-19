#include "musicqueryalbumrequest.h"

MusicQueryAlbumRequest::MusicQueryAlbumRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryAlbumRequest::startToSearch(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startToSearch(album);
}
