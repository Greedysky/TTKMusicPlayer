#include "musicqqdownloadtextrequest.h"

#define HOST_URL    "ellnUHg0Um83L2x1U29LbWw1UjFtandwRHNIRUxPcnQ="
#define REFER_URL   "YnZJaDZBVEFHSllTWlRualJFblR3U0NkYitRd1N1ZmNKaDZFQUdQVFRKND0="

MusicQQDownLoadTextRequest::MusicQQDownLoadTextRequest(const QString &url, const QString &path, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, MusicObject::Download::Lrc, parent)
{

}

void MusicQQDownLoadTextRequest::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            m_speedTimer.start();

            QNetworkRequest request;
            request.setUrl(m_url);
            request.setRawHeader("Host", MusicUtils::Algorithm::mdII(HOST_URL, false).toUtf8());
            request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
            MusicObject::setSslConfiguration(&request);

            m_reply = m_manager.get(request);
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
            QtNetworkErrorConnect(m_reply, this, replyError);
        }
        else
        {
            Q_EMIT downLoadDataChanged("The qq text file create failed");
            TTK_LOGGER_ERROR(QString("%1 file create failed").arg(className()));
            deleteAll();
        }
    }
}

void MusicQQDownLoadTextRequest::downLoadFinished()
{
    MusicAbstractDownLoadRequest::downLoadFinished();
    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes.replace("MusicJsonCallback(", "");
        bytes.chop(1);

        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            QByteArray lrcData;
            const QVariantMap &value = data.toMap();
            if(value.contains("retcode") && value["retcode"].toInt() == 0)
            {
                lrcData = value["lyric"].toByteArray();
                for(int i = 0; i < 255; ++i)
                {
                    lrcData.replace(QString("&#%1;").arg(i).toUtf8(), QString(QChar(i)).toUtf8());
                }
            }

            QTextStream outstream(m_file);
            outstream.setCodec("utf-8");
            outstream << lrcData;
            QtStreamEndl(outstream);
            m_file->close();
            TTK_LOGGER_INFO(QString("%1 download has finished").arg(className()));
        }
        else
        {
            TTK_LOGGER_ERROR(QString("%1 download file error").arg(className()));
            m_file->remove();
            m_file->close();
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
