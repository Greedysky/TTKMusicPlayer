#include "musicdownloadquerysinglethread.h"
#include "musicdownloadthreadabstract.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonArray>
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   ///QJson import
#   include "qjson/parser.h"
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
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
#ifdef MUSIC_GREATER_NEW
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
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 1 || value["code"].toInt() == 200)
            {
                QVariantList datas = value["data"].toList();
                foreach(QVariant var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;

                    QString songId = QString::number(value["singer_id"].toULongLong());
                    QString songName = value["song_name"].toString();
                    QString singerName = value["singer_name"].toString();

                    if(m_currentType != MovieQuery)
                    {
                        QString duration;
                        ///music normal songs urls
                        QVariantList auditions = value["audition_list"].toList();
                        foreach(QVariant audition, auditions)
                        {
                            QVariantMap audUrlsValue = audition.toMap();
                            if(audUrlsValue.isEmpty())
                            {
                                continue;
                            }

                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               audUrlsValue["typeDescription"].toString() == m_searchQuality))
                            {
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_url = audUrlsValue["url"].toString();
                                songAttr.m_size = audUrlsValue["size"].toString();
                                songAttr.m_format = audUrlsValue["suffix"].toString();
                                songAttr.m_bitrate = audUrlsValue["bitRate"].toInt();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = audUrlsValue["duration"].toString();
                                if(!m_queryAllRecords)
                                {
                                    break;
                                }
                            }
                        }
                        ///music cd songs urls
                        QVariantList llUrls = value["ll_list"].toList();
                        foreach(QVariant llUrl, llUrls)
                        {
                            QVariantMap llUrlValue = llUrl.toMap();
                            if(llUrlValue.isEmpty())
                            {
                                continue;
                            }

                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               llUrlValue["typeDescription"].toString() == m_searchQuality))
                            {
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_url = llUrlValue["url"].toString();
                                songAttr.m_size = llUrlValue["size"].toString();
                                songAttr.m_format = llUrlValue["suffix"].toString();
                                songAttr.m_bitrate = llUrlValue["bitRate"].toInt();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = llUrlValue["duration"].toString();
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
                        QVariantList mvs = value["mv_list"].toList();
                        if(!mvs.isEmpty())
                        {
                            QString duration;
                            foreach(QVariant mv, mvs)
                            {
                                QVariantMap mvUrlValue = mv.toMap();
                                if(mvUrlValue.isEmpty())
                                {
                                    continue;
                                }

                                duration = mvUrlValue["duration"].toString();
                                int bitRate = mvUrlValue["bitrate"].toInt();
                                if(bitRate == 0)
                                {
                                    continue;
                                }
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_bitrate = bitRate;
                                songAttr.m_format = mvUrlValue["suffix"].toString();
                                songAttr.m_url = mvUrlValue["url"].toString();
                                songAttr.m_size = mvUrlValue["size"].toString();
                                musicInfo.m_songAttrs << songAttr;
                            }
                            emit createSearchedItems(songName, singerName, duration);
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
