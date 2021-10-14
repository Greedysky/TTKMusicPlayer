#include "musicdownloadbingskinrequest.h"
#include "musicdownloadsourcerequest.h"

#define PREFIX_URL      "UEQvb1lxVXFnV0dqRmxzNkY0alFJUHZUSUhyZUVNY0Y2OGZ1L255cS9CMklCakk4Q1dNQkF3PT0="
#define DOWNLOAD_URL    "bkRaMGo0WEhveVlwbEV6a0FDbEsrNmNGVHVrZTh1VmFDZTBmdElkZ0ZCYXk2dDJMaXF3MUlrV2JndmlpUWVudkF5UVVaMklvSXQydGI3cFhaTFRtaUV2VUZBcz0="

MusicDownloadBingSkinRequest::MusicDownloadBingSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadBingSkinRequest::startToDownload()
{
    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(DOWNLOAD_URL, false));
}

void MusicDownloadBingSkinRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroups items;

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("images"))
        {
            MusicSkinRemoteGroup group;
            group.m_group = MUSIC_DAILY_DIR;
            const QVariantList &datas = value["images"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                MusicSkinRemoteItem item;
                item.m_name = value["copyright"].toString();
                item.m_useCount = value["startdate"].toInt();
                item.m_url = MusicUtils::Algorithm::mdII(PREFIX_URL, false).arg(value["urlbase"].toString());

                if(item.isValid())
                {
                    group.m_items << item;
                }
            }

            if(group.isValid())
            {
                items << group;
            }
        }
    }

    Q_EMIT downLoadDataChanged(items);
}
