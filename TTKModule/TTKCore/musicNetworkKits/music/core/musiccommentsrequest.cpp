#include "musiccommentsrequest.h"

MusicCommentsRequest::MusicCommentsRequest(QObject *parent)
    : MusicPageQueryRequest(parent)
{

}

void MusicCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_id = value;
    startToPage(0);
}
