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
        deleteLater();
        return;
    }

    m_decodeUrl = url.startsWith(TTK::Algorithm::mdII(KW_ALBUM_COVER_INFO_URL, false).left(TTK_ITEM_SIZE_S));

    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startToRequest(url);
}

void MusicKWCoverSourceRequest::downLoadFinished()
{
    MusicCoverRequest::downLoadFinished();
}

void MusicKWCoverSourceRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicCoverRequest::downLoadFinished();

    if(m_decodeUrl)
    {
        startToRequest(bytes);
        m_decodeUrl = false;
    }
    else
    {
        Q_EMIT downLoadRawDataChanged(bytes);
        deleteLater();
    }
}


MusicKWDownLoadCoverRequest::MusicKWDownLoadCoverRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, TTK::Download::Cover, parent)
{

}

void MusicKWDownLoadCoverRequest::startToRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly) || m_url.isEmpty())
    {
        Q_EMIT downLoadDataChanged("The kuwo cover file create failed");
        TTK_ERROR_STREAM(className() << "file create failed");
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
            Q_EMIT downLoadDataChanged("The kuwo cover url redirection failed");
            TTK_ERROR_STREAM(className() << "url redirection failed");
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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWDownLoadCoverRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        m_file->write(m_reply->readAll());
        m_file->flush();
        m_file->close();
        TTK_INFO_STREAM(className() << "download has finished");
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
