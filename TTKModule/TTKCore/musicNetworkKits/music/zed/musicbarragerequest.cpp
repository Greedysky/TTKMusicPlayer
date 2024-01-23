#include "musicbarragerequest.h"
#include "musicblqueryinterface.h"
#include "musicdatasourcerequest.h"

static const QString QUERY_URL = "QmhheGMwZFNKS3lzQjFEMGJSQ2VCVFdsZVc3Rms5cXJrNGtFM3pkVHMxU3dydVZiVHR4ZGVBPT0=";

MusicBarrageRequest::MusicBarrageRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicBarrageRequest::startToRequest(const QString &data)
{
    TTK_INFO_STREAM(className() << "startToRequest" << data);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(BL_MOVIE_URL, false).arg(data).arg(1));
    ReqBLInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicBarrageRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
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
                        MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
                        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SIGNAL(downLoadRawDataChanged(QByteArray)));
                        d->startToRequest(TTK::Algorithm::mdII(QUERY_URL, false).arg(cid));

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
