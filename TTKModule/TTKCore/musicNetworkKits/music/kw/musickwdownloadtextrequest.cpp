#include "musickwdownloadtextrequest.h"

MusicKWDownLoadTextRequest::MusicKWDownLoadTextRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, TTK::Download::Lrc, parent)
{

}

void MusicKWDownLoadTextRequest::startToRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) || m_url.isEmpty())
    {
        Q_EMIT downLoadDataChanged("The kuwo text file create failed");
        TTK_ERROR_STREAM(className() << "file create failed");
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

void MusicKWDownLoadTextRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QString lrcData;
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["lrclist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    lrcData.append(TTKTime(value["time"].toDouble() * 1000).toString("[mm:ss.zzz]"))
                           .append(value["lineLyric"].toByteArray()).append("\n");
                }
            }

            QTextStream outstream(m_file);
            outstream.setCodec("UTF-8");
            outstream << lrcData;
            QtStreamEndLine(outstream);
            m_file->close();
            TTK_INFO_STREAM(className() << "download has finished");
        }
        else
        {
            TTK_ERROR_STREAM(className() << "download file error");
            m_file->remove();
            m_file->close();
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
