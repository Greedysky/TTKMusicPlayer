#include "musicdownloadmanagerthread.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>

MusicDownLoadManagerThread::MusicDownLoadManagerThread(
    QObject *parent) : QObject(parent),
    m_reply(NULL)
{
    m_manager = new QNetworkAccessManager(this);
}

MusicDownLoadManagerThread::~MusicDownLoadManagerThread()
{
    deleteAll();///The release of all the objects
}

void MusicDownLoadManagerThread::deleteAll()
{
    if(m_reply)
    {
      m_reply->deleteLater();
      m_reply = NULL;
    }
    if(m_manager)
    {
      m_manager->deleteLater();
      m_manager = NULL;
    }
    this->deleteLater();
}

void MusicDownLoadManagerThread::startSearchSong(QueryType type, const QString& text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = "http://so.ard.iyyin.com/s/song_with_out?q=" + text + "&page=1&size=10000000";
    ///This is a ttop music API
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }
    m_reply = m_manager->get(QNetworkRequest(musicUrl));
    connect(m_reply, SIGNAL(finished()), this, SLOT(searchFinshed()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),this,
                   SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadManagerThread::searchFinshed()
{
    m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if(jsonError.error != QJsonParseError::NoError)
            return ;
        if(!parseDoucment.isObject())
            return ;

        emit clearAllItems();     ///Clear origin items
        m_musicSongInfo.clear();  ///Empty the last search to songsInfo
        QJsonObject jsonObject = parseDoucment.object();

        if(jsonObject.contains("data"))
        {
            QJsonArray array = jsonObject.take("data").toArray();
            for(int i=0; i < array.count(); i++ )
            {
                QJsonValue value = array.at(i);
                if(!value.isObject())
                   continue ;
                QJsonObject object = value.toObject();

                QStringList musicInfo;
                QString songId = QString::number(object.take("song_id").toVariant().toULongLong());
                QString songName = object.take("song_name").toString();
                QString singerName = object.take("singer_name").toString();

                if(m_currentType == Music)
                {
                    QJsonArray urls = object.take("audition_list").toArray();
                    for(int j=0; j<urls.count(); ++j)
                    {
                        object = urls[j].toObject();
                        if( object.value("type_description").toString() == "标准品质")
                        {
                            emit creatSearchedItems(songName, singerName,
                                                    object.value("duration").toString());
                            musicInfo << object.value("url").toString();
                            musicInfo << QString("http://lp.music.ttpod.com/lrc/down?lrcid=&artist=%1&title=%2&"
                                                 "song_id=%3").arg(singerName).arg(songName).arg(songId);
                            musicInfo << QString("http://lp.music.ttpod.com/pic/down?artist=%1").arg(singerName);
                            musicInfo << singerName;
                            m_musicSongInfo << musicInfo;
                            break;
                        }
                    }
                }
                else
                {
                    QJsonArray urls = object.take("audition_list").toArray();
                    for(int j=0; j<urls.count(); ++j)
                    {
                        object = urls[j].toObject();
                        if( object.value("type_description").toString() == "标准品质")
                        {
                            emit creatSearchedItems(songName, singerName,
                                                    object.value("duration").toString());
                            musicInfo << object.value("url").toString();
                            musicInfo << QString("http://lp.music.ttpod.com/lrc/down?lrcid=&artist=%1&title=%2&"
                                                 "song_id=%3").arg(singerName).arg(songName).arg(songId);
                            musicInfo << QString("http://lp.music.ttpod.com/pic/down?artist=%1").arg(singerName);
                            musicInfo << singerName;
                            m_musicSongInfo << musicInfo;
                            break;
                        }
                    }
                }
            }
            ///If there is no search to song_id, is repeated several times in the search
            ///If more than 5 times or no results give up
            static int counter = 5;
            if(m_musicSongInfo.isEmpty() && counter-- > 0)
            {
                startSearchSong(m_currentType, m_searchText);
            }
            else
            {
                qDebug()<<"not find the song_Id";
                emit showDownLoadInfoFinished("not find the song_Id");
            }
        }
    }
}

void MusicDownLoadManagerThread::replyError(QNetworkReply::NetworkError)
{
    qDebug() <<"Abnormal network connection";
    emit showDownLoadInfoFor(DisConnection);
    emit showDownLoadInfoFinished("Abnormal network connection");
}
