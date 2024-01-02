#include "musicdownloadtextrequest.h"

MusicDownLoadTextRequest::MusicDownLoadTextRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, TTK::Download::Lrc, parent)
{

}

void MusicDownLoadTextRequest::startRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) || m_url.isEmpty())
    {
        Q_EMIT downLoadDataChanged("The text file create failed");
        TTK_ERROR_STREAM("The text file create failed");
        deleteAll();
        return;
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

void MusicDownLoadTextRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        if(!bytes.isEmpty())
        {
            QTextStream outstream(m_file);
            outstream.setCodec("UTF-8");
            outstream << QString(bytes).remove("\r").toUtf8();
            QtStreamEndLine(outstream);
            m_file->close();
            TTK_INFO_STREAM("Text download has finished");
        }
        else
        {
            TTK_ERROR_STREAM("Text download file error");
            m_file->remove();
            m_file->close();
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
