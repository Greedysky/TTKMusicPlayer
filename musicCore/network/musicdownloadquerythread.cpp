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
    : QObject(parent), m_reply(nullptr)
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
        m_manager = nullptr;
    }
}

void MusicDownLoadQueryThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
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
        m_reply = nullptr;
    }

    m_reply = m_manager->get(QNetworkRequest(musicUrl));
    connect(m_reply, SIGNAL(finished()), SLOT(searchFinshed()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryThread::searchFinshed()
{
    if(m_reply == nullptr)
    {
        return;
    }

    emit clearAllItems();     ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

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
            return;
        }

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.contains("data"))
        {
            QJsonArray array = jsonObject.take("data").toArray();
            foreach(QJsonValue value, array)
            {
                if(!value.isObject())
                {
                   continue;
                }
                QJsonObject object = value.toObject();

                MusicSongInfomation musicInfo;
                if(m_currentType != MovieQuery)
                {
                    QString songId = QString::number(object.take("song_id").toVariant().toULongLong());
                    QString songName = object.take("song_name").toString();
                    QString singerName = object.take("singer_name").toString();
                    QString duration;
                    ///music normal songs urls
                    QJsonArray audUrls = object.value("audition_list").toArray();
                    foreach(QJsonValue url, audUrls)
                    {
                        QJsonObject urlObject = url.toObject();
                        if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                           urlObject.value("typeDescription").toString() == m_searchQuality))
                        {
                            MusicSongAttribute songAttr;
                            songAttr.m_url = urlObject.value("url").toString();
                            songAttr.m_size = urlObject.value("size").toString();
                            songAttr.m_format = urlObject.value("suffix").toString();
                            songAttr.m_bitrate = urlObject.value("bitRate").toInt();
                            musicInfo.m_songAttrs << songAttr;
                            ////set duration
                            duration = urlObject.value("duration").toString();
                            if(!m_queryAllRecords)
                            {
                                break;
                            }
                        }
                    }
                    ///music cd songs urls
                    QJsonArray llUrls = object.value("ll_list").toArray();
                    foreach(QJsonValue url, llUrls)
                    {
                        QJsonObject urlObject = url.toObject();
                        if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                           urlObject.value("typeDescription").toString() == m_searchQuality))
                        {
                            MusicSongAttribute songAttr;
                            songAttr.m_url = urlObject.value("url").toString();
                            songAttr.m_size = urlObject.value("size").toString();
                            songAttr.m_format = urlObject.value("suffix").toString();
                            songAttr.m_bitrate = urlObject.value("bitRate").toInt();
                            musicInfo.m_songAttrs << songAttr;
                            ////set duration
                            duration = urlObject.value("duration").toString();
                            if(!m_queryAllRecords)
                            {
                                break;
                            }
                        }
                    }

                    if(!musicInfo.m_songAttrs.isEmpty())
                    {
                        if(!m_queryAllRecords)
                        {
                            emit creatSearchedItems(songName, singerName, duration);
                        }
                        musicInfo.m_lrcUrl = MUSIC_LRC_URL.arg(singerName).arg(songName).arg(songId);
                        musicInfo.m_smallPicUrl = SML_BG_ART_URL.arg(singerName);
                        musicInfo.m_singerName = singerName;
                        musicInfo.m_songName = songName;
                        m_musicSongInfos << musicInfo;
                    }
                }
                else
                {
                    QString songName = object.take("videoName").toString();
                    QString singerName = object.take("singerName").toString();
                    QJsonArray mvUrls = object.take("mvList").toArray();
                    if( !mvUrls.isEmpty() )
                    {
                        foreach(QJsonValue url, mvUrls)
                        {
                            object = url.toObject();
                            MusicSongAttribute songAttr;
                            songAttr.m_format = object.value("suffix").toString();
                            songAttr.m_bitrate = object.value("bitRate").toInt();
                            songAttr.m_url = object.value("url").toString();
                            songAttr.m_size = object.value("size").toString();
                            musicInfo.m_songAttrs << songAttr;
                        }
                        emit creatSearchedItems(songName, singerName, object.value("duration").toString());
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
                    if(m_currentType != MovieQuery)
                    {
                        QString songId = QString::number(it.value().property("singer_id").toVariant().toULongLong());
                        QString songName = it.value().property("song_name").toString();
                        QString singerName = it.value().property("singer_name").toString();
                        QString duration;
                        ///music normal songs urls
                        QScriptValueIterator audUrlsIt(it.value().property("audition_list"));
                        while(audUrlsIt.hasNext())
                        {
                            audUrlsIt.next();
                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               audUrlsIt.value().property("typeDescription").toString() == m_searchQuality))
                            {
                                MusicSongAttribute songAttr;
                                songAttr.m_url = audUrlsIt.value().property("url").toString();
                                songAttr.m_size = audUrlsIt.value().property("size").toString();
                                songAttr.m_format = audUrlsIt.value().property("suffix").toString();
                                songAttr.m_bitrate = audUrlsIt.value().property("bitRate").toInt32();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = audUrlsIt.value().property("duration").toString();
                                if(!m_queryAllRecords)
                                {
                                    break;
                                }
                            }
                        }
                        ///music cd songs urls
                        QScriptValueIterator llUrlsIt(it.value().property("ll_list"));
                        while(llUrlsIt.hasNext())
                        {
                            llUrlsIt.next();
                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               llUrlsIt.value().property("typeDescription").toString() == m_searchQuality))
                            {
                                MusicSongAttribute songAttr;
                                songAttr.m_url = llUrlsIt.value().property("url").toString();
                                songAttr.m_size = llUrlsIt.value().property("size").toString();
                                songAttr.m_format = llUrlsIt.value().property("suffix").toString();
                                songAttr.m_bitrate = llUrlsIt.value().property("bitRate").toInt32();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = llUrlsIt.value().property("duration").toString();
                                if(!m_queryAllRecords)
                                {
                                    break;
                                }
                            }
                        }

                        if(!musicInfo.m_songAttrs.isEmpty())
                        {
                            if(!m_queryAllRecords)
                            {
                                emit creatSearchedItems(songName, singerName, duration);
                            }
                            musicInfo.m_lrcUrl = MUSIC_LRC_URL.arg(singerName).arg(songName).arg(songId);
                            musicInfo.m_smallPicUrl = SML_BG_ART_URL.arg(singerName);
                            musicInfo.m_singerName = singerName;
                            musicInfo.m_songName = songName;
                            m_musicSongInfos << musicInfo;
                        }
                    }
                    else
                    {
                        QString songName = it.value().property("videoName").toString();
                        QString singerName = it.value().property("singerName").toString();
                        QScriptValueIterator mvUrlIt(it.value().property("mvList"));
                        if( mvUrlIt.hasNext() )
                        {
                            while(mvUrlIt.hasNext())
                            {
                                mvUrlIt.next();
                                int bitRate = mvUrlIt.value().property("bitRate").toInt32();
                                if(bitRate == 0)
                                {
                                    continue;
                                }
                                MusicSongAttribute songAttr;
                                songAttr.m_bitrate = bitRate;
                                songAttr.m_format = mvUrlIt.value().property("suffix").toString();
                                songAttr.m_url = mvUrlIt.value().property("url").toString();
                                songAttr.m_size = mvUrlIt.value().property("size").toString();
                                musicInfo.m_songAttrs << songAttr;
                            }
                            emit creatSearchedItems(songName, singerName,
                                                    mvUrlIt.value().property("duration").toString());
                            musicInfo.m_singerName = singerName;
                            musicInfo.m_songName = songName;
                            m_musicSongInfos << musicInfo;
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
