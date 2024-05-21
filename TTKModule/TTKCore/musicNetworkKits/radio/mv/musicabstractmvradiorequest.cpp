#include "musicabstractmvradiorequest.h"
#include "musickgqueryinterface.h"

MusicAbstractMVRadioRequest::MusicAbstractMVRadioRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryValue = "1";
    m_queryType = MusicAbstractQueryRequest::QueryType::Movie;
}

void MusicAbstractMVRadioRequest::startToSearch(const QString &value)
{
    deleteAll();
    m_queryValue = value.isEmpty() ? "1" : value;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(MV_CATEGORY_URL, false));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}
