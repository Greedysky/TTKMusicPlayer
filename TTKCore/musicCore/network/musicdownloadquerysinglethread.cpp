#include "musicdownloadquerysinglethread.h"
#include "musicdownloadthreadabstract.h"

#ifdef MUSIC_GREATER_NEW
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

MusicDownLoadQuerySingleThread::MusicDownLoadQuerySingleThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_searchQuality = "标准品质";
}

MusicDownLoadQuerySingleThread::~MusicDownLoadQuerySingleThread()
{

}

QString MusicDownLoadQuerySingleThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQuerySingleThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = MUSIC_REQUERY_URL.arg(text);
    ///This is a ttop music API

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQuerySingleThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
#ifdef MUSIC_GREATER_NEW
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if( jsonError.error != QJsonParseError::NoError ||
            !parseDoucment.isObject())
        {
            emit downLoadDataChanged(QString());
            deleteAll();
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

                MusicObject::MusicSongInfomation musicInfo;

                QString songId = QString::number(object.take("song_id").toVariant().toULongLong());
                QString songName = object.take("song_name").toString();
                QString singerName = object.take("singer_name").toString();

                if(m_currentType != MovieQuery)
                {
                    QString duration;
                    ///music normal songs urls
                    QJsonArray audUrls = object.value("audition_list").toArray();
                    foreach(QJsonValue url, audUrls)
                    {
                        QJsonObject urlObject = url.toObject();
                        if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                           urlObject.value("typeDescription").toString() == m_searchQuality))
                        {
                            MusicObject::MusicSongAttribute songAttr;
                            songAttr.m_url = urlObject.value("url").toString();
                            songAttr.m_size = urlObject.value("size").toString();
                            songAttr.m_format = urlObject.value("suffix").toString();
                            songAttr.m_bitrate = urlObject.value("bitRate").toVariant().toInt();
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
                            MusicObject::MusicSongAttribute songAttr;
                            songAttr.m_url = urlObject.value("url").toString();
                            songAttr.m_size = urlObject.value("size").toString();
                            songAttr.m_format = urlObject.value("suffix").toString();
                            songAttr.m_bitrate = urlObject.value("bitRate").toVariant().toInt();
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
                            emit createSearchedItems(songName, singerName, duration);
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
                    ///music mv urls
                    QJsonArray mvUrls = object.value("mv_list").toArray();
                    if(!mvUrls.isEmpty())
                    {
                        foreach(QJsonValue url, mvUrls)
                        {
                            object = url.toObject();
                            MusicObject::MusicSongAttribute songAttr;
                            songAttr.m_format = object.value("suffix").toString();
                            songAttr.m_bitrate = object.value("bitRate").toVariant().toInt();
                            songAttr.m_url = object.value("url").toString();
                            songAttr.m_size = object.value("size").toString();
                            musicInfo.m_songAttrs << songAttr;
                        }
                        emit createSearchedItems(songName, singerName, object.value("duration").toString());
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
                    QScriptValue value = it.value();
                    if(value.isNull())
                    {
                        continue;
                    }

                    MusicObject::MusicSongInfomation musicInfo;

                    QString songId = QString::number(value.property("singer_id").toVariant().toULongLong());
                    QString songName = value.property("song_name").toString();
                    QString singerName = value.property("singer_name").toString();

                    if(m_currentType != MovieQuery)
                    {
                        QString duration;
                        ///music normal songs urls
                        QScriptValueIterator audUrlsIt(value.property("audition_list"));
                        while(audUrlsIt.hasNext())
                        {
                            audUrlsIt.next();
                            QScriptValue audUrlsValue = audUrlsIt.value();
                            if(audUrlsValue.isNull())
                            {
                                continue;
                            }

                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               audUrlsValue.property("typeDescription").toString() == m_searchQuality))
                            {
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_url = audUrlsValue.property("url").toString();
                                songAttr.m_size = audUrlsValue.property("size").toString();
                                songAttr.m_format = audUrlsValue.property("suffix").toString();
                                songAttr.m_bitrate = audUrlsValue.property("bitRate").toInt32();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = audUrlsValue.property("duration").toString();
                                if(!m_queryAllRecords)
                                {
                                    break;
                                }
                            }
                        }
                        ///music cd songs urls
                        QScriptValueIterator llUrlsIt(value.property("ll_list"));
                        while(llUrlsIt.hasNext())
                        {
                            llUrlsIt.next();
                            QScriptValue llUrlsValue = llUrlsIt.value();
                            if(llUrlsValue.isNull())
                            {
                                continue;
                            }

                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               llUrlsValue.property("typeDescription").toString() == m_searchQuality))
                            {
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_url = llUrlsValue.property("url").toString();
                                songAttr.m_size = llUrlsValue.property("size").toString();
                                songAttr.m_format = llUrlsValue.property("suffix").toString();
                                songAttr.m_bitrate = llUrlsValue.property("bitRate").toInt32();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = llUrlsValue.property("duration").toString();
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
                                emit createSearchedItems(songName, singerName, duration);
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
                        QScriptValueIterator mvUrlIt(value.property("mv_list"));
                        if( mvUrlIt.hasNext() )
                        {
                            while(mvUrlIt.hasNext())
                            {
                                mvUrlIt.next();
                                QScriptValue mvUrlValue = mvUrlIt.value();
                                if(mvUrlValue.isNull())
                                {
                                    continue;
                                }

                                int bitRate = mvUrlValue.property("bitRate").toInt32();
                                if(bitRate == 0)
                                {
                                    continue;
                                }
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_bitrate = bitRate;
                                songAttr.m_format = mvUrlValue.property("suffix").toString();
                                songAttr.m_url = mvUrlValue.property("url").toString();
                                songAttr.m_size = mvUrlValue.property("size").toString();
                                musicInfo.m_songAttrs << songAttr;
                            }
                            emit createSearchedItems(songName, singerName, mvUrlIt.value().property("duration").toString());
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
            M_LOGGER_ERROR("not find the song_Id");
        }
    }
    emit downLoadDataChanged(QString());
    deleteAll();
}
