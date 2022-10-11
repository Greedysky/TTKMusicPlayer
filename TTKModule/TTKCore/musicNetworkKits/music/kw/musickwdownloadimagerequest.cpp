#include "musickwdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"

#define ART_BACKGROUND_URL  "NUJnNFVlSHprVzdaMWxMdXRvbEp5a3lldU51Um9GeU5RKzRDWFNER2FHL3pSRE1uK1VNRzVhVk53Y1JBUTlMbnhjeFBvRFMySnpUSldlY21xQjBkWE5GTWVkVXFsa0lNa1RKSnE3VHEwMDFPdVRDbXhUSThhWkM4TFI4RUZqbHFzVFFnQkpOY2hUR2c2YWdzb3U2cjBKSUdMYnpnZktucEJpbDVBTDlzMGF0QVMwcEtLR2JWVnc9PQ=="

MusicKWDownLoadCoverRequest::MusicKWDownLoadCoverRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, MusicObject::Download::Cover, parent)
{

}

void MusicKWDownLoadCoverRequest::startToDownload()
{
    QNetworkRequest request;
    request.setUrl(m_url);
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicKWDownLoadCoverRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        if(bytes != "NO_PIC")
        {
            if(MusicUtils::String::isNetworkUrl(bytes))
            {
                MusicDownloadDataRequest *download = new MusicDownloadDataRequest(bytes, m_savePath, MusicObject::Download::Cover, this);
                connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataChanged()));
                download->startToDownload();
            }
            else
            {
                if(m_file->open(QIODevice::WriteOnly | QIODevice::Truncate))
                {
                    m_file->write(bytes);
                    m_file->close();
                }
            }
            return;
        }
    }
    else
    {
        TTK_LOGGER_ERROR("Download kw cover data error");
    }

    downLoadDataChanged();
}

void MusicKWDownLoadCoverRequest::downLoadDataChanged()
{
    deleteAll();
}



MusicKWDownloadBackgroundRequest::MusicKWDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicDownloadImageRequest(name, path, parent)
{

}

void MusicKWDownloadBackgroundRequest::startToDownload()
{
    TTK_LOGGER_INFO(QString("%1 startToDownload").arg(className()));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(m_artName));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicKWDownloadBackgroundRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadDataFinished").arg(className()));

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        if(bytes != "NO_PIC")
        {
            QJson::Parser json;
            bool ok;
            const QVariant &data = json.parse(bytes, &ok);
            if(ok)
            {
                QString lastUrl;
                QVariantMap value = data.toMap();
                const QVariantList &datas = value["array"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    value = var.toMap();
                    if(m_counter < MAX_IMAGE_COUNTER && !value.isEmpty())
                    {
                        const QString &url = value.values().front().toString();
                        if(url == lastUrl)
                        {
                            continue;
                        }

                        lastUrl = url;
                        MusicDownloadDataRequest *download = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL, m_savePath).arg(m_counter++).arg(SKN_FILE), MusicObject::Download::Background, this);
                        connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                        download->startToDownload();
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
