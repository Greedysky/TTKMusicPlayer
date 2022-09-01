#include "musicqqdownloadimagerequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicdownloaddatarequest.h"
#include "musicqqqueryinterface.h"

const QString ART_BACKGROUND_URL = "dGJmTlZOK1QvMDJENUxjMDk5UVhBWHVCb001eWtnQ1hKSnhsRWxLczNvRm9FV0kwbHhocTk4aml5SCs1Ym5mQU44SU05c1VZYVFzR2hLTEpGQ0hCNmM1ZUlZVnhnMm92QXNGMFN3PT0=";

MusicQQDownloadBackgroundRequest::MusicQQDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicDownloadImageRequest(name, path, parent)
{

}

void MusicQQDownloadBackgroundRequest::startToDownload()
{
    TTK_LOGGER_INFO(QString("%1 startToDownload").arg(className()));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_SONG_SEARCH_URL, false).arg(m_artName).arg(0).arg(50));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadDataFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicQQDownloadBackgroundRequest::downLoadDataFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadDataFinished").arg(className()));

    MusicAbstractNetwork::downLoadFinished();
    QString songId;
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["song"].toMap();
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

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
    TTK_LOGGER_INFO(QString("%1 downLoadUrlFinished").arg(className()));

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
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

        for(const QString &url : qAsConst(datas))
        {
            if(m_counter < 5)
            {
                MusicDownloadDataRequest *download = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL, m_savePath).arg(m_counter++).arg(SKN_FILE), MusicObject::Download::Background, this);
                connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                download->startToDownload();
            }
        }
    }

    emit downLoadDataChanged(QString::number(m_counter));
    if(m_counter == 0)
    {
        deleteAll();
    }
}

void MusicQQDownloadBackgroundRequest::downLoadUrl(const QString &id)
{
    TTK_LOGGER_INFO(QString("%1 downLoadUrl %2").arg(className(), id));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(id));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUrlFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}
