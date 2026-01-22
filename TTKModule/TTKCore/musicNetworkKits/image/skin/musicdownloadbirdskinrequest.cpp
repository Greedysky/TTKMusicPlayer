#include "musicdownloadbirdskinrequest.h"
#include "musicdatasourcerequest.h"

static constexpr const char *MAIN_URL = "Q1VDQlUvWGpxVXBjclBxZHR2MUpGaWZoeUJpUlZYRDBsYkwyV3VyRzNIY1hpWnQweTFLNWNpaCtoenp5SkR6Sg==";
static constexpr const char *QUERY_URL = "MlVINmI0aGlvRXpvQVcwSklUUDRyVFVFdUExT0Rib3VuZlQ5QThRWTNyQUl4RkdlYmJLTmdrb2hVN21JZ2RZRHNYV1d3b09pcWRMcmM3d09SWWdIUlVwQ2NjSHhiSVg5V2R5ZmgyaEMyMVk9";

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
                group.m_group = QString("%1%2").arg(SKIN_BIRD_DIR, value["category"].toString());
                group.m_id = value["old_id"].toString();
                groups << group;
            }
        }
    }

    Q_EMIT downloadDataChanged(groups);
}

void MusicDownloadBirdSkinRequest::downloadItemsFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroup group;

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value.contains("data"))
        {
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
                item.m_index = value["id"].toInt();
                item.m_useCount = item.m_index;
                item.m_url = value["url"].toString();

                if(group.m_id.isEmpty())
                {
                    group.m_id = value["class_id"].toString();
                }

                if(item.isValid())
                {
                    group.m_items << item;
                }
            }
        }
    }

    Q_EMIT downloadItemsChanged(group);
}
