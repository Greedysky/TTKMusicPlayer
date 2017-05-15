#include "musicdownloadqueryxmthread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMThread::MusicDownLoadQueryXMThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "XiaMi";
}

QString MusicDownLoadQueryXMThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    deleteAll();

    QNetworkRequest request;
    makeTokenQueryUrl(m_manager, &request,
                      MusicCryptographicHash::decryptData(XM_SONG_DATA_URL, URL_KEY).arg(text).arg(1).arg(30),
                      MusicCryptographicHash::decryptData(XM_SONG_URL, URL_KEY));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMThread::downLoadFinished()
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
        bytes = bytes.replace("xiami(", "");
        bytes = bytes.replace("callback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["singers"].toString();
                    musicInfo.m_songName = value["songName"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt());

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["songId"].toString();
                        musicInfo.m_artistId = value["artistId"].toString();
                        musicInfo.m_albumId = value["albumId"].toString();
                        if(!m_querySimplify)
                        {
                            musicInfo.m_smallPicUrl = value["albumLogo"].toString();

                            readFromMusicSongLrc(&musicInfo, m_manager, musicInfo.m_songId);
                            readFromMusicSongAttribute(&musicInfo, value["listenFiles"],
                                                       m_searchQuality, m_queryAllRecords);
                            if(musicInfo.m_songAttrs.isEmpty())
                            {
                                continue;
                            }

                            MusicSearchedItem item;
                            item.m_songname = musicInfo.m_songName;
                            item.m_artistname = musicInfo.m_singerName;
                            item.m_time = musicInfo.m_timeLength;
                            item.m_type = mapQueryServerString();
                            emit createSearchedItems(item);
                        }
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
                        //MV
//                        if(value["has_mv"].toInt() == 1)
//                        {
//                            musicInfo.m_songId = value["song_id"].toString();
//                            readFromMusicMVAttribute(&musicInfo, musicInfo.m_songId);
//                        }

//                        if(musicInfo.m_songAttrs.isEmpty())
//                        {
//                            continue;
//                        }

//                        MusicSearchedItem item;
//                        item.m_songname = musicInfo.m_songName;
//                        item.m_artistname = musicInfo.m_singerName;
//                        item.m_time = musicInfo.m_songAttrs.first().m_duration;
//                        item.m_type = mapQueryServerString();
//                        emit createSearchedItems(item);
//                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    ///extra yyt movie
    if(m_currentType == MovieQuery)
    {
        MusicSemaphoreLoop loop;
        MusicDownLoadQueryYYTThread *yyt = new MusicDownLoadQueryYYTThread(this);
        connect(yyt, SIGNAL(createSearchedItems(MusicSearchedItem)), SIGNAL(createSearchedItems(MusicSearchedItem)));
        connect(yyt, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        yyt->startSearchSong(MusicDownLoadQueryYYTThread::MovieQuery, m_searchText);
        loop.exec();
        m_musicSongInfos << yyt->getMusicSongInfos();
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
