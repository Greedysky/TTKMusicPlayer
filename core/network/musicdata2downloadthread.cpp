#include "musicdata2downloadthread.h"
#include <QJsonParseError>
#include <QJsonObject>

MusicData2DownloadThread::MusicData2DownloadThread(const QString &url, const QString &save,
                                                   Download_Type type, QObject *parent)
    : MusicDataDownloadThread(url, save, type, parent)
{
    m_dataReply = NULL;
    m_dataManager = new QNetworkAccessManager(this);
}

void MusicData2DownloadThread::startToDownload()
{
    m_dataReply = m_dataManager->get( QNetworkRequest(m_url));
    connect(m_dataReply, SIGNAL(finished()), SLOT(dataGetFinished()));
    connect(m_dataReply, SIGNAL(error(QNetworkReply::NetworkError)),
                         SLOT(dataReplyError(QNetworkReply::NetworkError)) );
}

void MusicData2DownloadThread::deleteAll()
{
    if(m_dataManager)
    {
        m_dataManager->deleteLater();;
        m_dataManager = NULL;
    }
    if(m_dataReply)
    {
        m_dataReply->deleteLater();
        m_dataReply = NULL;
    }
    MusicDataDownloadThread::deleteAll();
}

void MusicData2DownloadThread::dataGetFinished()
{
    if(!m_dataReply)
    {
        return;
    }

    m_dataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(m_dataReply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_dataReply->readAll();
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        if(jsonError.error != QJsonParseError::NoError || !parseDoucment.isObject())
        {
            return ;
        }

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.value("code").toInt() == 1)
        {
            m_url = jsonObject.value("data").toObject().value("singerPic").toString();
            MusicDataDownloadThread::startToDownload();
        }
        else
        {
            deleteAll();
        }
    }
}

void MusicData2DownloadThread::dataReplyError(QNetworkReply::NetworkError)
{
    emit musicDownLoadFinished("The data2 create failed");
    deleteAll();
}
