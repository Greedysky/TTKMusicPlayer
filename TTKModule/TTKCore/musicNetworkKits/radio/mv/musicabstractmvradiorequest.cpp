#include "musicabstractmvradiorequest.h"
#include "musickgqueryinterface.h"

MusicAbstractMVRadioRequest::MusicAbstractMVRadioRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryText = "1";
}

void MusicAbstractMVRadioRequest::startToSearch(QueryType type, const QString &text)
{
    Q_UNUSED(type);

    deleteAll();
    m_queryText = text.isEmpty() ? "1" : text;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MV_CATEGORY_URL, false));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}
