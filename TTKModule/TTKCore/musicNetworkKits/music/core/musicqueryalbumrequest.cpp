#include "musicqueryalbumrequest.h"

MusicQueryAlbumRequest::MusicQueryAlbumRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent),
      m_albumFound(false)
{

}

void MusicQueryAlbumRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << value);

    MusicAbstractQueryRequest::downLoadFinished();
    m_queryValue = value;
    m_albumFound = false;
    startToPage(0);
}
