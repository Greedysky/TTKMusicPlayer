#include "musicradiosongsthread.h"

#ifdef MUSIC_QT_5
#   include <QJsonParseError>
#   include <QJsonDocument>
#   include <QJsonObject>
#   include <QJsonArray>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#   include <QtScript/QScriptValueIterator>
#endif

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
        m_cookJar->setParent(nullptr);
    }
    m_reply = m_manager->get(networkRequest);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));

}

SongRadioInfo MusicRadioSongsThread::getMusicSongInfo()
{
    return m_songInfo;
}

void MusicRadioSongsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
#ifdef MUSIC_QT_5
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

                    m_songInfo.m_songUrl = object.value("songLink").toString();
                    m_songInfo.m_songName = object.value("songName").toString();
                    m_songInfo.m_artistName = object.value("artistName").toString();
                    m_songInfo.m_songPicUrl = object.value("songPicRadio").toString();
                    m_songInfo.m_albumName = object.value("albumName").toString();
                    m_songInfo.m_lrcUrl = "http://musicdata.baidu.com" + object.value("lrcLink").toString();
                }
            }
        }
#else
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + QString(bytes));
        if(sc.property("data").isValid())
        {
            sc = sc.property("data");
            if(sc.property("songList").isArray())
            {
                QScriptValueIterator it(sc.property("songList"));
                while(it.hasNext())
                {
                    it.next();
                    QScriptValue value = it.value();
                    if(value.isNull() || value.property("songLink").toString().isEmpty())
                    {
                        continue;
                    }

                    m_songInfo.m_songUrl = value.property("songLink").toString();
                    m_songInfo.m_songName = value.property("songName").toString();
                    m_songInfo.m_artistName = value.property("artistName").toString();
                    m_songInfo.m_songPicUrl = value.property("songPicRadio").toString();
                    m_songInfo.m_albumName = value.property("albumName").toString();
                    m_songInfo.m_lrcUrl = "http://musicdata.baidu.com" + value.property("lrcLink").toString();
                }
            }
        }
#endif
    }
    emit networkReplyFinished("query finished!");
    deleteAll();
}
