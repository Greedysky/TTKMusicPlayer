#include "musicpagequeryrequest.h"

MusicPageQueryRequest::MusicPageQueryRequest(QObject *parent)
    : MusicAbstractNetwork(parent),
      m_pageSize(0),
      m_totalSize(0),
      m_pageIndex(0)
{

}

void MusicPageQueryRequest::startToPage(int offset)
{
    Q_UNUSED(offset);
}
