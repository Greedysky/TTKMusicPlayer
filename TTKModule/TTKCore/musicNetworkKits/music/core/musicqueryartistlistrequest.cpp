#include "musicqueryartistlistrequest.h"

MusicQueryArtistListRequest::MusicQueryArtistListRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryArtistListRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    m_queryValue = value;
    startToPage(0);
}
