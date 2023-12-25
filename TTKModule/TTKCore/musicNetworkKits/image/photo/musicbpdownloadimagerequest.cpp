#include "musicbpdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"

static constexpr const char *ART_BACKGROUND_URL = "L2tVRE5IY2RSOVcyNysvc3RBNVNjS3pmUlNwNjlOTW8xYmlnT2sxNVJUc0VTQk1CTGtoL2Z3SG1nbDNiOWRaR1dPL1BxQ3ZOSmVUdG91TEZaUVJhNjVyb2Znd2xBYWtYYTV6bURubXFqdFFIV293cg==";

MusicBPDownloadBackgroundRequest::MusicBPDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent)
{

}

void MusicBPDownloadBackgroundRequest::startRequest()
{
    TTK_INFO_STREAM(QString("%1 startRequest").arg(className()));

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
    TTK_INFO_STREAM(QString("%1 downLoadDataFinished").arg(className()));

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
                value = value["data"].toMap();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    const QString &name = value["tag"].toString();
                    if(m_counter < MAX_IMAGE_COUNT && !name.isEmpty() && (name.contains(m_name) || m_name.contains(name)))
                    {
                        const QString &url = value["url"].toString();
                        MusicDownloadDataRequest *d = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL, m_path).arg(m_counter++).arg(SKN_FILE), TTK::Download::Background, this);
                        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished()));
                        d->startRequest();
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
