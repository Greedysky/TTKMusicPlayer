#include "musicbarragerequest.h"
#include "musicblqueryinterface.h"
#include "musicdatasourcerequest.h"

static constexpr const char *QUERY_URL = "QmhheGMwZFNKS3lzQjFEMGJSQ2VCVFdsZVc3Rms5cXJrNGtFM3pkVHMxU3dydVZiVHR4ZGVBPT0=";

MusicBarrageRequest::MusicBarrageRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicBarrageRequest::startToRequest(const QString &data)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << data);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(BL_MOVIE_URL, false).arg(data).arg(1));
    ReqBLInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicBarrageRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["result"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["bvid"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["title"].toString());

                    QString cid;
                    ReqBLInterface::parseFromMovieInfo(&info, cid);

                    if(!cid.isEmpty())
                    {
                        MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
                        connect(req, SIGNAL(downLoadRawDataChanged(QByteArray)), SIGNAL(downLoadRawDataChanged(QByteArray)));
                        req->startToRequest(TTK::Algorithm::mdII(QUERY_URL, false).arg(cid));

                        deleteAll();
                        return;
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
