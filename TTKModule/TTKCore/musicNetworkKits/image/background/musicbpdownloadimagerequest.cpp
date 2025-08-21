#include "musicbpdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"

static constexpr const char *ART_BACKGROUND_URL = "L2tVRE5IY2RSOVcyNysvc3RBNVNjS3pmUlNwNjlOTW8xYmlnT2sxNVJUc0VTQk1CTGtoL2Z3SG1nbDNiOWRaR1dPL1BxQ3ZOSmVUdG91TEZaUVJhNjVyb2Znd2xBYWtYYTV6bURubXFqdFFIV293cg==";

MusicBPDownloadBackgroundRequest::MusicBPDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent)
{

}

void MusicBPDownloadBackgroundRequest::startToRequest()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ART_BACKGROUND_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicBPDownloadBackgroundRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractDownloadImageRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
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
                    TTK_NETWORK_QUERY_CHECK();

                    const QString &name = value["tag"].toString();
                    if(m_counter < m_remainCount && !name.isEmpty() && (name.contains(m_name) || m_name.contains(name)))
                    {
                        const QString &url = value["url"].toString();
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
        }
    }

    TTK_INFO_STREAM(metaObject()->className() << "download image size" << m_counter);
    Q_EMIT downLoadDataChanged(QString::number(m_counter));
    //
    if(m_counter == 0)
    {
        deleteAll();
    }
}
