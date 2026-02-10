#include "musicquerysongrequest.h"

MusicQuerySongRequest::MusicQuerySongRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQuerySongRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    MusicAbstractQueryRequest::downloadFinished();
    m_queryValue = value;
    startToPage(0);
}
