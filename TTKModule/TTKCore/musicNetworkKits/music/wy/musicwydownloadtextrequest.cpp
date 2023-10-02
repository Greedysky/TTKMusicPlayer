#include "musicwydownloadtextrequest.h"

MusicWYDownLoadTextRequest::MusicWYDownLoadTextRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, TTK::Download::Lrc, parent)
{

}

void MusicWYDownLoadTextRequest::startRequest()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            m_speedTimer.start();

            QNetworkRequest request;
            request.setUrl(m_url);
            TTK::setSslConfiguration(&request);
            TTK::makeContentTypeHeader(&request);

            m_reply = m_manager.get(request);
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
            QtNetworkErrorConnect(m_reply, this, replyError);
        }
        else
        {
            Q_EMIT downLoadDataChanged("The wangyi text file create failed");
            TTK_ERROR_STREAM(QString("%1 file create failed").arg(className()));
            deleteAll();
        }
    }
}

void MusicWYDownLoadTextRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200)
            {
                value = value["lrc"].toMap();
                if(!value.isEmpty())
                {
                    const QString &data = value["lyric"].toString();
                    QTextStream outstream(m_file);
                    outstream.setCodec("UTF-8");
                    outstream << data.toUtf8();
                    QtStreamEndLine(outstream);
                    m_file->close();
                    TTK_INFO_STREAM(QString("%1 download has finished").arg(className()));
                }
            }
            else
            {
                TTK_ERROR_STREAM(QString("%1 download file error").arg(className()));
                m_file->remove();
                m_file->close();
            }
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
