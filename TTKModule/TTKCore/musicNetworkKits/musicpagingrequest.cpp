#include "musicpagingrequest.h"

MusicPagingRequest::MusicPagingRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{
    m_pageSize = 0;
    m_totalSize = 0;
    m_pageIndex = 0;
}

MusicPagingRequest::~MusicPagingRequest()
{
    deleteAll();
}

void MusicPagingRequest::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    setNetworkAbort(true);
}

void MusicPagingRequest::startToPage(int offset)
{
    Q_UNUSED(offset);
}
