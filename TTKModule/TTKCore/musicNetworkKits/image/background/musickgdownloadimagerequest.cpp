#include "musickgdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"
#include "musickgqueryrequest.h"

static constexpr const char *ART_BACKGROUND_URL = "ZkhTYXV4MmpWQy9hUU16NC8xZ01kRWNJZEFGQmNicVF1ek5OeGpkdzIxeDVLYU1MUzVtbGh0aVhzZVArM0FvNVgrL1lSeGdWY2hxalNadEpxZk81RG5nais2ZUlJSUtuelFvUi9OdEtnYkNFbStvT21PSDBnSzdXeVdzL3k5NWNDU3dwdjNURkJZZz0=";

MusicKGDownloadBackgroundRequest::MusicKGDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent)
{

}

void MusicKGDownloadBackgroundRequest::startToRequest()
{
    TTK_INFO_STREAM(className() << "startToRequest");

    MusicAbstractNetwork::deleteAll();

    TTKSemaphoreLoop loop;
    MusicKGQueryRequest query(this), *d = &query;
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->startToSearch(m_name);
    loop.exec();

    if(d->isEmpty())
    {
        TTK_INFO_STREAM(className() << "downLoadFinished");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(d->items().front().m_songId));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGDownloadBackgroundRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicAbstractDownloadImageRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                QVariantList datas = value["data"].toList();
                if(!datas.isEmpty())
                {
                    datas = datas.front().toList();
                    if(!datas.isEmpty())
                    {
                        value = datas.front().toMap();
                        value = value["imgs"].toMap();

                        parseFromBackgroundProperty(value["3"]);
                        parseFromBackgroundProperty(value["2"]);
                    }
                }
            }
        }
    }

    TTK_INFO_STREAM(className() << "download image size" << m_counter);
    Q_EMIT downLoadDataChanged(QString::number(m_counter));
    //
    if(m_counter == 0)
    {
        deleteAll();
    }
}

void MusicKGDownloadBackgroundRequest::parseFromBackgroundProperty(const QVariant &data)
{
    QVariantMap value;
    for(const QVariant &var : data.toList())
    {
        if(var.isNull())
        {
            continue;
        }

        value = var.toMap();
        TTK_NETWORK_QUERY_CHECK();

        if(m_counter < m_remainCount && !value.isEmpty())
        {
            const QString &url = value["sizable_portrait"].toString();
            if(url.isEmpty())
            {
                continue;
            }

            MusicDownloadDataRequest *d = new MusicDownloadDataRequest(url, QString("%1%2-%3%4").arg(BACKGROUND_DIR_FULL, m_path).arg(foundCount()).arg(SKN_FILE), TTK::Download::Background, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished()));
            d->startToRequest();
        }
    }
}
