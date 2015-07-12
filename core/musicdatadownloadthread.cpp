#include "musicdatadownloadthread.h"
#include <QJsonParseError>
#include <QJsonObject>

MusicDataDownloadThread::MusicDataDownloadThread(const QString &url, const QString &save,
                                                       QObject *parent)
    : MusicSongDownloadThread(url, save, parent)
{
    m_dataReply = NULL;
    m_dataManager = new QNetworkAccessManager(this);
}

void MusicDataDownloadThread::startToDownload()
{
    m_dataReply = m_dataManager->get( QNetworkRequest(m_url));
    connect(m_dataReply, SIGNAL(finished()), this, SLOT(dataGetFinished()));
    connect(m_dataReply, SIGNAL(error(QNetworkReply::NetworkError)),
                         SLOT(dataReplyError(QNetworkReply::NetworkError)) );
}

void MusicDataDownloadThread::deleteAll()
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
    MusicSongDownloadThread::deleteAll();
}

void MusicDataDownloadThread::dataGetFinished()
{
    if(!m_dataReply) return;

    m_dataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(m_dataReply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_dataReply->readAll();
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        if(jsonError.error != QJsonParseError::NoError || !parseDoucment.isObject())
            return ;

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.value("code").toInt() == 1)
        {
            m_url = jsonObject.value("data").toObject().value("singerPic").toString();
            MusicSongDownloadThread::startToDownload();
        }
        else
        {
            deleteAll();
        }
    }
}

void MusicDataDownloadThread::dataReplyError(QNetworkReply::NetworkError)
{
    emit musicDownLoadFinished("The data create failed");
    deleteAll();
}
