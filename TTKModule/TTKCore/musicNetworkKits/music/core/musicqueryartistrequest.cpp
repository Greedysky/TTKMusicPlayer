#include "musicqueryartistrequest.h"

MusicQueryArtistRequest::MusicQueryArtistRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent),
      m_artistFound(false)
{

}

void MusicQueryArtistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    m_queryValue = value;
    m_artistFound = false;
    startToPage(0);
}
