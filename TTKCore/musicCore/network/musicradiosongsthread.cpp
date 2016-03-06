#include "musicradiosongsthread.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>

MusicRadioSongsThread::MusicRadioSongsThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioSongsThread::~MusicRadioSongsThread()
{
    deleteAll();
}

void MusicRadioSongsThread::startToDownload(const QString &id)
{
    m_manager = new QNetworkAccessManager(this);
    QNetworkRequest networkRequest;

    networkRequest.setUrl(QUrl(songsUrl + id));
    if(m_cookJar)
    {
        m_manager->setCookieJar(m_cookJar);
    }
    m_reply = m_manager->get(networkRequest);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));

}

SongInfo MusicRadioSongsThread::getMusicSongInfo()
{
    return m_songInfo;
}

void MusicRadioSongsThread::downLoadFinished()
{
    QByteArray bytes = m_reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
    ///Put the data into Json
    if(jsonError.error != QJsonParseError::NoError ||
       !parseDoucment.isObject())
    {
        deleteAll();
        return ;
    }

    QJsonObject jsonObject = parseDoucment.object();
    if(jsonObject.contains("data"))
    {
        jsonObject = jsonObject.value("data").toObject();
        if(jsonObject.contains("songList"))
        {
            QJsonArray array = jsonObject.value("songList").toArray();
            foreach(QJsonValue value, array)
            {
                if(!value.isObject())
                {
                   continue;
                }
                QJsonObject object = value.toObject();

                m_songInfo.m_songRealLink = object.value("songLink").toString();
                m_songInfo.m_songName = object.value("songName").toString();
                m_songInfo.m_artistName = object.value("artistName").toString();
                m_songInfo.m_songPicSmall = object.value("songPicSmall").toString();
                m_songInfo.m_songPicRadio = object.value("songPicRadio").toString();
                m_songInfo.m_albumName = object.value("albumName").toString();
                m_songInfo.m_lrcLink = object.value("lrcLink").toString();
            }
        }
    }

    emit networkReplyFinished("query finished!");
    deleteAll();
}
