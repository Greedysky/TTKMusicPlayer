#include "musickgdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"
#include "musickgqueryrequest.h"

static constexpr const char *ART_BACKGROUND_URL = "ZkhTYXV4MmpWQy9hUU16NC8xZ01kRWNJZEFGQmNicVF1ek5OeGpkdzIxeDVLYU1MUzVtbGh0aVhzZVArM0FvNVgrL1lSeGdWY2hxalNadEpxZk81RG5nais2ZUlJSUtuelFvUi9OdEtnYkNFbStvT21PSDBnSzdXeVdzL3k5NWNDU3dwdjNURkJZZz0=";

MusicKGDownloadBackgroundRequest::MusicKGDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent)
{

}

void MusicKGDownloadBackgroundRequest::startRequest()
{
    TTK_INFO_STREAM(QString("%1 startRequest").arg(className()));

    MusicAbstractNetwork::deleteAll();

    TTKSemaphoreLoop loop;
    MusicKGQueryRequest request(this), *d = &request;
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->setQueryType(MusicAbstractQueryRequest::QueryType::Music);
    d->startToSearch(m_name);
    loop.exec();

    if(!d->isEmpty())
    {
        QNetworkRequest request;
        request.setUrl(TTK::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(d->songInfoList().front().m_songId));
        TTK::setSslConfiguration(&request);
        TTK::makeContentTypeHeader(&request);

        m_reply = m_manager.get(request);
        connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
        QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
    }
}

void MusicKGDownloadBackgroundRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

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
                        TTK_NETWORK_QUERY_CHECK();

                        value = value["imgs"].toMap();
                        datas = value["3"].toList();

                        for(const QVariant &var : qAsConst(datas))
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            if(m_counter < MAX_IMAGE_COUNT && !value.isEmpty())
                            {
                                const QString &url = value["sizable_portrait"].toString();
                                if(url.isEmpty())
                                {
                                    continue;
                                }

                                MusicDownloadDataRequest *d = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL, m_path).arg(m_counter++).arg(SKN_FILE), TTK::Download::Background, this);
                                connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished()));
                                d->startRequest();
                            }
                        }
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString::number(m_counter));
    if(m_counter == 0)
    {
        deleteAll();
    }
}
