#include "musicdownloadquerythread.h"
#include "musicconnectionpool.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>

MusicDownLoadQueryThread::MusicDownLoadQueryThread(QObject *parent)
    : QObject(parent),m_reply(NULL)
{
    m_searchQuality = "标准品质";
    m_manager = new QNetworkAccessManager(this);
    M_Connection->setValue("MusicDownLoadQueryThread", this);
    M_Connection->connect("MusicDownLoadQueryThread", "MusicDownloadStatusLabel");
}

MusicDownLoadQueryThread::~MusicDownLoadQueryThread()
{
    M_Connection->disConnect("MusicDownLoadQueryThread");
    deleteAll();///The release of all the objects
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = NULL;
    }
}

void MusicDownLoadQueryThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }
}

void MusicDownLoadQueryThread::startSearchSong(QueryType type, const QString &text)
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
    connect(m_reply, SIGNAL(finished()), SLOT(searchFinshed()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),this,
                   SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryThread::searchFinshed()
{
    if(m_reply == NULL)
    {
        return;
    }
    m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if(jsonError.error != QJsonParseError::NoError ||
           !parseDoucment.isObject())
        {
            return ;
        }

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
                {
                   continue ;
                }
                QJsonObject object = value.toObject();

                QStringList musicInfo;
                QString songId = QString::number(object.take("song_id").toVariant().toULongLong());
                QString songName = object.take("song_name").toString();
                QString singerName = object.take("singer_name").toString();

                if(m_currentType == MusicQuery)
                {
                    QJsonArray urls = object.take("audition_list").toArray();
                    for(int j=0; j<urls.count(); ++j)
                    {
                        object = urls[j].toObject();
                        if( object.value("type_description").toString() == m_searchQuality)
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
                    QJsonArray urls = object.take("mv_list").toArray();
                    if( urls.count() > 0)
                    {
                        object = urls[0].toObject();
                        emit creatSearchedItems(songName, singerName,
                                                object.value("duration").toString());
                        musicInfo << singerName << songName;
                        musicInfo << object.value("url").toString();
                        musicInfo << object.value("format").toString();
                        m_musicSongInfo << musicInfo;
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
            }
        }
    }
    emit resolvedSuccess();
}

void MusicDownLoadQueryThread::replyError(QNetworkReply::NetworkError)
{
    qDebug() <<"Abnormal network connection";
    deleteAll();
}
