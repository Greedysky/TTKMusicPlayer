#include "musicdownload360paperskinrequest.h"

static constexpr const char *MAIN_URL = "alRMemZ6aVh5aXBIcTlSUnQ5V0RjZzcwQ05zZ2ZHY1JwaXVXbis3bDJheitDYllwL1Y1NlArd1pzaFBwbGRGN1ArQVFteFVnRjRqMHV4QzE2NVQxQVBmY3J3RS9OeWlTSWRhdEIxVk5mYzFwUTZGL05SNlk3bUFXQzlDZnlIV2U2bm1zTlEwTHhxNUI3cXZaMEQrUFpuRDZuNWkyaHV0ajVNdFc3a2JxRTZCUmd4TysvUDhQcldSUHpYbnZVSEg0YkZudU44WDNqUldJdnFaRjIrOE1DaHpzVlg0MXFZL1IxbWZiZnFuZ2NZSGR2S0NCRTFBMVJKRjdXYWxoUlN6bkFwdmpENXhCQisrTitvQWRJSktJRHhCL0UraFZtV1JQaTF0QmY0WWRid0ZTUUVMbjVZQi9nblVuK0FNPQ==";
static constexpr const char *QUERY_URL = "VXpjUVU1K3FBWDFyR1JrdW9aYjgwSS9FWWtpb2FEZEdOS0ZHK09zYkFkNGp4UFJNMzJ6TGNUVHVMK1JaZjAyQk1BTk1ZOFhVMEFXZytHeStVd1ZqL01qRVpUMVFzSTk5bTFDeUZzaitHdVU1V2xuVzgrdDNqQTFtcG9oaVZZNGV1K0NZbEJvNjVOai9yTlMyMzJHV1QwanI2TkNLNzZaVGdhQ05KTzBUZHd1bytVM3JxUW9yNm94TEFkT2FuN2VpUWJCUlBEQjlxZ2p1clZYVDF1SDVBNlExKzRvZVVJSVpGQ2lEaU1mKzdZWEpDZ2hTTDR6b3UybEtjRDMzczlZa0FXMk5GdkdWVzFwZ0NpVys0VXhoUXRFbFloU2hWNlZaT2pEWmJHbnZJVUkyS3Zyd2FVZndyT2Y3akwyTTdNc2MrTEphTEk5RWp3dlk1QXkzMmlwRnJaeENGUWxwQXRONlV2RVRkSkNGb2ZSQm1EaUZjYnVVUkJTcVJIOUNkQnBP";

MusicDownload360PaperSkinRequest::MusicDownload360PaperSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownload360PaperSkinRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(MAIN_URL, false));
    TTK::setUserAgentHeader(&request);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDownload360PaperSkinRequest::startToRequest(const QString &id)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false).arg(id));
    TTK::setUserAgentHeader(&request);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadItemsFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDownload360PaperSkinRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicSkinRemoteGroupList groups;
    MusicAbstractDownloadSkinRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                {
                    const QVariantList &datas = value["img_list"].toList();
                    for(const QVariant &var : qAsConst(datas))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();

                        MusicSkinRemoteGroup group;
                        group.m_id = value["id"].toString();
                        group.m_name = value["name"].toString();
                        group.m_type = MusicSkinRemoteGroup::Type::QihooPaper;
                        groups << group;
                    }
                }
                {
                    const QVariantList &datas = value["list"].toList();
                    for(const QVariant &var : qAsConst(datas))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();

                        MusicSkinRemoteGroup group;
                        group.m_id = value["id"].toString();
                        group.m_name = value["name"].toString();
                        group.m_type = MusicSkinRemoteGroup::Type::QihooPaper;
                        groups << group;
                    }
                }
            }
        }
    }

    Q_EMIT downloadDataChanged(groups);
    deleteAll();
}

void MusicDownload360PaperSkinRequest::downloadItemsFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicSkinRemoteGroup group;
    group.m_type = MusicSkinRemoteGroup::Type::QihooPaper;

    MusicAbstractDownloadSkinRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                int index = 0;
                value = value["data"].toMap();
                const QString &id = value["id"].toString();
                const QString &name = value["title"].toString();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicSkinRemoteItem item;
                    item.m_name = value["img_title"].toString();
                    item.m_index = index++;
                    item.m_useCount = value["down_count"].toInt();
                    item.m_url = value["img"].toString();

                    if(group.m_id.isEmpty())
                    {
                        group.m_id = id;
                        group.m_name = name;
                    }

                    if(item.isValid())
                    {
                        group.m_items << item;
                    }
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({group});
    deleteAll();
}
