#include "musicqueryrequest.h"

MusicQueryRequest::MusicQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    MusicAbstractQueryRequest::downLoadFinished();
    m_queryValue = value;
    startToPage(0);
}
