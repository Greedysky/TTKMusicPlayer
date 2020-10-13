#include "musickwdownloadtextrequest.h"

MusicKWDownLoadTextRequest::MusicKWDownLoadTextRequest(const QString &url, const QString &save, MusicObject::DownloadType  type, QObject *parent)
    : MusicAbstractDownLoadRequest(url, save, type, parent)
{

}

void MusicKWDownLoadTextRequest::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            m_speedTimer.start(MT_S2MS);
            m_manager = new QNetworkAccessManager(this);

            QNetworkRequest request;
            request.setUrl(m_url);
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
            MusicObject::setSslConfiguration(&request);
#endif
            m_reply = m_manager->get(request);
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
        }
        else
        {
            Q_EMIT downLoadDataChanged("The kuwo text file create failed");
            TTK_LOGGER_ERROR(QString("%1 file create failed").arg(getClassName()));
            deleteAll();
        }
    }
}

void MusicKWDownLoadTextRequest::downLoadFinished()
{
    if(!m_reply || !m_file)
    {
        deleteAll();
        return;
    }
    m_speedTimer.stop();

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);

        if(ok)
        {
            QString lrcData;
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                const QVariantList &datas = value["lrclist"].toList();
                foreach(const QVariant &var, datas)
                {
                    value = var.toMap();
                    lrcData.append(MusicTime(value["time"].toString().toDouble(), MusicTime::All_Sec).toString("[mm:ss.zzz]"))
                           .append(value["lineLyric"].toByteArray()).append("\n");
                }
            }

            QTextStream outstream(m_file);
            outstream.setCodec("utf-8");
            outstream << lrcData;
#if TTK_QT_VERSION_CHECK(5,15,0)
            outstream << Qt::endl;
#else
            outstream << endl;
#endif
            m_file->close();
            TTK_LOGGER_INFO(QString("%1 download has finished").arg(getClassName()));
        }
        else
        {
            TTK_LOGGER_ERROR(QString("%1 download file error").arg(getClassName()));
            m_file->remove();
            m_file->close();
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
