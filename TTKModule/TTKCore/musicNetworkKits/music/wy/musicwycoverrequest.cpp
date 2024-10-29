#include "musicwycoverrequest.h"
#include "musicwyqueryinterface.h"
#include "musicdownloadqueryfactory.h"
#include "musicabstractqueryrequest.h"

static constexpr const char *WY_COVER_URL = "VUFVWVFwcE90dTIzME9XR2NsY3JYYldmTmpWZEJWZ0k5d0U1UXZUN1Jobz0=";
static constexpr const char *WY_COVER_DATA_URL = "STE3dncrRTVPdVBiRVlZaHk5M2FRRXhQamM4PQ==";

MusicWYCoverSourceRequest::MusicWYCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent)
{

}

void MusicWYCoverSourceRequest::startToRequest(const QString &url)
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServerIndex, 0);

    TTKSemaphoreLoop loop;
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->startToSearch(url);
    loop.exec();

    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServerIndex, index);

    if(d->isEmpty())
    {
        TTK_INFO_STREAM(className() << "downLoadFinished");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_PATH_V3_URL, false),
                      TTK::Algorithm::mdII(WY_COVER_URL, false),
                      TTK::Algorithm::mdII(WY_COVER_DATA_URL, false).arg(d->items().front().m_songId));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYCoverSourceRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicCoverRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QString url;
            QVariantMap value = data.toMap();

            if(value["code"].toInt() == 200)
            {
                value = value["data"].toMap();
                url = value["videoPlayUrl"].toString();
            }

            Q_EMIT downLoadDataChanged(url);
            deleteAll();
        }
    }
    else
    {
        TTK_ERROR_STREAM("Download dynamic cover data error");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
    }
}
