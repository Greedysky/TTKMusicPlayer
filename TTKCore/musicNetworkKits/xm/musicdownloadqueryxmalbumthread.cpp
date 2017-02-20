#include "musicdownloadqueryxmalbumthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMAlbumThread::MusicDownLoadQueryXMAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "XiaMi";
}

QString MusicDownLoadQueryXMAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMAlbumThread::startSearchSong(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startSearchSong(album);
}

void MusicDownLoadQueryXMAlbumThread::startSearchSong(const QString &album)
{
    m_searchText = album;
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_ALBUM_URL, URL_KEY).arg(album);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMAlbumThread::startLostAlbum(const QString &album)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_ALBUM_LOST_URL, URL_KEY).arg(album);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get( request );
    connect(reply, SIGNAL(finished()), SLOT(getLostAlbumFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                   SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMAlbumThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["trackList"].toList();
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
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = QString::number(value["songId"].toULongLong());
                        musicInfo.m_artistId = QString::number(value["artistId"].toULongLong());
                        musicInfo.m_albumId = value["name"].toString() + "<>" +
                                              value["language"].toString() + "<>" +
                                              value["company"].toString() + "<>" +
                        QDateTime::fromMSecsSinceEpoch(value["demoCreateTime"].toULongLong()).toString("yyyy-MM-dd");

                        musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(XM_SONG_PIC_URL, URL_KEY) +
                                                  value["album_logo"].toString().replace("_1.", "_4.");
                        QString lrcUrl = value["lyric_url"].toString();
                        if(!lrcUrl.endsWith("txt"))
                        {
                            musicInfo.m_lrcUrl = lrcUrl;
                        }

                        ///music normal songs urls
                        QVariantList auditions = value["allAudios"].toList();
                        foreach(const QVariant &audition, auditions)
                        {
                            QVariantMap audUrlsValue = audition.toMap();
                            if(audUrlsValue.isEmpty())
                            {
                                continue;
                            }

                            readFromMusicSongAttribute(&musicInfo, audUrlsValue, m_searchQuality, m_queryAllRecords);
                        }

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
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    if(m_musicSongInfos.isEmpty())
    {
        startLostAlbum(m_searchText);
        return;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryXMAlbumThread::getLostAlbumFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                qint64 publicTime = value["gmt_publish"].toULongLong()*1000;
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(value["album_status"].toInt() != 0)
                    {
                        continue;
                    }

                    if(m_currentType != MovieQuery)
                    {
                        MusicObject::MusicSongInfomation musicInfo = startLostSong(m_manager, QString::number(value["song_id"].toULongLong()));

                        musicInfo.m_albumId = value["album_name"].toString() + "<>" +
                                              value["language"].toString() + "<>" +
                                              value["company"].toString() + "<>" +
                        QDateTime::fromMSecsSinceEpoch(publicTime).toString("yyyy-MM-dd");

                        MusicSearchedItem item;
                        item.m_songname = musicInfo.m_songName;
                        item.m_artistname = musicInfo.m_singerName;
                        item.m_time = musicInfo.m_timeLength;
                        item.m_type = mapQueryServerString();
                        emit createSearchedItems(item);
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
