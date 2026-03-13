#include "musicdownloadbirdpaperskinrequest.h"

static constexpr const char *MAIN_URL = "Q1VDQlUvWGpxVXBjclBxZHR2MUpGaWZoeUJpUlZYRDBsYkwyV3VyRzNIY1hpWnQweTFLNWNpaCtoenp5SkR6Sg==";
static constexpr const char *QUERY_URL = "QmpGK1IwRDg4UFNQZGdza3FMSndaSG9XM2VBYm9NTGtydjNmMlcvMkx2OGlySVFHMGsvWEVHU1d3MklHNTFUa0NPOWRacFF5dG44NW5Uc1lVb2VUajRXTGwvS1VOb25iLytVT2hGUWhhbWc9";

MusicDownloadBirdPaperSkinRequest::MusicDownloadBirdPaperSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadBirdPaperSkinRequest::startToRequest()
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

void MusicDownloadBirdPaperSkinRequest::startToRequest(const QString &id)
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

void MusicDownloadBirdPaperSkinRequest::downloadFinished()
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
            if(value.contains("data"))
            {
                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicSkinRemoteGroup group;
                    group.m_id = value["old_id"].toString();
                    group.m_name = value["category"].toString();
                    group.m_type = MusicSkinRemoteGroup::BirdPaper;
                    groups << group;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged(groups);
    deleteAll();
}

void MusicDownloadBirdPaperSkinRequest::downloadItemsFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicSkinRemoteGroup group;
    group.m_type = MusicSkinRemoteGroup::BirdPaper;

    MusicAbstractDownloadSkinRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                int index = 0;
                value = value["data"].toMap();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicSkinRemoteItem item;
                    item.m_name = value["tag"].toString();
                    item.m_index = index++;
                    item.m_useCount = value["id"].toInt();
                    item.m_url = value["url"].toString();

                    if(group.m_id.isEmpty())
                    {
                        group.m_id = value["class_id"].toString();
                        group.m_name = value["category"].toString();
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
