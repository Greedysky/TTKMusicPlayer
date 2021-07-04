#include "musicpagingrequest.h"

MusicPagingRequest::MusicPagingRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{
    m_pageSize = 0;
    m_totalSize = 0;
    m_pageIndex = 0;
}

void MusicPagingRequest::startToPage(int offset)
{
    Q_UNUSED(offset);
}
