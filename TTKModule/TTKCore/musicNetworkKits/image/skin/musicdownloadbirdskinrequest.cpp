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
    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startToRequest(TTK::Algorithm::mdII(MAIN_URL, false));
}

void MusicDownloadBirdSkinRequest::startToRequest(const QString &id)
{
    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadItemsFinished(QByteArray)));
    d->startToRequest(TTK::Algorithm::mdII(QUERY_URL, false).arg(id));
}

void MusicDownloadBirdSkinRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroupList groups;

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
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
                group.m_group = QString("%1/%2").arg(MUSIC_BIRD_DIR, value["category"].toString());
                group.m_id = value["old_id"].toString();
                groups << group;
            }
        }
    }

    Q_EMIT downLoadDataChanged(groups);
}

void MusicDownloadBirdSkinRequest::downLoadItemsFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroup group;

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
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

    Q_EMIT downLoadItemsChanged(group);
}
