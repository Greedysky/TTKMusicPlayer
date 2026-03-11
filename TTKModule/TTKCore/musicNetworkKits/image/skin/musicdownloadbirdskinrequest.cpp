#include "musicdownloadbirdskinrequest.h"
#include "musicdatasourcerequest.h"

static constexpr const char *MAIN_URL = "Q1VDQlUvWGpxVXBjclBxZHR2MUpGaWZoeUJpUlZYRDBsYkwyV3VyRzNIY1hpWnQweTFLNWNpaCtoenp5SkR6Sg==";
static constexpr const char *QUERY_URL = "QmpGK1IwRDg4UFNQZGdza3FMSndaSG9XM2VBYm9NTGtydjNmMlcvMkx2OGlySVFHMGsvWEVHU1d3MklHNTFUa0NPOWRacFF5dG44NW5Uc1lVb2VUajRXTGwvS1VOb25iLytVT2hGUWhhbWc9";

MusicDownloadBirdSkinRequest::MusicDownloadBirdSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadBirdSkinRequest::startToRequest()
{
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
    req->startToRequest(TTK::Algorithm::mdII(MAIN_URL, false));
}

void MusicDownloadBirdSkinRequest::startToRequest(const QString &id)
{
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadItemsFinished(QByteArray)));
    req->startToRequest(TTK::Algorithm::mdII(QUERY_URL, false).arg(id));
}

void MusicDownloadBirdSkinRequest::downloadFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroupList groups;

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
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

    Q_EMIT downloadDataChanged(groups);
}

void MusicDownloadBirdSkinRequest::downloadItemsFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroup group;
    group.m_type = MusicSkinRemoteGroup::BirdPaper;

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
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

    Q_EMIT downloadDataChanged({group});
}
