#include "musicquerytoplistrequest.h"

MusicQueryToplistRequest::MusicQueryToplistRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    MusicAbstractQueryRequest::downloadFinished();
    m_queryValue = value;
    startToPage(0);
}
