#include "musicsourceupdaterequest.h"

#include "qsync/qsyncutils.h"

static constexpr const char *QUERY_VERSION_URL = "version";

MusicSourceUpdateRequest::MusicSourceUpdateRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicSourceUpdateRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(QSyncUtils::makeDataBucketUrl() + QUERY_VERSION_URL);
    TTK::setUserAgentHeader(&request);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

QString MusicSourceUpdateRequest::version() const noexcept
{
    return m_rawData["version"].toString();
}

QString MusicSourceUpdateRequest::description() const noexcept
{
    return m_rawData["data"].toString();
}

bool MusicSourceUpdateRequest::isLastedVersion() const noexcept
{
    const QString &v = version();
    if(v.isEmpty())
    {
        return true;
    }
    else
    {
        return TTK_VERSION_STR == v;
    }
}

void MusicSourceUpdateRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractNetwork::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            m_rawData = json.toVariant().toMap();
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
