#include "musicquerytoplistrequest.h"

MusicQueryToplistRequest::MusicQueryToplistRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    MusicAbstractQueryRequest::downLoadFinished();
    m_queryValue = value;
    startToPage(0);
}
