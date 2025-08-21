#include "musicqueryrequest.h"

MusicQueryRequest::MusicQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    MusicAbstractQueryRequest::downLoadFinished();
    m_queryValue = value;
    startToPage(0);
}
