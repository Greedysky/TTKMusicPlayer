#include "musicqueryalbumrequest.h"

MusicQueryAlbumRequest::MusicQueryAlbumRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryAlbumRequest::startToSearch(QueryType type, const QString &value)
{
    Q_UNUSED(type);
    startToSearch(value);
}
