#include "musicdownloadbingskinrequest.h"
#include "musicdatasourcerequest.h"

static constexpr const char *PREFIX_URL = "UEQvb1lxVXFnV0dqRmxzNkY0alFJUHZUSUhyZUVNY0Y2OGZ1L255cS9CMklCakk4Q1dNQkF3PT0=";
static constexpr const char *QUERY_URL = "bkRaMGo0WEhveVlwbEV6a0FDbEsrNmNGVHVrZTh1VmFDZTBmdElkZ0ZCYXk2dDJMaXF3MUlrV2JndmlpUWVudkF5UVVaMklvSXQydGI3cFhaTFRtaUV2VUZBcz0=";

MusicDownloadBingSkinRequest::MusicDownloadBingSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadBingSkinRequest::startToRequest()
{
    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startToRequest(TTK::Algorithm::mdII(QUERY_URL, false));
}

void MusicDownloadBingSkinRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroupList groups;

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("images"))
        {
            MusicSkinRemoteGroup group;
            group.m_group = MUSIC_BING_DIR;
            group.m_id = TTK_DEFAULT_STR;

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
                item.m_index = value["startdate"].toInt();
                item.m_useCount = item.m_index;
                item.m_url = TTK::Algorithm::mdII(PREFIX_URL, false).arg(value["urlbase"].toString());

                if(item.isValid())
                {
                    group.m_items << item;
                }
            }

            if(group.isValid())
            {
                groups << group;
            }
        }
    }

    Q_EMIT downLoadDataChanged(groups);
}
