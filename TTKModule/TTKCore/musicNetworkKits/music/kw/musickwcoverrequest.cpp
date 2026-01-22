#include "musickwcoverrequest.h"
#include "musicdatasourcerequest.h"
#include "musickwqueryinterface.h"

MusicKWCoverSourceRequest::MusicKWCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent),
      m_decodeUrl(false)
{

}

void MusicKWCoverSourceRequest::startToRequest(const QString &url)
{
    if(url.isEmpty() || url == "NO_PIC")
    {
        deleteAll();
        return;
    }

    m_decodeUrl = url.startsWith(TTK::Algorithm::mdII(KW_ALBUM_COVER_INFO_URL, false).left(TTK_ITEM_SIZE_S));

    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadRawDataFinished(QByteArray)));
    req->startToRequest(url);
}

void MusicKWCoverSourceRequest::downloadRawDataFinished(const QByteArray &bytes)
{
    MusicCoverRequest::downloadFinished();

    if(m_decodeUrl)
    {
        startToRequest(bytes);
        m_decodeUrl = false;
    }
    else
    {
        Q_EMIT downloadRawDataChanged(bytes);
        deleteAll();
    }
}


MusicKWDownloadCoverRequest::MusicKWDownloadCoverRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownloadRequest(url, path, TTK::Download::Cover, parent)
{

}

void MusicKWDownloadCoverRequest::startToRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly) || m_url.isEmpty())
    {
        Q_EMIT downloadDataChanged("The kuwo cover file create failed");
        TTK_ERROR_STREAM(metaObject()->className() << "file create failed");
        deleteAll();
        return;
    }

    if(m_url.startsWith(TTK::Algorithm::mdII(KW_ALBUM_COVER_INFO_URL, false).left(TTK_ITEM_SIZE_S)))
    {
        QNetworkRequest request;
        request.setUrl(m_url);
        ReqKWInterface::makeRequestRawHeader(&request);

        const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
        if(bytes.isEmpty())
        {
            Q_EMIT downloadDataChanged("The kuwo cover url redirection failed");
            TTK_ERROR_STREAM(metaObject()->className() << "url redirection failed");
            deleteAll();
            return;
        }

        m_url = bytes;
    }

    m_speedTimer.start();

    QNetworkRequest request;
    request.setUrl(m_url);
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWDownloadCoverRequest::downloadFinished()
{
    MusicAbstractDownloadRequest::downloadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        m_file->write(m_reply->readAll());
        m_file->flush();
        m_file->close();
        TTK_INFO_STREAM(metaObject()->className() << "download has finished");
    }

    Q_EMIT downloadDataChanged(mapCurrentQueryData());
    deleteAll();
}
