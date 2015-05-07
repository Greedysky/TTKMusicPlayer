#include "musicdownloadmanagerthread.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHostInfo>
#include <QFile>

MusicDownLoadManagerThread::MusicDownLoadManagerThread(
    QObject *parent) : QObject(parent),
    m_reply(NULL),m_songIdReply(NULL)
{
    m_manager = new QNetworkAccessManager(this);
    m_songIdManager = new QNetworkAccessManager(this);
}

MusicDownLoadManagerThread::~MusicDownLoadManagerThread()
{
    deleteAll();///The release of all the objects
}

void MusicDownLoadManagerThread::deleteAll()
{
    if(m_songIdReply)
    {
      m_songIdReply->deleteLater();
      m_songIdReply = NULL;
    }
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
    if(m_songIdManager)
    {
      m_songIdManager->deleteLater();
      m_songIdManager = NULL;
    }
    this->deleteLater();
}

void MusicDownLoadManagerThread::startSearchSong(const QString& text)
{
    ///Check the net is online
//    if(QHostInfo::fromName("www.baidu.com").addresses().isEmpty())
//    {
//       emit showDownLoadInfoFor(DisConnection);
//       return;
//    }

    m_searchText = text.trimmed();
//    QUrl musicUrl = "http://mp3.baidu.com/dev/api/?tn=getinfo&ct=o&ie=utf-8&word="
//                    + text + "&format=json";
    QUrl musicUrl = "http://tingapi.ting.baidu.com/v1/restserver/ting?from=webapp_music&" \
                    "method=baidu.ting.search.catalogSug&format=json&query=" + text;
    ///This is a baidu music API
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

        m_songIdIndex = 0;
        m_songIdList.clear();     ///Empty the last search to songsID
        m_musicSongInfo.clear();  ///Empty the last search to songsInfo

        QJsonObject jsonObject = parseDoucment.object();

        if(jsonObject.contains("song"))
        {
            QJsonArray array = jsonObject.take("song").toArray();
            for(int i=0; i < array.count(); i++ )
            {
                QJsonValue value = array.at(i);
                if(!value.isObject())
                   continue ;
                QJsonObject object = value.toObject();
                if(!object.contains("songid"))
                   continue ;
                QJsonValue songidValue = object.take("songid");
                if(!songidValue.isString())
                   continue ;
                QString songId = songidValue.toString();
                if(!songId.isEmpty())
                   m_songIdList<<songId;
            }
            ///If there is no search to song_id, is repeated several times in the search
            ///If more than 5 times or no results give up
            static int counter = 5;
            if(!m_songIdList.isEmpty())
            {
                counter = 5;
                startSearchSongId();
            }
            else if( counter-- > 0 )
            {
                startSearchSong(m_searchText);
            }
            else
            {
                qDebug()<<"not find the song_Id";
                emit showDownLoadInfoFinished("not find the song_Id");
            }
        }
    }
}

void MusicDownLoadManagerThread::startSearchSongId()
{
    if( m_songIdIndex < m_songIdList.size() )
    {
        QUrl url = "http://ting.baidu.com/data/music/links?songIds=" +
                    m_songIdList.at(m_songIdIndex++);
        if(m_songIdReply)
        {
            delete m_songIdReply;
            m_songIdReply = NULL;
        }
        m_songIdReply = m_songIdManager->get(QNetworkRequest(url));
        connect(m_songIdReply, SIGNAL(finished()), this, SLOT(songIdSearchFinshed()) );
        connect(m_songIdReply, SIGNAL(error(QNetworkReply::NetworkError)),this,
                             SLOT(songIdReplyError(QNetworkReply::NetworkError)) );
    }
}

void MusicDownLoadManagerThread::songIdSearchFinshed()
{
    m_songIdReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(m_songIdReply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_songIdReply->readAll();
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError );
        if(jsonError.error == QJsonParseError::NoError )
        {
          if(parseDoucment.isObject())
          {
            QJsonObject object = parseDoucment.object();
            if(object.contains("data"))
            {
              QJsonValue dataValue = object.take("data");
              if(dataValue.isObject())
              {
                QJsonObject dataObject = dataValue.toObject();
                if(dataObject.contains("songList") )
                {
                  QJsonValue songListValue = dataObject.take("songList");
                  if(songListValue.isArray())
                  {
                    /// songList:[{....}] is an array
                    QJsonArray array = songListValue.toArray();
                    for(int i=0; i < array.size(); i++ )
                    {
                       QJsonValue value = array.at(i);
                       if(!value.isObject())
                          continue ;         ///If the data error ,then continue
                       QJsonObject songListObject = value.toObject();
                       ///Gets the name of the song
                       QString songname ="";
                       if(songListObject.contains("songName"))
                       {
                         QJsonValue songnameValue = songListObject.take("songName");
                         if(songnameValue.isString())
                            songname = songnameValue.toString();
                       }
                       ///Gets the name of the artist
                       QString artistname ="";
                       if(songListObject.contains("artistName"))
                       {
                         QJsonValue artistnameValue = songListObject.take("artistName");
                         if(artistnameValue.isString())
                            artistname = artistnameValue.toString();
                       }
                       ///Gets the name of the album
//                       QString albumname = "";
//                       if(songListObject.contains("albumName"))
//                       {
//                         QJsonValue albumnameValue = songListObject.take("albumName");
//                         if(albumnameValue.isString())
//                            albumname = albumnameValue.toString();
//                       }
                       ///Gets the song duration
                       double time = 0;
                       if(songListObject.contains("time"))
                       {
                         QJsonValue timeValue = songListObject.take("time");
                         if(timeValue.isDouble())
                            time = timeValue.toDouble();
                       }
                       ///Gets the song format
//                       QString format = "";
//                       if(songListObject.contains("format"))
//                       {
//                         QJsonValue formatValue = songListObject.take("format");
//                         if(formatValue.isString())
//                            format = formatValue.toString();
//                       }
                       ///Gets the song format
                       QString songPicSmall = "";
                       if(songListObject.contains("songPicSmall"))
                       {
                         QJsonValue songPicSmallValue = songListObject.take("songPicSmall");
                         if(songPicSmallValue.isString())
                            songPicSmall = songPicSmallValue.toString();
                       }
                       ///Gets the song bit rate
//                       double rate = 0;
//                       if(songListObject.contains("rate"))
//                       {
//                         QJsonValue rateValue = songListObject.take("rate");
//                         if(rateValue.isDouble())
//                            rate = rateValue.toDouble();
//                       }
                       ///Gets the size of the song
//                       double size = 0;
//                       if(songListObject.contains("size"))
//                       {
//                         QJsonValue sizeValue = songListObject.take("size");
//                         if(sizeValue.isDouble())
//                            size = sizeValue.toDouble();
//                       }
                       ///Gets the song connection
                       QString songlink = "";
                       if(songListObject.contains("songLink"))
                       {
                         QJsonValue songlinkValue = songListObject.take("songLink");
                         if(songlinkValue.isString())
                            songlink = songlinkValue.toString();
                       }
                       //Gets the song lrc connection
                       QString lrclink = "";
                       if(songListObject.contains("lrcLink"))
                       {
                         QJsonValue lrclinkValue = songListObject.take("lrcLink");
                         if(lrclinkValue.isString())
                            lrclink = "http://ting.baidu.com" + lrclinkValue.toString();
                       }
                       ///Create the search results of the project
                       emit creatSearchedItems(songname,artistname,time);
                       m_musicSongInfo.append(QStringList()<<songlink<<
                                          lrclink<<songPicSmall<<artistname);
                       ///Get feedback from data
                   }
                 }
               }
             }
           }
         }
       }
    }
    ///The one song searching completed, the next will be continue
    startSearchSongId();
}

void MusicDownLoadManagerThread::replyError(QNetworkReply::NetworkError)
{
    qDebug() <<"Abnormal network connection";
    emit showDownLoadInfoFor(DisConnection);
    emit showDownLoadInfoFinished("Abnormal network connection");
}

void MusicDownLoadManagerThread::songIdReplyError(QNetworkReply::NetworkError)
{
    qDebug()<<"Find a error in song_id";
    emit showDownLoadInfoFinished("Find a error in song_id");
    startSearchSongId();
}
