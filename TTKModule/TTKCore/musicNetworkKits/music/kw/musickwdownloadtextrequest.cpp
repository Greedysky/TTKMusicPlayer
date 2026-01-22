#include "musickwdownloadtextrequest.h"

MusicKWDownloadTextRequest::MusicKWDownloadTextRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownloadRequest(url, path, TTK::Download::Lrc, parent)
{

}

void MusicKWDownloadTextRequest::startToRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) || m_url.isEmpty())
    {
        Q_EMIT downloadDataChanged("The kuwo text file create failed");
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
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWDownloadTextRequest::downloadFinished()
{
    MusicAbstractDownloadRequest::downloadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QString lrcData;
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["lrclist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    lrcData.append(TTKTime(value["time"].toDouble() * 1000).toString("[mm:ss.zzz]"))
                           .append(value["lineLyric"].toByteArray()).append(TTK_LINEFEED);
                }
            }

            QTextStream outstream(m_file);
#if TTK_QT_VERSION_CHECK(6,0,0)
            outstream.setEncoding(QStringConverter::Utf8);
#else
            outstream.setCodec("UTF-8");
#endif
            outstream << lrcData;
            outstream << QtNamespace(endl);
            m_file->close();
            TTK_INFO_STREAM(metaObject()->className() << "download has finished");
        }
        else
        {
            TTK_ERROR_STREAM(metaObject()->className() << "download file error");
            m_file->remove();
            m_file->close();
        }
    }

    Q_EMIT downloadDataChanged(mapCurrentQueryData());
    deleteAll();
}
