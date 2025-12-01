#include "musicwycoverrequest.h"
#include "musicwyqueryrequest.h"

static constexpr const char *WY_COVER_URL = "VUFVWVFwcE90dTIzME9XR2NsY3JYYldmTmpWZEJWZ0k5d0U1UXZUN1Jobz0=";
static constexpr const char *WY_COVER_DATA_URL = "STE3dncrRTVPdVBiRVlZaHk5M2FRRXhQamM4PQ==";

MusicWYCoverSourceRequest::MusicWYCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent)
{

}

void MusicWYCoverSourceRequest::startToRequest(const QString &url)
{
    TTKEventLoop loop;
    MusicWYQueryRequest query(this), *req = &query;
    connect(req, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    req->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    req->startToSearch(url);
    loop.exec();

    if(req->isEmpty())
    {
        TTK_INFO_STREAM(metaObject()->className() << "downLoadFinished");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_PATH_V3_URL, false),
                      TTK::Algorithm::mdII(WY_COVER_URL, false),
                      TTK::Algorithm::mdII(WY_COVER_DATA_URL, false).arg(req->items().first().m_songId));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYCoverSourceRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicCoverRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QString url;
            QVariantMap value = json.toVariant().toMap();

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
