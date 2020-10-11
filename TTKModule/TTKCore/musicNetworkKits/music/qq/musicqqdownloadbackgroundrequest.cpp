#include "musicqqdownloadbackgroundrequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicdownloaddatarequest.h"
#include "musicqqqueryinterface.h"

const QString BIG_ART_URL = "dGJmTlZOK1QvMDJENUxjMDk5UVhBWHVCb001eWtnQ1hKSnhsRWxLczNvRm9FV0kwbHhocTk4aml5SCs1Ym5mQU44SU05c1VZYVFzR2hLTEpGQ0hCNmM1ZUlZVnhnMm92QXNGMFN3PT0=";

MusicQQDownloadBackgroundRequest::MusicQQDownloadBackgroundRequest(const QString &name, const QString &save, QObject *parent)
    : MusicDownloadBackgroundRequest(name, save, parent)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

void MusicQQDownloadBackgroundRequest::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicQQDownloadBackgroundRequest::startToDownload()
{
    TTK_LOGGER_INFO(QString("%1 startToDownload").arg(getClassName()));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_SEARCH_URL, false).arg(m_artName).arg(0).arg(50);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadDataFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicQQDownloadBackgroundRequest::downLoadDataFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadDataFinished").arg(getClassName()));
    QString songId;
    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;

        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["song"].toMap();
                const QVariantList &datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    songId = value["songid"].toString();
                    break;
                }
            }
        }
    }

    downLoadUrl(songId);
}

void MusicQQDownloadBackgroundRequest::downLoadUrlFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadUrlFinished").arg(getClassName()));
    if(m_reply->error() == QNetworkReply::NoError)
    {
        QStringList datas;
        const QString text(m_reply->readAll());
        QRegExp regx(QString("<url>([^<]+)</url>"));

        int pos = text.indexOf(regx);
        while(pos != -1)
        {
            datas << regx.cap(0).remove("<url>").remove("</url>").trimmed();
            pos += regx.matchedLength();
            pos = regx.indexIn(text, pos);
        }

        foreach(const QString &url, datas)
        {
            if(m_counter < 5)
            {
                TTK_LOGGER_ERROR(url);
                MusicDownloadDataRequest *download = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL)
                                                        .arg(m_savePath).arg(m_counter++).arg(SKN_FILE), MusicObject::DownloadBigBackground, this);
                connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                download->startToDownload();
            }
        }
    }
}

void MusicQQDownloadBackgroundRequest::downLoadUrl(const QString &id)
{
    TTK_LOGGER_INFO(QString("%1 downLoadUrl %2").arg(getClassName()).arg(id));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BIG_ART_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUrlFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
