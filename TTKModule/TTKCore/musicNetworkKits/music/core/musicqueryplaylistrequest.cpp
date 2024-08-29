#include "musicqueryplaylistrequest.h"

MusicQueryPlaylistRequest::MusicQueryPlaylistRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryPlaylistRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << value);

    m_queryValue = value;
    startToPage(0);
}
