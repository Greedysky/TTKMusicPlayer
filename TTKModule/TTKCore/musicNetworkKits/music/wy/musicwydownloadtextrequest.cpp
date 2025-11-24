#include "musicwydownloadtextrequest.h"

MusicWYDownLoadTextRequest::MusicWYDownLoadTextRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, TTK::Download::Lrc, parent)
{

}

void MusicWYDownLoadTextRequest::startToRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) || m_url.isEmpty())
    {
        Q_EMIT downLoadDataChanged("The wangyi text file create failed");
        TTK_ERROR_STREAM(metaObject()->className() << "file create failed");
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

void MusicWYDownLoadTextRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200)
            {
                value = value["lrc"].toMap();
                if(!value.isEmpty())
                {
                    const QString &data = value["lyric"].toString();
                    QTextStream outstream(m_file);
#if TTK_QT_VERSION_CHECK(6,0,0)
                    outstream.setEncoding(QStringConverter::Utf8);
#else
                    outstream.setCodec("UTF-8");
#endif
                    outstream << data.toUtf8();
                    outstream << QtNamespace(endl);
                    m_file->close();
                    TTK_INFO_STREAM(metaObject()->className() << "download has finished");
                }
            }
            else
            {
                TTK_ERROR_STREAM(metaObject()->className() << "download file error");
                m_file->remove();
                m_file->close();
            }
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
