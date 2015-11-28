#include "musicdownloadquerythread.h"
#include "musicconnectionpool.h"
#include "musicdownloadthreadabstract.h"

#ifdef MUSIC_QT_5
#   include <QJsonArray>
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#   include <QtScript/QScriptValueIterator>
#endif
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>

MusicDownLoadQueryThread::MusicDownLoadQueryThread(QObject *parent)
    : QObject(parent), m_reply(NULL)
{
    m_searchQuality = "标准品质";
    m_manager = new QNetworkAccessManager(this);
    m_queryAllRecords = false;
    M_CONNECTION->setValue("MusicDownLoadQueryThread", this);
    M_CONNECTION->connect("MusicDownLoadQueryThread", "MusicDownloadStatusLabel");
}

MusicDownLoadQueryThread::~MusicDownLoadQueryThread()
{
    M_CONNECTION->disConnect("MusicDownLoadQueryThread");
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

    QUrl musicUrl = (type != MovieQuery ) ? MUSIC_REQUERY_URL.arg(text) : MV_REQUERY_URL.arg(text);
    ///This is a ttop music API

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }

    m_reply = m_manager->get(QNetworkRequest(musicUrl));
    connect(m_reply, SIGNAL(finished()), SLOT(searchFinshed()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
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
#ifdef MUSIC_QT_5
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if( jsonError.error != QJsonParseError::NoError ||
           !parseDoucment.isObject())
        {
            return ;
        }
#endif
        emit clearAllItems();     ///Clear origin items
        m_musicSongInfos.clear();  ///Empty the last search to songsInfo
#ifdef MUSIC_QT_5
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

                MusicSongInfomation musicInfo;
                if(m_currentType != MovieQuery)
                {
                    QString songId = QString::number(object.take("song_id").toVariant().toULongLong());
                    QString songName = object.take("song_name").toString();
                    QString singerName = object.take("singer_name").toString();
                    QJsonArray urls = object.take("audition_list").toArray();
                    for(int j=0; j<urls.count(); ++j)
                    {
                        object = urls[j].toObject();
                        if(m_queryAllRecords == true)
                        {
                            MusicSongAttribute songAttr;
                            songAttr.m_url = object.value("url").toString();
                            songAttr.m_size = object.value("size").toString();
                            songAttr.m_format = object.value("format").toString();
                            songAttr.m_bitrate = object.value("bitrate").toInt();
                            musicInfo.m_songAttrs << songAttr;

                            if(j == urls.count() - 1) //the last one
                            {
                                musicInfo.m_lrcUrl = MUSIC_LRC_URL.arg(singerName).arg(songName).arg(songId);
                                musicInfo.m_smallPicUrl = SML_BG_ART_URL.arg(singerName);
                                musicInfo.m_singerName = singerName;
                                musicInfo.m_songName = songName;
                                m_musicSongInfos << musicInfo;
                            }
                        }
                        else
                        {
                            if( object.value("type_description").toString() == m_searchQuality)
                            {
                                emit creatSearchedItems(songName, singerName,
                                                        object.value("duration").toString());
                                MusicSongAttribute songAttr;
                                songAttr.m_url = object.value("url").toString();
                                songAttr.m_size = object.value("size").toString();
                                songAttr.m_format = object.value("format").toString();
                                songAttr.m_bitrate = object.value("bitrate").toInt();
                                musicInfo.m_songAttrs << songAttr;

                                musicInfo.m_lrcUrl = MUSIC_LRC_URL.arg(singerName).arg(songName).arg(songId);
                                musicInfo.m_smallPicUrl = SML_BG_ART_URL.arg(singerName);
                                musicInfo.m_singerName = singerName;
                                musicInfo.m_songName = songName;
                                m_musicSongInfos << musicInfo;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    QString songName = object.take("videoName").toString();
                    QString singerName = object.take("singerName").toString();
                    QJsonArray urls = object.take("mvList").toArray();
                    if( urls.count() > 0)
                    {
                        object = urls[0].toObject();
                        emit creatSearchedItems(songName, singerName,
                                                object.value("duration").toString());
                        for(int i=0; i<urls.count(); ++i)
                        {
                            object = urls[i].toObject();
                            MusicSongAttribute songAttr;
                            songAttr.m_format = object.value("suffix").toString();
                            songAttr.m_bitrate = object.value("bitRate").toInt();
                            songAttr.m_url = object.value("url").toString();
                            songAttr.m_size = object.value("size").toString();
                            musicInfo.m_songAttrs << songAttr;
                        }
                        musicInfo.m_singerName = singerName;
                        musicInfo.m_songName = songName;
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
#else
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + QString(m_reply->readAll()));
        if(sc.property("code").toInt32() == 1 || sc.property("code").toInt32() == 200 )
        {
            if(sc.property("data").isArray())
            {
                QScriptValueIterator it(sc.property("data"));
                while(it.hasNext())
                {
                    it.next();
                    MusicSongInfomation musicInfo;
                    if(m_currentType != MVQuery)
                    {
                        QString songId = QString::number(it.value().property("singer_id").toVariant().toULongLong());
                        QString songName = it.value().property("song_name").toString();
                        QString singerName = it.value().property("singer_name").toString();
                        QScriptValueIterator urlIt(it.value().property("audition_list"));
                        while(urlIt.hasNext())
                        {
                            urlIt.next();
                            if( urlIt.value().property("type_description").toString() == m_searchQuality)
                            {
                                emit creatSearchedItems(songName, singerName,
                                                        urlIt.value().property("duration").toString());
                                MusicSongAttribute songAttr;
                                songAttr.m_url = urlIt.value().property("url").toString();
                                songAttr.m_size = urlIt.value().property("size").toString();
                                songAttr.m_format = urlIt.value().property("format").toString();
                                songAttr.m_bitrate = urlIt.value().property("bitrate").toInt32();
                                musicInfo.m_songAttrs << songAttr;

                                musicInfo.m_lrcUrl = MUSIC_LRC_URL.arg(singerName).arg(songName).arg(songId);
                                musicInfo.m_smallPicUrl = SML_BG_ART_URL.arg(singerName);
                                musicInfo.m_singerName = singerName;
                                musicInfo.m_songName = songName;
                                m_musicSongInfo << musicInfo;
                                break;
                            }
                        }
                    }
                    else
                    {
                        QString songName = it.value().property("videoName").toString();
                        QString singerName = it.value().property("singerName").toString();
                        QScriptValueIterator urlIt(it.value().property("mvList"));
                        if( urlIt.hasNext() )
                        {
                            while(urlIt.hasNext())
                            {
                                urlIt.next();
                                int bitRate = urlIt.value().property("bitRate").toInt32();
                                if(bitRate == 0)
                                {
                                    continue;
                                }
                                MusicSongAttribute songAttr;
                                songAttr.m_bitrate = bitRate;
                                songAttr.m_format = urlIt.value().property("suffix").toString();
                                songAttr.m_url = urlIt.value().property("url").toString();
                                musicInfo.m_songAttrs << songAttr;
                            }
                            emit creatSearchedItems(songName, singerName,
                                                    urlIt.value().property("duration").toString());
                            musicInfo.m_singerName = singerName;
                            musicInfo.m_songName = songName;
                            m_musicSongInfo << musicInfo;
                        }
                    }
                }
            }
        }
#endif
        ///If there is no search to song_id, is repeated several times in the search
        ///If more than 5 times or no results give up
        static int counter = 5;
        if(m_musicSongInfos.isEmpty() && counter-- > 0)
        {
            startSearchSong(m_currentType, m_searchText);
        }
        else
        {
            M_LOGGER << "not find the song_Id" << LOG_END;
        }
    }
    emit resolvedSuccess();
}

void MusicDownLoadQueryThread::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER << "Abnormal network connection" << LOG_END;
    deleteAll();
}
