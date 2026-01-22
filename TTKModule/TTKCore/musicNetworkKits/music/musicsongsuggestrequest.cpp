#include "musicsongsuggestrequest.h"
#include "musickgqueryinterface.h"

static constexpr const char *QUERY_URL = "Y1U5RUVkYWE4R205VEFEQ3pXZlNGQWNEcFlobFBMNVhMUXRxejhkbHZnaFhVbkNNdFE3T1BzdWRQTG11c3FsT2JPYi84ODFYcG9yd1phMFExQi9qVElDZmhPNlFUZDN5";

MusicSongSuggestRequest::MusicSongSuggestRequest(QObject *parent)
    : MusicPageQueryRequest(parent)
{

}

void MusicSongSuggestRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false).arg(value));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicSongSuggestRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    m_items.clear();
    MusicPageQueryRequest::downloadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();

            if(value["error_code"].toInt() == 0 && value.contains("data"))
            {
                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(value["LableName"].toString().isEmpty())
                    {
                        for(const QVariant &var : value["RecordDatas"].toList())
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            MusicResultDataItem item;
                            item.m_name = TTK::String::charactersReplace(value["HintInfo"].toString());
                            m_items << item;
                        }
                    }
                    break;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
