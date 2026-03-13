#include "musicnetworkoperator.h"

static constexpr const char *IP_CHECK_URL = "ZlhkcnFzd1RabVhCZXNWM1pnbk5hT1ErL2RpMUNjK0hYQ3FXUHdCOVNGSlpDU2ZmNTZnekhHUlo3WkwrUWhtQXljNitUcjJmZ0RId004OFc5QlVibjhvRGlRSzY3QXlVbmZHNFV3bkhZdGZMU2JTZ3lJTkNhOGZJUlNhcmlBUmcvRUVrQWc9PQ==";

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicNetworkOperator::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(IP_CHECK_URL, false));
    TTK::setUserAgentHeader(&request);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicNetworkOperator::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    QString line;
    MusicAbstractNetwork::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("result"))
            {
                value = value["result"].toMap();
                line = value["operators"].toString();
            }
        }
    }

    Q_EMIT downloadDataChanged(line);
    deleteAll();
    deleteLater();
}
