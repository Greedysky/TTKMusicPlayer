#include "musickwdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"

static constexpr const char *ART_BACKGROUND_URL = "NUJnNFVlSHprVzdaMWxMdXRvbEp5a3lldU51Um9GeU5RKzRDWFNER2FHL3pSRE1uK1VNRzVhVk53Y1JBUTlMbnhjeFBvRFMySnpUSldlY21xQjBkWE5GTWVkVXFsa0lNa1RKSnE3VHEwMDFPdVRDbXhUSThhWkM4TFI4RUZqbHFzVFFnQkpOY2hUR2c2YWdzb3U2cjBKSUdMYnpnZktucEJpbDVBTDlzMGF0QVMwcEtLR2JWVnc9PQ==";

MusicKWDownloadBackgroundRequest::MusicKWDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent)
{

}

void MusicKWDownloadBackgroundRequest::startToRequest()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(m_name));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWDownloadBackgroundRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractDownloadImageRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        if(bytes != "NO_PIC")
        {
            QJsonParseError ok;
            const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
            if(QJsonParseError::NoError == ok.error)
            {
                QString lastUrl;
                QVariantMap value = json.toVariant().toMap();

                const QVariantList &datas = value["array"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_counter < m_remainCount && !value.isEmpty())
                    {
                        const QString &url = value.values().front().toString();
                        if(url.isEmpty() || url == lastUrl)
                        {
                            continue;
                        }

                        lastUrl = url;
                        MusicDownloadDataRequest *req = new MusicDownloadDataRequest(url, QString("%1%2-%3%4").arg(BACKGROUND_DIR_FULL, m_path).arg(foundCount()).arg(SKN_FILE), TTK::Download::Background, this);
                        connect(req, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished()));
                        req->startToRequest();
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
